#ifndef FRAM_MODE_H
#define FRAM_MODE_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>
#include <foun/foun_color.h>

#include <fram/fram_screen.h>
#include <fram/fram_messageQueue.h>
#include <fram/fram_message.h>

#include <grap/grap_device.h>

namespace lct
{
namespace fram
{

class MessageQueue;
class Screen;

class Mode
{
public:
	Mode();
	virtual ~Mode();

	void SetAllocator(foun::Allocator* pAllocator);
	void SetProgramMessageQueue(lct::fram::MessageQueue* pMessageQueue);
	void SetScreen(Screen* pScreen);
	void SetGraphicsDevice(grap::Device* pGraphicsDevice);

	const lct::foun::FloatColor& GetClearColor() { return m_clearColor; }

	virtual void Init();

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();

	virtual void ReadInput();
	virtual void Update();
	virtual void Draw();

	bool IsFinished() { return m_finished; }

protected:
	// shared
	foun::Allocator* m_pAllocator;
	MessageQueue* m_pProgramMessageQueue;
	Screen* m_pScreen;
	grap::Device* m_pGraphicsDevice;

	// owned
	bool m_finished;
	
	lct::foun::FloatColor m_clearColor;
};

//namespace lct
}
//namespace fram
}

#endif//FRAM_MODE_H
