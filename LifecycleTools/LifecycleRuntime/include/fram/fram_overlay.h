#ifndef FRAM_OVERLAY_H
#define FRAM_OVERLAY_H

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

class Overlay
{
public:
	Overlay();
	virtual ~Overlay();

	struct Shared
	{
		foun::Allocator* pAllocator;
		MessageQueue* pProgramMessageQueue;
		grap::Screen* pScreen;
		grap::Device* pGraphicsDevice;
		audi::Device* pAudioDevice;
	};
	void SetShared(const Shared& shared);

	virtual void Init();

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();

	virtual void Activate();
	virtual void Deactivate();
	virtual bool IsActive() { return m_active; }
	virtual bool DoesBlockInput();

	virtual void ReadInput();
	virtual void Update();
	virtual void Draw();

protected:
	Shared m_shared;

	bool m_active;
};

//namespace lct
}
//namespace fram
}

#endif//FRAM_OVERLAY_H
