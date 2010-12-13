/** 
* @file guisceneinfo.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-22
*/

//============================================================================//
// include
//============================================================================//
#include <libguiex_core/guisceneinfo.h>
#include <libguiex_core/guiinterfacefilesys.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guistring.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiproperty.h>


//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUISceneInfo::CGUISceneInfo( const CGUIString& rSceneName )
		:CGUIResource( rSceneName, rSceneName, "SCENE" )
		,m_bDependenciesLoaded(false)
		,m_bResourceLoaded(false)
	{
		Reset();
	}
	//------------------------------------------------------------------------------
	CGUISceneInfo::~CGUISceneInfo()
	{
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief reset data
	*/
	void CGUISceneInfo::Reset()
	{
		m_strSceneFilePath.empty();

		m_vecWidgetFiles.clear();
		m_vecScriptFiles.clear();
		m_vecResourceFiles.clear();

		m_vecDependencies.clear();
		m_strTitle.clear();
	}
	//------------------------------------------------------------------------------
	bool CGUISceneInfo::IsDependenciesLoaded() const
	{
		return m_bDependenciesLoaded;
	}
	//------------------------------------------------------------------------------
	void CGUISceneInfo::SetDependenciesLoaded( bool bLoaded )
	{
		m_bDependenciesLoaded = bLoaded;
	}
	//------------------------------------------------------------------------------
	bool CGUISceneInfo::IsResourceLoaded() const
	{
		return m_bResourceLoaded;
	}
	//------------------------------------------------------------------------------
	void CGUISceneInfo::SetResourceLoaded( bool bLoaded )
	{
		m_bResourceLoaded = bLoaded;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief get scene file path
	*/
	const CGUIString& CGUISceneInfo::GetScenePath() const
	{
		return m_strSceneFilePath;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief read config file
	* @return 0 for success, others for failed
	*/
	int32 CGUISceneInfo::LoadFromPropertySet( 
		const CGUIString& rSceneFilePath, 
		const CGUIProperty& aPropertySet )
	{
		Reset();

		m_strSceneFilePath = rSceneFilePath;

		uint32 nSize = aPropertySet.GetPropertyNum();
		for( uint32 i=0; i<nSize; ++i )
		{
			const CGUIProperty* pProperty = aPropertySet.GetProperty(i);
			if( pProperty->GetName() == "widget_file" )
			{
				m_vecWidgetFiles.push_back(pProperty->GetValue());
			}
			else if( pProperty->GetName() == "script_file")
			{
				m_vecScriptFiles.push_back(pProperty->GetValue());
			}
			else if( pProperty->GetName() == "resource_file")
			{
				m_vecResourceFiles.push_back(pProperty->GetValue());
			}
			else if( pProperty->GetName() == "title")
			{
				m_strTitle = pProperty->GetValue();
			}
			else if( pProperty->GetName() == "dependencies")
			{
				m_vecDependencies.push_back( pProperty->GetValue() );
			}
			else
			{
				throw guiex::CGUIException("[CGUISceneInfo::LoadFromPropertySet], unknown property name! <%s>!", pProperty->GetName().c_str());
				return -1;
			}
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>& CGUISceneInfo::GetWidgetFiles() const
	{
		return m_vecWidgetFiles;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>& CGUISceneInfo::GetScriptFiles() const
	{
		return m_vecScriptFiles;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>& CGUISceneInfo::GetResourceFiles() const
	{
		return m_vecResourceFiles;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>& CGUISceneInfo::GetDependencies() const
	{
		return m_vecDependencies;
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief get scene title
	*/
	const CGUIString& CGUISceneInfo::GetTitle() const
	{
		return m_strTitle;
	}
	//------------------------------------------------------------------------------
	int32 CGUISceneInfo::DoLoad() const
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUISceneInfo::DoUnload()
	{

	}
	//------------------------------------------------------------------------------
}
