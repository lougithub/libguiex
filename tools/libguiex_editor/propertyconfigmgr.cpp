/** 
* @file propertyconfigmgr.cpp
* @brief used to manage property
* @author ken
* @date 2007-09-19
*/

//============================================================================//
// include
//============================================================================// 
#include "libguiex_editor.h"

#include "tinyxml.h"

#include "propertyconfigmgr.h"
#include "wxmainframe.h"
#include "wxmainapp.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------ 


//============================================================================//
// function
//============================================================================// 

//------------------------------------------------------------------------------
CProperty::CProperty()
:m_bMustExist(false)
{
}
//------------------------------------------------------------------------------
CProperty::~CProperty()
{
	Clear();
}
//------------------------------------------------------------------------------
void	CProperty::Clear( )
{
	m_setProperty.clear();
}
//------------------------------------------------------------------------------
void					CProperty::SetName(const std::string& rName)
{
	m_strName = rName;
}
//------------------------------------------------------------------------------
const std::string&	CProperty::GetName() const
{
	return m_strName;
}
//------------------------------------------------------------------------------
void					CProperty::SetLabel(const std::string& rLabel)
{
	m_strLabel = rLabel;
}
//------------------------------------------------------------------------------
const std::string&	CProperty::GetLabel() const
{
	return m_strLabel;
}
//------------------------------------------------------------------------------
void					CProperty::SetValue(const std::string& rValue)
{
	m_strValue = rValue;
}
//------------------------------------------------------------------------------
const std::string&	CProperty::GetValue() const
{
	return m_strValue;
}
//------------------------------------------------------------------------------
void					CProperty::SetType(const std::string& rType)
{
	m_strType = rType;
}
//------------------------------------------------------------------------------
const std::string&	CProperty::GetType() const
{
	return m_strType;
}
//------------------------------------------------------------------------------
void				CProperty::SetCategory( const std::string& rCategory)
{
	m_strCategories = rCategory;
}
//------------------------------------------------------------------------------
const std::string&			CProperty::GetCategory() const
{
	return m_strCategories;
}
//------------------------------------------------------------------------------
void				CProperty::SetPage(const std::string& rPage)
{
	m_strPage = rPage;
}
//------------------------------------------------------------------------------
const std::string&		CProperty::GetPage() const
{
	return m_strPage;
}
//------------------------------------------------------------------------------
void	CProperty::SetEnums(const std::vector<std::string>& rEnum)
{
	m_vecEnums = rEnum ;
}
//------------------------------------------------------------------------------
const std::vector<std::string>&		CProperty::GetEnums() const
{
	return m_vecEnums;
}
//------------------------------------------------------------------------------
void			CProperty::SetMustExist(bool bMustExist)
{
	m_bMustExist = bMustExist;
}
//------------------------------------------------------------------------------
bool			CProperty::IsMustExist() const
{
	return m_bMustExist;
}
//------------------------------------------------------------------------------
void				CProperty::AddProperty( const CProperty& rProperty )
{
	assert( HasProperty(rProperty.GetName() ) == false);
	m_setProperty.push_back(rProperty);
}
//------------------------------------------------------------------------------
void				CProperty::InsertProperty(  const CProperty& rProperty, guiex::int32 nIndex )
{
	assert( HasProperty(rProperty.GetName() ) == false);
	if( nIndex < 0 || guiex::uint32(nIndex) >= m_setProperty.size())
	{
		m_setProperty.push_back(rProperty);
	}
	else
	{
		m_setProperty.insert(m_setProperty.begin() + nIndex, rProperty);
	}
}
//------------------------------------------------------------------------------
unsigned int		CProperty::GetPropertyNum( ) const
{
	return static_cast<unsigned int>(m_setProperty.size());
}
//------------------------------------------------------------------------------
const CProperty*	CProperty::GetProperty( unsigned int nIdx ) const
{
	if( nIdx >= m_setProperty.size())
	{
		wxMessageBox(_T("the index of property is overflowed"), _T("error"), wxICON_ERROR|wxCENTRE);
		wxASSERT(0);
		return NULL;
	}

	return &m_setProperty[nIdx];
}
//------------------------------------------------------------------------------
const CProperty*	CProperty::GetProperty( const std::string& rName ) const
{
	for( TSetProperty::const_iterator itor = m_setProperty.begin();
		itor != m_setProperty.end();
		++itor)
	{
		if( (*itor).GetName() == rName )
		{
			return &(*itor);
		}
	}

	return NULL;
}
//------------------------------------------------------------------------------
bool			CProperty::HasProperty( const std::string& rName ) const
{
	for( TSetProperty::const_iterator itor = m_setProperty.begin();
		itor != m_setProperty.end();
		++itor)
	{
		if( (*itor).GetName() == rName )
		{
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------
int32				CProperty::GetPropertyIndex( const std::string& rName ) const
{
	for( uint32 i=0; i<m_setProperty.size(); ++i)
	{
		if( m_setProperty[i].GetName() == rName )
		{
			return int32(i);
		}
	}
	wxMessageBox(_T("failed to find property"), _T("error"), wxICON_ERROR|wxCENTRE);
	wxASSERT(0);
	return -1;
}
//------------------------------------------------------------------------------
void		CProperty::RemoveProperty( const std::string& rName )
{
	for( TSetProperty::const_iterator itor = m_setProperty.begin();
		itor != m_setProperty.end();
		++itor)
	{
		if( (*itor).GetName() == rName )
		{
			m_setProperty.erase(itor);
			return;
		}
	}

	wxMessageBox(_T("failed to remove property"), _T("error"), wxICON_ERROR|wxCENTRE);
	wxASSERT(0);
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
void	CPropertyConfigMgr::Clear()
{
	m_mapPropertySet.clear();
	for( TSetType::iterator itor=m_mapType.begin();
		itor != m_mapType.end();
		++itor)
	{
		delete itor->second;
	}
	m_mapType.clear();
}
//------------------------------------------------------------------------------
CPropertyConfigMgr* CPropertyConfigMgr::Instance()
{
	static CPropertyConfigMgr s_mgr;
	return &s_mgr;
}
//------------------------------------------------------------------------------
void	CPropertyConfigMgr::RegisterSet( const std::string& rSetName, const CProperty& rPropertySet )
{
	if( m_mapPropertySet.find(rSetName) != m_mapPropertySet.end())
	{
		std::string strError = std::string("[CPropertyConfigMgr::RegisterSet] the property <")+rSetName+"> has existed!";
		wxMessageBox(wxConvUTF8.cMB2WC(strError.c_str()), _T("Error"), wxICON_ERROR | wxCENTRE );
		wxASSERT(0);
	}
	m_mapPropertySet.insert( std::make_pair(rSetName,rPropertySet) );
}
//------------------------------------------------------------------------------
const std::map<std::string, CProperty>& 	CPropertyConfigMgr::GetPropertySetMap( ) const
{
	return m_mapPropertySet;
}
//------------------------------------------------------------------------------
const  CProperty*	CPropertyConfigMgr::GetPropertySet(const std::string& rSetName ) const
{
	TMapPropertySet::const_iterator itor = m_mapPropertySet.find(rSetName);
	if( itor == m_mapPropertySet.end())
	{
		std::string strError = std::string("[CPropertyConfigMgr::GetPropertySet] the property <")+rSetName+"> doesn't exist!";
		wxMessageBox(wxConvUTF8.cMB2WC(strError.c_str()), _T("Error"), wxICON_ERROR | wxCENTRE );
		wxASSERT(0);
		return NULL;
	}
	return &itor->second;
}
//------------------------------------------------------------------------------
std::string*	CPropertyConfigMgr::GetTypePtr( const std::string& rType )
{
	TSetType::iterator itor = m_mapType.find( rType );
	if(itor == m_mapType.end())
	{
		throw guiex::CGUIException("[CPropertyConfigMgr::GetTypePtr]: failed to find widget type <%s>", rType.c_str());
	}
	return itor->second;
}
//------------------------------------------------------------------------------
void	CPropertyConfigMgr::AddType( const std::string& rType )
{
	if( m_mapType.find(rType) == m_mapType.end())
	{
		m_mapType.insert( std::make_pair( rType, new std::string(rType)));
	}
}
//------------------------------------------------------------------------------
int		CPropertyConfigMgr::ReadPropertyConfig(const std::string& rFileName)
{
	Clear();

	///parse file
	TiXmlDocument aDoc;
	aDoc.LoadFile( rFileName.c_str() );
	if( aDoc.Error())
	{
		//failed to parse
		wxChar buf[1024];
		wxSnprintf( buf, 1024, _T("Failed to read config file! \n\n%s"), wxConvUTF8.cMB2WC(aDoc.ErrorDesc()));
		wxMessageBox(buf, _T("Error"), wxICON_ERROR, GetMainFrame() );
		return -1;
	}

	///get root node
	TiXmlElement* pRootNode = aDoc.RootElement();
	if( !pRootNode )
	{
		wxChar buf[1024];
		wxSnprintf( buf, 1024, _T("Failed to read project file! \n\n%s"), _T("can't get root node"));
		wxMessageBox(buf, _T("Error"), wxICON_ERROR, GetMainFrame() );
		return -1;
	}

	///get node that contain config information
	TiXmlElement* pNode = pRootNode->FirstChildElement();
	while( pNode )
	{
		if( std::string("widget") == pNode->Value())
		{
			if( 0 != ProcessWidgetNode(pNode) )
			{
				wxMessageBox(_T("Failed to load property config file!"), _T("Error"), wxICON_ERROR, GetMainFrame() );
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
int		CPropertyConfigMgr::ProcessWidgetNode(TiXmlElement* pWidgetNode)
{
	//	<widget type="CGUIWgtButton" inherit="CGUIWidget">
	const char* szType = pWidgetNode->Attribute( "type" );
	if( !szType )
	{
		return -1;
	}

	CProperty aPropSet;
	const char* szInherit = pWidgetNode->Attribute("inherit");
	if( szInherit )
	{
		//this config is inherited from other property config
		const CProperty* pSet = GetPropertySet(szInherit);
		if( !pSet )
		{
			return -1;
		}

		aPropSet = *pSet;
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
			if( 0 != ProcessPropertyNode(szPage, &aPropSet, pNode))
			{
				return -1;
			}
		}

		pNode = pNode->NextSiblingElement();
	}


	//register it to map
	RegisterSet(szType, aPropSet);

	return 0;
}
//------------------------------------------------------------------------------
int		CPropertyConfigMgr::ProcessPropertyNode(const std::string& rPage, CProperty* pPropertySet, TiXmlElement* pNode)
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
			const char* pEnum = pPropertyNode->Attribute("enum");
			const char* pMustExist = pPropertyNode->Attribute("must_exist");
			if( !pName || !pType)
			{	
				return -1;
			}
		
			AddType(pType);

			int32 nPropertyIndex = -1;
			//remove old property
			if( pPropertySet->HasProperty(pName) )
			{
				nPropertyIndex = pPropertySet->GetPropertyIndex(pName);
				pPropertySet->RemoveProperty(pName);
			}

			CProperty aProperty;

			//set info
			aProperty.SetPage(rPage);
			aProperty.SetName(pName);
			aProperty.SetType(pType);
			aProperty.SetLabel(pLabel?pLabel:pName);
			aProperty.SetCategory(pCategory ? pCategory : "Default" );

			if( pEnum )
			{
				std::vector< std::string> vecEnums;
				std::string strEnum = pEnum;
				std::string::size_type nStartPos = 0;
				std::string::size_type nEndPos = strEnum.find(";", nStartPos);
				while( nEndPos != std::string::npos )
				{
					vecEnums.push_back( strEnum.substr( nStartPos, nEndPos-nStartPos ));
					nStartPos = nEndPos + 1;
					nEndPos = strEnum.find(";", nStartPos);
				}
				aProperty.SetEnums( vecEnums );
			}
			if( pMustExist )
			{
				if( strcmp(pMustExist,"true") == 0)
				{
					aProperty.SetMustExist(true);
				}
				else
				{
					aProperty.SetMustExist(false);
				}
			}
			else
			{
				aProperty.SetMustExist(false);
			}

			if( pValue )
			{
				aProperty.SetValue(pValue);
			}

			//process child
			if( ProcessPropertyNode( rPage, &aProperty, pPropertyNode ) != 0 )
			{
				return -1;
			}

			pPropertySet->InsertProperty(aProperty, nPropertyIndex);
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
















