/** 
* @file guisceneInfomanager.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-22
*/


#ifndef	__KEN_GUISCENEINFOMANAGER_20091022_H__
#define __KEN_GUISCENEINFOMANAGER_20091022_H__

//============================================================================//
// include
//============================================================================//
#include "guibase.h"
#include "guisingleton.h"
#include "guistring.h"
#include <vector>
#include <map>


//============================================================================//
// declare
//============================================================================//

namespace guiex
{
	class IGUIInterfaceFileSys;
	class CGUISceneInfo;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUISceneInfoManager
	{
	public:
		int32 LoadScenes( const CGUIString& rSceneRootPath = "\\", const CGUIString& rSuffix = ".uip" );
		void UnloadScenes( );

		CGUISceneInfo* GetSceneInfo( const CGUIString& rSceneName ) const;
		CGUIString GetScenePath( const CGUIString& rSceneName ) const;
		const std::vector<CGUIString>& GetSceneFilePaths( ) const;
		const std::vector<CGUIString>& GetSceneFileNames( ) const;

		void ClearSceneResourceLoadFlags( );

		CGUISceneInfo* GenerateSceneInfo() const;
		void DestroySceneInfo( CGUISceneInfo* pSceneInfo) const;

	protected:
		//for singleton
		CGUISceneInfoManager();
		~CGUISceneInfoManager();

	protected:
		std::map<CGUIString, CGUISceneInfo*>	m_mapSceneInfos;	//
		std::vector<CGUIString>					m_vecSceneFilePaths;	//absolute file path
		std::vector<CGUIString>					m_vecSceneFileNames;	//file name

	public:
		//declare for singleton
		GUI_SINGLETON_DECLARE_EX(CGUISceneInfoManager);	
	};
}		//namespace guiex


#endif //__KEN_GUISCENEINFOMANAGER_20091022_H__
