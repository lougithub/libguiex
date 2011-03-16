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


//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIScene::CGUIScene( const CGUIString& rSceneName )
		:m_strSceneName( rSceneName )
		,m_bDependenciesLoaded(false)
	{
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
	/** 
	* @brief read config file
	* @return 0 for success, others for failed
	*/
	int32 CGUIScene::LoadFromPropertySet( const CGUIString& rScenePath, const CGUIProperty& aPropertySet )
	{
		m_strScenePath = rScenePath;

		uint32 nSize = aPropertySet.GetPropertyNum();
		for( uint32 i=0; i<nSize; ++i )
		{
			const CGUIProperty* pProperty = aPropertySet.GetProperty(i);
			switch( pProperty->GetType() )
			{
			case ePropertyType_Scene_Title:
				m_strTitle = pProperty->GetName();
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
			default:
				throw CGUIException("[CGUIScene::LoadFromPropertySet]: unknown property <%s:%s> in scene <%s>",
					pProperty->GetName().c_str(),
					pProperty->GetTypeAsString().c_str(),
					GetSceneName().c_str() );
				return -1;
			}
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
	const std::vector<CGUIString>& CGUIScene::GetDependencies() const
	{
		return m_vecDependencies;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief get scene title
	*/
	const CGUIString& CGUIScene::GetTitle() const
	{
		return m_strTitle;
	}
	//------------------------------------------------------------------------------
	int32 CGUIScene::DoLoad()
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIScene::DoUnload()
	{

	}
	//------------------------------------------------------------------------------
}
