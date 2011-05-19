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
#include <tinyxml.h>



//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	GUI_INTERFACE_IMPLEMENT(IGUIConfigFile_tinyxml);
	//------------------------------------------------------------------------------
	const char* IGUIConfigFile_tinyxml::StaticGetModuleName()
	{
		return "IGUIConfigFile_tinyxml";
	}
	//------------------------------------------------------------------------------
	IGUIConfigFile_tinyxml::IGUIConfigFile_tinyxml()
		:IGUIInterfaceConfigFile( StaticGetModuleName() )
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
	int32 IGUIConfigFile_tinyxml::LoadConfigFile( const CGUIString& rFileName, CGUIProperty& rPropertySet )
	{
		///read file
		IGUIInterfaceFileSys* pFileSys =  CGUIInterfaceManager::Instance()->GetInterfaceFileSys();
		CGUIDataChunk aDataChunk;
		if( 0 != pFileSys->ReadFile( rFileName, aDataChunk, IGUIInterfaceFileSys::eOpenMode_String ))
		{
			//failed
			GUI_THROW( GUI_FORMAT("[IGUIConfigFile_tinyxml::LoadResourceConfigFile]: failed to read file <%s>!", rFileName.c_str()));
			return -1;
		}

		///parse file
		TiXmlDocument aDoc;
		aDoc.Parse( (const char*)aDataChunk.GetDataPtr(), 0, TIXML_ENCODING_UTF8 );
		if( aDoc.Error())
		{
			//failed to parse
			GUI_THROW( GUI_FORMAT(
				"[IGUIConfigFile_tinyxml::LoadResourceConfigFile]: failed to parse file <%s>!\n\n<%s>", 
				rFileName.c_str(),
				aDoc.ErrorDesc()));
			return -1;
		}

		///get root node
		TiXmlElement* pRootNode = aDoc.RootElement();
		if( !pRootNode )
		{
			GUI_THROW( GUI_FORMAT("[IGUIConfigFile_tinyxml::LoadResourceConfigFile], failed to get root node from file <%s>!", rFileName.c_str()));
			return -1;
		}

		//get all property
		int32 ret = ProcessProperty( pRootNode, rPropertySet );
		if( ret != 0 )
		{
			return ret;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief process property, get sub property set of this property node
	* @return 0 for success
	*/
	int32 IGUIConfigFile_tinyxml::ProcessProperty( TiXmlElement* pNode, CGUIProperty&	rPropSet )
	{
		TiXmlElement* pPropertyNode = pNode->FirstChildElement();
		while( pPropertyNode )
		{
			if( CGUIString("reference") == pPropertyNode->Value())
			{
				/* the node is a <reference> node
				 * eg. <reference name="SET_IMAGE_1"/>
				*/
				CGUIString strReferenceName = pPropertyNode->Attribute("name");
				rPropSet.AddProperty(CGUIPropertyManager::Instance()->GetSet(strReferenceName));
			}
			else if( CGUIString("property") == pPropertyNode->Value())
			{
				CGUIProperty aProperty;

				const char* pName = pPropertyNode->Attribute("name");
				if( !pName )
				{
					GUI_THROW( "[IGUIConfigFile_tinyxml::ProcessProperty]: property node lack attribute <name>!" );
					return -1;
				}
				aProperty.SetName(pName);

				const char* pType = pPropertyNode->Attribute("type");
				if( !pType )
				{
					GUI_THROW( "[IGUIConfigFile_tinyxml::ProcessProperty]: property node lack attribute <type>!" );
					return -1;
				}
				aProperty.SetType(pType);

				const char* pValue = pPropertyNode->Attribute("value");
				if( pValue )
				{
					aProperty.SetValue(pValue);
				}
				
				int32 ret = ProcessProperty( pPropertyNode, aProperty );
				if( ret != 0 )
				{
					return ret;
				}

				rPropSet.AddProperty( aProperty );
			}
			else
			{
				//unknown node
				GUI_THROW( GUI_FORMAT(
					"[IGUIConfigFile_tinyxml::ProcessProperty]: unknown property <%s>!",
					pPropertyNode->Value()));
				return -1;
			}

			pPropertyNode = pPropertyNode->NextSiblingElement();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
