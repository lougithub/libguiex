/** 
* @file guiinterface.cpp
* @brief interface class
* @author ken
* @date 2006-04-04
*/

//============================================================================//
// include
//============================================================================// 
#include "guiinterface.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	IGUIInterface::IGUIInterface( const char* szModuleType, const char* szModuleName )
		:m_strModuleType( szModuleType )
		,m_strModuleName( szModuleName )
		,m_bInitialize(false)
	{
	}
	//------------------------------------------------------------------------------
	IGUIInterface::~IGUIInterface()
	{
	}
	//------------------------------------------------------------------------------
	int IGUIInterface::Initialize(void* pUserData)
	{
		if( m_bInitialize )
		{
			return 0;
		}
		else
		{
			if( 0 != DoInitialize(pUserData))
			{
				DoDestroy();
				return -1;
			}
			else
			{
				m_bInitialize = true;
				return 0;
			}
		}
	}
	//------------------------------------------------------------------------------ 
	void IGUIInterface::Destroy()
	{
		if( m_bInitialize )
		{
			DoDestroy();
			m_bInitialize = false;
		}
	}
	//------------------------------------------------------------------------------ 
	const CGUIString& IGUIInterface::GetModuleType( ) const
	{
		return m_strModuleType;
	}
	//------------------------------------------------------------------------------ 
	const CGUIString& IGUIInterface::GetModuleName( ) const
	{
		return m_strModuleName;
	}
	//------------------------------------------------------------------------------ 

}//namespace guiex
