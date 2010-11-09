/** 
* @file guitexture_opengles.h
* @brief use opengl to render gui
* @author ken
* @date 2010-11-09
*/

#ifndef __GUI_TEXTURE_OPENGLES_20101109_H__
#define __GUI_TEXTURE_OPENGLES_20101109_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guitextureimp.h>
#include <libguiex_core/guisize.h>

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>



//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class IGUIRender_opengles;
}




//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	class GUIEXPORT CGUITexture_opengles : public CGUITextureImp
	{
	public:
		/**
		* @brief destructor
		*/
		virtual ~CGUITexture_opengles();

		/**
		* @brief Returns the current pixel width of the texture
		*/
		virtual	uint16 GetWidth(void) const;

		/**
		* @brief Returns the current pixel height of the texture
		*/
		virtual	uint16 GetHeight(void) const;

		/**
		* @brief Returns the current buffer size of this texture
		*/
		virtual uint32 GetBufferSize() const;

		/**
		* @brief get the texture's buffer 
		* @return 0 for success
		*/
		virtual uint32 GetBuffer(uint8* pBuffer, uint32 nBufferSize, EGuiPixelFormat& rPixelFormat);

		/**
		* @brief Loads the specified image file into the texture.  The texture is resized 
		* as required to hold the image.
		* @exception throw CGUIException if failed.
		* @return -1 for failed
		*/
		virtual int32 LoadFromFile(const CGUIString& filename );


		/**
		* @brief Loads (copies) an image from memory into the texture.  The texture is resized as 
		required to hold the image.
		* @exception throw CGUIException if failed.
		* @return -1 for failed
		*/
		virtual int32 LoadFromMemory(const void* buffPtr, int32 buffWidth, int32 buffHeight,EGuiPixelFormat ePixelFormat );

		/**
		* @brief copy a sub_image to texture
		*/
		virtual void CopySubImage(uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat, uint8* pBuffer);


	protected:
		void SetOpenglTextureSize(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat);
		GLuint GetOGLTexid(void) const {return m_ogltexture;}

	protected:
		friend class IGUIRender_opengles;
		/**
		* @brief constructor
		*/
		CGUITexture_opengles(IGUIInterfaceRender* pRender);

	protected:
		GLuint 			m_ogltexture;		//!< The texture.
		uint16			m_nTextureWidth;
		uint16			m_nTextureHeight;

		uint8					m_nBytesPerPixel;	//!< how many bytes does one pixel contain
		EGuiPixelFormat			m_ePixelFormat;		//!< pixel format

	};



}//namespace guiex

#endif //__GUI_TEXTURE_OPENGL_20101109_H__
