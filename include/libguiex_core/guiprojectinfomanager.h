/** 
* @file GuiexProjectInfo.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-22
*/






#ifndef	__KEN_GUIEXPROJECTINFOMANAGER_20091022_H__
#define __KEN_GUIEXPROJECTINFOMANAGER_20091022_H__





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
	class CGUIProjectInfo;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUIProjectInfoManager
	{
	public:
		int32	LoadProjects( );
		void	UnloadProjects( );

		/**
		* @brief 
		* @param rProjectFile the filename of projects, for example <test.uip>
		*/
		CGUIProjectInfo*	GetProjectInfo( const CGUIString& rProjectName ) const;
		const CGUIString&	GetProjectFilePath( const CGUIString& rProjectName ) const;
		const std::vector<CGUIString>&	GetProjectFilePaths( ) const;
		const std::vector<CGUIString>&	GetProjectFileNames( ) const;

		void	ClearProjectLoadFlags( );

	protected:
		//for singleton
		CGUIProjectInfoManager();
		~CGUIProjectInfoManager();

	protected:
		std::map<CGUIString, CGUIProjectInfo*>	m_mapProjectInfos;	//
		std::vector<CGUIString>					m_vecProjectFilePaths;	//absolute file path
		std::vector<CGUIString>					m_vecProjectFileNames;	//file name

	public:
		//declare for singleton
		GUI_SINGLETON_DECLARE_EX(CGUIProjectInfoManager);	
	};
}		//namespace guiex


#endif //__KEN_GUIEXPROJECTINFOMANAGER_20091022_H__

