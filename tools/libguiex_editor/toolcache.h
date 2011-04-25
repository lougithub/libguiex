/** 
 * @file toolcache.h
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2009-11-05
 */

#ifndef __KEN_TOOLCACHE_200911015_H__
#define	__KEN_TOOLCACHE_200911015_H__


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
class CToolCache
{
public:
	~CToolCache();

	static CToolCache* Instance();

	void ParseCache( const std::string& rCacheFile );

	void AddCache( const std::string& rScene, const std::string& rPath );	

	void SetPathsBaseId(wxWindowID baseId)
	{
		m_nPathBaseId = baseId;
	}
	void SetScenesBaseId(wxWindowID baseId)
	{
		m_nSceneBaseId = baseId;
	}

	void SetMaxCacheSize( unsigned nSize )
	{
		m_nMaxSize = nSize;
	}

	void SetDefaultEditor(const std::string& rEditor)
	{
		m_strDefaultEditor = rEditor;

		SaveFile();
		UpdateMenu();
	}
	const std::string& GetDefaultEditor() const
	{
		return m_strDefaultEditor;
	}

	void UpdateMenu( );

protected:
	void SaveFile(  );

public:
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
	CToolCache();
	CToolCache& operator=(CToolCache&);
};


#endif //__KEN_FILEHISTORY_200911015_H__
