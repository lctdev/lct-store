#ifndef GRAP_DEVICE_H
#define GRAP_DEVICE_H

#include <grap/grap_data.h>
#include <grap/grap_resources.h>
#include <grap/grap_parameters.h>

#include <foun/foun_primitives.h>
#include <foun/foun_vector.h>
#include <foun/foun_matrix.h>
#include <foun/foun_color.h>

#if defined(LCT_WINDOWS)
#include <GL/glew.h>
#elif defined(LCT_ANDROID)
#include <GLES2/gl2.h>
#elif defined(LCT_OSX)
#include <GL/glew.h>
#endif

namespace lct
{
namespace grap
{

class Device
{
public:
	Device();

	void AcquireShaderResources(const ShaderSetupParameters& shaderSetupParameters);
	void ReleaseShaderResources(const ShaderSetupParameters& shaderSetupParameters);
	u32 GetUsedShaderResourceCount() { return m_usedShaderResourceCount; }

	void AcquireVertexResource(const VertexSetupParameters& vertexSetupParameters);
	void RefreshVertexResource(const VertexSetupParameters& vertexSetupParameters);
	void ReleaseVertexResource(const VertexSetupParameters& vertexSetupParameters);
	u32 GetUsedVertexResourceCount() { return m_usedVertexResourceCount; }

	void AcquireIndexResource(const IndexSetupParameters& indexSetupParameters);
	void RefreshIndexResource(const IndexSetupParameters& indexSetupParameters);
	void ReleaseIndexResource(const IndexSetupParameters& indexSetupParameters);
	u32 GetUsedIndexResourceCount() { return m_usedIndexResourceCount; }

	void AcquireTextureResource(const TextureSetupParameters& textureSetupParameters);
	void ReleaseTextureResource(const TextureSetupParameters& textureSetupParameters);
	u32 GetUsedTextureResourceCount() { return m_usedTextureResourceCount; }

	void ActivateRenderState(RenderStateParameters& renderStateParameters);

	void ActivateShader(const ShaderResource* pShaderResource);

	void ActivateUniform(const UniformResource* pUniformResource, const foun::Vector4& vector);
	void ActivateUniform(const UniformResource* pUniformResource, const foun::FloatColor& color);
	void ActivateUniform(const UniformResource* pUniformResource, const foun::Matrix33& matrix);
	void ActivateUniform(const UniformResource* pUniformResource, const foun::Matrix44& matrix);

	void ActivateVertex(const VertexResource* pVertexResource);
	void DeactivateVertex(const VertexResource* pVertexResource);

	void ActivateIndex(const IndexResource* pIndexResource);

	void ActivateTexture(TextureBindParameters& textureBindParameters);

	void Draw(u32 indexCount, u32 indexOffset, IndexType indexType);

protected:
	u32 m_usedShaderResourceCount;
	u32 m_usedVertexResourceCount;
	u32 m_usedIndexResourceCount;
	u32 m_usedTextureResourceCount;
};

//namespace grap
}
//namespace lct
}

#endif//GRAP_DEVICE_H
