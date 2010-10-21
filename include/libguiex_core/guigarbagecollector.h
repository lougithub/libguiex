/** 
 * @file guigarbagecollector.h
 * @brief 
 * @author ken
 * @date 2006-09-07
 */

#ifndef __GUI_GARBAGE_COLLECTOR_H_20060907__
#define __GUI_GARBAGE_COLLECTOR_H_20060907__

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
 * @class CGUIAutoRelease
 * @brief call Release() automatically when this object is out of life scope
 */

template <class TYPE>
class CGUIAutoRelease
{
public:
	/// constructor
	CGUIAutoRelease(TYPE*	pType)
		:m_pType(pType)
	{
	}

	/// destructor
	~CGUIAutoRelease()
	{
		if( m_pType)
		{
			m_pType->Release();
			m_pType = NULL;
		}
	}

	/// operation for call as pointer
	TYPE	* operator->()
	{
		return m_pType;
	}

	/// operation for call as pointer
	bool operator!()
	{
		return !m_pType;
	}

	/// get pointer
	TYPE*	GetPointer()
	{
		return m_pType;
	}

protected:
	TYPE	*m_pType;
};

}//guiex

#endif //__GUI_GARBAGE_COLLECTOR_H_20060907__

