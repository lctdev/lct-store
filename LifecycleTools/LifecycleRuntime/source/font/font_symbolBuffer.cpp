#include <font/font_symbolBuffer.h>
#include <font/font_data.h>
#include <font/font_resources.h>
#include <font/font_resourceHandler.h>
#include <font/font_constants.h>

#include <imag/imag_resources.h>

#include <util/util_indexMap.h>

#include <foun/foun_debug.h>
#include <foun/foun_graphicsDebug.h>

#include <string.h>

namespace lct
{
namespace font
{

/*
 * Internal Constants
 */

/*
 * Public Instance
 */
SymbolBuffer::SymbolBuffer()
: m_pAllocator(NULL)
, m_pResourceHandler(NULL)
, m_pSheetAsset(NULL)
, m_quadCapacity(0)
, m_quadCount(0)
, m_pVertexDataArray(NULL)
, m_pIndexDataArray(NULL)
, m_pQuadResource(NULL)
{
}

void SymbolBuffer::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

void SymbolBuffer::SetResourceHandler(ResourceHandler* pResourceHandler)
{
	m_pResourceHandler = pResourceHandler;
}

void SymbolBuffer::SetSheetAsset(SheetAsset* pSheetAsset)
{
	m_pSheetAsset = pSheetAsset;
}

void SymbolBuffer::CreateResources(u32 quadCapacity)
{
	m_quadCapacity = quadCapacity;
	m_quadCount = 0;

	u32 vertexCount = m_quadCapacity * QUAD_VERTEX_COUNT;
	m_pVertexDataArray = m_pAllocator->AllocTypeArray<VertexData>(vertexCount);

	u32 indexCount = m_quadCapacity * QUAD_INDEX_COUNT;
	m_pIndexDataArray = m_pAllocator->AllocTypeArray<IndexData>(indexCount);

	m_pQuadResource = m_pAllocator->AllocType<QuadResource>();
}

void SymbolBuffer::ResetResources()
{
	m_quadCount = 0;
}

void SymbolBuffer::AcquireResources()
{
	u32 vertexDataSize = sizeof(VertexData) * m_quadCapacity * QUAD_VERTEX_COUNT;
	u32 indexDataSize = sizeof(IndexData) * m_quadCapacity * QUAD_INDEX_COUNT;
	m_pResourceHandler->AcquireQuadResource(m_pQuadResource, vertexDataSize, m_pVertexDataArray, indexDataSize, m_pIndexDataArray);
}

void SymbolBuffer::UpdateResources()
{
	u32 vertexDataSize = sizeof(VertexData) * m_quadCapacity * QUAD_VERTEX_COUNT;
	u32 indexDataSize = sizeof(IndexData) * m_quadCapacity * QUAD_INDEX_COUNT;
	m_pResourceHandler->UpdateQuadResource(m_pQuadResource, vertexDataSize, m_pVertexDataArray, indexDataSize, m_pIndexDataArray);
}

void SymbolBuffer::ReleaseResources()
{
	m_pResourceHandler->ReleaseQuadResource(m_pQuadResource);
}

/*
 * Protected Instance
 */

//namespace spri
}
//namespace lct
}
