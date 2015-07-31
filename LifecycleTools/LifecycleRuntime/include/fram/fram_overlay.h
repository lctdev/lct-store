#ifndef FRAM_OVERLAY_H
#define FRAM_OVERLAY_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>
#include <foun/foun_color.h>

#include <fram/fram_messageQueue.h>
#include <fram/fram_message.h>

#include <grap/grap_screen.h>
#include <grap/grap_device.h>

namespace lct
{
namespace fram
{

class Overlay
{
public:
	Overlay();
	virtual ~Overlay();

	void SetAllocator(foun::Allocator* pAllocator);
	void SetProgramMessageQueue(fram::MessageQueue* pMessageQueue);
	void SetScreen(grap::Screen* pScreen);
	void SetGraphicsDevice(grap::Device* pGraphicsDevice);

	virtual void Init();

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();

	virtual void ReadInput();
	virtual void Update();
	virtual void Draw();

protected:
	// shared
	foun::Allocator* m_pAllocator;
	fram::MessageQueue* m_pProgramMessageQueue;
	grap::Screen* m_pScreen;
	grap::Device* m_pGraphicsDevice;
};

//namespace lct
}
//namespace fram
}

#endif//FRAM_OVERLAY_H
