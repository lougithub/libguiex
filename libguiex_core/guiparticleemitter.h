/** 
* @file guiparticleemitter.h
* @brief 
* @author ken
* @date 2011-01-19
*/

#ifndef __GUI_PARTICLEEMITTER_20110119_H__
#define __GUI_PARTICLEEMITTER_20110119_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guivector3.h"
#include "guiparticle.h"

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
	class CGUIParticleEmitter : public CGUIParticle
    {
     public:
        CGUIParticleEmitter(CGUIParticleSystem* psys);
        virtual ~CGUIParticleEmitter();

        virtual void setPosition(const CGUIVector3& pos);
        virtual const CGUIVector3& getPosition(void) const;

        virtual void setDirection(const CGUIVector3& direction);
        virtual const CGUIVector3& getDirection(void) const;

        virtual void setAngle(const CGUIRadian& angle);
        virtual const CGUIRadian& getAngle(void) const;

        virtual void setParticleVelocity(real speed);
        virtual void setParticleVelocity(real min, real max);
        virtual void setMinParticleVelocity(real min);
        virtual void setMaxParticleVelocity(real max);
        virtual real getParticleVelocity(void) const;
        virtual real getMinParticleVelocity(void) const;
        virtual real getMaxParticleVelocity(void) const;

        virtual void setEmissionRate(real particlesPerSecond);
        virtual real getEmissionRate(void) const;

        virtual void setTimeToLive(real ttl);
        virtual void setTimeToLive(real minTtl, real maxTtl);
        virtual void setMinTimeToLive(real min);
        virtual void setMaxTimeToLive(real max);
        virtual real getTimeToLive(void) const;
        virtual real getMinTimeToLive(void) const;
        virtual real getMaxTimeToLive(void) const;
 
        virtual void setColour(const CGUIColor& colour);
        virtual void setColour(const CGUIColor& colourStart, const CGUIColor& colourEnd);
        virtual void setColourRangeStart(const CGUIColor& colour);
        virtual void setColourRangeEnd(const CGUIColor& colour);
        virtual const CGUIColor& getColour(void) const;
        virtual const CGUIColor& getColourRangeStart(void) const;
        virtual const CGUIColor& getColourRangeEnd(void) const;

        /** Gets the number of particles which this emitter would like to emit based on the time elapsed.
        @remarks
            For efficiency the emitter does not actually create new Particle instances (these are reused
            by the CGUIParticleSystem as existing particles 'die'). The implementation for this method must
            return the number of particles the emitter would like to emit given the number of seconds which
            have elapsed (passed in as a parameter).
        @par
            Based on the return value from this method, the CGUIParticleSystem class will call 
            _initParticle once for each particle it chooses to allow to be emitted by this emitter.
            The emitter should not track these _initParticle calls, it should assume all emissions
            requested were made (even if they could not be because of particle quotas).
        */
        virtual uint16 _getEmissionCount(real timeElapsed) = 0;

        virtual void _initParticle(CGUIParticle* pParticle);

        const CGUIString &getType(void) const;

        virtual void setEnabled(bool enabled);
        virtual bool getEnabled(void) const;

        virtual void setStartTime(real startTime);
        virtual real getStartTime(void) const;

        virtual void setDuration(real duration);
        virtual real getDuration(void) const;
        virtual void setDuration(real min, real max);
        virtual void setMinDuration(real min);
        virtual void setMaxDuration(real max);
        virtual real getMinDuration(void) const;
        virtual real getMaxDuration(void) const;

        virtual void setRepeatDelay(real duration);
        virtual real getRepeatDelay(void) const;

        virtual void setRepeatDelay(real min, real max);
        virtual void setMinRepeatDelay(real min);
        virtual void setMaxRepeatDelay(real max);
        virtual real getMinRepeatDelay(void) const;
        virtual real getMaxRepeatDelay(void) const;

		const CGUIString &getName(void) const;
		virtual void setName(const CGUIString& newName);

		const CGUIString &getEmittedEmitter(void) const;
		virtual void setEmittedEmitter(const CGUIString& emittedEmitter);

		virtual bool isEmitted(void) const;
		virtual void setEmitted(bool emitted);

   protected:
        /// Parent particle system
        CGUIParticleSystem* mParent;
        /// Position relative to the center of the CGUIParticleSystem
        CGUIVector3 mPosition;
        /// Rate in particles per second at which this emitter wishes to emit particles
        real mEmissionRate;
        /// Name of the type of emitter, MUST be initialised by subclasses
        CGUIString mType;
        /// Base direction of the emitter, may not be used by some emitters
        CGUIVector3 mDirection;
        // Notional up vector, just used to speed up generation of variant directions
        CGUIVector3 mUp;
        /// Angle around direction which particles may be emitted, internally radians but angleunits for interface
        CGUIRadian mAngle;
        /// Min speed of particles
        real mMinSpeed;
        /// Max speed of particles
        real mMaxSpeed;
        /// Initial time-to-live of particles (min)
        real mMinTTL;
        /// Initial time-to-live of particles (max)
        real mMaxTTL;
        /// Initial colour of particles (range start)
        CGUIColor mColourRangeStart;
        /// Initial colour of particles (range end)
        CGUIColor mColourRangeEnd;

        /// Whether this emitter is currently enabled (defaults to true)
        bool mEnabled;

        /// Start time (in seconds from start of first call to CGUIParticleSystem to update)
        real mStartTime;
        /// Minimum length of time emitter will run for (0 = forever)
        real mDurationMin;
        /// Maximum length of time the emitter will run for (0 = forever)
        real mDurationMax;
        /// Current duration remainder
        real mDurationRemain;

        /// Time between each repeat
        real mRepeatDelayMin;
        real mRepeatDelayMax;
        /// Repeat delay left
        real mRepeatDelayRemain;

		// Fractions of particles wanted to be emitted last time
		real mRemainder;

        /// The name of the emitter. The name is optional unless it is used as an emitter that is emitted itself.
        CGUIString mName;

		/// The name of the emitter to be emitted (optional)
        CGUIString mEmittedEmitter;

		// If 'true', this emitter is emitted by another emitter.
		bool mEmitted;

		// NB Method below here are to help out people implementing emitters by providing the
        // most commonly used approaches as piecemeal methods
        virtual void genEmissionDirection(CGUIVector3& destVector);
        virtual void genEmissionVelocity(CGUIVector3& destVector);
        virtual real genEmissionTTL(void);
        virtual void genEmissionColour(CGUIColor& destColour);
        virtual uint16 genConstantEmissionCount(real timeElapsed);
        void addBaseParameters(void);
        void initDurationRepeat(void);
    };
}

#endif	//__GUI_PARTICLEEMITTER_20110119_H__

