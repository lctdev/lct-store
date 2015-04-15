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
, m_pScreen(NULL)
{
}

Overlay::~Overlay()
{
}

void Overlay::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

void Overlay::SetScreen(Screen* pScreen)
{
	m_pScreen = pScreen;
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
