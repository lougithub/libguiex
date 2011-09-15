/** 
 * @file toolhistory.cpp
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2009-11-05
 */

//============================================================================//
// include
//============================================================================//
#include "toolshistory.h"
#include "toolsmisc.h"
#include "tinyxml.h"
#include <algorithm>


//============================================================================//
// function
//============================================================================//

//------------------------------------------------------------------------------
CToolsHistory::CToolsHistory()
:m_nPathBaseId( 0 )
,m_nSceneBaseId( 0 )
,m_nMaxSize(10)
{
	m_pPathMenu = new wxMenu( );
	m_pSceneMenu = new wxMenu( );
}
//------------------------------------------------------------------------------
CToolsHistory::~CToolsHistory()
{
}
//------------------------------------------------------------------------------
CToolsHistory* CToolsHistory::Instance()
{
	static CToolsHistory s_mgr;
	return &s_mgr;
}
//------------------------------------------------------------------------------
void CToolsHistory::ParseHistoryFile( const std::string& rCacheFile )
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
		else if( std::string("default_editor") == pNode->Value() )
		{
			const char* path = pNode->Attribute("path");
			m_strDefaultEditor = path;
		}

		pNode = pNode->NextSiblingElement();
	}
}
//------------------------------------------------------------------------------
void CToolsHistory::AddScenePath( const std::string& rScene, const std::string& rPath )
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
const std::vector<std::string>& CToolsHistory::GetHistoryPaths() const
{
	return m_pathHistory;
}
//------------------------------------------------------------------------------
const std::vector<std::pair< std::string, std::string> >& CToolsHistory::GetHistoryScenes() const
{
	return m_sceneHistory;
}
//------------------------------------------------------------------------------
wxMenu* CToolsHistory::GetPathMenu() const
{
	return m_pPathMenu;
}
//------------------------------------------------------------------------------
wxMenu* CToolsHistory::GetSceneMenu() const
{
	return m_pSceneMenu;
}
//------------------------------------------------------------------------------
void CToolsHistory::UpdateMenu( )
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
void CToolsHistory::SaveFile(  )
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

	//default editor
	TiXmlElement aEditorNode("default_editor");
	aEditorNode.SetAttribute("path", m_strDefaultEditor.c_str());
	pRootNode->InsertEndChild(aEditorNode);

	aDoc.SaveFile(m_strCacheFile.c_str());
}
//------------------------------------------------------------------------------
void CToolsHistory::SetPathsBaseId(wxWindowID baseId)
{
	m_nPathBaseId = baseId;
}
//------------------------------------------------------------------------------
void CToolsHistory::SetScenesBaseId(wxWindowID baseId)
{
	m_nSceneBaseId = baseId;
}
//------------------------------------------------------------------------------
void CToolsHistory::SetMaxCacheSize( unsigned nSize )
{
	m_nMaxSize = nSize;
}
//------------------------------------------------------------------------------
void CToolsHistory::SetDefaultEditor(const std::string& rEditor)
{
	m_strDefaultEditor = rEditor;

	SaveFile();
	UpdateMenu();
}
//------------------------------------------------------------------------------
const std::string& CToolsHistory::GetDefaultEditor() const
{
	return m_strDefaultEditor;
}
//------------------------------------------------------------------------------



