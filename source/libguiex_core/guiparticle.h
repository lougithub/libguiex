/** 
* @file guiparticle.h
* @brief 
* @author ken
* @date 2011-01-18
*/

#ifndef __GUI_PARTICLE_20110118_H__
#define __GUI_PARTICLE_20110118_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guivector3.h"
#include "guicolor.h"
#include "guimath.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIParticleSystem;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex 
{
	/** Class representing a single particle instance. */
	class CGUIParticle
    {
	public:
		CGUIParticle();
		virtual ~CGUIParticle();

        void setDimensions(real width, real height); 
        bool hasOwnDimensions(void) const;

        real getOwnWidth(void) const;
        real getOwnHeight(void) const;
        
        void setRotation(const CGUIRadian& rad);
        const CGUIRadian& getRotation(void) const;

        void _notifyOwner(CGUIParticleSystem* owner);

        void resetDimensions(void);

	public:
		/// Type of particle
		enum EParticleType
		{
			Visual,
			Emitter
		};

		/// Does this particle have it's own dimensions?
		bool mOwnDimensions;
		/// Personal width if mOwnDimensions == true
		real mWidth;
		/// Personal height if mOwnDimensions == true
		real mHeight;
		/// Current rotation value
		CGUIRadian rotation;
		// Note the intentional public access to internal variables
		// Accessing via get/set would be too costly for 000's of particles
		/// World position
		CGUIVector3 position;
		/// Direction (and speed) 
		CGUIVector3 direction;
		/// Current colour
		CGUIColor colour;
		/// Time to live, number of seconds left of particles natural life
		real timeToLive;
		/// Total Time to live, number of seconds of particles natural life
		real totalTimeToLive;
		/// Speed of rotation in radians/sec
		CGUIRadian rotationSpeed;
		/// Determines the type of particle.
		EParticleType particleType;

	protected:
		/// Parent CGUIParticleSystem
		CGUIParticleSystem* mParentSystem;
    };
}

#endif	//__GUI_PARTICLE_20110118_H__

