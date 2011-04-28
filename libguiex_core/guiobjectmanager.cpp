/** 
 * @file guiobjectmanager.cpp
 * @brief object manager, used to manage to free object automatically
 * @author ken
 * @date 2006-05-31
 */


//============================================================================//
// include 
//============================================================================// 
#include "guiobjectmanager.h"
#include "guicleanup.h"


namespace guiex
{
//============================================================================//
// class
//============================================================================// 
class GUIEXPORT CGUIObjectManagerManager
{
public:
	CGUIObjectManagerManager (void);
	~CGUIObjectManagerManager (void);
};
//------------------------------------------------------------------------------
CGUIObjectManagerManager::CGUIObjectManagerManager (void)
{
	// Ensure that the Object_Manager gets initialized before any
	// application threads have been spawned.  Because this will be called
	// during construction of static objects, that should always be the
	// case.
	CGUIObjectManager::Instance ();
}
//------------------------------------------------------------------------------
CGUIObjectManagerManager::~CGUIObjectManagerManager (void)
{
	delete CGUIObjectManager::m_pInstance;
	CGUIObjectManager::m_pInstance = NULL;

	// if this destructor is not called by the main thread, then do
	// not delete the CGUIObjectManagerManager.  That causes problems, on
	// WIN32 at least.
}
//------------------------------------------------------------------------------
static CGUIObjectManagerManager GUI_Object_Manager_Manager_instance;
//------------------------------------------------------------------------------


//============================================================================//
// function
//============================================================================// 

//------------------------------------------------------------------------------
CGUIObjectManager *CGUIObjectManager::m_pInstance = NULL;
//------------------------------------------------------------------------------
CGUIObjectManager::CGUIObjectManager (void)
{
	// If m_pInstance was not NULL, then another CGUIObjectManager has
	// already been instantiated (it is likely to be one initialized by way
	// of library/DLL loading).  Let this one go through construction in
	// case there really is a good reason for it, but the original one will be 
	// the one retrieved from calls to CGUIObjectManager::Instance().

	// Be sure that no further instances are created via instance ().
	if (m_pInstance == NULL)
	{
		m_pInstance = this;
	}

	Init ();
}
//------------------------------------------------------------------------------
CGUIObjectManager::~CGUIObjectManager (void)
{
	m_bDynamicallyAllocated = false;   // Don't delete this again in Fini()
	Fini ();
}
//------------------------------------------------------------------------------
CGUIObjectManager * CGUIObjectManager::Instance (void)
{
	if (m_pInstance == NULL)
	{
		CGUIObjectManager *pInstance = new CGUIObjectManager;
		if( !pInstance )
		{
			return NULL;
		}
		pInstance->m_bDynamicallyAllocated = true;
		return pInstance;
	}
	else
	{
		return m_pInstance;
	}
}
//------------------------------------------------------------------------------
bool CGUIObjectManager::IsStartingUp (void)
{
	return CGUIObjectManager::m_pInstance  ?  m_pInstance->IsStartingUp_i ()  :  true;
}
//------------------------------------------------------------------------------
bool CGUIObjectManager::IsShuttingDown (void)
{
	return CGUIObjectManager::m_pInstance  ?  m_pInstance->IsShuttingDown_i ()  :  true;
}
//------------------------------------------------------------------------------
// Initialize an CGUIObjectManager.  There can be instances of this object
// other than The Instance.  This can happen if a user creates one for some
// reason. 
int32 CGUIObjectManager::Init (void)
{
	if (IsStartingUp_i ())
	{
		// First, indicate that the CGUIObjectManager instance is being
		// initialized.
		m_eObjectManagerState = OBJ_MAN_INITIALIZING;

		// Finally, indicate that the CGUIObjectManager instance has
		// been initialized.
		m_eObjectManagerState = OBJ_MAN_INITIALIZED;

		return 0;
	} 
	else 
	{
		// Had already initialized.
		return 1;
	}
}
//------------------------------------------------------------------------------
// Clean up an CGUIObjectManager.  There can be instances of this object
// other than The Instance.  
int32 CGUIObjectManager::Fini (void)
{
	if (IsShuttingDown_i ())
	{
		// Too late.  Or, maybe too early.  Either fini () has already
		// been called, or init () was never called.
		return m_eObjectManagerState == OBJ_MAN_SHUT_DOWN  ?  1  :  -1;
	}

	// Indicate that this CGUIObjectManager instance is being
	// shut down.
	m_eObjectManagerState = OBJ_MAN_SHUTTING_DOWN;

	//cleanup the object
	UnregisterAll();



	// Indicate that this CGUIObjectManager instance has been shut down.
	m_eObjectManagerState = OBJ_MAN_SHUT_DOWN;


	if (m_bDynamicallyAllocated)
	{
		delete this;
	}

	if (this == m_pInstance)
	{
		m_pInstance = NULL;
	}

	return NULL;
}
//------------------------------------------------------------------------------
int32 CGUIObjectManager::Register(CGUICleanup *object)
{
	SCleanupNode aNode;
	aNode.m_pFunc = (GUI_CLEANUP_FUNC)GUI_Cleanup_Destroyer;
	aNode.m_pPointer = object;
	m_listCleanup.push_back( aNode );
	return 0;
}
//------------------------------------------------------------------------------
int32 CGUIObjectManager::Unregister(CGUICleanup *object)
{
	std::list<SCleanupNode>::iterator itorEnd = m_listCleanup.end();
	for( std::list<SCleanupNode>::iterator itor=m_listCleanup.begin();
		itor != itorEnd;
		itor++)
	{
		if((*itor).m_pPointer == object )
		{
			(*itor).m_pFunc((*itor).m_pPointer);
			m_listCleanup.erase(itor);
			return 0;
		}
	}
	return -1;
}
//------------------------------------------------------------------------------
void CGUIObjectManager::UnregisterAll()
{
	std::list<SCleanupNode>::reverse_iterator itorEnd = m_listCleanup.rend();
	for( std::list<SCleanupNode>::reverse_iterator itor=m_listCleanup.rbegin();
		itor != itorEnd;
		itor++)
	{
		(*itor).m_pFunc((*itor).m_pPointer);
	}
	m_listCleanup.clear();
}
//------------------------------------------------------------------------------
}//namespace guiex

