/** 
 * @file guireference.cpp
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2011-03-15
 */



//============================================================================//
// include
//============================================================================//
#include "guireference.h"
#include "guiexception.h"



//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIReference::CGUIReference( )
		:m_nReferenceCount( 0 )
	{
	}
	//------------------------------------------------------------------------------
	CGUIReference::~CGUIReference()
	{
		if( m_nReferenceCount != 0 )
		{
			CGUIException::ThrowException("[CGUIReference::~CGUIReference]: the reference count is not equal zero");
		}
	}
	//------------------------------------------------------------------------------
	void CGUIReference::RefRetain()
	{
		DoIncreaseReference();
	}
	//------------------------------------------------------------------------------
	void CGUIReference::RefRelease()
	{
		DoDecreaseReference();
	}
	//------------------------------------------------------------------------------
	void CGUIReference::RefClear()
	{
		m_nReferenceCount = 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIReference::GetRefCount() const
	{
		return m_nReferenceCount;
	}
	//------------------------------------------------------------------------------
	void CGUIReference::DoIncreaseReference()
	{
		++m_nReferenceCount;
	}
	//------------------------------------------------------------------------------
	void CGUIReference::DoDecreaseReference()
	{
		if( m_nReferenceCount == 0)
		{
			CGUIException::ThrowException( "[CGUIReference::DoDecreaseReference]:invalid reference count" );
			return;
		}
		--m_nReferenceCount;
	}
	//------------------------------------------------------------------------------
}
