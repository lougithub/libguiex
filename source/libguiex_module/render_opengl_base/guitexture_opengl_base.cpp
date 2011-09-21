/** 
* @file guitexture_opengl_base.cpp
* @brief use opengl to render gui
* @author ken
* @date 2006-07-06
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/render_opengl_base/guitexture_opengl_base.h>
#include <libguiex_module/render_opengl_base/guirender_opengl_base.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guidatachunk.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacefilesys.h>
#include <libguiex_core/guiinterfaceimageloader.h>
#include <libguiex_core/guilogmsgmanager.h>

#if defined(GUIEX_TARGET_WIN32)
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#elif defined(GUIEX_TARGET_IOS)
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#elif defined(GUIEX_TARGET_MACOS)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#elif defined(GUIEX_TARGET_ANDROID)
#include <GLES/gl.h>
#include <GLES/glext.h>
#else
#error "unknown target"	
#endif

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUITexture_opengl_base::CGUITexture_opengl_base(IGUIInterfaceRender* pRender)
		:CGUITextureImp(pRender)
		,m_nTextureWidth(0)
		,m_nTextureHeight(0)
		,m_nBytesPerPixel(0)
	{
		TRY_THROW_OPENGL_ERROR("CGUITexture_opengl_base::CGUITexture_opengl_base: begin");

		// generate a OGL texture that we will use.
		glGenTextures(1, &m_ogltexture);

		// set some parameters for this texture.
		glBindTexture(GL_TEXTURE_2D, m_ogltexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// GL_CLAMP_TO_EDGE GL_CLAMP
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// GL_CLAMP_TO_EDGE GL_CLAMP
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		TRY_THROW_OPENGL_ERROR("CGUITexture_opengl_base::CGUITexture_opengl_base: end");
	}
	//------------------------------------------------------------------------------
	CGUITexture_opengl_base::~CGUITexture_opengl_base()
	{
		TRY_THROW_OPENGL_ERROR("CGUITexture_opengl_base::~CGUITexture_opengl_base:begin");

		// otherwise delete OGL texture associated with this object.
		glDeleteTextures(1, &m_ogltexture);
		m_ogltexture = 0;

		TRY_THROW_OPENGL_ERROR("CGUITexture_opengl_base::~CGUITexture_opengl_base:end");
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Returns the current pixel width of the texture
	*/
	uint16 CGUITexture_opengl_base::GetWidth(void) const
	{
		return m_nTextureWidth;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Returns the current pixel height of the texture
	*/
	uint16 CGUITexture_opengl_base::GetHeight(void) const
	{
		return m_nTextureHeight;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Returns the current buffer size of this texture
	*/
	uint32 CGUITexture_opengl_base::GetBufferSize() const
	{
		return static_cast<uint32>(m_nTextureWidth * m_nTextureHeight * m_nBytesPerPixel);
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get the texture's buffer 
	* @return 0 for success
	*/
	uint32 CGUITexture_opengl_base::GetBuffer(uint8* pBuffer, uint32 nBufferSize, EGuiPixelFormat& rPixelFormat)
	{
		GUI_ASSERT(0, "TODO: implement it later" );
		return -1;
		/*
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
		GUI_THROW( "[CGUITexture_opengl_base::LoadFromMemory]: unsupported pixel format;");
		}

		glBindTexture(GL_TEXTURE_2D, m_ogltexture);
		glGetTexImage( GL_TEXTURE_2D,0, eFormat,GL_UNSIGNED_BYTE,pBuffer);

		rPixelFormat = m_ePixelFormat;

		return 0;
		*/
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Loads the specified image file into the texture.  The texture is resized 
	* as required to hold the image.
	* @exception CGUIException::ThrowException if failed.
	* @return -1 for failed
	*/
	int32 CGUITexture_opengl_base::LoadFromFile(const CGUIString& filename )
	{
		IGUIInterfaceImageLoader* pImageLoader = static_cast<IGUIInterfaceImageLoader*>(CGUIInterfaceManager::Instance()->GetInterface("IGUIImageLoader"));
		if( !pImageLoader )
		{
			GUI_THROW( "[CGUITexture_opengl_base::LoadFromFile]: - failed to get image loader!");
			return -1;
		}

		CGUIImageData* pImageData = pImageLoader->LoadFromFile( filename );
		if( !pImageData )
		{
			GUI_THROW( GUI_FORMAT("[CGUITexture_opengl_base::LoadFromFile]: - failed to Load image file <%s>!", filename.c_str()));
			return -1;
		}

		int32 ret = LoadFromMemory( pImageData->GetData(), pImageData->GetWidth(), pImageData->GetHeight(), pImageData->GetPixelFormat());
		delete pImageData;

		TRY_THROW_OPENGL_ERROR("CGUITexture_opengl_base::LoadFromFile");
		return ret;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief Loads (copies) an image from memory into the texture.  The texture is resized as 
	required to hold the image.
	* @exception CGUIException::ThrowException if failed.
	* @return -1 for failed
	*/
	int32 CGUITexture_opengl_base::LoadFromMemory(const void* buffPtr, int32 buffWidth, int32 buffHeight, EGuiPixelFormat ePixelFormat/* = PF_RGBA_32*/)
	{
		glBindTexture(GL_TEXTURE_2D, m_ogltexture);

		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		m_nBytesPerPixel = CGUIImageData::GetBytePerPixel(ePixelFormat);
		m_ePixelFormat = ePixelFormat;

		switch(ePixelFormat)
		{
		case GUI_PF_RGBA_32:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, buffWidth, buffHeight, 0, GL_RGBA ,GL_UNSIGNED_BYTE, buffPtr);
			break;

		case GUI_PF_RGB_24:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, buffWidth, buffHeight, 0, GL_RGB ,GL_UNSIGNED_BYTE, buffPtr);
			break;

		case GUI_PF_ALPHA_8:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, buffWidth, buffHeight, 0, GL_ALPHA ,GL_UNSIGNED_BYTE, buffPtr);
			break;

			//case GUI_PF_LUMINANCE_ALPHA_16:
			//	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, buffWidth, buffHeight, 0, GL_LUMINANCE_ALPHA ,GL_UNSIGNED_BYTE, buffPtr);
			//	m_nBytesPerPixel = 2;
			//	m_ePixelFormat = GUI_PF_LUMINANCE_ALPHA_16;
			//	break;

		default:
			GUI_THROW( "[CGUITexture_opengl_base::LoadFromMemory]: unsupported pixel format;");
		}

		//glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		m_nTextureWidth  = static_cast<uint16>(buffWidth);
		m_nTextureHeight = static_cast<uint16>(buffHeight);

		TRY_THROW_OPENGL_ERROR("CGUITexture_opengl_base::LoadFromMemory");
		return 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief copy a sub_image to texture
	*/
	void CGUITexture_opengl_base::CopySubImage(uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat, uint8* pBuffer)
	{
		GUI_ASSERT( m_ePixelFormat == ePixelFormat, "invalid pixel format" );

		glBindTexture( GL_TEXTURE_2D, m_ogltexture );

		switch(ePixelFormat)
		{
		case GUI_PF_RGBA_32:
			glTexSubImage2D(GL_TEXTURE_2D, 0,nX,nY,nWidth, nHeight,GL_RGBA, GL_UNSIGNED_BYTE,pBuffer);
			break;

		case GUI_PF_ARGB_32:
			for( uint32 i=0; i<nHeight; ++i)
			{
				for( uint32 j=0; j<nWidth; ++j)
				{
					uint32 nStartIdx = i*nWidth * 4+j*4;
					std::swap(pBuffer[nStartIdx+0], pBuffer[nStartIdx+2]);
				}	
			}
			glTexSubImage2D(GL_TEXTURE_2D, 0,nX,nY,nWidth, nHeight,GL_RGBA, GL_UNSIGNED_BYTE,pBuffer);
			break;

		case GUI_PF_LUMINANCE_ALPHA_16:
			glTexSubImage2D(GL_TEXTURE_2D, 0,nX,nY,nWidth, nHeight,GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE,pBuffer);
			break;

		default:
			GUI_THROW( "[CGUITexture_opengl_base::CopySubImage]: unsupported pixel format;");
		}

		TRY_THROW_OPENGL_ERROR("CGUITexture_opengl_base::CopySubImage");
	}
	//------------------------------------------------------------------------------
	void CGUITexture_opengl_base::SetOpenglTextureSize(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat)
	{
		glBindTexture(GL_TEXTURE_2D, m_ogltexture);

		m_ePixelFormat = ePixelFormat;
		switch(m_ePixelFormat)
		{
		case GUI_PF_RGBA_32:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight, 0, GL_RGBA ,GL_UNSIGNED_BYTE, NULL);
			m_nBytesPerPixel = 4;
			break;

		case GUI_PF_LUMINANCE_ALPHA_16:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, nWidth, nHeight, 0, GL_LUMINANCE_ALPHA ,GL_UNSIGNED_BYTE, NULL);
			m_nBytesPerPixel = 2;
			break;

		default:
			GUI_THROW( "[CGUITexture_opengl_base::LoadFromMemory]: unsupported pixel format;");
		}

		m_nTextureWidth = nWidth;
		m_nTextureHeight = nHeight;

		TRY_THROW_OPENGL_ERROR("CGUITexture_opengl_base::SetOpenglTextureSize");
	}
	//------------------------------------------------------------------------------
}//namespace guiex
