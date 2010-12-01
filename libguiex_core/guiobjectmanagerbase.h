/** 
 * @file guiobjectmanagerbase.h
 * @brief base class of CGUIObjectManagerBase
 * @author ken
 * @date 2006-05-31
 */


#ifndef __GUI_OBJECTMANAGERBASE_H_20060531__
#define __GUI_OBJECTMANAGERBASE_H_20060531__


//============================================================================//
// include
//============================================================================// 
#include "guibase.h"


namespace guiex
{
//============================================================================//
// class
//============================================================================// 

/**
 * @class CGUIObjectManagerBase
 * @brief base class for CGUIObjectManager
 */
class GUIEXPORT CGUIObjectManagerBase
{
protected:

	/**
	 * @brief default constructor
	 */
	CGUIObjectManagerBase (void);

	/**
	 * @brief destructor
	 */
	virtual ~CGUIObjectManagerBase (void);

	/**
	 * @brief explicitly initialize manager
	 * @return 
	 *	- 0 for success
	 *	- -1 for failure due to dynamic allocation failure
	 *	- 1 if it has already been called.
	 */
	virtual int Init(void) = 0;

	/**
	 * @brief explicitly destroy manager.
	 * @return 
	 *	- 0 for success
	 *	- -1 for failure because the number of this function calls hasn't
	 *		 reached the number of Init() calls.
	 *	- 1 if it had already been called.
	 */
	virtual int Fini (void) = 0;

	enum EObject_Manager_State
	{
		OBJ_MAN_UNINITIALIZED = 0,
		OBJ_MAN_INITIALIZING,
		OBJ_MAN_INITIALIZED,
		OBJ_MAN_SHUTTING_DOWN,
		OBJ_MAN_SHUT_DOWN
	};

protected:
	/**
	 * @return return true before CGUIObjectManagerBase has been constructed.
	 * This flag can be used to determine if the program is constructing
	 * static objects.
	 */
	bool IsStartingUp_i (void);

	/**
	 * @return return true after CGUIObjectManagerBase has been destroyed.
	 * This flag can be used to determine if the program is in the midst of
	 * destroying static objects.
	 */
	bool IsShuttingDown_i(void);

protected:
	/// State of the Object_Manager;
	EObject_Manager_State m_eObjectManagerState;

	/**
	 * Flag indicating whether the ACE_Object_Manager was dynamically
	 * allocated by libGuiEX.  (If is was dynamically allocated by the
	 * application, then the application is responsible for destroying
	 * it.)
	 */
	bool m_bDynamicallyAllocated;

private:
	// Disallow copying by not implementing the following . . .
	CGUIObjectManagerBase (const CGUIObjectManagerBase &);
	CGUIObjectManagerBase &operator= (const CGUIObjectManagerBase &);
};


}//namespace guied


#endif // __GUI_OBJECTMANAGERBASE_H_20060531__ 
