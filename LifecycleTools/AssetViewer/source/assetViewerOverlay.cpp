#include "assetViewerOverlay.h"

/*
 * Internal Constants
 */

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
