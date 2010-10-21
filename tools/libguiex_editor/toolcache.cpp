/** 
 * @file toolcache.cpp
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2009-11-05
 */


//============================================================================//
// include
//============================================================================//
#include "libguiex_editor.h"
#include "toolcache.h"

#include "tinyxml.h"
#include <algorithm>



//============================================================================//
// function
//============================================================================//

//------------------------------------------------------------------------------
CToolCache::CToolCache()
:m_nPathBaseId( 0 )
,m_nProjectBaseId( 0 )
,m_nMaxSize(10)
{
	m_pPathMenu = new wxMenu( );
	m_pProjectMenu = new wxMenu( );
}
//------------------------------------------------------------------------------
CToolCache::~CToolCache()
{
}
//------------------------------------------------------------------------------
CToolCache* CToolCache::Instance()
{
	static CToolCache s_mgr;
	return &s_mgr;
}
//------------------------------------------------------------------------------
void	CToolCache::ParseCache( const std::string& rCacheFile )
{
	m_projectHistory.clear();
	m_pathHistory.clear();

	m_strCacheFile = rCacheFile;
	TiXmlDocument aDoc;
	aDoc.LoadFile( rCacheFile.c_str() );
	if( aDoc.Error())
	{
		return;
	}

	///get root node
	TiXmlElement* pRootNode = aDoc.RootElement();
	if( !pRootNode )
	{
		return;
	}

	///get node that contain config information
	TiXmlElement* pNode = pRootNode->FirstChildElement();
	while( pNode )
	{
		if( std::string("projects") == pNode->Value())
		{
			TiXmlElement* pHistoryNode = pNode->FirstChildElement();
			while( pHistoryNode )
			{
				if( std::string("history") == pHistoryNode->Value())
				{
					const char* project = pHistoryNode->Attribute("project");
					const char* path = pHistoryNode->Attribute("path");

					m_projectHistory.push_back( std::make_pair( std::string(project), std::string(path)));
				}
				pHistoryNode = pHistoryNode->NextSiblingElement();
			}

		}
		else if( std::string("paths") == pNode->Value())
		{
			TiXmlElement* pHistoryNode = pNode->FirstChildElement();
			while( pHistoryNode )
			{
				if( std::string("history") == pHistoryNode->Value())
				{
					const char* path = pHistoryNode->Attribute("path");

					m_pathHistory.push_back( path );
				}
				pHistoryNode = pHistoryNode->NextSiblingElement();
			}
		}

		pNode = pNode->NextSiblingElement();
	}
}
//------------------------------------------------------------------------------
void	CToolCache::AddCache( const std::string& rProject, const std::string& rPath )
{
	bool bChanged = false;

	//for path
	if( std::find( m_pathHistory.begin(), m_pathHistory.end(), rPath ) == m_pathHistory.end())
	{
		m_pathHistory.push_back(rPath);
		while( !m_pathHistory.empty() && m_pathHistory.size() > m_nMaxSize )
		{
			m_pathHistory.erase( m_pathHistory.begin());
		}
		bChanged = true;
	}


	//for project
	if( std::find( m_projectHistory.begin(), m_projectHistory.end(), std::make_pair(rProject, rPath) ) == m_projectHistory.end())
	{
		m_projectHistory.push_back(std::make_pair(rProject, rPath));
		while( !m_projectHistory.empty() && m_projectHistory.size() > m_nMaxSize )
		{
			m_projectHistory.erase( m_projectHistory.begin());
		}
		bChanged = true;
	}

	if( bChanged )
	{
		SaveFile();
		UpdateMenu();
	}
}
//------------------------------------------------------------------------------
void	CToolCache::UpdateMenu( )
{
	// Remove all existing menu items.
	for( unsigned x = 0 ; x < m_nMaxSize ; ++x )
	{
		if( m_pProjectMenu->FindItem( x+m_nProjectBaseId ) )
		{
			m_pProjectMenu->Delete( x+m_nProjectBaseId );
		}
		if( m_pPathMenu->FindItem( x+m_nPathBaseId ) )
		{
			m_pPathMenu->Delete( x+m_nPathBaseId );
		}
	}

	// Add the MRU items to the menu.
	for( unsigned x = 0 ; x < m_pathHistory.size() && x<m_nMaxSize ; ++x )
	{
		wxString strText = wxConvUTF8.cMB2WC( m_pathHistory[x].c_str());
		m_pPathMenu->Append( m_nPathBaseId + x, strText );
	}
	// Add the MRU items to the menu.
	for( unsigned x = 0 ; x < m_projectHistory.size() && x<m_nMaxSize ; ++x )
	{
		std::string strStdText = std::string("<") + m_projectHistory[x].first + ">: " + m_projectHistory[x].second;
		wxString strText = wxConvUTF8.cMB2WC( strStdText.c_str());
		m_pProjectMenu->Append( m_nProjectBaseId + x, strText );
	}
}
//------------------------------------------------------------------------------
void	CToolCache::SaveFile(  )
{
	TiXmlDocument aDoc;
	TiXmlNode* pRootNode = aDoc.InsertEndChild( TiXmlElement( "cache" ));

	TiXmlNode* pProjectNode = pRootNode->InsertEndChild( TiXmlElement("projects"));
	TiXmlNode* pPathsNode = pRootNode->InsertEndChild( TiXmlElement("paths"));

	//project
	for( unsigned i=0; i<m_projectHistory.size(); ++i )
	{
		TiXmlElement aHistoryNode("history");
		aHistoryNode.SetAttribute("project", m_projectHistory[i].first.c_str());
		aHistoryNode.SetAttribute("path", m_projectHistory[i].second.c_str());
		pProjectNode->InsertEndChild(aHistoryNode);
	}

	//path
	for( unsigned i=0; i<m_pathHistory.size(); ++i )
	{
		TiXmlElement aHistoryNode("history");
		aHistoryNode.SetAttribute("path", m_pathHistory[i].c_str());
		pPathsNode->InsertEndChild(aHistoryNode);
	}

	aDoc.SaveFile(m_strCacheFile.c_str());
}
//------------------------------------------------------------------------------




