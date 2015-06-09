#include <fram/fram_overlay.h>

namespace lct
{
namespace fram
{

/*
 * Internal Constants
 */

/*
 * Public Instance
 */
Overlay::Overlay()
: m_pAllocator(NULL)
, m_pProgramMessageQueue(NULL)
, m_pScreen(NULL)
, m_pGraphicsDevice(NULL)
{
}

Overlay::~Overlay()
{
}

void Overlay::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

void Overlay::SetProgramMessageQueue(lct::fram::MessageQueue* pMessageQueue)
{
	m_pProgramMessageQueue = pMessageQueue;
}

void Overlay::SetScreen(Screen* pScreen)
{
	m_pScreen = pScreen;
}

void Overlay::SetGraphicsDevice(grap::Device* pGraphicsDevice)
{
	m_pGraphicsDevice = pGraphicsDevice;
}

void Overlay::Init()
{
}

void Overlay::AcquireGraphics()
{
}

void Overlay::ReleaseGraphics()
{
}

void Overlay::ReadInput()
{
}

void Overlay::Update()
{
}

void Overlay::Draw()
{
}

/*
 * Protected Instance
 */

//namespace lct
}
//namespace fram
}
