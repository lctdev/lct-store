#ifndef IMAG_RESOURCE_HANDLER_H
#define IMAG_RESOURCE_HANDLER_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>

namespace lct
{

namespace imag
{

struct TextureResource;

class ResourceHandler
{
public:
	ResourceHandler();
	virtual ~ResourceHandler();

	void AcquireTextureResource(TextureResource* pTextureResource, u32 width, u32 height, void* pTextureBinary);
	void ReleaseTextureResource(TextureResource* pTextureResource);

	u32 GetTextureResourceCount() { return m_textureResourceCount; }

private:
	u32 m_textureResourceCount;
};

//namespace imag
}
//namespace lct
}

#endif//IMAG_RESOURCE_HANDLER_H
