/** 
 * @file guicameramanager.cpp
 * @brief camera manager used in system
 * @author ken
 * @date 2010-12-22
 */


//============================================================================//
// include
//============================================================================// 
#include "guicameramanager.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUICameraManager * CGUICameraManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUICameraManager::CGUICameraManager()
	{
		GUI_ASSERT( !m_pSingleton, "[CGUICameraManager::CGUICameraManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUICameraManager::~CGUICameraManager()
	{
	}
	//------------------------------------------------------------------------------
	CGUICameraManager* CGUICameraManager::Instance()
	{
		GUI_ASSERT( m_pSingleton, "not initialized" );
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------

}
