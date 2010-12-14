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
#include "guistring.h"
#include <vector>
#include <map>


//============================================================================//
// declare
//============================================================================//
namespace guiex
{
	class CGUIScene;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUISceneManager
	{
	public:
		static CGUISceneManager* Instance();

		int32 RegisterScenesFromDir( const CGUIString& rSceneRootPath = "/", const CGUIString& rSuffix = ".uip", bool bAutoBuildDepency = true );
		int32 RegisterScene( const CGUIString& rSceneFilePath, bool bAutoBuildDepency = true );
		void UnregisterAllScenes( );
		void UnregisterScene( const CGUIString& rSceneName );

		int32 BuildScenesDependency( );
		int32 CheckScenesDependency( const CGUIString& rSceneName );

		CGUIScene* GetScene( const CGUIString& rSceneName ) const;

		int32 LoadResource(const CGUIString& strSceneName);
		int32 ReleaseResource(const CGUIString& strSceneName);
		void ReleaseAllResources( );

		CGUIString GetScenePath( const CGUIString& rSceneName ) const;
		const std::vector<CGUIString>& GetSceneNames( ) const;

	protected:
		friend class CGUISystem;
		CGUISceneManager();
		~CGUISceneManager();

		CGUIScene* GenerateScene( const CGUIString& rSceneName ) const;
		void DestroyScene( CGUIScene* pSceneInfo) const;

		void LoadResourceImp( CGUIScene* pScene );
		void ReleaseResourceImp( CGUIScene* pScene );

	protected:
		std::map<CGUIString, CGUIScene*> m_mapScenes; 
		std::vector<CGUIString> m_vecSceneNames; //scene names

	private:
		static CGUISceneManager* m_pSingleton;
	};
} //namespace guiex


#endif //__KEN_GUISCENEINFOMANAGER_20091022_H__
