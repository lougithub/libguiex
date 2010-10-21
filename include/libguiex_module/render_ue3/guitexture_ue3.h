/** 
* @file guitexture_ue3.h
* @brief use unreal 3 to render gui
* @author ken
* @date 2009-09-21
*/


#ifndef __GUI_TEXTURE_UE3_20090921_H__
#define __GUI_TEXTURE_UE3_20090921_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guitextureimp.h>
#include <libguiex_core\guisize.h>

#include <d3d9.h>


namespace guiex
{

	//============================================================================//
	// class
	//============================================================================// 
	class GUIEXPORT		CGUITexture_UE3 : public CGUITextureImp
	{
	public:
		/**
		* @brief destructor
		*/
		virtual ~CGUITexture_UE3();

		/**
		* @brief Returns the current pixel width of the texture
		*/
		virtual	uint16	GetWidth(void) const;

		/**
		* @brief Returns the current pixel height of the texture
		*/
		virtual	uint16	GetHeight(void) const;

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
		virtual int32	LoadFromFile(const CGUIString& filename );


		/**
		* @brief Loads (copies) an image from memory into the texture.  The texture is resized as 
		required to hold the image.
		* @exception throw CGUIException if failed.
		* @return -1 for failed
		*/
		virtual int32	LoadFromMemory(const void* buffPtr, int32 buffWidth, int32 buffHeight,EGuiPixelFormat ePixelFormat );


		/**
		* @brief copy a sub_image to texture
		*/
		virtual void		CopySubImage(uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat, uint8* pBuffer);

	protected:
		class FUE3Texture*		GetUE3Texture(void) const 
		{
			return m_pUE3Texture;
		}

		void				SetUE3TextureSize(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat);
		

		void				FreeUE3Texture();

	protected:
		friend class IGUIRender_UE3;
		/**
		* @brief constructor
		*/
		CGUITexture_UE3(IGUIInterfaceRender* pRender);

	protected:
		class FUE3Texture*	m_pUE3Texture;
	};



}//namespace guiex

#endif //__GUI_TEXTURE_UE3_20090921_H__
