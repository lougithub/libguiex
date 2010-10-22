/** 
* @file guiresource.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-26
*/






//============================================================================//
// include
//============================================================================//
#include <libguiex_core\guibase.h>
#include <libguiex_core\guistring.h>
#include <libguiex_core\guiexception.h>
#include <libguiex_core\guiresource.h>





//============================================================================//
// function
//============================================================================//


namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIResource::CGUIResource(const CGUIString& rName, const CGUIString& rProjectName, const CGUIString& rResourceType)
		:m_strName(rName)
		,m_strProjectName(rProjectName)
		,m_eIsLoaded(LOADSTATE_Unloaded)
		,m_strResourceType(rResourceType)
	{
	}
	//------------------------------------------------------------------------------
	CGUIResource::~CGUIResource()
	{
	}
	//------------------------------------------------------------------------------
	int32	CGUIResource::Load()
	{
		if( m_eIsLoaded ==  LOADSTATE_Loaded)
		{
			return 0;
		}
		int32 ret = DoLoad();
		if( ret != 0 )
		{
			throw CGUIException( "[CGUIResource::Load]: failed to load resource <%s: %s>",
				m_strProjectName.c_str(),
				m_strName.c_str());
			return ret;
		}
		m_eIsLoaded = LOADSTATE_Loaded;
		return ret;
	}
	//------------------------------------------------------------------------------
	void	CGUIResource::Unload()
	{
		if( m_eIsLoaded == LOADSTATE_Unloaded )
		{
			return;
		}

		DoUnload();
		m_eIsLoaded = LOADSTATE_Unloaded;
	}
	//------------------------------------------------------------------------------
	bool	CGUIResource::IsLoaded() const
	{
		return m_eIsLoaded == LOADSTATE_Loaded;
	}
	//------------------------------------------------------------------------------
	const CGUIString&	CGUIResource::GetName() const
	{
		return m_strName;
	}
	//------------------------------------------------------------------------------
	const CGUIString&	CGUIResource::GetProjectName() const
	{
		return m_strProjectName;
	}
	//------------------------------------------------------------------------------
	const CGUIString&	CGUIResource::GetResourceType() const
	{
		return m_strResourceType;
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------


}
