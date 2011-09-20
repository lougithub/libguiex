/** 
* @file guitextureimp.h
* @brief abstract class for texture
* @author ken
* @date 2006-07-05
*/

#ifndef __GUI_TEXTURE_IMP_20060705_H__
#define __GUI_TEXTURE_IMP_20060705_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class IGUIInterfaceRender;
	class CGUISize;
}


//============================================================================//
// enum
//============================================================================// 
namespace guiex
{
	class CGUITexture;
}//namespace guiex


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUITextureImp
	* @brief abstract class of texture
	*/
	class GUIEXPORT CGUITextureImp
	{
	public:
		/**
		* @brief destructor
		*/
		virtual ~CGUITextureImp();

		/**
		* @brief Returns the current pixel width of the texture
		*/
		virtual	uint16	GetWidth(void) const = 0;

		/**
		* @brief Returns the current pixel height of the texture
		*/
		virtual	uint16	GetHeight(void) const = 0;

		/**
		* @brief Returns the current buffer size of this texture
		*/
		virtual uint32 GetBufferSize() const = 0;

		/**
		* @brief get the texture's buffer 
		* @return 0 for success
		*/
		virtual uint32 GetBuffer(uint8* pBuffer, uint32 nBufferSize, EGuiPixelFormat& rPixelFormat) = 0;

		/**
		* @brief destroy this texture
		*/
		void Destroy();

		/**
		* @brief copy a sub_image to texture
		*/
		virtual void CopySubImage(uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat, uint8* pBuffer) = 0;

		/**
		* @brief Loads the specified image file into the texture.  The texture is resized 
		* as required to hold the image.
		* @exception CGUIException::ThrowException if failed.
		* @return -1 for failed
		*/
		virtual int32 LoadFromFile(const CGUIString& filename ) = 0;


		/**
		* @brief Loads (copies) an image from memory into the texture.  The texture is resized as 
		required to hold the image.
		* @param ePixelFormat pixel format.
		* @exception CGUIException::ThrowException if failed.
		* @return -1 for failed
		*/
		virtual int32 LoadFromMemory(
			const void* buffPtr, 
			int32 buffWidth, 
			int32 buffHeight, 
			EGuiPixelFormat ePixelFormat ) = 0;

		//!< save pointer of texture
		void SetTexture( CGUITexture* pTexture);

		//!< get pointer of texture
		CGUITexture* GetTexture(  ) const;

	protected:
		/**
		* @brief constructor
		*/
		CGUITextureImp(IGUIInterfaceRender* pRender);

	protected:
		IGUIInterfaceRender* m_pRender;	//!< the render which create this texture
		CGUITexture* m_pTexture; //!< pointer of texture class
		EGuiPixelFormat m_ePixelFormat;
	};

}//namespace guiex

#endif //__GUI_TEXTURE_IMP_20060705_H__
