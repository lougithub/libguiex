/** 
* @file guiparticleiterator.cpp
* @brief 
* @author ken
* @date 2011-01-19
*/

//============================================================================//
// include
//============================================================================// 
#include "guiparticleiterator.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//-----------------------------------------------------------------------
	CGUIParticleIterator::CGUIParticleIterator(
		std::list<CGUIParticle*>::iterator start, 
		std::list<CGUIParticle*>::iterator last)
	{
		mStart = mPos = start;
		mEnd = last;
	}
	//-----------------------------------------------------------------------
	bool CGUIParticleIterator::end(void)
	{
		return (mPos == mEnd);
	}
	//-----------------------------------------------------------------------
	CGUIParticle* CGUIParticleIterator::getNext(void)
	{
		return static_cast<CGUIParticle*>(*mPos++);
	}
	//-----------------------------------------------------------------------
}
