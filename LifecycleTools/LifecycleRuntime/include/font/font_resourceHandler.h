#ifndef FONT_RESOURCE_HANDLER_H
#define FONT_RESOURCE_HANDLER_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>

namespace lct
{

namespace font
{

struct QuadResource;

class ResourceHandler
{
public:
	ResourceHandler();
	virtual ~ResourceHandler();

	void AcquireQuadResource(QuadResource* pQuadResource, u32 vertexBinarySize, void* pVertexBinary, u32 indexBinarySize, void* pIndexBinary);
	void UpdateQuadResource(QuadResource* pQuadResource, u32 vertexBinarySize, void* pVertexBinary, u32 indexBinarySize, void* pIndexBinary);
	void ReleaseQuadResource(QuadResource* pTextureResource);

	u32 GetQuadResourceCount() { return m_quadResourceCount; }

private:
	u32 m_quadResourceCount;
};

//namespace font
}
//namespace lct
}

#endif//FONT_RESOURCE_HANDLER_H
