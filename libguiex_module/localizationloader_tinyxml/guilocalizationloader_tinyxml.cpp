/** 
* @file guilocalizationloader_tinyxml.cpp
* @brief use xml and tinyxml to analyse the localization file
* @author ken
* @date 2011-04-28
*/



//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/localizationloader_tinyxml/guilocalizationloader_tinyxml.h>
#include <libguiex_core/guiex.h>
#include <libguiex_core/guipropertyconvertor.h>
#include <tinyxml.h>



//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	GUI_INTERFACE_IMPLEMENT(IGUILocalizationLoader_tinyxml);
	//------------------------------------------------------------------------------
	const char* IGUILocalizationLoader_tinyxml::StaticGetModuleName()
	{
		return "IGUILocalizationLoader_tinyxml";
	}
	//------------------------------------------------------------------------------
	IGUILocalizationLoader_tinyxml::IGUILocalizationLoader_tinyxml()
		:IGUIInterfaceLocalizationLoader( StaticGetModuleName() )
	{
	}
	//------------------------------------------------------------------------------
	IGUILocalizationLoader_tinyxml::~IGUILocalizationLoader_tinyxml()
	{
	}
	//------------------------------------------------------------------------------
	void IGUILocalizationLoader_tinyxml::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	int	IGUILocalizationLoader_tinyxml::DoInitialize(void* )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUILocalizationLoader_tinyxml::DoDestroy()
	{
	}
	//------------------------------------------------------------------------------
	int32 IGUILocalizationLoader_tinyxml::LoadLocalizationFile( const CGUIString& rFileName, 
		const CGUIString& rCurrentLocalConfig,
		std::vector<std::pair<CGUIString, CGUIStringW> >& rLocalizations )
	{
		///read file
		IGUIInterfaceFileSys* pFileSys =  CGUIInterfaceManager::Instance()->GetInterfaceFileSys();
		CGUIDataChunk aDataChunk;
		if( 0 != pFileSys->ReadFile( rFileName, aDataChunk, IGUIInterfaceFileSys::eOpenMode_String ))
		{
			//failed
			CGUIException::ThrowException("[IGUILocalizationLoader_tinyxml::LoadLocalizationFile]: failed to read file <%s>!", rFileName.c_str());
			return -1;
		}

		///parse file
		TiXmlDocument aDoc;
		aDoc.Parse( (const char*)aDataChunk.GetDataPtr(), 0, TIXML_ENCODING_UTF8 );
		if( aDoc.Error())
		{
			//failed to parse
			CGUIException::ThrowException(
				"[IGUILocalizationLoader_tinyxml::LoadLocalizationFile]: failed to parse file <%s>!\n\n<%s>", 
				rFileName.c_str(),
				aDoc.ErrorDesc());
			return -1;
		}

		///get root node
		TiXmlElement* pRootNode = aDoc.RootElement();
		if( !pRootNode )
		{
			CGUIException::ThrowException("[IGUILocalizationLoader_tinyxml::LoadLocalizationFile], failed to get root node from file <%s>!", rFileName.c_str());
			return -1;
		}

		//	<localization language="english">
		TiXmlElement* pLocalizationNode = pRootNode->FirstChildElement();
		while( pLocalizationNode )
		{
			const char* szLocal = pLocalizationNode->Attribute("local");
			if( szLocal && rCurrentLocalConfig == szLocal )
			{
				//import this local:  <string name="ok"> ok </string>
				TiXmlElement* pStringNode = pLocalizationNode->FirstChildElement();
				while( pStringNode )
				{
					const char* szKey = pStringNode->Attribute("key");
					if( !szKey )
					{
						CGUIException::ThrowException("[IGUILocalizationLoader_tinyxml::LoadLocalizationFile], failed to get localization name from local <%s>!", rCurrentLocalConfig.c_str());
						return -1;
					}
					const char* szContent = pStringNode->GetText();
					if( !szContent )
					{
						CGUIException::ThrowException("[IGUILocalizationLoader_tinyxml::LoadLocalizationFile], failed to get localization content from name from local <%s>!", szKey, rCurrentLocalConfig.c_str());
						return -1;
					}
					CGUIStringW strContentW;
					AppMultiByteToWideChar( szContent, strContentW );
					if( strContentW.empty() )
					{
						CGUIException::ThrowException("[IGUILocalizationLoader_tinyxml::LoadLocalizationFile], failed to get localization content from name from local <%s>!", szKey, rCurrentLocalConfig.c_str());
						return -1;
					}

					rLocalizations.push_back( std::make_pair( CGUIString(szKey), strContentW ));

					pStringNode = pStringNode->NextSiblingElement();
				}
			}

			pLocalizationNode = pLocalizationNode->NextSiblingElement();
		}
		return 0;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
