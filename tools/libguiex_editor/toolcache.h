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
#include <string>
#include <map>




//============================================================================//
// declare
//============================================================================//





//============================================================================//
// class
//============================================================================//
class CToolCache
{
public:
	~CToolCache();

	static CToolCache* Instance();

	void	ParseCache( const std::string& rCacheFile );

	void	AddCache( const std::string& rProject, const std::string& rPath );	

	void	SetPathsBaseId(wxWindowID baseId)
	{
		m_nPathBaseId = baseId;
	}
	void	SetProjectsBaseId(wxWindowID baseId)
	{
		m_nProjectBaseId = baseId;
	}

	void	SetMaxCacheSize( unsigned nSize )
	{
		m_nMaxSize = nSize;
	}

	void	UpdateMenu( );

protected:
	void	SaveFile(  );

public:
	std::string	m_strCacheFile;

	std::vector<std::pair< std::string, std::string> >	m_projectHistory;	//<project, path>
	std::vector<std::string>	m_pathHistory;

	wxWindowID	m_nPathBaseId;
	wxWindowID	m_nProjectBaseId;
	wxMenu		*m_pPathMenu;
	wxMenu		*m_pProjectMenu;

	unsigned			m_nMaxSize;

protected:
	CToolCache();
	CToolCache& operator=(CToolCache&);
};


#endif //__KEN_FILEHISTORY_200911015_H__
