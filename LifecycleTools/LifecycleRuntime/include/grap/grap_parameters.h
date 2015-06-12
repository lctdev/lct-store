#ifndef GRAP_PARAMETERS_H
#define GRAP_PARAMETERS_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace grap
{

struct ShaderResource;
struct AttributeResource;
struct AttributeData;
struct UniformResource;
struct UniformData;
struct VertexResource;
struct IndexResource;
struct TextureResource;

struct ShaderSetupParameters
{
	ShaderResource* pShaderResource;
	void* pVertexShaderBinary;
	void* pFragmentShaderBinary;
	AttributeData* pAttributeDataArray;
	u32 attributeCount;
	UniformResource* pUniformResourceArray;
	UniformData* pUniformDataArray;
	u32 uniformCount;
};

struct VertexSetupParameters
{
	VertexResource* pVertexResource;
	void* pVertexBinary;
	u32 vertexBinarySize;
	AttributeData* pAttributeDataArray;
	u32 attributeCount;
	bool dynamic;
};

struct IndexSetupParameters
{
	IndexResource* pIndexResource;
	void* pIndexBinary;
	u32 indexBinarySize;
	bool dynamic;
};

struct TextureSetupParameters
{
	TextureResource* pTextureResource;
	void* pTextureBinary;
	u32 width;
	u32 height;
};

struct TextureBindParameters
{
	const TextureResource* pTextureResource;
	const UniformResource* pUniformResource;
	u32 textureUnitIndex;
};

struct RenderStateParameters
{
	bool enableBlend;
};

//namespace grap
}
//namespace lct
}

#endif//GRAP_PARAMETERS_H
