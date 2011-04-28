/** 
* @file guiconfigfileloader.h
* @brief utility function for load config file.
* @author ken
* @date 2010-11-25
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiconfigfileloader.h"
#include "guiexception.h"
#include "guiinterfacemanager.h"
#include "guiinterfaceconfigfile.h"
#include "guiinterfacescript.h"
#include "guipropertymanager.h"
#include "guiwidgetmanager.h"
#include "guisystem.h"
#include "guifontmanager.h"
#include "guiimagemanager.h"
#include "guiasmanager.h"
#include "guianimationmanager.h"
#include "guisoundmanager.h"
#include "guimusicmanager.h"
#include "guiparticle2dmanager.h"
#include "guitiledmapmanager.h"
#include "guiscenemanager.h"


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	static CGUIWidget* DoLoadConfig_Widget( const CGUIProperty* pPropertySet, const CGUIString& rSceneName, const CGUIString& rFileName );
	static int32 DoLoadConfig_Set( const CGUIProperty* pPropertySet, const CGUIString& rSceneName );

	//------------------------------------------------------------------------------
	CGUIWidget* DoLoadConfig_Widget( const CGUIProperty* pPropertySet, const CGUIString& rSceneName, const CGUIString& rFileName )
	{
		/// create widget
		const CGUIString& strWidgetType = pPropertySet->GetValue( );
		const CGUIString& strWidgetName = pPropertySet->GetName( );
		CGUIWidget* pWidget = CGUIWidgetManager::Instance()->CreateWidget(strWidgetType, strWidgetName, rSceneName);
		if( !pWidget )
		{
			CGUIException::ThrowException(
				"[DoLoadConfig_Widget]: failed to create widget <%s:%s>!",
				strWidgetType.c_str(),
				strWidgetName.c_str());
			return NULL;
		}
		/// set property to widget
		pWidget->LoadFromProperty( *pPropertySet );
		pWidget->Create();

		if( GSystem->IsEditorMode() )
		{
			pWidget->SetProperty( *pPropertySet );
		}

		return pWidget;
	}
	//------------------------------------------------------------------------------
	int32 DoLoadConfig_Set( const CGUIProperty* pPropertySet, const CGUIString& rSceneName )
	{
		const CGUIString& strSetName = pPropertySet->GetName( );
		//register set
		CGUIPropertyManager::Instance()->RegisterSet( strSetName, *pPropertySet );

		return 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief read config file and load data
	*/
	int32 CGUIConfigFileLoader::LoadResourceConfigFile( const CGUIString& rPath, const CGUIString& rSceneName)
	{
		//get interface of config file
		IGUIInterfaceConfigFile* pConfigFile = CGUIInterfaceManager::Instance()->GetInterfaceConfigFile();
		if( !pConfigFile )
		{
			CGUIException::ThrowException("[LoadResourceConfigFile]: failed to get interface <IGUIConfigFile>!");
		}

		//get property set
		CGUIProperty aPropertySet;
		if( 0 != pConfigFile->LoadConfigFile(rPath, aPropertySet) )
		{
			CGUIException::ThrowException(
				"[LoadResourceConfigFile]: failed to load config file <%s : %s>!", 
				rSceneName.c_str(), 
				rPath.c_str());
		}

		//parse it
		uint32 nSize = aPropertySet.GetPropertyNum();
		for( uint32 i=0; i<nSize; ++i )
		{
			const CGUIProperty* pProperty = aPropertySet.GetProperty(i);
			CGUIResourceManagerBase* pResourceManager = NULL;
			switch( pProperty->GetType() )
			{
			case ePropertyType_Set:
				if( 0 != DoLoadConfig_Set( pProperty, rSceneName ))
				{
					return -1;
				}
				break;

			case ePropertyType_ImageDefine:
				pResourceManager = CGUIImageManager::Instance();
				break;

			case ePropertyType_AnimationDefine:
				pResourceManager = CGUIAnimationManager::Instance();
				break;

			case ePropertyType_FontDefine:
				pResourceManager = CGUIFontManager::Instance();
				break;

			case ePropertyType_SoundDefine:
				pResourceManager = CGUISoundManager::Instance();
				break;

			case ePropertyType_MusicDefine:
				pResourceManager = CGUIMusicManager::Instance();
				break;
				
			case ePropertyType_AsDefine:
				pResourceManager = CGUIAsManager::Instance();
				break;

			case ePropertyType_Particle2DDefine:
				pResourceManager = CGUIParticle2DManager::Instance();
				break;

			case ePropertyType_TiledMapDefine:
				pResourceManager = CGUITiledMapManager::Instance();
				break;

			default:
				{
					CGUIException::ThrowException(
						"[LoadResourceConfigFile], unknown property type <%s:%s:%s>", 
						pProperty->GetName().c_str(),
						pProperty->GetTypeAsString().c_str(),
						pProperty->GetValue().c_str());
				}
				return -1;
			}

			if( pResourceManager )
			{
				if( 0 != pResourceManager->RegisterResource( rSceneName, *pProperty ) )
				{
					CGUIException::ThrowException(
						"[CGUIConfigFileLoader::LoadResourceConfigFile], failed to register resource with name <%s:%s:%s>!", 
						pProperty->GetName().c_str(),
						pProperty->GetTypeAsString().c_str(),
						pProperty->GetValue().c_str());
					return -1;
				}
			}
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief read config file and load data
	*/
	CGUIWidget* CGUIConfigFileLoader::LoadWidgetConfigFile( const CGUIString& rPath, const CGUIString& rSceneName, const CGUIString& rFileName)
	{
		//get interface of config file
		IGUIInterfaceConfigFile* pConfigFile = CGUIInterfaceManager::Instance()->GetInterfaceConfigFile();
		if( !pConfigFile )
		{
			CGUIException::ThrowException("[LoadWidgetConfigFile]: failed to get interface <IGUIConfigFile>!");
		}

		//get property set
		CGUIProperty aPropertySet;
		if( 0 != pConfigFile->LoadConfigFile(rPath, aPropertySet) )
		{
			CGUIException::ThrowException(
				"[LoadWidgetConfigFile]: failed to load config file <%s : %s>!", 
				rSceneName.c_str(), 
				rPath.c_str());
		}

		//parse it
		uint32 nSize = aPropertySet.GetPropertyNum();
		CGUIWidget* pPage = NULL;
		for( uint32 i=0; i<nSize; ++i )
		{
			const CGUIProperty* pProperty = aPropertySet.GetProperty(i);
			switch( pProperty->GetType() )
			{
			case ePropertyType_WidgetDefine:
				{
					CGUIWidget* pWidget = DoLoadConfig_Widget( pProperty, rSceneName, rFileName );
					if( !pWidget )
					{
						return NULL;
					}
					if( !pPage )
					{
						if( pWidget->GetParent() != NULL )
						{
							CGUIException::ThrowException(
								"[LoadWidgetConfigFile], first widget <%s:%s> is not the root of page", 
								pWidget->GetName().c_str(),
								pWidget->GetType().c_str());
							return NULL;
						}
						else
						{
							pPage = pWidget;
						}
					}
					else
					{
						if( pWidget->GetParent() == NULL )
						{
							CGUIException::ThrowException(
								"[LoadWidgetConfigFile], the widget <%s:%s> hasn't parent", 
								pWidget->GetName().c_str(),
								pWidget->GetType().c_str());
							return NULL;
						}
					}
				}
				break;

			default:
				{
					CGUIException::ThrowException(
						"[LoadWidgetConfigFile], unknown property type <%s:%s:%s>", 
						pProperty->GetName().c_str(),
						pProperty->GetTypeAsString().c_str(),
						pProperty->GetValue().c_str());
				}
				return NULL;
			}
		}

		if( !pPage )
		{
			return NULL;
		}
		return pPage;
	}
	//------------------------------------------------------------------------------
}
