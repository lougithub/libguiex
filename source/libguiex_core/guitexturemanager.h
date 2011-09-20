/** 
* @file guitexturemanager.h
* @brief texture manager.
* @author ken
* @date 2007-06-02
*/

#ifndef __GUI_TEXTUREMANAGER_20070602_H__
#define __GUI_TEXTUREMANAGER_20070602_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include <map>
#include <set>


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUITexture;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUITextureManager
	* @brief manager of texture
	* 
	*/
	class GUIEXPORT CGUITextureManager
	{
	public:
		CGUITextureManager();
		~CGUITextureManager();

		static CGUITextureManager* Instance(); 

		CGUITexture* CreateTexture(const CGUIString& rImageName);
		CGUITexture* CreateTexture( uint32 nWidth, uint32 nHeight, EGuiPixelFormat ePixelFormat );
		CGUITexture* CreateTexture( const void* buffPtr, int32 buffWidth, int32 buffHeight, EGuiPixelFormat ePixelFormat );
		CGUITexture* CreateDefaultTexture();
		int32 DestroyTexture(CGUITexture* pTexture);

		int32 CreateAllTextureImplement();
		void DestroyAllTextureImplement();

		int32 DumpAllTextureImplement();
		int32 LoadAllTextureImplement();

	protected:
		enum ETextureType
		{
			eTT_Default = 0,
			eTT_File,
			eTT_Memory
		};

		typedef std::map<CGUIString, CGUITexture*> TMapTexture;
		TMapTexture	m_aMapTexture;		/// contain all texture which load from file.

		typedef std::set<CGUITexture*> TSetTexture;
		TSetTexture	m_aSetTexture;		/// contain all texture which load from memory.

		CGUITexture* m_pDefaultTexture;	///default texture, which has a white color

	private:
		static CGUITextureManager* m_pSingleton;
	};

}//namespace guiex

#endif		//__GUI_TEXTUREMANAGER_20070602_H__

