#include <imag/imag_resourceHandler.h>
#include <imag/imag_resources.h>

#include <foun/foun_debug.h>
#include <foun/foun_graphicsDebug.h>

namespace lct
{
namespace imag
{

/*
 * Internal Constants
 */

/*
 * Public Instance
 */
ResourceHandler::ResourceHandler()
: m_textureResourceCount(0)
{
}

ResourceHandler::~ResourceHandler()
{
}

void ResourceHandler::AcquireTextureResource(TextureResource* pTextureResource, u32 width, u32 height, void* pTextureBinary)
{
	if (pTextureResource->hTexture != 0)
	{
		LCT_TRACE("Texture Resource already acquired!\n");
		return;
	}

	// TEMP?
	const GLint INTERNAL_FORMAT = GL_RGBA;
	const GLint FORMAT = GL_RGBA;
	const GLint TYPE = GL_UNSIGNED_BYTE;

	FOUN_TRACE_GL_ERROR();
	glGenTextures(1, &pTextureResource->hTexture);
	glBindTexture(GL_TEXTURE_2D, pTextureResource->hTexture);
	//FOUN_TRACE_GL_ERROR();
	//FOUN_TRACE_GL_ERROR();
	glTexImage2D(GL_TEXTURE_2D, 0, INTERNAL_FORMAT, width, height, 0, FORMAT, TYPE, pTextureBinary);
	//FOUN_TRACE_GL_ERROR();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//FOUN_TRACE_GL_ERROR();

	FOUN_TRACE_GL_ERROR();

	++m_textureResourceCount;
}

void ResourceHandler::ReleaseTextureResource(TextureResource* pTextureResource)
{
	if (pTextureResource->hTexture == 0)
	{
		LCT_TRACE("Texture Resource not yet acquired!\n");
		return;
	}

	glDeleteTextures(1, &pTextureResource->hTexture);

	pTextureResource->hTexture = 0;

	FOUN_TRACE_GL_ERROR();

	--m_textureResourceCount;
}

//namespace imag
}
//namespace lct
}
