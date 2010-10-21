/** 
 * @file guicleanup.h
 * @brief cleanup object
 * @author ken
 * @date 2006-05-31
 */


#ifndef __GUI_CLEANUO_H_20060531__
#define __GUI_CLEANUO_H_20060531__

//============================================================================//
// include
//============================================================================// 
#include "guiconfig.h"
#include "guitypes.h"


namespace guiex
{
//============================================================================//
// class
//============================================================================// 
/**
 * @class CGUICleanup
 * @brief Base class for objects that are cleaned by CGUIObjectManager.
 */
class GUIEXPORT CGUICleanup
{
public:
	/**
	 * @brief default constructor
	 */
	CGUICleanup (void);

	/**
	 * @brief destructor
	 */
	virtual ~CGUICleanup (void);

	/**
	 * @brief Cleanup method that, by default, simply deletes itself.
	 */
	virtual void Cleanup ();
};


/**
 * @class CGUICleanupAdapter
 *
 * @brief Adapter for CGUICleanup objects that allows them to be readily
 * managed by the CGUIObjectManager.
 *
 * This template class adapts an object of any type to be an
 * CGUICleanup object.  The object can then be destroyed
 * by the CGUIObjectManager. 
 */
template <class TYPE>
class CGUICleanupAdapter : public CGUICleanup
{
public:
	/// Default constructor.
	CGUICleanupAdapter (TYPE* pObject)
	{
		m_object = pObject;
	}

	/// Virtual destructor, needed by some compilers for vtable placement.
	virtual ~CGUICleanupAdapter (void)
	{
	}

	/// Accessor for contained object.
	virtual void Cleanup ()
	{
		delete m_object;
		TYPE::AfterDestroyInstance();
		delete this;
	}

private:
	/// Contained object.
	TYPE *m_object;
};

}//namespace guiex


namespace guiex
{
//============================================================================//
// function
//============================================================================// 
/**
 * @brief Adapter for cleanup, used by CGUIObjectManager.
 */
extern "C" GUIEXPORT void GUI_Cleanup_Destroyer (CGUICleanup *);

}//namespace guiex

#endif //__GUI_CLEANUO_H_20060531__
