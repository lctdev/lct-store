#ifndef FRAM_MODE_H
#define FRAM_MODE_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>
#include <foun/foun_color.h>

#include <fram/fram_messageQueue.h>
#include <fram/fram_message.h>

#include <grap/grap_screen.h>
#include <grap/grap_device.h>

#include <audi/audi_device.h>

namespace lct
{
namespace fram
{

class MessageQueue;

class Mode
{
public:
	Mode();
	virtual ~Mode();

	struct Shared
	{
		foun::Allocator* pAllocator;
		MessageQueue* pProgramMessageQueue;
		grap::Screen* pScreen;
		grap::Device* pGraphicsDevice;
		audi::Device* pAudioDevice;
	};
	void SetShared(const Shared& shared);

	const lct::foun::FloatColor4& GetClearColor() { return m_clearColor; }

	virtual void Init();

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();
	virtual void AcquireAudio();
	virtual void ReleaseAudio();

	virtual void ReadInput();
	virtual void Update();
	virtual void Draw();

	bool IsFinished() { return m_finished; }

protected:
	Shared m_shared;

	bool m_finished;
	
	lct::foun::FloatColor4 m_clearColor;
};

//namespace lct
}
//namespace fram
}

#endif//FRAM_MODE_H
