#include "assetViewerMode.h"

/*
 * Internal Constants
 */

/*
 * Public Instance
 */
AssetViewerMode::AssetViewerMode()
: Mode()
, m_pAccessor(NULL)
, m_pCursor(NULL)
, m_pImageResourceHandler(NULL)
, m_pFillDrawContext(NULL)
, m_pFontAssetContainer(NULL)
, m_pFontDrawContext(NULL)
, m_pSpriteDrawContext(NULL)
{
}

AssetViewerMode::~AssetViewerMode()
{
}

void AssetViewerMode::SetAccessor(lct::file::Accessor* pAccessor)
{
	m_pAccessor = pAccessor;
}

void AssetViewerMode::SetCursor(lct::inpu::Cursor* pCursor)
{
	m_pCursor = pCursor;
}

void AssetViewerMode::SetImageResourceHandler(lct::imag::ResourceHandler* pResourceHandler)
{
	m_pImageResourceHandler = pResourceHandler;
}

void AssetViewerMode::SetFillDrawContext(lct::fill::DrawContext* pDrawContext)
{
	m_pFillDrawContext = pDrawContext;
}

void AssetViewerMode::SetFontAssetContainer(lct::font::AssetContainer* pAssetContainer)
{
	m_pFontAssetContainer = pAssetContainer;
}

void AssetViewerMode::SetFontDrawContext(lct::font::DrawContext* pDrawContext)
{
	m_pFontDrawContext = pDrawContext;
}

void AssetViewerMode::SetSpriteDrawContext(lct::spri::DrawContext* pDrawContext)
{
	m_pSpriteDrawContext = pDrawContext;
}

void AssetViewerMode::Init()
{
	Mode::Init();
}

void AssetViewerMode::AcquireGraphics()
{
	Mode::AcquireGraphics();

	LCT_TRACE("AssetViewerMode::AcquireGraphics Vertex Resource Count: %u\n", m_pGraphicsDevice->GetUsedVertexResourceCount());
	LCT_TRACE("AssetViewerMode::AcquireGraphics Index Resource Count: %u\n", m_pGraphicsDevice->GetUsedIndexResourceCount());
}

void AssetViewerMode::ReleaseGraphics()
{
	LCT_TRACE("AssetViewerMode::ReleaseGraphics Vertex Resource Count: %u\n", m_pGraphicsDevice->GetUsedVertexResourceCount());
	LCT_TRACE("AssetViewerMode::ReleaseGraphics Index Resource Count: %u\n", m_pGraphicsDevice->GetUsedIndexResourceCount());

	Mode::ReleaseGraphics();
}

/*
 * Protected Instance
 */
