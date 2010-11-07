/** 
* @file guiprojectinfo.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-22
*/

//============================================================================//
// include
//============================================================================//
#include <libguiex_core/guiprojectinfo.h>
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
	CGUIProjectInfo::CGUIProjectInfo()
		:m_bDependenciesLoaded(false)
		,m_bResourceLoaded(false)
	{
		Reset();
	}
	//------------------------------------------------------------------------------
	CGUIProjectInfo::~CGUIProjectInfo()
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIProjectInfo::Reset()
	{
		m_strProjectFilename.empty();
		m_strProjectFilePath.empty();

		m_vecWidgetFiles.clear();
		m_vecScriptFiles.clear();
		m_vecResourceFiles.clear();
		m_vecFontInfos.clear();

		m_vecDependencies.clear();
		m_strTitle.clear();
	}
	//------------------------------------------------------------------------------
	bool	CGUIProjectInfo::IsDependenciesLoaded() const
	{
		return m_bDependenciesLoaded;
	}
	//------------------------------------------------------------------------------
	void	CGUIProjectInfo::SetDependenciesLoaded( bool bLoaded )
	{
		m_bDependenciesLoaded = bLoaded;
	}
	//------------------------------------------------------------------------------
	bool	CGUIProjectInfo::IsResourceLoaded() const
	{
		return m_bResourceLoaded;
	}
	//------------------------------------------------------------------------------
	void	CGUIProjectInfo::SetResourceLoaded( bool bLoaded )
	{
		m_bResourceLoaded = bLoaded;
	}
	//------------------------------------------------------------------------------
	const CGUIString&		CGUIProjectInfo::GetProjectFilePath() const
	{
		return m_strProjectFilePath;
	}
	//------------------------------------------------------------------------------
	const CGUIString&		CGUIProjectInfo::GetProjectFilename() const
	{
		return m_strProjectFilename;
	}
	//------------------------------------------------------------------------------
	int32		CGUIProjectInfo::LoadFromPropertySet( 
		const CGUIString& rProjectFileName, 
		const CGUIString& rProjectFilePath, 
		const CGUIProperty& aPropertySet )
	{
		Reset();

		m_strProjectFilename = rProjectFileName;
		m_strProjectFilePath = rProjectFilePath;

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
				throw guiex::CGUIException("[CGUIProjectInfo::LoadFromPropertySet], unknown property name! <%s>!", pProperty->GetName().c_str());
				return -1;
			}
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>&	CGUIProjectInfo::GetWidgetFiles() const
	{
		return m_vecWidgetFiles;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>&	CGUIProjectInfo::GetScriptFiles() const
	{
		return m_vecScriptFiles;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>&	CGUIProjectInfo::GetResourceFiles() const
	{
		return m_vecResourceFiles;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIProjectInfo::SFontInfo>&	CGUIProjectInfo::GetFontInfos() const
	{
		return m_vecFontInfos;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>&	CGUIProjectInfo::GetDependencies() const
	{
		return m_vecDependencies;
	}
	//------------------------------------------------------------------------------
	const CGUIString&		CGUIProjectInfo::GetTitle() const
	{
		return m_strTitle;
	}
	//------------------------------------------------------------------------------
}
