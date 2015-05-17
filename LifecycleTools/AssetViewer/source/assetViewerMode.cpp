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
, m_pFontResourceHandler(NULL)
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

void AssetViewerMode::SetFontResourceHandler(lct::font::ResourceHandler* pResourceHandler)
{
	m_pFontResourceHandler = pResourceHandler;
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

	LCT_TRACE("AssetViewerMode::AcquireGraphics Image Texture Resource Count: %u\n", m_pImageResourceHandler->GetTextureResourceCount());
	LCT_TRACE("AssetViewerMode::AcquireGraphics Font Quad Resource Count: %u\n", m_pFontResourceHandler->GetQuadResourceCount());
}

void AssetViewerMode::ReleaseGraphics()
{
	LCT_TRACE("AssetViewerMode::ReleaseGraphics Image Texture Resource Count: %u\n", m_pImageResourceHandler->GetTextureResourceCount());
	LCT_TRACE("AssetViewerMode::ReleaseGraphics Font Quad Resource Count: %u\n", m_pFontResourceHandler->GetQuadResourceCount());

	Mode::ReleaseGraphics();
}

/*
 * Protected Instance
 */
