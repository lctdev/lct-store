#ifndef AUDI_DEVICE_H
#define AUDI_DEVICE_H

#include <audi/audi_resources.h>
#include <audi/audi_parameters.h>
#include <audi/audi_constants.h>

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>
#include <foun/foun_stack.h>

#if defined(LCT_WINDOWS)
#include <AL/al.h>
#include <AL/alc.h>
#elif defined(LCT_OSX)
#include <AL/al.h>
#include <AL/alc.h>
#endif

namespace lct
{
namespace audi
{

struct VoiceResource;

class Device
{
public:
	Device();

	void SetAllocator(foun::Allocator* pAllocator);

	void CreateVoiceResources();
	void AcquireVoiceResources();
	void ReleaseVoiceResources();

	void AcquireWaveResource(const WaveSetupParameters& waveSetupParameters);
	void ReleaseWaveResource(const WaveSetupParameters& waveSetupParameters);
	u32 GetAcquiredWaveResourceCount() { return m_acquiredWaveResourceCount; }

	VoiceResource* UseVoice();
	void ReturnVoice(VoiceResource* pVoiceResource);	
	void PlayVoice(VoiceResource* pVoiceResource);	
	void StopVoice(VoiceResource* pVoiceResource);
	bool IsVoicePlaying(VoiceResource* pVoiceResource);
	void SetVoiceLoop(VoiceResource* pVoiceResource, bool loop);
	void SetVoiceVolume(VoiceResource* pVoiceResource, f32 volume);
	void SetVoicePitch(VoiceResource* pVoiceResource, f32 pitch);

	void AddWave(VoiceResource* pVoiceResource, WaveResource* pWaveResource);
	void RemoveWave(VoiceResource* pVoiceResource, WaveResource* pWaveResource);
	bool IsWavePlaying(VoiceResource* pVoiceResource, WaveResource* pWaveResource);
	u32 GetPlayedWaveCount(VoiceResource* pVoiceResource);

protected:
	foun::Allocator* m_pAllocator;

	VoiceResource* m_pVoiceResourceArray;
	foun::Stack<VoiceResource*, VOICE_COUNT> m_freeVoiceResourceStack;

	u32 m_acquiredWaveResourceCount;
};

//namespace audi
}
//namespace lct
}

#endif//AUDI_DEVICE_H
