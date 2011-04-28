/** 
 * @file guiobjectmanagerbase.cpp
 * @brief base class of CGUIObjectManager
 * @author ken
 * @date 2006-05-31
 */


//============================================================================//
// include 
//============================================================================// 
#include "guiobjectmanagerBase.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
//------------------------------------------------------------------------------
CGUIObjectManagerBase::CGUIObjectManagerBase (void)
:m_eObjectManagerState(OBJ_MAN_UNINITIALIZED)
,m_bDynamicallyAllocated (false)
{
}
//------------------------------------------------------------------------------
CGUIObjectManagerBase::~CGUIObjectManagerBase (void)
{
}
//------------------------------------------------------------------------------
bool CGUIObjectManagerBase::IsStartingUp_i ()
{
	return m_eObjectManagerState < OBJ_MAN_INITIALIZED;
}
//------------------------------------------------------------------------------
bool CGUIObjectManagerBase::IsShuttingDown_i ()
{
	return m_eObjectManagerState > OBJ_MAN_INITIALIZED;
}
//------------------------------------------------------------------------------
}//namespace guiex

