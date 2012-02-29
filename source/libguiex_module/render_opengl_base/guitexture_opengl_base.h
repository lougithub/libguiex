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
		virtual ~CGUITexture_opengl_base();

		virtual	uint16 GetWidth(void) const;
		virtual	uint16 GetHeight(void) const;

#if !GUI_TEXTURE_NPOT_SUPPORT
		virtual	uint16 GetPOTWidth(void) const;
		virtual	uint16 GetPOTHeight(void) const;
#endif 

		virtual uint32 GetBufferSize() const;
		virtual uint32 GetBuffer(uint8* pBuffer, uint32 nBufferSize, EGuiPixelFormat& rPixelFormat);


		virtual int32 LoadFromFile(const CGUIString& filename );
		virtual int32 LoadFromMemory(const void* buffPtr, uint32 buffWidth, uint32 buffHeight,EGuiPixelFormat ePixelFormat );

		virtual void CopySubImage(uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat, uint8* pBuffer);
		
		uint32 GetOGLTexid(void) const {return m_ogltexture;}

	protected:
		void SetOpenglTextureSize(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat);

	protected:
		friend class IGUIRender_opengl_base;
		CGUITexture_opengl_base(IGUIInterfaceRender* pRender);

	protected:
		uint32 m_ogltexture; //!< The texture.
		uint16 m_nTextureWidth;
		uint16 m_nTextureHeight;

#if !GUI_TEXTURE_NPOT_SUPPORT
		uint16 m_nPOTTextureWidth;
		uint16 m_nPOTTextureHeight;
#endif

		uint8 m_nBytesPerPixel;	//!< how many bytes does one pixel contain
	};
}//namespace guiex

#endif //__GUI_TEXTURE_OPENGL_BASE_20060706_H__
