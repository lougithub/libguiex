/** 
* @file guiobjectmanager.h
* @brief object manager, used to free object automatically
* @author ken
* @date 2006-05-31
*/


#ifndef __GUI_OBJECTMANAGER_20060531_H__
#define __GUI_OBJECTMANAGER_20060531_H__

//============================================================================//
// include
//============================================================================// 
#include "guiobjectmanagerbase.h"
#include <list>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUICleanup;
	typedef void (*GUI_CLEANUP_FUNC)(void *object);
}//namespace guiex

//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	/**
	* @class CGUIObjectManager
	* @brief manager for singleton cleanup and other objects at program
	* termination.it provides an interface for application to register
	* objects to be cleaned up.the order of such cleanup calls is in the
	* reverse order of registration.
	*/
	class GUIEXPORT CGUIObjectManager : public CGUIObjectManagerBase
	{

	public:
		/**
		* Accessor to singleton instance. 
		*/
		static CGUIObjectManager *Instance (void);

		/**
		* @brief Register an CGUICleanup object for cleanup at process
		* termination.  The object is deleted via the * <GUI_Cleanup_Destroyer>.  
		* @return 
		*		- 0 on success.  
		*		- -1 On failure
		*/
		int32 Register (CGUICleanup *object);

		/**
		* @brief unregister an CGUICleanup object 
		* @return 
		*		- 0 on success.  
		*		- -1 On failure
		*/
		int32 Unregister(CGUICleanup *object);

		/**
		* @brief unregister an CGUICleanup object 
		*/
		void UnregisterAll();

		/**
		* @return Returns true before the CGUIObjectManager has been constructed.
		* This flag can be used to determine if the program is constructing
		* static objects.  
		*/
		static bool IsStartingUp (void);

		/**
		* @return Returns true after the CGUIObjectManager has been destroyed.  This
		* flag can be used to determine if the program is in the midst of
		* destroying static objects.
		*/
		static bool IsShuttingDown (void);

	protected:
		/**
		* @brief Explicitly initialize the 
		* @return 
		*	- 0 on success, 
		*	- -1 on failure
		*	- 1 if it had already been called.
		*/
		virtual int32 Init (void);

		/**
		* @brief explicitly destroy the instance of the CGUIObjectManager
		* @return
		*	- 0 on success
		*	- -1 on failure
		*	- 1 if it had already been called.
		*/
		virtual int32 Fini (void);

	protected:
		// Application code should not use these explicitly, so they're
		// hidden here. 
		CGUIObjectManager(void);
		virtual ~CGUIObjectManager(void);

	private:
		// Disallow copying by not implementing the following . . .
		CGUIObjectManager(const CGUIObjectManager &);
		CGUIObjectManager &operator= (const CGUIObjectManager &);

	private:
		friend class CGUIObjectManagerManager;

		static CGUIObjectManager *m_pInstance;/// Singleton pointer.

		struct SCleanupNode
		{
			void*				m_pPointer;
			GUI_CLEANUP_FUNC	m_pFunc;
		};
		std::list<SCleanupNode>		m_listCleanup;
	};


}//namespace


#endif //__GUI_OBJECTMANAGER_20060531_H__
