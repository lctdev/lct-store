#ifndef SHAD_RESOURCE_HANDLER_H
#define SHAD_RESOURCE_HANDLER_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>

namespace lct
{
namespace shad
{

struct ShaderResource;
struct ShaderData;
struct AttributeResource;
struct AttributeData;
struct UniformResource;
struct UniformData;

class ResourceHandler
{
public:
	ResourceHandler();

	void SetAllocator(lct::foun::Allocator* pAllocator);

	void AcquireShaderResources(ShaderResource* pShaderResource, ShaderData* pShaderData, void* pVertexShaderBinary, void* pFragmentShaderBinary,
			AttributeResource* pAttributeResourceArray, AttributeData* pAttributeDataArray);
	void ReleaseShaderResources(ShaderResource* pShaderResource, ShaderData* pShaderData, AttributeResource* pAttributeResourceArray);

	void AcquireUniformResource(ShaderResource* pShaderResource, UniformResource* pUniformResource, UniformData* pUniformData);
	void ReleaseUniformResource(UniformResource* pUniformResource);

private:
	lct::foun::Allocator* m_pAllocator;
};

//namespace shad
}
//namespace lct
}

#endif//SHAD_RESOURCE_HANDLER_H
