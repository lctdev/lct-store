#include "assetViewerMode.h"

/*
 * Internal Constants
 */

/*
 * Public Instance
 */
AssetViewerMode::AssetViewerMode()
: Mode()
, m_subShared()
{
}

AssetViewerMode::~AssetViewerMode()
{
}

void AssetViewerMode::SetSubShared(const SubShared& subShared)
{
	m_subShared = subShared;
}

void AssetViewerMode::Init()
{
	Mode::Init();
}

void AssetViewerMode::AcquireGraphics()
{
	Mode::AcquireGraphics();

	LCT_TRACE("AssetViewerMode::AcquireGraphics Vertex Resource Count: %u\n", m_shared.pGraphicsDevice->GetUsedVertexResourceCount());
	LCT_TRACE("AssetViewerMode::AcquireGraphics Index Resource Count: %u\n", m_shared.pGraphicsDevice->GetUsedIndexResourceCount());
}

void AssetViewerMode::ReleaseGraphics()
{
	LCT_TRACE("AssetViewerMode::ReleaseGraphics Vertex Resource Count: %u\n", m_shared.pGraphicsDevice->GetUsedVertexResourceCount());
	LCT_TRACE("AssetViewerMode::ReleaseGraphics Index Resource Count: %u\n", m_shared.pGraphicsDevice->GetUsedIndexResourceCount());

	Mode::ReleaseGraphics();
}

/*
 * Protected Instance
 */
