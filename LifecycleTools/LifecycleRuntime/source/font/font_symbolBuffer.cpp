#include <font/font_symbolBuffer.h>
#include <font/font_data.h>
#include <font/font_constants.h>
#include <font/font_arrays.h>

#include <grap/grap_device.h>
#include <grap/grap_data.h>
#include <grap/grap_resources.h>
#include <grap/grap_parameters.h>

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
: m_pSheetAsset(NULL)
, m_quadCapacity(0)
, m_quadCount(0)
, m_pVertexDataArray(NULL)
, m_pIndexDataArray(NULL)
, m_pQuadVertexResource(NULL)
, m_pQuadIndexResource(NULL)
{
}

void SymbolBuffer::SetSheetAsset(SheetAsset* pSheetAsset)
{
	m_pSheetAsset = pSheetAsset;
}

void SymbolBuffer::CreateStructure(u32 quadCapacity, foun::Allocator* pAllocator)
{
	m_quadCapacity = quadCapacity;
	m_quadCount = 0;

	u32 vertexCount = m_quadCapacity * QUAD_VERTEX_COUNT;
	m_pVertexDataArray = pAllocator->AllocTypeArray<VertexData>(vertexCount);

	u32 indexCount = m_quadCapacity * QUAD_INDEX_COUNT;
	m_pIndexDataArray = pAllocator->AllocTypeArray<IndexData>(indexCount);

	m_pQuadVertexResource = pAllocator->AllocType<grap::VertexResource>();
	m_pQuadIndexResource = pAllocator->AllocType<grap::IndexResource>();
}

void SymbolBuffer::ResetQuads()
{
	m_quadCount = 0;
}

void SymbolBuffer::AcquireResources(grap::Device* pGraphicsDevice)
{
	grap::VertexResourceParameters vertexResourceParameters;
	grap::IndexResourceParameters indexResourceParameters;
	FillQuadResourceParameters(vertexResourceParameters, indexResourceParameters);
	pGraphicsDevice->AcquireVertexResource(vertexResourceParameters);
	pGraphicsDevice->AcquireIndexResource(indexResourceParameters);
}

void SymbolBuffer::RefreshResources(grap::Device* pGraphicsDevice)
{
	grap::VertexResourceParameters vertexResourceParameters;
	grap::IndexResourceParameters indexResourceParameters;
	FillQuadResourceParameters(vertexResourceParameters, indexResourceParameters);
	pGraphicsDevice->RefreshVertexResource(vertexResourceParameters);
	pGraphicsDevice->RefreshIndexResource(indexResourceParameters);
}

void SymbolBuffer::ReleaseResources(grap::Device* pGraphicsDevice)
{
	grap::VertexResourceParameters vertexResourceParameters;
	grap::IndexResourceParameters indexResourceParameters;
	FillQuadResourceParameters(vertexResourceParameters, indexResourceParameters);
	pGraphicsDevice->ReleaseVertexResource(vertexResourceParameters);
	pGraphicsDevice->ReleaseIndexResource(indexResourceParameters);
}

/*
 * Protected Instance
 */
void SymbolBuffer::FillQuadResourceParameters(grap::VertexResourceParameters& vertexResourceParameters, grap::IndexResourceParameters& indexResourceParameters)
{
	u32 vertexDataSize = sizeof(VertexData) * m_quadCapacity * QUAD_VERTEX_COUNT;

	vertexResourceParameters.pVertexResource = m_pQuadVertexResource;
	vertexResourceParameters.pVertexBinary = m_pVertexDataArray;
	vertexResourceParameters.vertexBinarySize = vertexDataSize;
	vertexResourceParameters.pAttributeDataArray = ATTRIBUTE_DATA_ARRAY;
	vertexResourceParameters.attributeCount = ATTRIBUTE_COUNT;
	vertexResourceParameters.dynamic = true;

	u32 indexDataSize = sizeof(IndexData) * m_quadCapacity * QUAD_INDEX_COUNT;

	indexResourceParameters.pIndexResource = m_pQuadIndexResource;
	indexResourceParameters.pIndexBinary = m_pIndexDataArray;
	indexResourceParameters.indexBinarySize = indexDataSize;
	indexResourceParameters.dynamic = true;
}

//namespace spri
}
//namespace lct
}
