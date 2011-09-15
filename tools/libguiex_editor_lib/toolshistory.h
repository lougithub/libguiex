/** 
 * @file toolhistory.h
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2009-11-05
 */

#ifndef __KEN_TOOLHISTORY_200911015_H__
#define	__KEN_TOOLHISTORY_200911015_H__


//============================================================================//
// include
//============================================================================//
#include <wx/wxprec.h>
#include <string>
#include <map>
#include <vector>

//============================================================================//
// class
//============================================================================//
class CToolsHistory
{
public:
	~CToolsHistory();

	static CToolsHistory* Instance();

	void ParseHistoryFile( const std::string& rCacheFile );

	void AddScenePath( const std::string& rScene, const std::string& rPath );
	const std::vector<std::string>& GetHistoryPaths() const;
	const std::vector<std::pair< std::string, std::string> >& GetHistoryScenes() const;
	wxMenu* GetPathMenu() const;
	wxMenu* GetSceneMenu() const;

	void SetDefaultEditor(const std::string& rEditor);
	const std::string& GetDefaultEditor() const;

	void SetPathsBaseId(wxWindowID baseId);
	void SetScenesBaseId(wxWindowID baseId);
	void SetMaxCacheSize( unsigned nSize );

	void UpdateMenu( );


protected:
	void SaveFile(  );

protected:
	std::string	m_strCacheFile;
	std::string m_strDefaultEditor;

	std::vector<std::pair< std::string, std::string> >	m_sceneHistory;	//<Scene, path>
	std::vector<std::string> m_pathHistory;

	wxWindowID m_nPathBaseId;
	wxWindowID m_nSceneBaseId;
	wxMenu *m_pPathMenu;
	wxMenu *m_pSceneMenu;

	unsigned m_nMaxSize;

protected:
	CToolsHistory();
	CToolsHistory& operator=(CToolsHistory&);
};


#endif //__KEN_TOOLHISTORY_200911015_H__
