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
#include "guiresource.h"
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
	class GUIEXPORT CGUISceneInfo : public CGUIResource
	{
	public:
		~CGUISceneInfo();

		void Reset();

		int32 LoadFromPropertySet( const CGUIString& rSceneFilePath, const CGUIProperty& aPropertySet );

		const CGUIString& GetScenePath() const;
		const CGUIString& GetTitle() const;

		const std::vector<CGUIString>&	GetWidgetFiles() const;
		const std::vector<CGUIString>&	GetScriptFiles() const;
		const std::vector<CGUIString>&	GetResourceFiles() const;
		const std::vector<CGUIString>&	GetDependencies() const;

		bool IsDependenciesLoaded() const;
		void SetDependenciesLoaded( bool bLoaded );

		bool IsResourceLoaded() const;
		void SetResourceLoaded( bool bLoaded );

	protected:
		friend class CGUISceneInfoManager;
		CGUISceneInfo( const CGUIString& rSceneName );

		virtual int32 DoLoad() const;
		virtual void DoUnload();

	protected:
		CGUISceneInfo( const CGUISceneInfo& );
		const CGUISceneInfo& operator=(const CGUISceneInfo& );

	protected:
		CGUIString	m_strSceneFilePath; //!< path of scene file

		//files
		std::vector<CGUIString> m_vecWidgetFiles; //widget file info
		std::vector<CGUIString> m_vecScriptFiles; //script file info
		std::vector<CGUIString> m_vecResourceFiles; //resource file list

		std::vector<CGUIString>	m_vecDependencies; //dependent scenes
		bool m_bDependenciesLoaded; //whether the dependencies has been loaded
		bool m_bResourceLoaded; //whether the resource of this scene has been loaded

		CGUIString m_strTitle; //title of this scene
	};
} //namespace guiex


#endif //__KEN_SCENEINFO_20091022_H__

