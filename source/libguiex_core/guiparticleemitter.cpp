/** 
* @file guiparticleemitter.h
* @brief 
* @author ken
* @date 2011-01-19
*/

//============================================================================//
// include
//============================================================================// 
#include "guiparticleemitter.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//-----------------------------------------------------------------------
	CGUIParticleEmitter::CGUIParticleEmitter(CGUIParticleSystem* psys)
		: mParent(psys),
		mStartTime(0),
		mDurationMin(0),
		mDurationMax(0),
		mDurationRemain(0),
		mRepeatDelayMin(0),
		mRepeatDelayMax(0),
		mRepeatDelayRemain(0)
	{
		// Reasonable defaults
		mAngle = 0;
		setDirection(CGUIVector3::UNIT_X);
		mEmissionRate = 10;
		mMaxSpeed = mMinSpeed = 1;
		mMaxTTL = mMinTTL = 5;
		mPosition = CGUIVector3::ZERO;
		mColourRangeStart = mColourRangeEnd = CGUIColor::White;
		mEnabled = true;
		mRemainder = 0;
		//mName = CGUIString();
		//mEmittedEmitter = CGUIString();
		mEmitted = false;
	}
	//-----------------------------------------------------------------------
	CGUIParticleEmitter::~CGUIParticleEmitter() 
	{
	}
	//-----------------------------------------------------------------------
	/** Sets the position of this emitter relative to the particle system center. */
	void CGUIParticleEmitter::setPosition(const CGUIVector3& pos) 
	{ 
		mPosition = pos; 
	}
	//-----------------------------------------------------------------------
	/** Returns the position of this emitter relative to the center of the particle system. */
	const CGUIVector3& CGUIParticleEmitter::getPosition(void) const 
	{ 
		return mPosition; 
	}
	//-----------------------------------------------------------------------
	/** Sets the direction of the emitter.
	@remarks
	Most emitters will have a base direction in which they emit particles (those which
	emit in all directions will ignore this parameter). They may not emit exactly along this
	vector for every particle, many will introduce a random scatter around this vector using 
	the angle property.
	@param direction
	The base direction for particles emitted.
	*/
	void CGUIParticleEmitter::setDirection(const CGUIVector3& inDirection) 
	{ 
		mDirection = inDirection; 
		mDirection.Normalise();
		// Generate an up vector (any will do)
		mUp = mDirection.Perpendicular();
		mUp.Normalise();
	}
	//-----------------------------------------------------------------------
	/** Returns the base direction of the emitter. */
	const CGUIVector3& CGUIParticleEmitter::getDirection(void) const
	{ 
		return mDirection; 
	}
	//-----------------------------------------------------------------------
	/** Sets the maximum angle away from the emitter direction which particle will be emitted.
	@remarks
	Whilst the direction property defines the general direction of emission for particles, 
	this property defines how far the emission angle can deviate away from this base direction.
	This allows you to create a scatter effect - if set to 0, all particles will be emitted
	exactly along the emitters direction vector, whereas if you set it to 180 degrees or more,
	particles will be emitted in a sphere, i.e. in all directions.
	@param degrees
	Maximum angle which initial particle direction can deviate from the emitter base direction vector.
	*/
	void CGUIParticleEmitter::setAngle(const CGUIRadian& angle)
	{
		// Store as radians for efficiency
		mAngle = angle;
	}
	//-----------------------------------------------------------------------
	/** Returns the maximum angle which the initial particle direction can deviate from the emitters base direction. */
	const CGUIRadian& CGUIParticleEmitter::getAngle(void) const
	{
		return mAngle;
	}
	//-----------------------------------------------------------------------
	/** Sets the initial velocity of particles emitted.
	@remarks
	This method sets a constant speed for emitted particles. See the alternate version
	of this method which takes 2 parameters if you want a variable speed. 
	@param
	speed The initial speed in world units per second which every particle emitted starts with.
	*/
	void CGUIParticleEmitter::setParticleVelocity(real speed)
	{
		mMinSpeed = mMaxSpeed = speed;
	}
	//-----------------------------------------------------------------------
	/** Sets the initial velocity range of particles emitted.
	@remarks
	This method sets the range of starting speeds for emitted particles. 
	See the alternate version of this method which takes 1 parameter if you want a 
	constant speed. This emitter will randomly choose a speed between the minimum and 
	maximum for each particle.
	@param max The maximum speed in world units per second for the initial particle speed on emission.
	@param min The minimum speed in world units per second for the initial particle speed on emission.
	*/
	void CGUIParticleEmitter::setParticleVelocity(real min, real max)
	{
		mMinSpeed = min;
		mMaxSpeed = max;
	}
	//-----------------------------------------------------------------------
	/** Sets the emission rate for this emitter.
	@remarks
	This method tells the emitter how many particles per second should be emitted. The emitter
	subclass does not have to emit these in a continuous burst - this is a relative parameter
	and the emitter may choose to emit all of the second's worth of particles every half-second
	for example. This is controlled by the emitter's getEmissionCount method.
	@par
	Also, if the CGUIParticleSystem's particle quota is exceeded, not all the particles requested
	may be actually emitted.
	@param
	particlesPerSecond The number of particles to be emitted every second.
	*/
	void CGUIParticleEmitter::setEmissionRate(real particlesPerSecond) 
	{ 
		mEmissionRate = particlesPerSecond; 
	}
	//-----------------------------------------------------------------------
	/** Returns the emission rate set for this emitter. */
	real CGUIParticleEmitter::getEmissionRate(void) const 
	{ 
		return mEmissionRate; 
	}
	//-----------------------------------------------------------------------
	/** Sets the lifetime of all particles emitted.
	@remarks
	The emitter initialises particles with a time-to-live (TTL), the number of seconds a particle
	will exist before being destroyed. This method sets a constant TTL for all particles emitted.
	Note that affectors are able to modify the TTL of particles later.
	@par
	Also see the alternate version of this method which takes a min and max TTL in order to 
	have the TTL vary per particle.
	@param ttl The number of seconds each particle will live for.
	*/
	void CGUIParticleEmitter::setTimeToLive(real ttl)
	{
		mMinTTL = mMaxTTL = ttl;
	}
	//-----------------------------------------------------------------------
	/** Sets the range of lifetime for particles emitted.
	@remarks
	The emitter initialises particles with a time-to-live (TTL), the number of seconds a particle
	will exist before being destroyed. This method sets a range for the TTL for all particles emitted;
	the ttl may be randomised between these 2 extremes or will vary some other way depending on the
	emitter.
	Note that affectors are able to modify the TTL of particles later.
	@par
	Also see the alternate version of this method which takes a single TTL in order to 
	set a constant TTL for all particles.
	@param minTtl The minimum number of seconds each particle will live for.
	@param maxTtl The maximum number of seconds each particle will live for.
	*/
	void CGUIParticleEmitter::setTimeToLive(real minTtl, real maxTtl)
	{
		mMinTTL = minTtl;
		mMaxTTL = maxTtl;
	}
	//-----------------------------------------------------------------------
	/** Sets the initial colour of particles emitted.
	@remarks
	Particles have an initial colour on emission which the emitter sets. This method sets
	this colour. See the alternate version of this method which takes 2 colours in order to establish 
	a range of colours to be assigned to particles.
	@param colour The colour which all particles will be given on emission.
	*/
	void CGUIParticleEmitter::setColour(const CGUIColor& inColour)
	{
		mColourRangeStart = mColourRangeEnd = inColour;
	}
	//-----------------------------------------------------------------------
	/** Sets the range of colours for emitted particles.
	@remarks
	Particles have an initial colour on emission which the emitter sets. This method sets
	the range of this colour. See the alternate version of this method which takes a single colour
	in order to set a constant colour for all particles. Emitters may choose to randomly assign
	a colour in this range, or may use some other method to vary the colour.
	@param colourStart The start of the colour range
	@param colourEnd The end of the colour range
	*/
	void CGUIParticleEmitter::setColour(const CGUIColor& colourStart, const CGUIColor& colourEnd)
	{
		mColourRangeStart = colourStart;
		mColourRangeEnd = colourEnd;
	}
	//-----------------------------------------------------------------------
	/** Returns the name of the emitter */
	const CGUIString& CGUIParticleEmitter::getName(void) const
	{
		return mName;
	}
	//-----------------------------------------------------------------------
	/** Sets the name of the emitter */
	void CGUIParticleEmitter::setName(const CGUIString& newName)
	{
		mName = newName;
	}
	//-----------------------------------------------------------------------
	/** Returns the name of the emitter to be emitted */
	const CGUIString& CGUIParticleEmitter::getEmittedEmitter(void) const
	{
		return mEmittedEmitter;
	}
	//-----------------------------------------------------------------------
	/** Sets the name of the emitter to be emitted*/
	void CGUIParticleEmitter::setEmittedEmitter(const CGUIString& emittedEmitter)
	{
		mEmittedEmitter = emittedEmitter;
	}
	//-----------------------------------------------------------------------
	/** Return true if the emitter is emitted by another emitter */
	bool CGUIParticleEmitter::isEmitted(void) const
	{
		return mEmitted;
	}
	//-----------------------------------------------------------------------
	/** Set the indication (true/false) to indicate that the emitter is emitted by another emitter */
	void CGUIParticleEmitter::setEmitted(bool emitted)
	{
		mEmitted = emitted;
	}
	//-----------------------------------------------------------------------
	        /** Internal utility method for generating particle exit direction
        @param destVector Reference to vector to complete with new direction (normalised)
        */
	void CGUIParticleEmitter::genEmissionDirection(CGUIVector3& destVector)
	{
		if (mAngle != CGUIRadian(0))
		{
			// Randomise angle
			CGUIRadian angle = CGUIMath::UnitRandom() * mAngle;

			// Randomise direction
			destVector = mDirection.RandomDeviant(angle, mUp);
		}
		else
		{
			// Constant angle
			destVector = mDirection;
		}

		// Don't normalise, we can assume that it will still be a unit vector since
		// both direction and 'up' are.

	}
	//-----------------------------------------------------------------------
	        /** Internal utility method to apply velocity to a particle direction.
        @param destVector The vector to scale by a randomly generated scale between min and max speed.
            Assumed normalised already, and likely already oriented in the right direction.
        */
	void CGUIParticleEmitter::genEmissionVelocity(CGUIVector3& destVector)
	{
		real scalar;
		if (mMinSpeed != mMaxSpeed)
		{
			scalar = mMinSpeed + (CGUIMath::UnitRandom() * (mMaxSpeed - mMinSpeed));
		}
		else
		{
			scalar = mMinSpeed;
		}

		destVector *= scalar;
	}
	//-----------------------------------------------------------------------
	/** Internal utility method for generating a time-to-live for a particle. */
	real CGUIParticleEmitter::genEmissionTTL(void)
	{
		if (mMaxTTL != mMinTTL)
		{
			return mMinTTL + (CGUIMath::UnitRandom() * (mMaxTTL - mMinTTL));
		}
		else
		{
			return mMinTTL;
		}
	}
	//-----------------------------------------------------------------------
	/** Internal utility method for generating an emission count based on a constant emission rate. */
	uint16 CGUIParticleEmitter::genConstantEmissionCount(real timeElapsed)
	{
		uint16 intRequest;

		if (mEnabled)
		{
			// Keep fractions, otherwise a high frame rate will result in zero emissions!
			mRemainder += mEmissionRate * timeElapsed;
			intRequest = (uint16)mRemainder;
			mRemainder -= intRequest;

			// Check duration
			if (mDurationMax)
			{
				mDurationRemain -= timeElapsed;
				if (mDurationRemain <= 0) 
				{
					// Disable, duration is out (takes effect next time)
					setEnabled(false);
				}
			}
			return intRequest;
		}
		else
		{
			// Check repeat
			if (mRepeatDelayMax)
			{
				mRepeatDelayRemain -= timeElapsed;
				if (mRepeatDelayRemain <= 0)
				{
					// Enable, repeat delay is out (takes effect next time)
					setEnabled(true);
				}
			}
			if(mStartTime)
			{
				mStartTime -= timeElapsed;
				if(mStartTime <= 0)
				{
					setEnabled(true);
					mStartTime = 0;
				}
			}
			return 0;
		}

	}
	//-----------------------------------------------------------------------
	/** Internal utility method for generating a colour for a particle. */
	void CGUIParticleEmitter::genEmissionColour(CGUIColor& destColour)
	{
		if (mColourRangeStart != mColourRangeEnd)
		{
			// Randomise
			//real t = CGUIMath::UnitRandom();
			destColour.r = mColourRangeStart.r + (CGUIMath::UnitRandom() * (mColourRangeEnd.r - mColourRangeStart.r));
			destColour.g = mColourRangeStart.g + (CGUIMath::UnitRandom() * (mColourRangeEnd.g - mColourRangeStart.g));
			destColour.b = mColourRangeStart.b + (CGUIMath::UnitRandom() * (mColourRangeEnd.b - mColourRangeStart.b));
			destColour.a = mColourRangeStart.a + (CGUIMath::UnitRandom() * (mColourRangeEnd.a - mColourRangeStart.a));
		}
		else
		{
			destColour = mColourRangeStart;
		}
	}
	//-----------------------------------------------------------------------
	        /** Internal method for setting up the basic parameter definitions for a subclass. 
        @remarks
            Because StringInterface holds a dictionary of parameters per class, subclasses need to
            call this to ask the base class to add it's parameters to their dictionary as well.
            Can't do this in the constructor because that runs in a non-virtual context.
        @par
            The subclass must have called it's own createParamDictionary before calling this method.
        */
	void CGUIParticleEmitter::addBaseParameters(void)    
	{
		/*	ParamDictionary* dict = getParamDictionary();

		dict->addParameter(ParameterDef("angle", 
		"The angle up to which particles may vary in their initial direction "
		"from the emitters direction, in degrees." , PT_REAL),
		&msAngleCmd);

		dict->addParameter(ParameterDef("colour", 
		"The colour of emitted particles.", PT_COLOURVALUE),
		&msColourCmd);

		dict->addParameter(ParameterDef("colour_range_start", 
		"The start of a range of colours to be assigned to emitted particles.", PT_COLOURVALUE),
		&msColourRangeStartCmd);

		dict->addParameter(ParameterDef("colour_range_end", 
		"The end of a range of colours to be assigned to emitted particles.", PT_COLOURVALUE),
		&msColourRangeEndCmd);

		dict->addParameter(ParameterDef("direction", 
		"The base direction of the emitter." , PT_VECTOR3),
		&msDirectionCmd);

		dict->addParameter(ParameterDef("emission_rate", 
		"The number of particles emitted per second." , PT_REAL),
		&msEmissionRateCmd);

		dict->addParameter(ParameterDef("position", 
		"The position of the emitter relative to the particle system center." , PT_VECTOR3),
		&msPositionCmd);

		dict->addParameter(ParameterDef("velocity", 
		"The initial velocity to be assigned to every particle, in world units per second." , PT_REAL),
		&msVelocityCmd);

		dict->addParameter(ParameterDef("velocity_min", 
		"The minimum initial velocity to be assigned to each particle." , PT_REAL),
		&msMinVelocityCmd);

		dict->addParameter(ParameterDef("velocity_max", 
		"The maximum initial velocity to be assigned to each particle." , PT_REAL),
		&msMaxVelocityCmd);

		dict->addParameter(ParameterDef("time_to_live", 
		"The lifetime of each particle in seconds." , PT_REAL),
		&msTTLCmd);

		dict->addParameter(ParameterDef("time_to_live_min", 
		"The minimum lifetime of each particle in seconds." , PT_REAL),
		&msMinTTLCmd);

		dict->addParameter(ParameterDef("time_to_live_max", 
		"The maximum lifetime of each particle in seconds." , PT_REAL),
		&msMaxTTLCmd);

		dict->addParameter(ParameterDef("duration", 
		"The length of time in seconds which an emitter stays enabled for." , PT_REAL),
		&msDurationCmd);

		dict->addParameter(ParameterDef("duration_min", 
		"The minimum length of time in seconds which an emitter stays enabled for." , PT_REAL),
		&msMinDurationCmd);

		dict->addParameter(ParameterDef("duration_max", 
		"The maximum length of time in seconds which an emitter stays enabled for." , PT_REAL),
		&msMaxDurationCmd);

		dict->addParameter(ParameterDef("repeat_delay", 
		"If set, after disabling an emitter will repeat (reenable) after this many seconds." , PT_REAL),
		&msRepeatDelayCmd);

		dict->addParameter(ParameterDef("repeat_delay_min", 
		"If set, after disabling an emitter will repeat (reenable) after this minimum number of seconds." , PT_REAL),
		&msMinRepeatDelayCmd);

		dict->addParameter(ParameterDef("repeat_delay_max", 
		"If set, after disabling an emitter will repeat (reenable) after this maximum number of seconds." , PT_REAL),
		&msMaxRepeatDelayCmd);

		dict->addParameter(ParameterDef("name", 
		"This is the name of the emitter" , PT_STRING),
		&msNameCmd);

		dict->addParameter(ParameterDef("emit_emitter", 
		"If set, this emitter will emit other emitters instead of visual particles" , PT_STRING),
		&msEmittedEmitterCmd);*/
	}
	//-----------------------------------------------------------------------
	/** Returns the initial velocity of particles emitted. */
	real CGUIParticleEmitter::getParticleVelocity(void) const
	{
		return mMinSpeed;
	}
	//-----------------------------------------------------------------------
	/** Returns the minimum particle velocity. */
	real CGUIParticleEmitter::getMinParticleVelocity(void) const
	{
		return mMinSpeed;
	}
	//-----------------------------------------------------------------------
	/** Returns the maximum particle velocity. */
	real CGUIParticleEmitter::getMaxParticleVelocity(void) const
	{
		return mMaxSpeed;
	}
	//-----------------------------------------------------------------------
	/** Returns the minimum particle velocity. */
	void CGUIParticleEmitter::setMinParticleVelocity(real min)
	{
		mMinSpeed = min;
	}
	//-----------------------------------------------------------------------
	/** Returns the maximum particle velocity. */
	void CGUIParticleEmitter::setMaxParticleVelocity(real max)
	{
		mMaxSpeed = max;
	}
	//-----------------------------------------------------------------------
	/** Gets the time each particle will live for. */
	real CGUIParticleEmitter::getTimeToLive(void) const
	{
		return mMinTTL;
	}
	//-----------------------------------------------------------------------
	/** Gets the minimum time each particle will live for. */
	real CGUIParticleEmitter::getMinTimeToLive(void) const
	{
		return mMinTTL;
	}
	//-----------------------------------------------------------------------
	/** Gets the maximum time each particle will live for. */
	real CGUIParticleEmitter::getMaxTimeToLive(void) const
	{
		return mMaxTTL;
	}
	//-----------------------------------------------------------------------
	/** Sets the minimum time each particle will live for. */
	void CGUIParticleEmitter::setMinTimeToLive(real min)
	{
		mMinTTL = min;
	}
	//-----------------------------------------------------------------------
	/** Sets the maximum time each particle will live for. */
	void CGUIParticleEmitter::setMaxTimeToLive(real max)
	{
		mMaxTTL = max;
	}
	//-----------------------------------------------------------------------
	/** Gets the colour of particles to be emitted. */
	const CGUIColor& CGUIParticleEmitter::getColour(void) const
	{
		return mColourRangeStart;
	}
	//-----------------------------------------------------------------------
	/** Gets the minimum colour of particles to be emitted. */
	const CGUIColor& CGUIParticleEmitter::getColourRangeStart(void) const
	{
		return mColourRangeStart;
	}
	//-----------------------------------------------------------------------
	/** Gets the maximum colour of particles to be emitted. */
	const CGUIColor& CGUIParticleEmitter::getColourRangeEnd(void) const
	{
		return mColourRangeEnd;
	}
	//-----------------------------------------------------------------------
	/** Sets the minimum colour of particles to be emitted. */
	void CGUIParticleEmitter::setColourRangeStart(const CGUIColor& val)
	{
		mColourRangeStart = val;
	}
	//-----------------------------------------------------------------------
	/** Sets the maximum colour of particles to be emitted. */
	void CGUIParticleEmitter::setColourRangeEnd(const CGUIColor& val )
	{
		mColourRangeEnd = val;
	}
	//-----------------------------------------------------------------------
	/** Sets whether or not the emitter is enabled.
	@remarks
	You can turn an emitter off completely by setting this parameter to false.
	*/
	void CGUIParticleEmitter::setEnabled(bool enabled)
	{
		mEnabled = enabled;
		// Reset duration & repeat
		initDurationRepeat();
	}
	//-----------------------------------------------------------------------
	/** Gets the flag indicating if this emitter is enabled or not. */
	bool CGUIParticleEmitter::getEnabled(void) const
	{
		return mEnabled;
	}
	//-----------------------------------------------------------------------
	        /** Sets the 'start time' of this emitter.
        @remarks
            By default an emitter starts straight away as soon as a CGUIParticleSystem is first created,
            or also just after it is re-enabled. This parameter allows you to set a time delay so
            that the emitter does not 'kick in' until later.
        @param startTime The time in seconds from the creation or enabling of the emitter.
        */
	void CGUIParticleEmitter::setStartTime(real startTime)
	{
		setEnabled(false);
		mStartTime = startTime;
	}
	//-----------------------------------------------------------------------
	/** Gets the start time of the emitter. */
	real CGUIParticleEmitter::getStartTime(void) const
	{
		return mStartTime;
	}
	//-----------------------------------------------------------------------
	        /** Sets the duration of the emitter.
        @remarks
            By default emitters run indefinitely (unless you manually disable them). By setting this
            parameter, you can make an emitter turn off on it's own after a set number of seconds. It
            will then remain disabled until either setEnabled(true) is called, or if the 'repeatAfter' parameter
            has been set it will also repeat after a number of seconds.
        @par
            Also see the alternative version of this method which allows you to set a min and max duration for
            a random variable duration.
        @param duration The duration in seconds.
        */
	void CGUIParticleEmitter::setDuration(real duration)
	{
		setDuration(duration, duration);
	}
	//-----------------------------------------------------------------------
	/** Gets the duration of the emitter from when it is created or re-enabled. */
	real CGUIParticleEmitter::getDuration(void) const
	{
		return mDurationMin;
	}
	//-----------------------------------------------------------------------
	        /** Sets the range of random duration for this emitter. 
        @remarks
            By default emitters run indefinitely (unless you manually disable them). By setting this
            parameter, you can make an emitter turn off on it's own after a random number of seconds. It
            will then remain disabled until either setEnabled(true) is called, or if the 'repeatAfter' parameter
            has been set it will also repeat after a number of seconds.
        @par
            Also see the alternative version of this method which allows you to set a constant duration.
        @param min The minimum duration in seconds.
        @param max The minimum duration in seconds.
        */
	void CGUIParticleEmitter::setDuration(real min, real max)
	{
		mDurationMin = min;
		mDurationMax = max;
		initDurationRepeat();
	}
	//-----------------------------------------------------------------------
	/** Sets the minimum duration of this emitter in seconds (see setDuration for more details) */
	void CGUIParticleEmitter::setMinDuration(real min)
	{
		mDurationMin = min;
		initDurationRepeat();
	}
	//-----------------------------------------------------------------------
	/** Sets the maximum duration of this emitter in seconds (see setDuration for more details) */
	void CGUIParticleEmitter::setMaxDuration(real max)
	{
		mDurationMax = max;
		initDurationRepeat();
	}
	//-----------------------------------------------------------------------
	/** Internal method for initialising the duration & repeat of an emitter. */
	void CGUIParticleEmitter::initDurationRepeat(void)
	{
		if (mEnabled)
		{
			if (mDurationMin == mDurationMax)
			{
				mDurationRemain = mDurationMin;
			}
			else
			{
				mDurationRemain = CGUIMath::RangeRandom(mDurationMin, mDurationMax);
			}
		}
		else
		{
			// Reset repeat
			if (mRepeatDelayMin == mRepeatDelayMax)
			{
				mRepeatDelayRemain = mRepeatDelayMin;
			}
			else
			{
				mRepeatDelayRemain = CGUIMath::RangeRandom(mRepeatDelayMax, mRepeatDelayMin);
			}

		}
	}
	//-----------------------------------------------------------------------
	        /** Sets the time between repeats of the emitter.
        @remarks
            By default emitters run indefinitely (unless you manually disable them). However, if you manually
            disable the emitter (by calling setEnabled(false), or it's duration runs out, it will cease to emit
        @par
            Also see the alternative version of this method which allows you to set a min and max duration for
            a random variable duration.
        @param duration The duration in seconds.
        */
	void CGUIParticleEmitter::setRepeatDelay(real delay)
	{
		setRepeatDelay(delay, delay);
	}
	//-----------------------------------------------------------------------
	/** Gets the duration of the emitter from when it is created or re-enabled. */
	real CGUIParticleEmitter::getRepeatDelay(void) const
	{
		return mRepeatDelayMin;
	}
	//-----------------------------------------------------------------------
	        /** Sets the range of random duration for this emitter. 
        @remarks
            By default emitters run indefinitely (unless you manually disable them). By setting this
            parameter, you can make an emitter turn off on it's own after a random number of seconds. It
            will then remain disabled until either setEnabled(true) is called, or if the 'repeatAfter' parameter
            has been set it will also repeat after a number of seconds.
        @par
            Also see the alternative version of this method which allows you to set a constant duration.
        @param min The minimum duration in seconds.
        @param max The minimum duration in seconds.
        */
	void CGUIParticleEmitter::setRepeatDelay(real min, real max)
	{
		mRepeatDelayMin = min;
		mRepeatDelayMax = max;
		initDurationRepeat();
	}
	//-----------------------------------------------------------------------
	/** Sets the minimum duration of this emitter in seconds (see setRepeatDelay for more details) */
	void CGUIParticleEmitter::setMinRepeatDelay(real min)
	{
		mRepeatDelayMin = min;
		initDurationRepeat();
	}
	//-----------------------------------------------------------------------
	/** Sets the maximum duration of this emitter in seconds (see setRepeatDelay for more details) */
	void CGUIParticleEmitter::setMaxRepeatDelay(real max)
	{
		mRepeatDelayMax = max;
		initDurationRepeat();
	}
	//-----------------------------------------------------------------------
	/** Gets the minimum duration of this emitter in seconds (see setDuration for more details) */
	real CGUIParticleEmitter::getMinDuration(void) const
	{
		return mDurationMin;
	}
	//-----------------------------------------------------------------------
	/** Gets the maximum duration of this emitter in seconds (see setDuration for more details) */
	real CGUIParticleEmitter::getMaxDuration(void) const
	{
		return mDurationMax;
	}
	//-----------------------------------------------------------------------
	/** Gets the minimum duration of this emitter in seconds (see setRepeatDelay for more details) */
	real CGUIParticleEmitter::getMinRepeatDelay(void) const
	{
		return mRepeatDelayMin;    
	}
	//-----------------------------------------------------------------------
	/** Gets the maximum duration of this emitter in seconds (see setRepeatDelay for more details) */
	real CGUIParticleEmitter::getMaxRepeatDelay(void) const
	{
		return mRepeatDelayMax;    
	}
	//-----------------------------------------------------------------------
        /** Returns the name of the type of emitter. 
        @remarks
            This property is useful for determining the type of emitter procedurally so another
            can be created.
        */
        const CGUIString &CGUIParticleEmitter::getType(void) const 
		{
			return mType; 
		}
		//-----------------------------------------------------------------------
		/** Initialises a particle based on the emitter's approach and parameters.
		@remarks
		See the _getEmissionCount method for details of why there is a separation between
		'requested' emissions and actual initialised particles.
		@param
		pParticle Pointer to a particle which must be initialised based on how this emitter
		starts particles. This is passed as a pointer rather than being created by the emitter so the
		CGUIParticleSystem can reuse Particle instances, and can also set defaults itself.
		*/
		void CGUIParticleEmitter::_initParticle(CGUIParticle* pParticle)
		{
			// Initialise size in case it's been altered
			pParticle->resetDimensions();
		}
		//-----------------------------------------------------------------------
}
