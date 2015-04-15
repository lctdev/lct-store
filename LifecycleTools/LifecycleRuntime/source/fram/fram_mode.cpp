#include <fram/fram_mode.h>

#include <foun/foun_debug.h>

namespace lct
{
namespace fram
{

/*
 * Internal Constants
 */
static const lct::foun::FloatColor DEFAULT_CLEAR_COLOR = { 0.0f, 0.0f, 0.0f, 1.0f };

/*
 * Public Instance
 */
Mode::Mode()
: m_pAllocator(NULL)
, m_pProgramMessageQueue(NULL)
, m_finished(false)
, m_pScreen(NULL)
, m_clearColor(DEFAULT_CLEAR_COLOR)
{
}

Mode::~Mode()
{
}

void Mode::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

void Mode::SetProgramMessageQueue(lct::fram::MessageQueue* pMessageQueue)
{
	m_pProgramMessageQueue = pMessageQueue;
}

void Mode::SetScreen(Screen* pScreen)
{
	m_pScreen = pScreen;
}

void Mode::Init()
{
	LCT_TRACE("Mode::Init\n");
}

void Mode::AcquireGraphics()
{
	LCT_TRACE("Mode::AcquireGraphics\n");
}

void Mode::ReleaseGraphics()
{
	LCT_TRACE("Mode::ReleaseGraphics\n");
}

void Mode::ReadInput()
{
}

void Mode::Update()
{
}

void Mode::Draw()
{
}

/*
 * Protected Instance
 */

//namespace lct
}
//namespace fram
}
