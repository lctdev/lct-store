#include <audi/audi_device.h>
#include <audi/audi_constants.h>
#include <audi/audi_debug.h>

#include <foun/foun_debug.h>

#include <string.h>

namespace lct
{
namespace audi
{

/*
* Public Instance
*/
Device::Device()
: m_pAllocator(NULL)
, m_pVoiceResourceArray(NULL)
, m_freeVoiceResourceStack()
, m_acquiredWaveResourceCount(0)
{
}

void Device::SetAllocator(foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

void Device::CreateVoiceResources()
{
	m_pVoiceResourceArray = m_pAllocator->AllocTypeArray<VoiceResource>(VOICE_COUNT);
	
	for (u32 voiceIndex = 0; voiceIndex < VOICE_COUNT; ++voiceIndex)
	{
		VoiceResource* pVoiceResource = m_pVoiceResourceArray + voiceIndex;

		m_freeVoiceResourceStack.PushValue(pVoiceResource);
	}
}

void Device::AcquireVoiceResources()
{
	for (u32 voiceIndex = 0; voiceIndex < VOICE_COUNT; ++voiceIndex)
	{
		VoiceResource* pVoiceResource = m_pVoiceResourceArray + voiceIndex;

		alGenSources(1, &pVoiceResource->hSource);

		LCT_TRACE_AL_ERROR();
	}
}

void Device::ReleaseVoiceResources()
{
	for (u32 voiceIndex = 0; voiceIndex < VOICE_COUNT; ++voiceIndex)
	{
		VoiceResource* pVoiceResource = m_pVoiceResourceArray + voiceIndex;

		alDeleteSources(1, &pVoiceResource->hSource);

		LCT_TRACE_AL_ERROR();
	}
}

void Device::AcquireWaveResource(const WaveSetupParameters& waveSetupParameters)
{
	WaveResource* pWaveResource = waveSetupParameters.pWaveResource;
	if (pWaveResource->hBuffer != 0)
	{
		LCT_TRACE("Wave Resource already acquired!\n");
		return;
	}

	ALenum format;
	if (waveSetupParameters.channelCount == 1)
	{
		if (waveSetupParameters.sampleSize == 1)
		{
			format = AL_FORMAT_MONO8;
		}
		else
		{
			format = AL_FORMAT_MONO16;
		}
	}
	else
	{
		if (waveSetupParameters.sampleSize == 1)
		{
			format = AL_FORMAT_STEREO8;
		}
		else
		{
			format = AL_FORMAT_STEREO16;
		}
	}

	alGenBuffers(1, &pWaveResource->hBuffer);
	alBufferData(pWaveResource->hBuffer, format, waveSetupParameters.pWaveBinary, waveSetupParameters.size, waveSetupParameters.sampleRate);

	LCT_TRACE_AL_ERROR();

	++m_acquiredWaveResourceCount;
}

void Device::ReleaseWaveResource(const WaveSetupParameters& waveSetupParameters)
{
	WaveResource* pWaveResource = waveSetupParameters.pWaveResource;
	if (pWaveResource->hBuffer == 0)
	{
		LCT_TRACE("Wave Resource not yet acquired!\n");
		return;
	}

	alDeleteBuffers(1, &pWaveResource->hBuffer);

	pWaveResource->hBuffer = 0;

	LCT_TRACE_AL_ERROR();

	--m_acquiredWaveResourceCount;
}

VoiceResource* Device::UseVoice()
{
	if (m_freeVoiceResourceStack.GetValueCount() == 0)
	{
		return NULL;
	}

	VoiceResource* pVoice = m_freeVoiceResourceStack.PopValue();
	pVoice->used = true;
	return pVoice;
}

void Device::ReturnVoice(VoiceResource* pVoiceResource)
{
	if (!pVoiceResource->used)
	{
		return;
	}

	pVoiceResource->used = false;
	m_freeVoiceResourceStack.PushValue(pVoiceResource);
}

void Device::PlayVoice(VoiceResource* pVoiceResource)
{
	alSourcePlay(pVoiceResource->hSource);

	LCT_TRACE_AL_ERROR();
}

void Device::StopVoice(VoiceResource* pVoiceResource)
{
	alSourceStop(pVoiceResource->hSource);
	alSourcei(pVoiceResource->hSource, AL_BUFFER, AL_NONE);

	LCT_TRACE_AL_ERROR();
}

bool Device::IsVoicePlaying(VoiceResource* pVoiceResource)
{
	ALenum state;

	alGetSourcei(pVoiceResource->hSource, AL_SOURCE_STATE, &state);

	LCT_TRACE_AL_ERROR();

	return (state == AL_PLAYING);
}

void Device::SetVoiceLoop(VoiceResource* pVoiceResource, bool loop)
{
	alSourcei(pVoiceResource->hSource, AL_LOOPING, loop);

	LCT_TRACE_AL_ERROR();
}

void Device::SetVoiceVolume(VoiceResource* pVoiceResource, f32 volume)
{
	alSourcef(pVoiceResource->hSource, AL_GAIN, volume);

	LCT_TRACE_AL_ERROR();
}

void Device::SetVoicePitch(VoiceResource* pVoiceResource, f32 pitch)
{
	alSourcef(pVoiceResource->hSource, AL_PITCH, pitch);

	LCT_TRACE_AL_ERROR();
}

void Device::AddWave(VoiceResource* pVoiceResource, WaveResource* pWaveResource)
{
	alSourceQueueBuffers(pVoiceResource->hSource, 1, &pWaveResource->hBuffer);

	LCT_TRACE_AL_ERROR();
}

void Device::RemoveWave(VoiceResource* pVoiceResource, WaveResource* pWaveResource)
{
	alSourceUnqueueBuffers(pVoiceResource->hSource, 1, &pWaveResource->hBuffer);

	LCT_TRACE_AL_ERROR();
}

bool Device::IsWavePlaying(VoiceResource* pVoiceResource, WaveResource* pWaveResource)
{
	ALint buffer;

	alGetSourcei(pVoiceResource->hSource, AL_BUFFER, &buffer);

	LCT_TRACE_AL_ERROR();

	return (buffer == pWaveResource->hBuffer);
}

/*
* Protected Instance
*/


//namespace audi
}
//namespace lct
}
