#ifndef ASSET_VIEWER_MODE_H
#define ASSET_VIEWER_MODE_H

#include <fram/fram_mode.h>

namespace lct
{
namespace file
{
	class Accessor;
}
namespace inpu
{
	class Cursor;
}
namespace imag
{
	class ResourceHandler;
}
namespace fill
{
	class DrawContext;
}
namespace font
{
	class AssetContainer;
	class ResourceHandler;
	class DrawContext;
}
namespace spri
{
	class DrawContext;
}
namespace test
{
	class Menu;
}
}

class AssetViewerMode : public lct::fram::Mode
{
public:
	AssetViewerMode();
	virtual ~AssetViewerMode();

	void SetAccessor(lct::file::Accessor* pAccessor);
	void SetCursor(lct::inpu::Cursor* pCursor);
	void SetImageResourceHandler(lct::imag::ResourceHandler* pResourceHandler);
	void SetFillDrawContext(lct::fill::DrawContext* pDrawContext);
	void SetFontAssetContainer(lct::font::AssetContainer* pAssetContainer);
	void SetFontResourceHandler(lct::font::ResourceHandler* pResourceHandler);
	void SetFontDrawContext(lct::font::DrawContext* pDrawContext);
	void SetSpriteDrawContext(lct::spri::DrawContext* pDrawContext);

	virtual void Init();

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();

protected:
	lct::file::Accessor* m_pAccessor;

	lct::inpu::Cursor* m_pCursor;

	lct::imag::ResourceHandler* m_pImageResourceHandler;

	lct::fill::DrawContext* m_pFillDrawContext;

	lct::font::AssetContainer* m_pFontAssetContainer;
	lct::font::ResourceHandler* m_pFontResourceHandler;
	lct::font::DrawContext* m_pFontDrawContext;

	lct::spri::DrawContext* m_pSpriteDrawContext;
};

#endif//ASSET_VIEWER_MODE_H
