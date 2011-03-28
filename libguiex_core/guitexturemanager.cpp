/** 
* @file guitexturemanager.cpp
* @brief texture manager.
* @author ken
* @date 2007-06-02
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guitexturemanager.h>
#include <libguiex_core/guiimagemanager.h>
#include <libguiex_core/guitexture.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guicolor.h>
#include <algorithm>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUITextureManager * CGUITextureManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
	CGUITextureManager::CGUITextureManager()
		:m_pDefaultTexture(NULL)
	{
		GUI_ASSERT( !m_pSingleton, "[CGUITextureManager::CGUITextureManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	/**
	* @brief destructor;
	*/
	CGUITextureManager::~CGUITextureManager()
	{
		if( m_pDefaultTexture )
		{
			throw CGUIException("[CGUITextureManager::~CGUITextureManager]: default texture havn't been released!");
		}
		if( !m_aMapTexture.empty() )
		{
			throw CGUIException("[CGUITextureManager::~CGUITextureManager]: texture load from file havn't been released!");
		}
		if( !m_aSetTexture.empty() )
		{
			throw CGUIException("[CGUITextureManager::~CGUITextureManager]: texture load from memoryhavn't been released!");
		}

		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	CGUITextureManager* CGUITextureManager::Instance()
	{
		GUI_ASSERT( m_pSingleton, "not initialized" );
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	/**
	* @brief create a texture by image path;
	*/
	CGUITexture* CGUITextureManager::CreateTexture(const CGUIString& rImageName)
	{
		TMapTexture::iterator itorFind = m_aMapTexture.find(rImageName);
		if( itorFind == m_aMapTexture.end())
		{
			//create a image
			CGUITexture* pTexture = new CGUITexture;
			pTexture->SetTextureType(eTT_File);
			if( pTexture->CreateTextureByFile(rImageName) != 0 )
			{
				delete pTexture;
				throw CGUIException("[CGUITextureManager::CreateTexture]: failed to load image <%s>!", rImageName.c_str());
				return NULL;
			}
			m_aMapTexture.insert( std::make_pair( rImageName,  pTexture ));
			pTexture->RefRetain();
			return pTexture;
		}
		else
		{
			//return a exist texture
			itorFind->second->RefRetain();
			return itorFind->second;
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief create a default texture
	*/
	CGUITexture* CGUITextureManager::CreateDefaultTexture()
	{
		if( !m_pDefaultTexture)
		{
			m_pDefaultTexture = new CGUITexture;
			m_pDefaultTexture->SetTextureType(eTT_Default);
			GUIARGB argb = 0xFFFFFFFF;
			if( 0 != m_pDefaultTexture->CreateTextureByMemory(&argb,1,1,GUI_PF_RGBA_32))
			{
				throw CGUIException("[CGUITextureManager::CreateDefaultTexture]: failed to load texture from memory!");
				return NULL;
			}
		}

		m_pDefaultTexture->RefRetain();
		return m_pDefaultTexture;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief create a texture by size
	*/
	CGUITexture* CGUITextureManager::CreateTexture(
		uint32 nWidth, 
		uint32 nHeight, 
		EGuiPixelFormat ePixelFormat)
	{
		CGUITexture* pTexture = new CGUITexture;
		pTexture->SetTextureType(eTT_Memory);

		if( 0 != pTexture->CreateTextureBySize(nWidth,nHeight,ePixelFormat))
		{
			throw CGUIException("[CGUITextureManager::CreateTexture]: failed to load texture by given size!");
			return NULL;
		}
		pTexture->RefRetain();
		m_aSetTexture.insert(pTexture);
		return pTexture;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief create a texture from given memory
	*/
	CGUITexture* CGUITextureManager::CreateTexture(
		const void* buffPtr, 
		int32 buffWidth, 
		int32 buffHeight, 
		EGuiPixelFormat ePixelFormat)
	{
		CGUITexture* pTexture = new CGUITexture;
		pTexture->SetTextureType(eTT_Memory);

		if( 0 != pTexture->CreateTextureByMemory(buffPtr,buffWidth,buffHeight,ePixelFormat))
		{
			throw CGUIException("[CGUITextureManager::CreateTexture]: failed to load texture from memory!");
			return NULL;
		}
		pTexture->RefRetain();
		m_aSetTexture.insert(pTexture);
		return pTexture;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief destroy a texture
	*/
	int32 CGUITextureManager::DestroyTexture(CGUITexture* pTexture)
	{
		switch( pTexture->GetTextureType())
		{
		case eTT_Default:
			{
				//default texture
				if( pTexture == m_pDefaultTexture )
				{
					pTexture->RefRelease();
					if( pTexture->GetRefCount() == 0)
					{
						//real delete it now
						delete pTexture;
						m_pDefaultTexture = NULL;
					}
					return 0;
				}
			}
			break;

		case eTT_File:
			{
				//load from file
				for( TMapTexture::iterator itor = m_aMapTexture.begin();
					itor != m_aMapTexture.end();
					itor++)
				{
					if( itor->second == pTexture)
					{
						pTexture->RefRelease();
						if( pTexture->GetRefCount() == 0)
						{
							//real delete it now
							delete pTexture;
							m_aMapTexture.erase(itor);
						}
						return 0;
					}
				}
			}
			break;

		case eTT_Memory:
			{
				//load from memory
				TSetTexture::iterator itorSet = m_aSetTexture.find(pTexture);
				if( itorSet != m_aSetTexture.end())
				{
					pTexture->RefRelease();
					if( pTexture->GetRefCount() == 0)
					{
						//real delete it now
						delete pTexture;
						m_aSetTexture.erase(itorSet);
					}
					return 0;
				}
			}
			break;

		default:
			break;
		}


		throw CGUIException("[CGUITextureManager::DestroyTexture]: the image has been destroyed");
		return -1;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief (re)create all texture implements,
	* usually the texture implement should be created manually when a new
	* render is loaded.
	* @return 0 for success
	*/
	int32 CGUITextureManager::CreateAllTextureImplement()
	{
		//default texture
		if( m_pDefaultTexture )
		{
			uint32 ret = m_pDefaultTexture->CreateTextureImplement();
			if( ret )
			{
				return ret;
			}
		}

		//load from file
		for( TMapTexture::iterator itor = m_aMapTexture.begin();
			itor != m_aMapTexture.end();
			itor++)
		{
			uint32 ret = itor->second->CreateTextureImplement();
			if( ret )
			{
				return ret;
			}
		}

		//load from memory
		for( TSetTexture::iterator itor = m_aSetTexture.begin();
			itor != m_aSetTexture.end();
			itor++)
		{
			uint32 ret = (*itor)->CreateTextureImplement();
			if( ret )
			{
				return ret;
			}
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief destroy all texture implements
	* usually the texture implement should be destroyed manually when a 
	* render is unloaded and readily to load a new render
	*/
	void CGUITextureManager::DestroyAllTextureImplement()
	{
		//default texture
		if( m_pDefaultTexture )
		{
			m_pDefaultTexture->DestoryTextureImplement();
		}

		//load from file
		for( TMapTexture::iterator itor = m_aMapTexture.begin();
			itor != m_aMapTexture.end();
			itor++)
		{
			itor->second->CreateTextureImplement();
		}

		//load from memory
		for( TSetTexture::iterator itor = m_aSetTexture.begin();
			itor != m_aSetTexture.end();
			itor++)
		{
			(*itor)->CreateTextureImplement();
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief dump all texture to memory
	* @return 0 for success
	*/
	int32 CGUITextureManager::DumpAllTextureImplement()
	{
		//default texture
		if( m_pDefaultTexture )
		{
			uint32 ret = m_pDefaultTexture->SaveToMemory();
			if( ret )
			{
				return ret;
			}
		}

		//load from file
		for( TMapTexture::iterator itor = m_aMapTexture.begin();
			itor != m_aMapTexture.end();
			itor++)
		{
			uint32 ret = itor->second->SaveToMemory();
			if( ret )
			{
				return ret;
			}
		}

		//load from memory
		for( TSetTexture::iterator itor = m_aSetTexture.begin();
			itor != m_aSetTexture.end();
			itor++)
		{
			uint32 ret = (*itor)->SaveToMemory();
			if( ret )
			{
				return ret;
			}
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief load all texture from memory
	* @return 0 for success
	*/
	int32 CGUITextureManager::LoadAllTextureImplement()
	{
		//default texture
		if( m_pDefaultTexture )
		{
			uint32 ret = m_pDefaultTexture->LoadFromMemory();
			if( ret )
			{
				return ret;
			}
			m_pDefaultTexture->FreeMemory();
		}

		//load from file
		for( TMapTexture::iterator itor = m_aMapTexture.begin();
			itor != m_aMapTexture.end();
			itor++)
		{
			uint32 ret = itor->second->LoadFromMemory();
			if( ret )
			{
				return ret;
			}
			itor->second->FreeMemory();
		}

		//load from memory
		for( TSetTexture::iterator itor = m_aSetTexture.begin();
			itor != m_aSetTexture.end();
			itor++)
		{
			uint32 ret = (*itor)->LoadFromMemory();
			if( ret )
			{
				return ret;
			}
			(*itor)->FreeMemory();
		}
		return 0;
	}
	//------------------------------------------------------------------------------

}//namespace guiex
