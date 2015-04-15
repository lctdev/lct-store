#ifndef FRAM_MODE_H
#define FRAM_MODE_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>
#include <foun/foun_color.h>

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

	void SetAllocator(lct::foun::Allocator* pAllocator);
	void SetProgramMessageQueue(lct::fram::MessageQueue* pMessageQueue);
	void SetScreen(Screen* pScreen);

	const lct::foun::FloatColor& GetClearColor() { return m_clearColor; }

	virtual void Init();

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();

	virtual void ReadInput();
	virtual void Update();
	virtual void Draw();

	bool IsFinished() { return m_finished; }

protected:
	lct::foun::Allocator* m_pAllocator;
	lct::fram::MessageQueue* m_pProgramMessageQueue;

	bool m_finished;

	Screen* m_pScreen;
	lct::foun::FloatColor m_clearColor;
};

//namespace lct
}
//namespace fram
}

#endif//FRAM_MODE_H
