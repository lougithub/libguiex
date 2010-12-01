/** 
* @file guitexture.h
* @brief wrapper of texture
* @author ken
* @date 2007-06-02
*/

#ifndef __GUI_TEXTURE_20070602_H__
#define __GUI_TEXTURE_20070602_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guisize.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUITextureImp;
}





//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUITexture
	* @brief abstract class of texture
	*/
	class GUIEXPORT CGUITexture
	{
	public:
		/**
		* @brief Returns the current pixel width of the texture
		*/
		uint16	GetWidth(void) const;

		/**
		* @brief Returns the current pixel height of the texture
		*/
		uint16	GetHeight(void) const;

		/**
		* @brief convert size to texture's standard size
		*/
		static uint16	ConvertToTextureSize(uint16);

		/**
		* @brief get "real" texture
		*/
		CGUITextureImp*	GetTextureImplement()
		{
			return m_pTextureImp;
		}

		/**
		* @brief copy a sub_image to texture
		*/
		void		CopySubImage(uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat, uint8* pBuffer);


		/// Increments the Reference count.
		void	Reference() 
		{ 
			++m_nRefCount;
		}

		// Decrements the Referent count.
		void	Unreference();

	protected:
		friend class CGUITextureManager;

		/**
		* @brief constructor
		*/
		CGUITexture( );

		/**
		* @brief destructor
		*/
		virtual ~CGUITexture();

		/**
		* @brief Loads the specified image file into the texture.  The texture is resized 
		* as required to hold the image.
		* @exception throw CGUIException if failed.
		* @return -1 for failed
		*/
		int32	CreateTextureByFile(const CGUIString& filename );


		/**
		* @brief Loads (copies) an image from memory into the texture.  The texture is resized as 
		required to hold the image.
		* @param ePixelFormat pixel format.
		* @exception throw CGUIException if failed.
		* @return -1 for failed
		*/
		int32	CreateTextureByMemory(
			const void* buffPtr, 
			int32 buffWidth, 
			int32 buffHeight, 
			EGuiPixelFormat ePixelFormat );

		/**
		* @brief create texture by given size and pixelformat
		* @return -1 for failed
		*/
		int32	CreateTextureBySize(uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat);

		/**
		* @brief dump texture to memory and release texture implement,
		* usually it will happen when changing render.
		* @return 0 for success, others for failed
		*/
		int32	SaveToMemory();

		/**
		* @brief create a new texture implement and load texture from memory,
		* usually it will happen when changing render.
		* @return 0 for success, others for failed
		*/
		int32	LoadFromMemory();

		/** 
		 * @brief free memory which contains the texture data
		 */
		void	FreeMemory();


		// Returns the current reference count.
		uint16	RefCount() const 
		{ 
			return m_nRefCount; 
		}

		/// set texture type, used by texture manager
		void		SetTextureType(uint8 nType)
		{
			m_nType = nType;
		}

		/// get texture type, used by texture manager
		uint8		GetTextureType( ) const
		{
			return m_nType;
		}

	protected:
		friend class CGUITextureImp;

		//!< notify when texture imp is deleted
		void	NotifyDeletedFromImp();


	private:
		/** 
		 * @brief create a texture implement from current render
		 */
		uint32		CreateTextureImplement();

		/** 
		* @brief destroy a texture implement from current render
		*/
		void		DestoryTextureImplement();

	protected:
		CGUITextureImp*		m_pTextureImp;		//implement of texture
		uint8*				m_pDumpMemory;		//memory which contains the texture data.
		EGuiPixelFormat		m_eDumpPixelFormat;	//format of texture data in the dump memory
		uint16				m_aDumpTextureWidth;	//the size of dump texture
		uint16				m_aDumpTextureHeight;	//the size of dump texture

		uint8				m_nType;			//texture type.

		uint16				m_nRefCount;		//reference count

	};

}//namespace guiex

#endif //__GUI_TEXTURE_20070602_H__
