/** 
* @file guiparticleaffector.h
* @brief 
* @author ken
* @date 2011-01-18
*/

#ifndef __GUI_PARTICLEAFFECTOR_20110118_H__
#define __GUI_PARTICLEAFFECTOR_20110118_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIParticle;
	class CGUIParticleSystem;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex 
{
	class CGUIParticleAffector
	{
	public:
		CGUIParticleAffector(CGUIParticleSystem* parent);
		virtual ~CGUIParticleAffector();

		virtual void _initParticle( CGUIParticle* pParticle );

		/** Method called to allow the affector to 'do it's stuff' on all active particles in the system.
		@remarks
		This is where the affector gets the chance to apply it's effects to the particles of a system.
		The affector is expected to apply it's effect to some or all of the particles in the system
		passed to it, depending on the affector's approach.
		@param
		pSystem Pointer to a CGUIParticleSystem to affect.
		@param
		timeElapsed The number of seconds which have elapsed since the last call.
		*/
		virtual void _affectParticles(CGUIParticleSystem* pSystem, real timeElapsed) = 0;

		/** Returns the name of the type of affector. 
		@remarks
		This property is useful for determining the type of affector procedurally so another
		can be created.
		*/
		const CGUIString &getType(void) const 
		{
			return mType;
		}

	protected:
		/// Name of the type of affector, MUST be initialised by subclasses
		CGUIString mType;

		/** Internal method for setting up the basic parameter definitions for a subclass. 
		@remarks
		Because StringInterface holds a dictionary of parameters per class, subclasses need to
		call this to ask the base class to add it's parameters to their dictionary as well.
		Can't do this in the constructor because that runs in a non-virtual context.
		@par
		The subclass must have called it's own createParamDictionary before calling this method.
		*/
		void addBaseParameters(void) { /* actually do nothing - for future possible use */ }

		CGUIParticleSystem* mParent;
	};
}

#endif	//__GUI_PARTICLEAFFECTOR_20110118_H__

