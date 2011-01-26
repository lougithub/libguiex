/** 
* @file guitexture_opengl.cpp
* @brief use opengl to render gui
* @author ken
* @date 2006-07-06
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module\render_opengl\guitexture_opengl.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guidatachunk.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacefilesys.h>
#include <libguiex_core/guiinterfaceimageloader.h>
#include <libguiex_core/guigarbagecollector.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	CGUITexture_opengl::CGUITexture_opengl(IGUIInterfaceRender* pRender)
		:CGUITextureImp(pRender)
		,m_nTextureWidth(0)
		,m_nTextureHeight(0)
		,m_nBytesPerPixel(0)
		,m_ePixelFormat(GUI_PF_RGBA_32)
	{
		// generate a OGL texture that we will use.
		glGenTextures(1, &m_ogltexture);

		// set some parameters for this texture.
		glBindTexture(GL_TEXTURE_2D, m_ogltexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST/*GL_LINEAR*/);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST/*GL_LINEAR*/); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);	// GL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);	// GL_CLAMP_TO_EDGE
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	//------------------------------------------------------------------------------
	CGUITexture_opengl::~CGUITexture_opengl()
	{
		// otherwise delete OGL texture associated with this object.
		glDeleteTextures(1, &m_ogltexture);
		m_ogltexture = 0;
	}
	//------------------------------------------------------------------------------
	uint16	CGUITexture_opengl::GetWidth(void) const
	{
		return m_nTextureWidth;
	}
	//------------------------------------------------------------------------------
	uint16	CGUITexture_opengl::GetHeight(void) const
	{
		return m_nTextureHeight;
	}
	//------------------------------------------------------------------------------
	uint32 CGUITexture_opengl::GetBufferSize() const
	{
		return static_cast<uint32>(m_nTextureWidth * m_nTextureHeight * m_nBytesPerPixel);
	}
	//------------------------------------------------------------------------------
	uint32 CGUITexture_opengl::GetBuffer(uint8* pBuffer, uint32 nBufferSize, EGuiPixelFormat& rPixelFormat)
	{
		GUI_ASSERT( pBuffer && nBufferSize == GetBufferSize(), "invalid parameter");

		GLenum eFormat = 0;
		switch(m_ePixelFormat)
		{
		case GUI_PF_RGBA_32:
			eFormat = GL_RGBA;
			break;

		//case GUI_PF_LUMINANCE_ALPHA_16:
		//	eFormat = GL_LUMINANCE_ALPHA;
		//	break;

		default:
			throw CGUIException("[CGUITexture_opengl::LoadFromMemory]: unsupported pixel format;");
		}

		glBindTexture(GL_TEXTURE_2D, m_ogltexture);
		glGetTexImage( GL_TEXTURE_2D,0, eFormat,GL_UNSIGNED_BYTE,pBuffer);

		rPixelFormat = m_ePixelFormat;

		return 0;
	}
	//------------------------------------------------------------------------------
	int32	CGUITexture_opengl::LoadFromFile(const CGUIString& filename )
	{
		IGUIInterfaceImageLoader* pImageLoader = static_cast<IGUIInterfaceImageLoader*>(CGUIInterfaceManager::Instance()->GetInterface("IGUIImageLoader"));
		if( !pImageLoader )
		{
			throw CGUIException("[CGUITexture_opengl::LoadFromFile]: - failed to get image loader!");
			return -1;
		}

		CGUIAutoRelease<CGUIImageData> pImageData(pImageLoader->LoadFromFile( filename ));
		if( !pImageData )
		{
			throw CGUIException("[CGUITexture_opengl::LoadFromFile]: - failed to Load image file <%s>!", filename.c_str());
			return -1;
		}

		int32 ret = LoadFromMemory( pImageData->GetData(), pImageData->GetWidth(), pImageData->GetHeight(), pImageData->GetPixelFormat());

		return ret;
	}
	//------------------------------------------------------------------------------
	int32	CGUITexture_opengl::LoadFromMemory(const void* buffPtr, int32 buffWidth, int32 buffHeight, EGuiPixelFormat ePixelFormat/* = PF_RGBA_32*/)
	{
		glBindTexture(GL_TEXTURE_2D, m_ogltexture);

		switch(ePixelFormat)
		{
		case GUI_PF_RGBA_32:
			glTexImage2D(GL_TEXTURE_2D, 0, 4, buffWidth, buffHeight, 0, GL_RGBA ,GL_UNSIGNED_BYTE, buffPtr);
			m_nBytesPerPixel = 4;
			m_ePixelFormat = GUI_PF_RGBA_32;
			break;

		case GUI_PF_LUMINANCE_8:
			glTexImage2D(GL_TEXTURE_2D, 0, 1, buffWidth, buffHeight, 0, GL_LUMINANCE ,GL_UNSIGNED_BYTE, buffPtr);
			m_nBytesPerPixel = 4;
			m_ePixelFormat = GUI_PF_LUMINANCE_8;
			break;

		//case GUI_PF_LUMINANCE_ALPHA_16:
		//	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, buffWidth, buffHeight, 0, GL_LUMINANCE_ALPHA ,GL_UNSIGNED_BYTE, buffPtr);
		//	m_nBytesPerPixel = 2;
		//	m_ePixelFormat = GUI_PF_LUMINANCE_ALPHA_16;
		//	break;

		default:
			throw CGUIException("[CGUITexture_opengl::LoadFromMemory]: unsupported pixel format;");
		}

		m_nTextureWidth  = static_cast<uint16>(buffWidth);
		m_nTextureHeight = static_cast<uint16>(buffHeight);

		return 0;
	}
	//------------------------------------------------------------------------------
	void	CGUITexture_opengl::CopySubImage(uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat, uint8* pBuffer)
	{
		glBindTexture( GL_TEXTURE_2D, m_ogltexture );
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		switch(ePixelFormat)
		{
		case GUI_PF_RGBA_32:
			break;

		case GUI_PF_ARGB_32:
			{
				for( uint32 i=0; i<nHeight; ++i)
				{
					for( uint32 j=0; j<nWidth; ++j)
					{
						uint32 nStartIdx = i*nWidth * 4+j*4;
						std::swap(pBuffer[nStartIdx+0], pBuffer[nStartIdx+2]);
					}	
				}
			}
			break;

		//case GUI_PF_LUMINANCE_ALPHA_16:
		//	glTexSubImage2D(GL_TEXTURE_2D, 0,nX,nY,nWidth, nHeight,GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE,pBuffer);
		//	break;

		default:
			throw CGUIException("[CGUITexture_opengl::CopySubImage]: unsupported pixel format;");
		}
			
		glTexSubImage2D(GL_TEXTURE_2D, 0,nX,nY,nWidth, nHeight,GL_RGBA, GL_UNSIGNED_BYTE,pBuffer);

		if( glGetError( ) != GL_NO_ERROR)
		{
			throw CGUIException("[CGUITexture_opengl::CopySubImage]: failed to run function <glTexSubImage2D>, error code <%d>;",glGetError());
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}
	//------------------------------------------------------------------------------
	void	CGUITexture_opengl::SetOpenglTextureSize(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat)
	{
		glBindTexture(GL_TEXTURE_2D, m_ogltexture);

		// allocate temp buffer for texture
		uint8* buff = NULL;

		switch(ePixelFormat)
		{
		case GUI_PF_RGBA_32:
			buff = new uint8[nWidth * nHeight * 4];
			glTexImage2D(GL_TEXTURE_2D, 0, 4, nWidth, nHeight, 0, GL_RGBA ,GL_UNSIGNED_BYTE, buff);
			m_nBytesPerPixel = 4;
			m_ePixelFormat = GUI_PF_RGBA_32;
			break;

		//case GUI_PF_LUMINANCE_ALPHA_16:
		//	buff = new uint8[nWidth * nHeight * 2];
		//	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, nWidth, nHeight, 0, GL_LUMINANCE_ALPHA ,GL_UNSIGNED_BYTE, buff);
		//	m_nBytesPerPixel = 2;
		//	m_ePixelFormat = GUI_PF_LUMINANCE_ALPHA_16;
		//	break;

		default:
			throw CGUIException("[CGUITexture_opengl::LoadFromMemory]: unsupported pixel format;");
		}

		// delete buffer
		delete[] buff;

		m_nTextureWidth = nWidth;
		m_nTextureHeight = nHeight;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
