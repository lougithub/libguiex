/** 
* @file guiscene.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-22
*/

//============================================================================//
// include
//============================================================================//
#include "guiscene.h"
#include "guiinterfacefilesys.h"
#include "guiinterfacemanager.h"
#include "guistring.h"
#include "guiexception.h"
#include "guiproperty.h"
#include "guipropertymanager.h"
#include "guiinterfaceconfigfile.h"
#include "guiinterfacemanager.h"



//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIScene::CGUIScene( const CGUIString& rSceneConfigFileName )
		:m_strSceneConfigFileName(rSceneConfigFileName)
		,m_bDependenciesLoaded(false)
	{
		m_strScenePath = CGUIInterfaceManager::Instance()->GetInterfaceFileSys()->GetFileDir( m_strSceneConfigFileName );
	}
	//------------------------------------------------------------------------------
	CGUIScene::~CGUIScene()
	{
	}
	//------------------------------------------------------------------------------
	bool CGUIScene::IsDependenciesLoaded() const
	{
		return m_bDependenciesLoaded;
	}
	//------------------------------------------------------------------------------
	void CGUIScene::SetDependenciesLoaded( bool bLoaded )
	{
		m_bDependenciesLoaded = bLoaded;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIScene::GetSceneName() const
	{
		return m_strSceneName;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief get scene file path
	*/
	const CGUIString& CGUIScene::GetScenePath() const
	{
		return m_strScenePath;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIScene::GetSceneConfigFileName() const
	{
		return m_strSceneConfigFileName;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief read config file
	* @return 0 for success, others for failed
	*/
	int32 CGUIScene::LoadConfigFile( )
	{
		//clear data
		m_strSceneName.clear();
		m_vecDependencies.clear();
		m_vecWidgetFiles.clear();
		m_vecScriptFiles.clear();
		m_vecResourceFiles.clear();
		m_vecLocalizationFiles.clear();

		//load data
		//get interface of config file
		IGUIInterfaceConfigFile* pConfigFile = CGUIInterfaceManager::Instance()->GetInterfaceConfigFile();
		if( !pConfigFile )
		{
			GUI_THROW( "[CGUIScene::LoadConfigFile]: failed to get config file interface." );
			return -1;
		}

		CGUIProperty aPropertySet;
		if( 0 != pConfigFile->LoadConfigFile( m_strSceneConfigFileName, aPropertySet ))
		{
			GUI_THROW( GUI_FORMAT( 
				"[CGUIScene::LoadConfigFile]: failed to read scene info config file <%s>.",
				m_strSceneConfigFileName.c_str()));
			return -1;
		}

		uint32 nSize = aPropertySet.GetPropertyCount();
		for( uint32 i=0; i<nSize; ++i )
		{
			const CGUIProperty* pProperty = aPropertySet.GetProperty(i);
			switch( pProperty->GetType() )
			{
			case ePropertyType_Scene_Name:
				m_strSceneName = pProperty->GetName();
				break;
			case ePropertyType_Scene_Dependency:
				m_vecDependencies.push_back( pProperty->GetName() );
				break;
			case ePropertyType_Scene_WidgetFile:
				m_vecWidgetFiles.push_back(pProperty->GetName());
				break;
			case ePropertyType_Scene_ScriptFile:
				m_vecScriptFiles.push_back(pProperty->GetName());
				break;
			case ePropertyType_Scene_ResourceFile:
				m_vecResourceFiles.push_back(pProperty->GetName());
				break;
			case ePropertyType_Scene_LocalizationFile:
				m_vecLocalizationFiles.push_back(pProperty->GetName());
				break;
			default:
				GUI_THROW( GUI_FORMAT("[CGUIScene::LoadConfigFile]: unknown property <%s:%s> in scene <%s>",
					pProperty->GetName().c_str(),
					pProperty->GetTypeAsString().c_str(),
					m_strSceneConfigFileName.c_str()));
				return -1;
			}
		}

		if( m_strSceneName.empty() )
		{
			GUI_THROW( GUI_FORMAT("[CGUIScene::LoadConfigFile]: unknown scene name in scene <%s>",
				m_strSceneConfigFileName.c_str()));
			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>& CGUIScene::GetWidgetFiles() const
	{
		return m_vecWidgetFiles;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>& CGUIScene::GetScriptFiles() const
	{
		return m_vecScriptFiles;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>& CGUIScene::GetResourceFiles() const
	{
		return m_vecResourceFiles;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>& CGUIScene::GetLocalizationFiles() const
	{
		return m_vecLocalizationFiles;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>& CGUIScene::GetDependencies() const
	{
		return m_vecDependencies;
	}
	//------------------------------------------------------------------------------
}
