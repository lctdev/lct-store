#ifndef FONT_TEXT_BUFFER_H
#define FONT_TEXT_BUFFER_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>

namespace lct
{
namespace util
{
	class IntegerIndexMap;
}
namespace imag
{
	struct TextureAsset;
}

namespace font
{

struct SheetAsset;
struct VertexData;
typedef u16 IndexData;
struct QuadResource;
class ResourceHandler;

class SymbolBuffer
{
public:
	SymbolBuffer();

	void SetAllocator(foun::Allocator* pAllocator);
	void SetResourceHandler(ResourceHandler* pResourceHandler);
	void SetSheetAsset(SheetAsset* pSheetAsset);

	void CreateResources(u32 quadCapacity);
	void ResetResources();
	void AcquireResources();
	void UpdateResources();
	void ReleaseResources();

	u32 GetQuadCount() { return m_quadCount; }

protected:
	foun::Allocator* m_pAllocator;
	ResourceHandler* m_pResourceHandler;
	SheetAsset* m_pSheetAsset;

	u32 m_quadCapacity;
	u32 m_quadCount;
	VertexData* m_pVertexDataArray;
	IndexData* m_pIndexDataArray;
	QuadResource* m_pQuadResource;

	friend class SymbolWriter;
	friend class DrawContext;
};

//namespace font
}
//namespace lct
}

#endif//FONT_TEXT_BUFFER_H
