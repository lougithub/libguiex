/** 
* @file guitexture_opengl_base.h
* @brief use opengl to render gui
* @author ken
* @date 2006-07-06
*/

#ifndef __GUI_TEXTURE_OPENGL_BASE_20060706_H__
#define __GUI_TEXTURE_OPENGL_BASE_20060706_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guitextureimp.h>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class IGUIRender_opengl_base;
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	class GUIEXPORT CGUITexture_opengl_base : public CGUITextureImp
	{
	public:
		/**
		* @brief destructor
		*/
		virtual ~CGUITexture_opengl_base();

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
		uint32 GetOGLTexid(void) const {return m_ogltexture;}

	protected:
		friend class IGUIRender_opengl_base;
		/**
		* @brief constructor
		*/
		CGUITexture_opengl_base(IGUIInterfaceRender* pRender);

	protected:
		uint32 m_ogltexture; //!< The texture.
		uint16 m_nTextureWidth;
		uint16 m_nTextureHeight;

		uint8 m_nBytesPerPixel;	//!< how many bytes does one pixel contain
	};



}//namespace guiex

#endif //__GUI_TEXTURE_OPENGL_BASE_20060706_H__
