#ifndef FONT_TEXT_BUFFER_H
#define FONT_TEXT_BUFFER_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>

namespace lct
{
namespace grap
{
class Device;
struct VertexResource;
struct IndexResource;
struct VertexSetupParameters;
struct IndexSetupParameters;
}

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

class SymbolBuffer
{
public:
	SymbolBuffer();

	void SetSheetAsset(SheetAsset* pSheetAsset);

	void CreateStructure(u32 quadCapacity, foun::Allocator* pAllocator);
	void ResetQuads();

	void AcquireResources(grap::Device* pGraphicsDevice);
	void RefreshResources(grap::Device* pGraphicsDevice);
	void ReleaseResources(grap::Device* pGraphicsDevice);

	u32 GetQuadCount() { return m_quadCount; }

protected:	
	void FillQuadSetupParameters(grap::VertexSetupParameters& vertexSetupParameters, grap::IndexSetupParameters& indexSetupParameters);

	SheetAsset* m_pSheetAsset;

	u32 m_quadCapacity;
	u32 m_quadCount;
	VertexData* m_pVertexDataArray;
	IndexData* m_pIndexDataArray;
	grap::VertexResource* m_pQuadVertexResource;
	grap::IndexResource* m_pQuadIndexResource;

	friend class SymbolWriter;
	friend class DrawContext;
};

//namespace font
}
//namespace lct
}

#endif//FONT_TEXT_BUFFER_H
