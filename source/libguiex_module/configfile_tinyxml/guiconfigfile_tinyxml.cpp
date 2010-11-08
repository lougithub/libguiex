/** 
* @file guiconfigfile_tinyxml.cpp
* @brief use xml and tinyxml to analyse the config file
* @author ken
* @date 2006-09-13
*/


//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/configfile_tinyxml/guiconfigfile_tinyxml.h>
#include <libguiex_core/guiex.h>
#include <libguiex_core/guipropertyconvertor.h>
#include "tinyxml.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	GUI_INTERFACE_IMPLEMENT(IGUIConfigFile_tinyxml);
	//------------------------------------------------------------------------------
	IGUIConfigFile_tinyxml::IGUIConfigFile_tinyxml()
	{
	}
	//------------------------------------------------------------------------------
	IGUIConfigFile_tinyxml::~IGUIConfigFile_tinyxml()
	{
	}
	//------------------------------------------------------------------------------
	void IGUIConfigFile_tinyxml::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	int	IGUIConfigFile_tinyxml::DoInitialize(void* )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIConfigFile_tinyxml::DoDestroy()
	{
	}
	//------------------------------------------------------------------------------
	CGUIString IGUIConfigFile_tinyxml::DoGetFilename( const CGUIString& rPath ) 
	{
		char fname[_MAX_FNAME];
		char fext[_MAX_EXT];
		_splitpath( rPath.c_str(), NULL, NULL, fname, fext ); 
		return CGUIString( fname ) + fext;
	}
	//------------------------------------------------------------------------------
	CGUIString IGUIConfigFile_tinyxml::DoGetFileDir( const CGUIString& rPath ) 
	{
		char fdir[_MAX_DIR];
		_splitpath( rPath.c_str(), NULL, fdir, NULL, NULL ); 
		return CGUIString( fdir );
	}
	//------------------------------------------------------------------------------
	CGUIProjectInfo* IGUIConfigFile_tinyxml::LoadProjectInfoFile( const CGUIString& rFileName )
	{
		///read file
		IGUIInterfaceFileSys* pFileSys =  CGUIInterfaceManager::Instance()->GetInterfaceFileSys();
		CGUIDataChunk aDataChunk;
		if( 0 != pFileSys->ReadFile( rFileName, aDataChunk, IGUIInterfaceFileSys::eOpenMode_String ))
		{
			//failed
			throw CGUIException("[IGUIConfigFile_tinyxml::LoadProjectInfoFile]: failed to read file <%s>!", rFileName.c_str());
			return NULL;
		}

		///parse file
		TiXmlDocument aDoc;
		aDoc.Parse( (const char*)aDataChunk.GetDataPtr(), 0, TIXML_ENCODING_UTF8 );
		if( aDoc.Error())
		{
			//failed to parse
			throw CGUIException(
				"[IGUIConfigFile_tinyxml::LoadResourceConfigFile]: failed to parse file <%s>!\n\n<%s>", 
				rFileName.c_str(),
				aDoc.ErrorDesc());
			return NULL;
		}

		///get root node
		TiXmlElement* pRootNode = aDoc.RootElement();
		if( !pRootNode )
		{
			throw guiex::CGUIException("[IGUIConfigFile_tinyxml::LoadProjectInfoFile], failed to get root node from file <%s>!", rFileName.c_str());
			return NULL;
		}

		CGUIProperty aProjectPropertySet;
		int32 ret = ProcessProperty( pRootNode, aProjectPropertySet );
		if( ret != 0 )
		{
			return NULL;
		}

		CGUIProjectInfo * pProjectInfo = CGUIProjectInfoManager::Instance()->GenerateProjectInfo();
		if( 0 != pProjectInfo->LoadFromPropertySet( DoGetFilename(rFileName), DoGetFileDir(rFileName), aProjectPropertySet ))
		{
			CGUIProjectInfoManager::Instance()->DestroyProjectInfo( pProjectInfo );
			return NULL;
		}
		return pProjectInfo;
	}
	//------------------------------------------------------------------------------
	int32	IGUIConfigFile_tinyxml::LoadResourceConfigFile(const CGUIString& rFileName, const CGUIString& rProjectName )
	{
		///read file
		IGUIInterfaceFileSys* pFileSys =  CGUIInterfaceManager::Instance()->GetInterfaceFileSys();
		CGUIDataChunk aDataChunk;
		if( 0 != pFileSys->ReadFile( rFileName, aDataChunk, IGUIInterfaceFileSys::eOpenMode_String ))
		{
			//failed
			throw CGUIException("[IGUIConfigFile_tinyxml::LoadResourceConfigFile]: failed to read file <%s>!", rFileName.c_str());
			return -1;
		}

		///parse file
		TiXmlDocument aDoc;
		aDoc.Parse( (const char*)aDataChunk.GetDataPtr(), 0, TIXML_ENCODING_UTF8 );
		if( aDoc.Error())
		{
			//failed to parse
			throw CGUIException(
				"[IGUIConfigFile_tinyxml::LoadResourceConfigFile]: failed to parse file <%s>!\n\n<%s>", 
				rFileName.c_str(),
				aDoc.ErrorDesc());
			return -1;
		}

		///get root node
		TiXmlElement* pRootNode = aDoc.RootElement();
		if( !pRootNode )
		{
			throw guiex::CGUIException("[IGUIConfigFile_tinyxml::LoadResourceConfigFile], failed to get root node from file <%s>!", rFileName.c_str());
			return -1;
		}

		///get node that contain config information
		TiXmlElement* pNode = pRootNode->FirstChildElement();
		while( pNode )
		{
			if( CGUIString("image") == pNode->Value())
			{
				/*
				<image>
				<property name="btn_ok_hover" type="CGUIImage">
				...
				...
				</image>
				*/

				//create named image
				CGUIProperty aPropertySet;
				int32 ret = ProcessProperty( pNode, aPropertySet );
				if( ret != 0 )
				{
					return ret;
				}

				uint32 nSize = aPropertySet.GetPropertyNum();
				for( uint32 i=0; i<nSize; ++i )
				{
					const CGUIProperty* pProperty = aPropertySet.GetProperty(i);
					if( !CGUIImageManager::Instance()->CreateImage(pProperty->GetName(), rProjectName, *pProperty) )
					{
						throw guiex::CGUIException(
							"[IGUIConfigFile_tinyxml::LoadResourceConfigFile], failed to create image with name <%s>!", 
							pProperty->GetName().c_str());
						return ret;
					}
				}
			}
			else if( CGUIString("font") == pNode->Value())
			{
				/*
				<font>
					<property name="0" type="FONT">
						<property name="PATH" type="CGUIString" value="font/simfang.ttf"/>
					</property>
				</font>

				*/
				//create font
				CGUIProperty aPropertySet;
				int32 ret = ProcessProperty( pNode, aPropertySet );
				if( ret != 0 )
				{
					return ret;
				}

				uint32 nSize = aPropertySet.GetPropertyNum();
				for( uint32 i=0; i<nSize; ++i )
				{
					const CGUIProperty* pProperty = aPropertySet.GetProperty(i);
					CGUIString strName = pProperty->GetName();
					CGUIString strPath = pProperty->GetProperty("path")->GetValue();
					const CGUIProperty* pIndexProp = pProperty->GetProperty("index");
					if( !pIndexProp )
					{
						throw guiex::CGUIException(
							"[IGUIConfigFile_tinyxml::LoadResourceConfigFile], invalid property <%s> in file <%s>!", 
							pProperty->GetName().c_str(),
							rFileName.c_str());
						return -1;
					}
					else
					{
						uint32 nIndex = 0;
						PropertyToValue( *pIndexProp, nIndex);
						CGUIFontManager::Instance()->CreateGUIFont( strName, rProjectName, strPath, nIndex );
					}
				}
			}
			else
			{
				throw guiex::CGUIException(
					"[IGUIConfigFile_tinyxml::LoadResourceConfigFile], unknown xml node <%s> in file <%s>!", 
					pNode->Value(),
					rFileName.c_str());
				return -1;
			}
			pNode = pNode->NextSiblingElement();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	IGUIConfigFile_tinyxml::LoadWidgetConfigFile(const CGUIString& rFileName, const CGUIString& rProjectName)
	{
		///read file
		IGUIInterfaceFileSys* pFileSys = CGUIInterfaceManager::Instance()->GetInterfaceFileSys();;
		CGUIDataChunk aDataChunk;
		if( 0 != pFileSys->ReadFile( rFileName, aDataChunk, IGUIInterfaceFileSys::eOpenMode_String ))
		{
			//failed
			throw CGUIException("[IGUIConfigFile_tinyxml::LoadWidgetConfigFile]: failed to read file <%s>!", rFileName.c_str());
			return NULL;
		}

		///parse file
		TiXmlDocument aDoc;
		aDoc.Parse( (const char*)aDataChunk.GetDataPtr(), 0, TIXML_ENCODING_UTF8 );
		if( aDoc.Error())
		{
			//failed to parse
			throw CGUIException(
				"[IGUIConfigFile_tinyxml::LoadWidgetConfigFile]: failed to parse file <%s>!\n\n<%s>", 
				rFileName.c_str(),
				aDoc.ErrorDesc());
			return NULL;
		}

		///get root node
		TiXmlElement* pRootNode = aDoc.RootElement();
		if( !pRootNode )
		{
			throw guiex::CGUIException("[IGUIConfigFile_tinyxml::LoadWidgetConfigFile], failed to get root node from file <%s>!", rFileName.c_str());
			return NULL;
		}

		///get node that contain config information
		guiex::IGUIInterfaceScript* pScript = CGUIInterfaceManager::Instance()->GetInterfaceScript();
		TiXmlElement* pNode = pRootNode->FirstChildElement();
		std::list<CGUIWidget*>	aWidgetList;
		CGUIWidget* pPageWidget = NULL;
		while( pNode )
		{
			if( CGUIString("widget") == pNode->Value())
			{
				/*
				<widget type="CGUIWgtStaticImage" name="sample_bg">
				<property name="SIZE" type="SIZE" value="800,600"/>
				<property name="OnOpen" type="EVENT" value="OnOpen_sample_bg"/>
				</widget>
				*/
				/// create widget
				CGUIString strWidgetType = pNode->Attribute( "type" );
				CGUIString strWidgetName = pNode->Attribute( "name" );
				CGUIWidget* pWidget = GUI_CREATE_WIDGET(strWidgetType, strWidgetName, rProjectName);
				if( !pWidget )
				{
					throw CGUIException(
						"[IGUIConfigFile_tinyxml::LoadWidgetConfigFile]: failed to create widget <%s:%s>!",
						strWidgetType.c_str(),
						strWidgetName.c_str());
					return NULL;
				}

				/// load property for widget
				CGUIProperty aPropertySet;
				int32 ret = ProcessProperty( pNode, aPropertySet );
				if( ret != 0 )
				{
					return NULL;
				}

				/// set property to widget
				pWidget->SetPropertySet( aPropertySet );
				aWidgetList.push_back( pWidget );

				if( !pPageWidget  )
				{
					if( !pWidget->GetParent() )
					{
						pPageWidget = pWidget;
					}
					else
					{
						throw CGUIException(
							"[IGUIConfigFile_tinyxml::LoadWidgetConfigFile]: the first widget should be root, but has parent! <%s:%s>",
							strWidgetType.c_str(),
							strWidgetName.c_str());
						return NULL;
					}
				}
			}
			else if( CGUIString("set") == pNode->Value())
			{
				/*
				<set name="SET_FRAME_TOP_STATIC_IMG">
				<property ...
				...
				</set>
				*/
				CGUIString strSetName = pNode->Attribute( "name" );

				/// load property for set
				CGUIProperty aPropertySet;
				int32 ret = ProcessProperty( pNode, aPropertySet );
				if( ret != 0 )
				{
					return NULL;
				}
				//register set
				CGUIPropertyManager::Instance()->RegisterSet( strSetName, aPropertySet );
			}
			else if( CGUIString("include") == pNode->Value())
			{
				/*
				<include type="SCRIPT" value="script/configfile_tinyxml.lua" />
				<include type="CONFIG" value="config/configfile_tinyxml_set.xml" />
				*/
				CGUIString strType = pNode->Attribute( "type" );
				CGUIString strValue = pNode->Attribute( "value" );
				if( strType == "SCRIPT" )
				{
					if( guiex::CGUIWidgetSystem::Instance()->ShouldRunScript())
					{
						// create script
						pScript->CreateScript( rProjectName );

						// load script
						CGUIString strPath = CGUIProjectInfoManager::Instance()->GetProjectFilePath( rProjectName ) + strValue;
						pScript->ExecuteFile(strPath, rProjectName);
					}
				}
				else
				{
					//unknown node
					throw CGUIException(
						"[IGUIConfigFile_tinyxml::LoadWidgetConfigFile]: unknown node <%s>!",
						pNode->Value());
					return NULL;
				}
			}
			else
			{
				//unknown node
				throw CGUIException(
					"[IGUIConfigFile_tinyxml::LoadWidgetConfigFile]: unknown node <%s>!",
					pNode->Value());
				return NULL;
			}

			pNode = pNode->NextSiblingElement();
		}

		bool bHasScript = pScript->HasScript(rProjectName);
		for( std::list<CGUIWidget*>::iterator itor = aWidgetList.begin();
			itor != aWidgetList.end();
			itor++)
		{
			(*itor)->LoadProperty();
			(*itor)->Create();

			if( bHasScript )
			{
				pScript->RegisterWidget((*itor));
			}

			CGUIEventNotification aEvent;
			aEvent.SetEventId(eEVENT_LOAD);
			aEvent.SetReceiver(*itor);
			CGUIWidgetSystem::Instance()->SendEvent( &aEvent);
		}
		
		return pPageWidget;
	}
	//------------------------------------------------------------------------------
	int32		IGUIConfigFile_tinyxml::ProcessProperty( TiXmlElement* pNode, CGUIProperty&	rPropSet )
	{
		TiXmlElement* pPropertyNode = pNode->FirstChildElement();
		while( pPropertyNode )
		{
			if( CGUIString("reference") == pPropertyNode->Value())
			{
				/* the node is a <reference> node
				eg. <reference name="SET_IMAGE_1"/>
			 */
				CGUIString strReferenceName = pPropertyNode->Attribute("name");
				rPropSet.AddProperty(CGUIPropertyManager::Instance()->GetSet(strReferenceName));
			}
			else if( CGUIString("property") == pPropertyNode->Value())
			{
				CGUIProperty aProperty;

				const char* pName = pPropertyNode->Attribute("name");
				const char* pType = pPropertyNode->Attribute("type");
				const char* pValue = pPropertyNode->Attribute("value");

				if( !pName )
				{
					throw CGUIException("[IGUIConfigFile_tinyxml::ProcessProperty]: property node lack attribute <name>!" );
					return -1;
				}
				aProperty.SetName(pName);

				if( !pType )
				{
					throw CGUIException("[IGUIConfigFile_tinyxml::ProcessProperty]: property node lack attribute <type>!" );
					return -1;
				}
				aProperty.SetType(pType);

				if( !pValue )
				{
					int32 ret = ProcessProperty( pPropertyNode, aProperty );
					if( ret != 0 )
					{
						return ret;
					}
				}
				else
				{
					aProperty.SetValue(pValue);
				}

				rPropSet.AddProperty( aProperty );
			}
			else
			{
				//unknown node
				throw CGUIException(
					"[IGUIConfigFile_tinyxml::ProcessProperty]: unknown property <%s>!",
					pPropertyNode->Value());
				return -1;
			}

			pPropertyNode = pPropertyNode->NextSiblingElement();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
