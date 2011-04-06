/** 
* @file guiscene.h
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
#include "guireference.h"

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
	class GUIEXPORT CGUIScene : public CGUIReference
	{
	public:
		virtual ~CGUIScene();

		const CGUIString& GetSceneName() const;
		const CGUIString& GetScenePath() const;
		const CGUIString& GetSceneConfigFileName() const;

		const std::vector<CGUIString>& GetWidgetFiles() const;
		const std::vector<CGUIString>& GetScriptFiles() const;
		const std::vector<CGUIString>& GetResourceFiles() const;
		const std::vector<CGUIString>& GetLocalizationFiles() const;
		const std::vector<CGUIString>& GetDependencies() const;

		bool IsDependenciesLoaded() const;
		void SetDependenciesLoaded( bool bLoaded );

	protected:
		friend class CGUISceneManager;
		CGUIScene( const CGUIString& rSceneConfigFileName );

		int32 LoadConfigFile( );

	protected:
		CGUIScene( const CGUIScene& );
		const CGUIScene& operator=(const CGUIScene& );

	protected:
		//files
		std::vector<CGUIString> m_vecWidgetFiles; //widget file info
		std::vector<CGUIString> m_vecScriptFiles; //script file info
		std::vector<CGUIString> m_vecResourceFiles; //resource file list
		std::vector<CGUIString> m_vecLocalizationFiles; //localization file list

		std::vector<CGUIString>	m_vecDependencies; //dependent scenes
		bool m_bDependenciesLoaded; //whether the dependencies has been loaded
		
		CGUIString m_strSceneConfigFileName;
		CGUIString m_strScenePath;
		CGUIString m_strSceneName;
	};
} //namespace guiex


#endif //__KEN_SCENEINFO_20091022_H__

