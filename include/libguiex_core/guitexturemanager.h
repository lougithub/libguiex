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
#include "guisingleton.h"
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
		/**
		* @brief constructor
		*/
		CGUITextureManager();

		/**
		* @brief destructor;
		*/
		~CGUITextureManager();

		/**
		* @brief create a texture by image path;
		*/
		CGUITexture*	CreateTexture(const CGUIString& rImageName);


		/**
		* @brief create a texture by size
		*/
		CGUITexture*	CreateTexture(uint32 nWidth, uint32 nHeight,EGuiPixelFormat ePixelFormat);

		/**
		* @brief create a texture from given memory
		*/
		CGUITexture*	CreateTexture(
			const void* buffPtr, 
			int32 buffWidth, 
			int32 buffHeight, 
			EGuiPixelFormat ePixelFormat);

		/**
		* @brief create a default texture
		*/
		CGUITexture*	CreateDefaultTexture();

		/**
		* @brief destroy a texture
		*/
		int32			DestroyTexture(CGUITexture* pTexture);




		/**
		* @brief (re)create all texture implements,
		* usually the texture implement should be created manually when a new
		* render is loaded.
		* @return 0 for success
		*/
		int32			CreateAllTextureImplement();

		/**
		* @brief destroy all texture implements
		* usually the texture implement should be destroyed manually when a 
		* render is unloaded and readily to load a new render
		*/
		void			DestroyAllTextureImplement();

		/**
		* @brief dump all texture to memory
		* @return 0 for success
		*/
		int32			DumpAllTextureImplement();

		/**
		* @brief load all texture from memory
		* @return 0 for success
		*/
		int32			LoadAllTextureImplement();

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

		CGUITexture*	m_pDefaultTexture;	///default texture, which has a white color

		//declare for singleton
		GUI_SINGLETON_DECLARE_EX(CGUITextureManager);
	};

}//namespace guiex

#endif		//__GUI_TEXTUREMANAGER_20070602_H__

