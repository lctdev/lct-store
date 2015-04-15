#include <font/font_resourceHandler.h>
#include <font/font_resources.h>

#include <foun/foun_debug.h>
#include <foun/foun_graphicsDebug.h>

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
ResourceHandler::ResourceHandler()
: m_quadResourceCount(0)
{
}

ResourceHandler::~ResourceHandler()
{
}

void ResourceHandler::AcquireQuadResource(QuadResource* pQuadResource, u32 vertexBinarySize, void* pVertexBinary, u32 indexBinarySize, void* pIndexBinary)
{
	if ((pQuadResource->hVertexBuffer != 0) || (pQuadResource->hIndexBuffer != 0))
	{
		LCT_TRACE("Quad Resource already acquired!\n");
		return;
	}

	glGenBuffers(1, &pQuadResource->hVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, pQuadResource->hVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexBinarySize, pVertexBinary, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &pQuadResource->hIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pQuadResource->hIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBinarySize, pIndexBinary, GL_DYNAMIC_DRAW);

	FOUN_TRACE_GL_ERROR();

	++m_quadResourceCount;
}

void ResourceHandler::UpdateQuadResource(QuadResource* pQuadResource, u32 vertexBinarySize, void* pVertexBinary, u32 indexBinarySize, void* pIndexBinary)
{
	if ((pQuadResource->hVertexBuffer == 0) || (pQuadResource->hIndexBuffer == 0))
	{
		LCT_TRACE("Quad Resource not yet acquired!\n");
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, pQuadResource->hVertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBinarySize, pVertexBinary);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pQuadResource->hIndexBuffer);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexBinarySize, pIndexBinary);

	FOUN_TRACE_GL_ERROR();
}

void ResourceHandler::ReleaseQuadResource(QuadResource* pQuadResource)
{
	if ((pQuadResource->hVertexBuffer == 0) || (pQuadResource->hIndexBuffer == 0))
	{
		LCT_TRACE("Quad Resource not yet acquired!\n");
		return;
	}

	glDeleteBuffers(1, &pQuadResource->hVertexBuffer);

	pQuadResource->hVertexBuffer = 0;

	glDeleteBuffers(1, &pQuadResource->hIndexBuffer);

	pQuadResource->hIndexBuffer = 0;

	FOUN_TRACE_GL_ERROR();

	--m_quadResourceCount;
}

//namespace font
}
//namespace lct
}
