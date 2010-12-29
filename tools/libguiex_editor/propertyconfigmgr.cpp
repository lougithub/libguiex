/** 
* @file propertyconfigmgr.cpp
* @brief used to manage property
* @author ken
* @date 2007-09-19
*/

//============================================================================//
// include
//============================================================================// 
#include "propertyconfigmgr.h"
#include "wxmainframe.h"
#include "wxmainapp.h"
#include "editorutility.h"
#include <algorithm>

//------------------------------------------------------------------------------

//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
const CPropertyData* CPropertyData::GetPropertyData( const guiex::CGUIProperty& rProperty )
{
	return reinterpret_cast<CPropertyData*>( rProperty.GetData() );
}
//------------------------------------------------------------------------------
wxString CPropertyData::GetPropertyLabel( const CGUIProperty& rProperty )
{
	return Gui2wxString(GetPropertyData( rProperty )->GetLabel());
}
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
CPropertyConfigMgr::CPropertyConfigMgr()
{
}
//------------------------------------------------------------------------------
CPropertyConfigMgr::~CPropertyConfigMgr()
{
	Clear();
}
//------------------------------------------------------------------------------
void CPropertyConfigMgr::Clear()
{
	m_mapPropertySet.clear();
	m_setWidgetTyps.clear();

	for( TMapType::iterator itor = m_mapType.begin();
		itor != m_mapType.end();
		++itor )
	{
		delete itor->second;
	}
	m_mapType.clear();


	for( std::vector<CPropertyData*>::iterator itor=m_arrayPropertyDataCache.begin();
		itor != m_arrayPropertyDataCache.end();
		++itor)
	{
		delete *itor;
	}
	m_arrayPropertyDataCache.clear();
}
//------------------------------------------------------------------------------
CPropertyConfigMgr* CPropertyConfigMgr::Instance()
{
	static CPropertyConfigMgr s_mgr;
	return &s_mgr;
}
//------------------------------------------------------------------------------
void CPropertyConfigMgr::RegisterSet( const std::string& rSetName, const guiex::CGUIProperty& rPropertySet, bool bIsTemplate )
{
	if( m_mapPropertySet.find(rSetName) != m_mapPropertySet.end())
	{
		std::string strError = std::string("[CPropertyConfigMgr::RegisterSet] the property <")+rSetName+"> has existed!";
		wxMessageBox(Gui2wxString(strError), _T("Error") );
		wxASSERT(0);
	}
	m_mapPropertySet.insert( std::make_pair(rSetName,rPropertySet) );

	if( bIsTemplate == false )
	{
		m_setWidgetTyps.push_back( rSetName );
		std::sort( m_setWidgetTyps.begin(), m_setWidgetTyps.end() );
	}
}
//------------------------------------------------------------------------------
void CPropertyConfigMgr::RegisterEnumDefine( const guiex::CGUIString& rEnumName, const wxArrayString& rEnumValue )
{
	if( m_mapEnums.find(rEnumName) != m_mapEnums.end())
	{
		std::string strError = std::string("[CPropertyConfigMgr::RegisterSet] the property <")+rEnumName+"> has existed!";
		wxMessageBox(Gui2wxString(strError), _T("Error") );
		wxASSERT(0);
	}
	m_mapEnums.insert( std::make_pair(rEnumName,rEnumValue) );
}
//------------------------------------------------------------------------------
const CPropertyConfigMgr::TMapPropertySet& 	CPropertyConfigMgr::GetPropertySetMap( ) const
{
	return m_mapPropertySet;
}
//------------------------------------------------------------------------------
const guiex::CGUIProperty& CPropertyConfigMgr::GetPropertySet(const std::string& rSetName ) const
{
	TMapPropertySet::const_iterator itor = m_mapPropertySet.find(rSetName);
	if( itor == m_mapPropertySet.end())
	{
		std::string strError = std::string("[CPropertyConfigMgr::GetPropertySet] the property <")+rSetName+"> doesn't exist!";
		wxMessageBox(Gui2wxString(strError), _T("Error") );
		wxASSERT(0);
	}
	return itor->second;
}
//------------------------------------------------------------------------------
const wxArrayString& CPropertyConfigMgr::GetEnumDefine( const guiex::CGUIString& rEnumName ) const
{
	TEnumMap::const_iterator itor = m_mapEnums.find(rEnumName);
	if( itor == m_mapEnums.end())
	{
		std::string strError = std::string("[CPropertyConfigMgr::GetEnumDefine] the enum <")+rEnumName+"> doesn't exist!";
		wxMessageBox(Gui2wxString(strError), _T("Error") );
		wxASSERT(0);
	}
	return itor->second;
}
//------------------------------------------------------------------------------
std::string* CPropertyConfigMgr::GetTypePtr( const std::string& rType )
{
	TMapType::iterator itor = m_mapType.find( rType );
	if(itor == m_mapType.end())
	{
		throw guiex::CGUIException("[CPropertyConfigMgr::GetTypePtr]: failed to find widget type <%s>", rType.c_str());
	}
	return itor->second;
}
//------------------------------------------------------------------------------
void CPropertyConfigMgr::AddType( const std::string& rType )
{
	if( m_mapType.find(rType) == m_mapType.end())
	{
		m_mapType.insert( std::make_pair( rType, new std::string(rType)));
	}
}
//------------------------------------------------------------------------------
int CPropertyConfigMgr::ReadPropertyConfig(const std::string& rFileName)
{
	//parse file
	TiXmlDocument aDoc;
	aDoc.LoadFile( rFileName.c_str() );
	if( aDoc.Error())
	{
		//failed to parse
		wxChar buf[1024];
		wxSnprintf( buf, 1024, _T("Failed to read config file! \n\n%s"), Gui2wxString(aDoc.ErrorDesc()));
		wxMessageBox( buf, _T("Error") );
		return -1;
	}

	///get root node
	TiXmlElement* pRootNode = aDoc.RootElement();
	if( !pRootNode )
	{
		wxChar buf[1024];
		wxSnprintf( buf, 1024, _T("Failed to read Scene file! \n\n%s"), _T("can't get root node"));
		wxMessageBox(buf, _T("Error") );
		return -1;
	}

	///get node that contain config information
	TiXmlElement* pNode = pRootNode->FirstChildElement();
	while( pNode )
	{
		if( std::string("enum") == pNode->Value())
		{
			if( 0 != ProcessEnumNode(pNode) )
			{
				wxMessageBox(_T("Failed to load property config file!"), _T("Error") );
				wxASSERT(0);
				return -1;
			}
		}
		else if( std::string("widget") == pNode->Value())
		{
			if( 0 != ProcessWidgetNode(pNode) )
			{
				wxMessageBox(_T("Failed to load property config file!"), _T("Error") );
				wxASSERT(0);
				return -1;
			}
		}
		else
		{
			wxASSERT(0);
		}

		pNode = pNode->NextSiblingElement();
	}

	return 0;
}
//------------------------------------------------------------------------------
int		CPropertyConfigMgr::ProcessEnumNode(TiXmlElement* pWidgetNode)
{
	//process sub node
	TiXmlElement* pNode = pWidgetNode->FirstChildElement();
	while( pNode )
	{
		const char* szName = pNode->Attribute( "name" );
		const char* szType = pNode->Attribute( "type" );
		const char* szValue = pNode->Attribute( "value" );
		if( !szName || !szType || !szValue)
		{
			return -1;
		}

		if( std::string("enum") != szType )
		{
			return -1;
		};

		wxArrayString vecEnum;
		std::string strEnum = szValue;
		std::string::size_type nStartPos = 0;
		std::string::size_type nEndPos = strEnum.find(";", nStartPos);
		while( nEndPos != std::string::npos )
		{
			std::string aEnum = strEnum.substr( nStartPos, nEndPos-nStartPos );
			vecEnum.push_back( Gui2wxString( aEnum));
			nStartPos = nEndPos + 1;
			nEndPos = strEnum.find(";", nStartPos);
		}

		//register it to map
		RegisterEnumDefine(szName, vecEnum);

		pNode = pNode->NextSiblingElement();
	}

	return 0;
}
//------------------------------------------------------------------------------
int		CPropertyConfigMgr::ProcessWidgetNode(TiXmlElement* pWidgetNode)
{
	//	<widget type="CGUIWgtButton" inherit="CGUIWidget">
	const char* szType = pWidgetNode->Attribute( "type" );
	if( !szType )
	{
		return -1;
	}

	guiex::CGUIProperty aPropSet;
	const char* szInherit = pWidgetNode->Attribute("inherit");
	if( szInherit )
	{
		//this config is inherited from other property config
		aPropSet = GetPropertySet(szInherit);
	}

	bool bIsTemplate = false;
	const char* szTemplate = pWidgetNode->Attribute("template");
	if( szTemplate && strcmp(szTemplate,"true") == 0)
	{
		bIsTemplate = true;
	}

	//process sub node
	TiXmlElement* pNode = pWidgetNode->FirstChildElement();
	while( pNode )
	{
		if( std::string("page") == pNode->Value())
		{
			const char* szPage = pNode->Attribute( "name" );
			if( !szPage )
			{
				return -1;
			}
			
			//process property node
			if( 0 != ProcessPropertyNode(szPage, aPropSet, pNode))
			{
				return -1;
			}
		}

		pNode = pNode->NextSiblingElement();
	}


	//register it to map
	RegisterSet( szType, aPropSet, bIsTemplate);

	return 0;
}
//------------------------------------------------------------------------------
int		CPropertyConfigMgr::ProcessPropertyNode(const std::string& rPage, CGUIProperty& rPropertySet, TiXmlElement* pNode)
{
	TiXmlElement* pPropertyNode = pNode->FirstChildElement();
	while( pPropertyNode )
	{
		if( std::string("property") == pPropertyNode->Value())
		{
			const char* pCategory = pPropertyNode->Attribute("category");
			const char* pName = pPropertyNode->Attribute("name");
			const char* pType = pPropertyNode->Attribute("type");
			const char* pValue = pPropertyNode->Attribute("value");
			const char* pLabel = pPropertyNode->Attribute("label");
			const char* pMustExist = pPropertyNode->Attribute("must_exist");
			const char* pReadOnly = pPropertyNode->Attribute("readonly");
			if( !pName || !pType)
			{	
				return -1;
			}
		
			AddType(pType);

			int32 nPropertyIndex = -1;
			//remove old property
			if( rPropertySet.HasProperty(pName) )
			{
				nPropertyIndex = rPropertySet.GetPropertyIndex(pName);
				rPropertySet.RemoveProperty( nPropertyIndex );
			}

			guiex::CGUIProperty aProperty(pName, pType);
			if( pValue )
			{
				aProperty.SetValue(pValue);
			}

			//set info
			CPropertyData* pData = new CPropertyData;
			m_arrayPropertyDataCache.push_back( pData );
			aProperty.SetData(pData);

			pData->SetPage(rPage);
			pData->SetLabel(pLabel?pLabel:pName);
			pData->SetCategory(pCategory ? pCategory : "Default" );

			pData->SetMustExist(false);
			if( pMustExist && strcmp(pMustExist,"true") == 0)
			{
				pData->SetMustExist(true);
			}


			pData->SetReadOnly(false);
			if( pReadOnly && strcmp(pReadOnly,"true") == 0)
			{
				pData->SetReadOnly(true);
			}

			//process child
			if( ProcessPropertyNode( rPage, aProperty, pPropertyNode ) != 0 )
			{
				return -1;
			}

			rPropertySet.InsertProperty(aProperty, nPropertyIndex);
		}
		else
		{
			assert(0);
		}

		pPropertyNode = pPropertyNode->NextSiblingElement();
	}

	return 0;
}
//------------------------------------------------------------------------------


