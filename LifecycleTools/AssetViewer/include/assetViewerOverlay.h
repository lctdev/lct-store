#ifndef ASSET_VIEWER_OVERLAY_H
#define ASSET_VIEWER_OVERLAY_H

#include <foun/foun_matrix.h>
#include <foun/foun_vector.h>
#include <foun/foun_callback.h>

#include <fram/fram_overlay.h>

namespace lct
{
namespace inpu
{
	class Cursor;
}
namespace fill
{
	class DrawContext;
}
namespace font
{
	class AssetContainer;
	class ResourceHandler;
	class SymbolBuffer;
	class SymbolWriter;
	class DrawContext;
}
namespace test
{
	class Menu;
}
namespace fram
{
	class MessageQueue;
}
}

class SpriteViewerOverlay : public lct::fram::Overlay
{
public:
	SpriteViewerOverlay();
	virtual ~SpriteViewerOverlay();

	void SetProgramMessageQueue(lct::fram::MessageQueue* pMessageQueue);
	void SetInputCursor(lct::inpu::Cursor* pCursor);
	void SetFillDrawContext(lct::fill::DrawContext* pDrawContext);
	void SetFontResourceHandler(lct::font::ResourceHandler* pResourceHandler);
	void SetFontAssetContainer(lct::font::AssetContainer* pAssetContainer);
	void SetFontDrawContext(lct::font::DrawContext* pDrawContext);

	virtual void Init();

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();

	virtual void ReadInput();
	virtual void Update();
	virtual void Draw();

protected:
	lct::fram::MessageQueue* m_pProgramMessageQueue;

	lct::inpu::Cursor* m_pInputCursor;

	lct::fill::DrawContext* m_pFillDrawContext;

	lct::font::AssetContainer* m_pFontAssetContainer;
	lct::font::ResourceHandler* m_pFontResourceHandler;
	static const u32 SYMBOL_BUFFER_COUNT = 2;
	lct::font::SymbolBuffer* m_pSymbolBufferArray;
	u32 m_currSymbolBufferIndex;
	lct::font::SymbolWriter* m_pSymbolWriter;
	lct::font::DrawContext* m_pFontDrawContext;

	int OnBackAction(int parameter);

	lct::foun::InstanceCallback<SpriteViewerOverlay, int, int> m_backActionCallback;

	lct::test::Menu* m_pMenu;

	lct::foun::Matrix44 m_projectionTransform;
};

#endif//ASSET_VIEWER_OVERLAY_H
