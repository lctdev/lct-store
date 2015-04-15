#ifndef FRAM_OVERLAY_H
#define FRAM_OVERLAY_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>
#include <foun/foun_color.h>

namespace lct
{
namespace fram
{

class Screen;

class Overlay
{
public:
	Overlay();
	virtual ~Overlay();

	void SetAllocator(lct::foun::Allocator* pAllocator);
	void SetScreen(Screen* pScreen);

	virtual void Init();

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();

	virtual void ReadInput();
	virtual void Update();
	virtual void Draw();

protected:
	lct::foun::Allocator* m_pAllocator;

	Screen* m_pScreen;
};

//namespace lct
}
//namespace fram
}

#endif//FRAM_OVERLAY_H
