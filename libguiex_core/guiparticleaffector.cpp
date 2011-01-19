/** 
* @file guiparticleaffector.cpp
* @brief 
* @author ken
* @date 2011-01-18
*/

//============================================================================//
// include
//============================================================================// 
#include "guiparticleaffector.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//-----------------------------------------------------------------------
	CGUIParticleAffector::CGUIParticleAffector(CGUIParticleSystem* parent)
		: mParent(parent) 
	{
	}
	//-----------------------------------------------------------------------
	CGUIParticleAffector::~CGUIParticleAffector() 
	{
	}
	//-----------------------------------------------------------------------
	/** Method called to allow the affector to initialize all newly created particles in the system.
	@remarks
	This is where the affector gets the chance to initialize it's effects to the particles of a system.
	The affector is expected to initialize some or all of the particles in the system
	passed to it, depending on the affector's approach.
	@param
	pParticle Pointer to a Particle to initialize.
	*/
	void CGUIParticleAffector::_initParticle( CGUIParticle* /*pParticle*/ )
	{
		/* by default do nothing */
	}
	//-----------------------------------------------------------------------
}
