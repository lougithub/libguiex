/** 
 * @file toolcache.cpp
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2009-11-05
 */


//============================================================================//
// include
//============================================================================//
#include "toolcache.h"
#include "editorutility.h"

#include "tinyxml.h"
#include <algorithm>


//============================================================================//
// function
//============================================================================//

//------------------------------------------------------------------------------
CToolCache::CToolCache()
:m_nPathBaseId( 0 )
,m_nSceneBaseId( 0 )
,m_nMaxSize(10)
{
	m_pPathMenu = new wxMenu( );
	m_pSceneMenu = new wxMenu( );
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
	m_sceneHistory.clear();
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
		if( std::string("Scenes") == pNode->Value())
		{
			TiXmlElement* pHistoryNode = pNode->FirstChildElement();
			while( pHistoryNode )
			{
				if( std::string("history") == pHistoryNode->Value())
				{
					const char* Scene = pHistoryNode->Attribute("Scene");
					const char* path = pHistoryNode->Attribute("path");

					m_sceneHistory.push_back( std::make_pair( std::string(Scene), std::string(path)));
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
void	CToolCache::AddCache( const std::string& rScene, const std::string& rPath )
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


	//for Scene
	if( std::find( m_sceneHistory.begin(), m_sceneHistory.end(), std::make_pair(rScene, rPath) ) == m_sceneHistory.end())
	{
		m_sceneHistory.push_back(std::make_pair(rScene, rPath));
		while( !m_sceneHistory.empty() && m_sceneHistory.size() > m_nMaxSize )
		{
			m_sceneHistory.erase( m_sceneHistory.begin());
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
		if( m_pSceneMenu->FindItem( x+m_nSceneBaseId ) )
		{
			m_pSceneMenu->Delete( x+m_nSceneBaseId );
		}
		if( m_pPathMenu->FindItem( x+m_nPathBaseId ) )
		{
			m_pPathMenu->Delete( x+m_nPathBaseId );
		}
	}

	// Add the MRU items to the menu.
	for( unsigned x = 0 ; x < m_pathHistory.size() && x<m_nMaxSize ; ++x )
	{
		wxString strText = Gui2wxString( m_pathHistory[x]);
		m_pPathMenu->Append( m_nPathBaseId + x, strText );
	}
	// Add the MRU items to the menu.
	for( unsigned x = 0 ; x < m_sceneHistory.size() && x<m_nMaxSize ; ++x )
	{
		std::string strStdText = std::string("<") + m_sceneHistory[x].first + ">: " + m_sceneHistory[x].second;
		wxString strText = Gui2wxString( strStdText);
		m_pSceneMenu->Append( m_nSceneBaseId + x, strText );
	}
}
//------------------------------------------------------------------------------
void	CToolCache::SaveFile(  )
{
	TiXmlDocument aDoc;
	TiXmlNode* pRootNode = aDoc.InsertEndChild( TiXmlElement( "cache" ));

	TiXmlNode* pSceneNode = pRootNode->InsertEndChild( TiXmlElement("Scenes"));
	TiXmlNode* pPathsNode = pRootNode->InsertEndChild( TiXmlElement("paths"));

	//Scene
	for( unsigned i=0; i<m_sceneHistory.size(); ++i )
	{
		TiXmlElement aHistoryNode("history");
		aHistoryNode.SetAttribute("Scene", m_sceneHistory[i].first.c_str());
		aHistoryNode.SetAttribute("path", m_sceneHistory[i].second.c_str());
		pSceneNode->InsertEndChild(aHistoryNode);
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




