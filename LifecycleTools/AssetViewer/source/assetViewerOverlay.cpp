#include "assetViewerOverlay.h"

/*
 * Internal Constants
 */
static const lct::foun::FloatColor4 BACKER_COLOR = { 0.0f, 0.0f, 0.0f, 0.5f };

/*
 * Public Instance
 */
AssetViewerOverlay::AssetViewerOverlay()
: Overlay()
, m_subShared()
{
}

AssetViewerOverlay::~AssetViewerOverlay()
{
}

void AssetViewerOverlay::SetSubShared(const SubShared& subShared)
{
	m_subShared = subShared;
}
