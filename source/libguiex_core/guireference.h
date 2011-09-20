/** 
 * @file guireference.h
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2011-03-15
 */


#ifndef __KEN_GUIREFERENCE_20100315_H__
#define	__KEN_GUIREFERENCE_20100315_H__


//============================================================================//
// include
//============================================================================//
#include "guibase.h"


//============================================================================//
// class
//============================================================================//
namespace guiex
{
	class GUIEXPORT CGUIReference
	{
	public:
		CGUIReference( );
		virtual ~CGUIReference();

		virtual void RefRetain();
		virtual void RefRelease();
		void RefClear();
		uint32 GetRefCount() const;

	protected:
		void DoIncreaseReference();
		void DoDecreaseReference();

	private:
		uint32 m_nReferenceCount;
	};
}

#endif //__KEN_GUIREFERENCE_20100315_H__
