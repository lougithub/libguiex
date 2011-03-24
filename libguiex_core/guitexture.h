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
		uint16 GetWidth(void) const;
		uint16 GetHeight(void) const;

		static uint16 ConvertToTextureSize(uint16);

		/**
		* @brief get "real" texture
		*/
		const CGUITextureImp* GetTextureImplement() const 
		{
			return m_pTextureImp;
		}

		void CopySubImage(uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat, uint8* pBuffer);

		/// Increments the Reference count.
		void Reference() 
		{ 
			++m_nRefCount;
		}

		// Decrements the Referent count.
		void Unreference();

		void SetBottomUp( bool bIsBottomUp );
		bool IsBottomUp( ) const;

		real UVConvertTopleft2Engine_v( real v ) const;

	protected:
		friend class CGUITextureManager;

		CGUITexture( );
		virtual ~CGUITexture();

		int32 CreateTextureByFile( const CGUIString& filename );
		int32 CreateTextureByMemory( const void* buffPtr, int32 buffWidth, int32 buffHeight, EGuiPixelFormat ePixelFormat );
		int32 CreateTextureBySize( uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat );

		int32 SaveToMemory();
		int32 LoadFromMemory();
		void FreeMemory();

		// Returns the current reference count.
		uint16 RefCount() const 
		{ 
			return m_nRefCount; 
		}

		/// set texture type, used by texture manager
		void SetTextureType(uint8 nType)
		{
			m_nType = nType;
		}

		/// get texture type, used by texture manager
		uint8 GetTextureType( ) const
		{
			return m_nType;
		}

	protected:
		friend class CGUITextureImp;
		void NotifyDeletedFromImp();

	private:
		uint32 CreateTextureImplement();
		void DestoryTextureImplement();

	protected:
		CGUITextureImp* m_pTextureImp; //implement of texture
		uint8* m_pDumpMemory; //memory which contains the texture data.
		EGuiPixelFormat m_eDumpPixelFormat;	//format of texture data in the dump memory
		uint16 m_aDumpTextureWidth; //the size of dump texture
		uint16 m_aDumpTextureHeight; //the size of dump texture

		uint8 m_nType; //texture type.

		uint16 m_nRefCount; //reference count
		bool m_bIsBottomUp;
	};

}//namespace guiex

#endif //__GUI_TEXTURE_20070602_H__
