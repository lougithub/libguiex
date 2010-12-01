/** 
* @file guisceneinfo.h
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-22
*/


#ifndef	__KEN_SCENEINFO_20091022_H__
#define __KEN_SCENEINFO_20091022_H__



//============================================================================//
// include
//============================================================================//
#include "guibase.h"
#include "vector"




//============================================================================//
// declare
//============================================================================//
namespace guiex
{
	class CGUIProperty;
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUISceneInfo
	{
	public:
		struct SFontInfo
		{
			CGUIString						m_strFontFile;
			uint32							m_nFontIndex;
		};

	public:
		CGUISceneInfo();
		~CGUISceneInfo();

		/** 
		* @brief reset data
		*/
		void	Reset();

		/** 
		* @brief read config file
		* @return 0 for success, others for failed
		*/
		int32		LoadFromPropertySet( 
			const CGUIString& rSceneFileName, 
			const CGUIString& rSceneFilePath, 
			const CGUIProperty& aPropertySet );

		/** 
		* @brief get scene file path
		*/
		const CGUIString&		GetScenePath() const;

		/** 
		* @brief get scene file name
		*/
		const CGUIString&		GetSceneFilename() const;

		/** 
		* @brief get scene title
		*/
		const CGUIString&		GetTitle() const;

		/** 
		* @brief get files
		*/
		const std::vector<CGUIString>&	GetWidgetFiles() const;
		const std::vector<CGUIString>&	GetScriptFiles() const;
		const std::vector<CGUIString>&	GetResourceFiles() const;
		const std::vector<SFontInfo>&	GetFontInfos() const;

		const std::vector<CGUIString>&	GetDependencies() const;

		bool	IsDependenciesLoaded() const;
		
		void	SetDependenciesLoaded( bool bLoaded );

		bool	IsResourceLoaded() const;
		
		void	SetResourceLoaded( bool bLoaded );

	protected:
		CGUIString	m_strSceneFilePath;		//!< path of scene file
		CGUIString	m_strSceneFilename;		//!< name of scene file

		//files
		std::vector<CGUIString>	m_vecWidgetFiles;			//widget file info
		std::vector<CGUIString>	m_vecScriptFiles;			//script file info
		std::vector<CGUIString>	m_vecResourceFiles;			//resource file list
		std::vector<SFontInfo>	m_vecFontInfos;				//font info

		std::vector<CGUIString>	m_vecDependencies;			//dependent scenes
		bool					m_bDependenciesLoaded;		//whether the dependencies has been loaded
		
		bool					m_bResourceLoaded;			//whether the resource of this scene has been loaded

		CGUIString				m_strTitle;			//title of this scene
	};
}		//namespace guiex


#endif //__KEN_SCENEINFO_20091022_H__

