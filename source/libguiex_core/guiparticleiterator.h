/** 
* @file guiparticleiterator.h
* @brief 
* @author ken
* @date 2011-01-19
*/

#ifndef __GUI_PARTICLE_20110119_H__
#define __GUI_PARTICLE_20110119_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include <list>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIParticle;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex 
{
	class CGUIParticleIterator
	{
	public:
		// Returns true when at the end of the particle list
		bool end(void);

		/** Returns a pointer to the next particle, and moves the iterator on by 1 element. */
		CGUIParticle* getNext(void);

	protected:
		/// Protected constructor, only available from ParticleSystem::getIterator
		CGUIParticleIterator(std::list<CGUIParticle*>::iterator start, std::list<CGUIParticle*>::iterator end);

	protected:
		friend class ParticleSystem;

		std::list<CGUIParticle*>::iterator mPos;
		std::list<CGUIParticle*>::iterator mStart;
		std::list<CGUIParticle*>::iterator mEnd;
	};
}

#endif	//__GUI_PARTICLE_20110119_H__

