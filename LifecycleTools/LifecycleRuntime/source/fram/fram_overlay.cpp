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
: m_shared()

, m_active(false)
{
}

Overlay::~Overlay()
{
}

void Overlay::SetShared(const Shared& shared)
{
	m_shared = shared;
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

void Overlay::Activate()
{
	m_active = true;
}

void Overlay::Deactivate()
{
	m_active = false;
}

bool Overlay::DoesBlockInput()
{
	return false;
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
