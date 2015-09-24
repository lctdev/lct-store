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

	LCT_TRACE("AssetViewerMode::AcquireGraphics Vertex Resource Count: %u\n", m_shared.pGraphicsDevice->GetAcquiredVertexResourceCount());
	LCT_TRACE("AssetViewerMode::AcquireGraphics Index Resource Count: %u\n", m_shared.pGraphicsDevice->GetAcquiredIndexResourceCount());
	LCT_TRACE("AssetViewerMode::AcquireGraphics Texture Resource Count: %u\n", m_shared.pGraphicsDevice->GetAcquiredTextureResourceCount());
}

void AssetViewerMode::ReleaseGraphics()
{
	LCT_TRACE("AssetViewerMode::ReleaseGraphics Vertex Resource Count: %u\n", m_shared.pGraphicsDevice->GetAcquiredVertexResourceCount());
	LCT_TRACE("AssetViewerMode::ReleaseGraphics Index Resource Count: %u\n", m_shared.pGraphicsDevice->GetAcquiredIndexResourceCount());
	LCT_TRACE("AssetViewerMode::ReleaseGraphics Texture Resource Count: %u\n", m_shared.pGraphicsDevice->GetAcquiredTextureResourceCount());

	Mode::ReleaseGraphics();
}

void AssetViewerMode::AcquireAudio()
{
	Mode::AcquireAudio();

	LCT_TRACE("AssetViewerMode::AcquireGraphics Wave Resource Count: %u\n", m_shared.pAudioDevice->GetAcquiredWaveResourceCount());
}

void AssetViewerMode::ReleaseAudio()
{
	LCT_TRACE("AssetViewerMode::ReleaseAudio Wave Resource Count: %u\n", m_shared.pAudioDevice->GetAcquiredWaveResourceCount());

	Mode::ReleaseAudio();
}

/*
 * Protected Instance
 */
