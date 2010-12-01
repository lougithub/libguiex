/** 
* @file guisingleton.inl
* @brief singleton used in this system
* @author ken
* @date 2006-05-31
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guiobjectmanager.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	template <class TYPE> 
	CGUISingleton<TYPE> * CGUISingleton<TYPE>::m_pSingleton = NULL;
	//------------------------------------------------------------------------------
	template <class TYPE> 
	CGUISingleton<TYPE>::CGUISingleton( )
	{
	}
	//------------------------------------------------------------------------------
	template <class TYPE> 
	TYPE * CGUISingleton<TYPE>::Instance ()
	{
		CGUISingleton<TYPE> *&pSingleton = CGUISingleton<TYPE>::Instance_i ();

		// Perform the Double-Check pattern...
		if (pSingleton == NULL)
		{
			if (CGUIObjectManager::IsStartingUp () ||
				CGUIObjectManager::IsShuttingDown ())
			{
				// The program is still starting up, Or, the CGUIObjectManager
				// instance has been destroyed,so the preallocated lock is not 
				// available.  Either way,don't register for destruction with the
				// CGUIObjectManager:  we'll have to leak this instance.
				if( !(pSingleton = new CGUISingleton<TYPE> ))
				{
					return NULL;
				}
			}
			else
			{
				if( !(pSingleton = new CGUISingleton<TYPE> ))
				{
					return NULL;
				}

				// Register for destruction with CGUIObjectManager.
				CGUIObjectManager::Instance()->Register(pSingleton);
			}
		}

		return &pSingleton->m_aInstance;
	}
	//------------------------------------------------------------------------------
	template <class TYPE> 
	void CGUISingleton<TYPE>::DestroyInstance()
	{
		CGUISingleton<TYPE> *&pSingleton = CGUISingleton<TYPE>::Instance_i ();
		if (pSingleton != NULL)
		{
			CGUIObjectManager::Instance()->Unregister(pSingleton);
		}
		//else the instance of this singleton class has been destroyed
	}
	//------------------------------------------------------------------------------
	template <class TYPE> 
	CGUISingleton<TYPE> *&CGUISingleton<TYPE>::Instance_i (void)
	{
		return CGUISingleton<TYPE>::m_pSingleton;
	}
	//------------------------------------------------------------------------------
	template <class TYPE> void
		CGUISingleton<TYPE>::Cleanup ( )
	{
		delete this;
		CGUISingleton<TYPE>::Instance_i() = 0;
	}

	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------

}//namespace guiex

