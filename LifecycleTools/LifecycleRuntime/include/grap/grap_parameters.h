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

struct ShaderResourceParameters
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

struct VertexResourceParameters
{
	VertexResource* pVertexResource;
	void* pVertexBinary;
	u32 vertexBinarySize;
	AttributeData* pAttributeDataArray;
	u32 attributeCount;
	bool dynamic;
};

struct IndexResourceParameters
{
	IndexResource* pIndexResource;
	void* pIndexBinary;
	u32 indexBinarySize;
	bool dynamic;
};

//namespace grap
}
//namespace lct
}

#endif//GRAP_PARAMETERS_H
