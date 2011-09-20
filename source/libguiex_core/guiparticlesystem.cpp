/** 
* @file guiparticlesystem.cpp
* @brief 
* @author ken
* @date 2011-01-18
*/

//============================================================================//
// include
//============================================================================// 
#include "guiparticlesystem.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	// Init statics
	//RadixSort<CGUIParticleSystem::TActiveParticleList, CGUIParticle*, float> CGUIParticleSystem::mRadixSorter;

	real CGUIParticleSystem::msDefaultIterationInterval = 0;
	real CGUIParticleSystem::msDefaultNonvisibleTimeout = 0;

	//-----------------------------------------------------------------------
	// Local class for updating based on time
	//class ParticleSystemUpdateValue : public ControllerValue<real>
	//{
	//protected:
	//	CGUIParticleSystem* mTarget;
	//public:
	//	ParticleSystemUpdateValue(CGUIParticleSystem* target) : mTarget(target) {}

	//	real getValue(void) const { return 0; } // N/A

	//	void setValue(real value) { mTarget->_update(value); }

	//};
	//-----------------------------------------------------------------------
	CGUIParticleSystem::CGUIParticleSystem() 
		: //mAABB(),
		mBoundingRadius(1.0f),
		mBoundsAutoUpdate(true),
		mBoundsUpdateTime(10.0f),
		mUpdateRemainTime(0),
//		mWorldAABB(),
//		mResourceGroupName(ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME),
		mIsRendererConfigured(false),
		mSpeedFactor(1.0f),
		mIterationInterval(0),
		mIterationIntervalSet(false),
		mSorted(false),
		mLocalSpace(false),
		mNonvisibleTimeout(0),
		mNonvisibleTimeoutSet(false),
		mTimeSinceLastVisible(0),
//		mLastVisibleFrame(0),
//		mTimeController(0),
		mEmittedEmitterPoolInitialised(false),
		mIsEmitting(true),
		mRenderer(0),
		mCullIndividual(false),
		mPoolSize(0),
		mEmittedEmitterPoolSize(0)
	{
		initParameters();

		// Default to billboard renderer
		setRenderer("billboard");

	}
	//-----------------------------------------------------------------------
	/** Creates a particle system with no emitters or affectors.
	@remarks
	You should use the CGUIParticleSystemManager to create particle systems rather than creating
	them directly.
	*/
	CGUIParticleSystem::CGUIParticleSystem(const CGUIString& name, const CGUIString& resourceGroup)
		: //MovableObject(name),
		//mAABB(),
		mBoundingRadius(1.0f),
		mBoundsAutoUpdate(true),
		mBoundsUpdateTime(10.0f),
		mUpdateRemainTime(0),
//		mWorldAABB(),
//		mResourceGroupName(resourceGroup),
		mIsRendererConfigured(false),
		mSpeedFactor(1.0f),
		mIterationInterval(0),
		mIterationIntervalSet(false),
		mSorted(false),
		mLocalSpace(false),
		mNonvisibleTimeout(0),
		mNonvisibleTimeoutSet(false),
		mTimeSinceLastVisible(0),
//		mLastVisibleFrame(Root::getSingleton().getNextFrameNumber()),
//		mTimeController(0),
		mEmittedEmitterPoolInitialised(false),
		mIsEmitting(true),
		mRenderer(0), 
		mCullIndividual(false),
		mPoolSize(0),
		mEmittedEmitterPoolSize(0)
	{
		setDefaultDimensions( 100, 100 );
		setMaterialName( "BaseWhite" );
		// Default to 10 particles, expect app to specify (will only be increased, not decreased)
		setParticleQuota( 10 );
		setEmittedEmitterQuota( 3 );
		initParameters();

		// Default to billboard renderer
		setRenderer("billboard");
	}
	//-----------------------------------------------------------------------
	CGUIParticleSystem::~CGUIParticleSystem()
	{
		//if (mTimeController)
		//{
		//	// Destroy controller
		//	ControllerManager::getSingleton().destroyController(mTimeController);
		//	mTimeController = 0;
		//}

		// Arrange for the deletion of emitters & affectors
		removeAllEmitters();
		removeAllEmittedEmitters();
		removeAllAffectors();

		// Deallocate all particles
		destroyVisualParticles(0, mParticlePool.size());
		// Free pool items
		TParticlePool::iterator i;
		for (i = mParticlePool.begin(); i != mParticlePool.end(); ++i)
		{
			delete *i;
		}

		if (mRenderer)
		{
			CGUIParticleSystemManager::getSingleton()._destroyRenderer(mRenderer);
			mRenderer = 0;
		}

	}
	//-----------------------------------------------------------------------
	/** Adds an emitter to this particle system.
	@remarks
	Particles are created in a particle system by emitters - see the CGUIParticleEmitter
	class for more details.
	@param 
	emitterType CGUIString identifying the emitter type to create. Emitter types are defined
	by registering new factories with the manager - see ParticleEmitterFactory for more details.
	Emitter types can be extended by OGRE, plugin authors or application developers.
	*/
	CGUIParticleEmitter* CGUIParticleSystem::addEmitter(const CGUIString& emitterType)
	{
		CGUIParticleEmitter* em = 
			CGUIParticleSystemManager::getSingleton()._createEmitter(emitterType, this);
		mEmitters.push_back(em);
		return em;
	}
	//-----------------------------------------------------------------------
	/** Retrieves an emitter by it's index (zero-based).
	@remarks
	Used to retrieve a pointer to an emitter for a particle system to procedurally change
	emission parameters etc.
	You should check how many emitters are registered against this system before calling
	this method with an arbitrary index using getNumEmitters.
	@param
	index Zero-based index of the emitter to retrieve.
	*/
	CGUIParticleEmitter* CGUIParticleSystem::getEmitter(uint16 index) const
	{
		assert(index < mEmitters.size() && "Emitter index out of bounds!");
		return mEmitters[index];
	}
	//-----------------------------------------------------------------------
	/** Returns the number of emitters for this particle system. */
	uint16 CGUIParticleSystem::getNumEmitters(void) const
	{
		return static_cast< uint16 >( mEmitters.size() );
	}
	//-----------------------------------------------------------------------
	/** Removes an emitter from the system.
	@remarks
	Drops the emitter with the index specified from this system.
	You should check how many emitters are registered against this system before calling
	this method with an arbitrary index using getNumEmitters.
	@param
	index Zero-based index of the emitter to retrieve.
	*/
	void CGUIParticleSystem::removeEmitter(uint16 index)
	{
		assert(index < mEmitters.size() && "Emitter index out of bounds!");
		TParticleEmitterList::iterator ei = mEmitters.begin() + index;
		CGUIParticleSystemManager::getSingleton()._destroyEmitter(*ei);
		mEmitters.erase(ei);
	}
	//-----------------------------------------------------------------------
	/** Removes all the emitters from this system. */
	void CGUIParticleSystem::removeAllEmitters(void)
	{
		// DON'T delete directly, we don't know what heap these have been created on
		TParticleEmitterList::iterator ei;
		for (ei = mEmitters.begin(); ei != mEmitters.end(); ++ei)
		{
			CGUIParticleSystemManager::getSingleton()._destroyEmitter(*ei);
		}
		mEmitters.clear();
	}
	//-----------------------------------------------------------------------
	/** Adds an affector to this particle system.
	@remarks
	Particles are modified over time in a particle system by affectors - see the CGUIParticleAffector
	class for more details.
	@param 
	affectorType CGUIString identifying the affector type to create. Affector types are defined
	by registering new factories with the manager - see ParticleAffectorFactory for more details.
	Affector types can be extended by OGRE, plugin authors or application developers.
	*/
	CGUIParticleAffector* CGUIParticleSystem::addAffector(const CGUIString& affectorType)
	{
		CGUIParticleAffector* af = 
			CGUIParticleSystemManager::getSingleton()._createAffector(affectorType, this);
		mAffectors.push_back(af);
		return af;
	}
	//-----------------------------------------------------------------------
	/** Retrieves an affector by it's index (zero-based).
	@remarks
	Used to retrieve a pointer to an affector for a particle system to procedurally change
	affector parameters etc.
	You should check how many affectors are registered against this system before calling
	this method with an arbitrary index using getNumAffectors.
	@param
	index Zero-based index of the affector to retrieve.
	*/
	CGUIParticleAffector* CGUIParticleSystem::getAffector(uint16 index) const
	{
		assert(index < mAffectors.size() && "Affector index out of bounds!");
		return mAffectors[index];
	}
	//-----------------------------------------------------------------------
	/** Returns the number of affectors for this particle system. */
	uint16 CGUIParticleSystem::getNumAffectors(void) const
	{
		return static_cast< uint16 >( mAffectors.size() );
	}
	//-----------------------------------------------------------------------

	/** Removes an affector from the system.
	@remarks
	Drops the affector with the index specified from this system.
	You should check how many affectors are registered against this system before calling
	this method with an arbitrary index using getNumAffectors.
	@param
	index Zero-based index of the affector to retrieve.
	*/
	void CGUIParticleSystem::removeAffector(uint16 index)
	{
		assert(index < mAffectors.size() && "Affector index out of bounds!");
		TParticleAffectorList::iterator ai = mAffectors.begin() + index;
		CGUIParticleSystemManager::getSingleton()._destroyAffector(*ai);
		mAffectors.erase(ai);
	}
	//-----------------------------------------------------------------------
	/** Removes all the affectors from this system. */
	void CGUIParticleSystem::removeAllAffectors(void)
	{
		// DON'T delete directly, we don't know what heap these have been created on
		TParticleAffectorList::iterator ai;
		for (ai = mAffectors.begin(); ai != mAffectors.end(); ++ai)
		{
			CGUIParticleSystemManager::getSingleton()._destroyAffector(*ai);
		}
		mAffectors.clear();
	}
	//-----------------------------------------------------------------------
	/** Assignment operator for copying.
	@remarks
	This operator deep copies all particle emitters and effectors, but not particles. The
	system's name is also not copied.
	*/
	CGUIParticleSystem& CGUIParticleSystem::operator=(const CGUIParticleSystem& rhs)
	{
		// Blank this system's emitters & affectors
		removeAllEmitters();
		removeAllEmittedEmitters();
		removeAllAffectors();

		// Copy emitters
		for(uint16 i = 0; i < rhs.getNumEmitters(); ++i)
		{
			CGUIParticleEmitter* rhsEm = rhs.getEmitter(i);
			CGUIParticleEmitter* newEm = addEmitter(rhsEm->getType());
			rhsEm->copyParametersTo(newEm);
		}
		// Copy affectors
		for(uint16 i = 0; i < rhs.getNumAffectors(); ++i)
		{
			CGUIParticleAffector* rhsAf = rhs.getAffector(i);
			CGUIParticleAffector* newAf = addAffector(rhsAf->getType());
			rhsAf->copyParametersTo(newAf);
		}
		setParticleQuota(rhs.getParticleQuota());
		setEmittedEmitterQuota(rhs.getEmittedEmitterQuota());
		setMaterialName(rhs.mMaterialName);
		setDefaultDimensions(rhs.mDefaultWidth, rhs.mDefaultHeight);
		mCullIndividual = rhs.mCullIndividual;
		mSorted = rhs.mSorted;
		mLocalSpace = rhs.mLocalSpace;
		mIterationInterval = rhs.mIterationInterval;
		mIterationIntervalSet = rhs.mIterationIntervalSet;
		mNonvisibleTimeout = rhs.mNonvisibleTimeout;
		mNonvisibleTimeoutSet = rhs.mNonvisibleTimeoutSet;
		// last frame visible and time since last visible should be left default

		setRenderer(rhs.getRendererName());
		// Copy settings
		if (mRenderer && rhs.getRenderer())
		{
			rhs.getRenderer()->copyParametersTo(mRenderer);
		}

		return *this;

	}
	//-----------------------------------------------------------------------
	/** Gets the number of individual particles in the system right now.
	@remarks
	The number of particles active in a system at a point in time depends on 
	the number of emitters, their emission rates, the time-to-live (TTL) each particle is
	given on emission (and whether any affectors modify that TTL) and the maximum
	number of particles allowed in this system at once (particle quota).
	*/
	size_t CGUIParticleSystem::getNumParticles(void) const
	{
		return mActiveParticles.size();
	}
	//-----------------------------------------------------------------------
	/** Returns the maximum number of particles this system is allowed to have active at once.
	@remarks
	See CGUIParticleSystem::setParticleQuota for more info.
	*/
	size_t CGUIParticleSystem::getParticleQuota(void) const
	{
		return mPoolSize;
	}
	//-----------------------------------------------------------------------
	/** Sets the maximum number of particles this system is allowed to have active at once.
	@remarks
	Particle systems all have a particle quota, i.e. a maximum number of particles they are 
	allowed to have active at a time. This allows the application to set a keep particle systems
	under control should they be affected by complex parameters which alter their emission rates
	etc. If a particle system reaches it's particle quota, none of the emitters will be able to 
	emit any more particles. As existing particles die, the spare capacity will be allocated
	equally across all emitters to be as consistent to the origina particle system style as possible.
	@param quota The maximum number of particles this system is allowed to have.
	*/
	void CGUIParticleSystem::setParticleQuota(size_t size)
	{
		// Never shrink below size()
		size_t currSize = mParticlePool.size();

		if( currSize < size )
		{
			// Will allocate particles on demand
			mPoolSize = size;

		}
	}
	//-----------------------------------------------------------------------
	/** Returns the maximum number of emitted emitters this system is allowed to have active at once.
	@remarks
	See CGUIParticleSystem::setEmittedEmitterQuota for more info.
	*/
	size_t CGUIParticleSystem::getEmittedEmitterQuota(void) const
	{
		return mEmittedEmitterPoolSize;
	}
	//-----------------------------------------------------------------------
	/** Sets the maximum number of emitted emitters this system is allowed to have active at once.
	@remarks
	Particle systems can have - besides a particle quota - also an emitted emitter quota.
	@param quota The maximum number of emitted emitters this system is allowed to have.
	*/
	void CGUIParticleSystem::setEmittedEmitterQuota(size_t size)
	{
		// Never shrink below size()
		TEmittedEmitterPool::iterator i;
		size_t currSize = 0;
		for (i = mEmittedEmitterPool.begin(); i != mEmittedEmitterPool.end(); ++i)
		{
			currSize += i->second.size();
		}

		if( currSize < size )
		{
			// Will allocate emitted emitters on demand
			mEmittedEmitterPoolSize = size;
		}
	}
	//-----------------------------------------------------------------------
	/** Sets when the particle system should stop updating after it hasn't been
	visible for a while.
	@remarks
	By default, visible particle systems update all the time, even when 
	not in view. This means that they are guaranteed to be consistent when 
	they do enter view. However, this comes at a cost, updating particle
	systems can be expensive, especially if they are perpetual.
	@par
	This option lets you set a 'timeout' on the particle system, so that
	if it isn't visible for this amount of time, it will stop updating
	until it is next visible.
	@param timeout The time after which the particle system will be disabled
	if it is no longer visible. 0 to disable the timeout and always update.
	*/
	void CGUIParticleSystem::setNonVisibleUpdateTimeout(real timeout)
	{
		mNonvisibleTimeout = timeout;
		mNonvisibleTimeoutSet = true;
	}
	//-----------------------------------------------------------------------
	/** Gets when the particle system should stop updating after it hasn't been
	visible for a while.
	*/
	real CGUIParticleSystem::getNonVisibleUpdateTimeout(void) const 
	{ 
		return mNonvisibleTimeout; 
	}
	//-----------------------------------------------------------------------
	/** Set the default nonvisible timeout for all CGUIParticleSystem instances.
	*/
	static void CGUIParticleSystem::setDefaultNonVisibleUpdateTimeout(real timeout) 
	{
		msDefaultNonvisibleTimeout = timeout;
	}
	//-----------------------------------------------------------------------
	/** Get the default nonvisible timeout for all CGUIParticleSystem instances.
	*/
	static real CGUIParticleSystem::getDefaultNonVisibleUpdateTimeout(void) 
	{
		return msDefaultNonvisibleTimeout; 
	}
	//-----------------------------------------------------------------------
	/** Sets a 'iteration interval' on this particle system.
	@remarks
	The default Particle system update interval, based on elapsed frame time,
	will cause different behavior between low frame-rate and high frame-rate. 
	By using this option, you can make the particle system update at
	a fixed interval, keeping the behavior the same no matter what frame-rate 
	is.
	@par
	When iteration interval is set to zero, it means the update occurs based 
	on an elapsed frame time, otherwise each iteration will take place 
	at the given interval, repeating until it has used up all the elapsed 
	frame time.
	@param
	iterationInterval The iteration interval, default to zero.
	*/
	void CGUIParticleSystem::setIterationInterval(real interval)
	{
		mIterationInterval = interval;
		mIterationIntervalSet = true;
	}
	//-----------------------------------------------------------------------
	/** Gets a 'iteration interval' on this particle system.
	*/
	real CGUIParticleSystem::getIterationInterval(void) const
	{ 
		return mIterationInterval;
	}

	//-----------------------------------------------------------------------
	/** Set the default iteration interval for all CGUIParticleSystem instances.
	*/
	static void CGUIParticleSystem::setDefaultIterationInterval(real iterationInterval)
	{
		msDefaultIterationInterval = iterationInterval; 
	}
	//-----------------------------------------------------------------------
	/** Get the default iteration interval for all CGUIParticleSystem instances.
	*/
	static real CGUIParticleSystem::getDefaultIterationInterval(void) 
	{ 
		return msDefaultIterationInterval;
	}
	//-----------------------------------------------------------------------
	/** Updates the particles in the system based on time elapsed.
	@remarks
	This is called automatically every frame by OGRE.
	@param
	timeElapsed The amount of time, in seconds, since the last frame.
	*/
	void CGUIParticleSystem::_update(real timeElapsed)
	{
		//// Only update if attached to a node
		//if (!mParentNode)
		//	return;

		//real nonvisibleTimeout = mNonvisibleTimeoutSet ? mNonvisibleTimeout : msDefaultNonvisibleTimeout;

		//if (nonvisibleTimeout > 0)
		//{
		//	// Check whether it's been more than one frame (update is ahead of
		//	// camera notification by one frame because of the ordering)
		//	long frameDiff = Root::getSingleton().getNextFrameNumber() - mLastVisibleFrame;
		//	if (frameDiff > 1 || frameDiff < 0) // < 0 if wrap only
		//	{
		//		mTimeSinceLastVisible += timeElapsed;
		//		if (mTimeSinceLastVisible >= nonvisibleTimeout)
		//		{
		//			// No update
		//			return;
		//		}
		//	}
		//}

		//// Scale incoming speed for the rest of the calculation
		//timeElapsed *= mSpeedFactor;

		//// Init renderer if not done already
		//configureRenderer();

		//// Initialise emitted emitters list if not done already
		//initialiseEmittedEmitters();

		//real iterationInterval = mIterationIntervalSet ? mIterationInterval : msDefaultIterationInterval;
		//if (iterationInterval > 0)
		//{
		//	mUpdateRemainTime += timeElapsed;

		//	while (mUpdateRemainTime >= iterationInterval)
		//	{
		//		// Update existing particles
		//		_expire(iterationInterval);
		//		_triggerAffectors(iterationInterval);
		//		_applyMotion(iterationInterval);

		//		if(mIsEmitting)
		//		{
		//			// Emit new particles
		//			_triggerEmitters(iterationInterval);
		//		}

		//		mUpdateRemainTime -= iterationInterval;
		//	}
		//}
		//else
		//{
		//	// Update existing particles
		//	_expire(timeElapsed);
		//	_triggerAffectors(timeElapsed);
		//	_applyMotion(timeElapsed);

		//	if(mIsEmitting)
		//	{
		//		// Emit new particles
		//		_triggerEmitters(timeElapsed);
		//	}
		//}

		//if (!mBoundsAutoUpdate && mBoundsUpdateTime > 0.0f)
		//	mBoundsUpdateTime -= timeElapsed; // count down 
		//_updateBounds();

	}
	//-----------------------------------------------------------------------
	/** Internal method used to expire dead particles. */
	void CGUIParticleSystem::_expire(real timeElapsed)
	{
		TActiveParticleList::iterator i, itEnd;
		CGUIParticle* pParticle;
		CGUIParticleEmitter* pParticleEmitter;

		itEnd = mActiveParticles.end();

		for (i = mActiveParticles.begin(); i != itEnd; )
		{
			pParticle = static_cast<CGUIParticle*>(*i);
			if (pParticle->timeToLive < timeElapsed)
			{
				// Notify renderer
				mRenderer->_notifyParticleExpired(pParticle);

				// Identify the particle type
				if (pParticle->particleType == CGUIParticle::Visual)
				{
					// Destroy this one
					mFreeParticles.splice(mFreeParticles.end(), mActiveParticles, i++);
				}
				else
				{
					// For now, it can only be an emitted emitter
					pParticleEmitter = static_cast<CGUIParticleEmitter*>(*i);
					list<CGUIParticleEmitter*>::type* fee = findFreeEmittedEmitter(pParticleEmitter->getName());
					fee->push_back(pParticleEmitter);

					// Also erase from mActiveEmittedEmitters
					removeFromActiveEmittedEmitters (pParticleEmitter);

					// And erase from mActiveParticles
					i = mActiveParticles.erase( i );
				}
			}
			else
			{
				// Decrement TTL
				pParticle->timeToLive -= timeElapsed;
				++i;
			}

		}
	}
	//-----------------------------------------------------------------------
	/** Spawn new particles based on free quota and emitter requirements. */
	void CGUIParticleSystem::_triggerEmitters(real timeElapsed)
	{
		// Add up requests for emission
		static vector<unsigned>::type requested;
		if( requested.size() != mEmitters.size() )
			requested.resize( mEmitters.size() );

		size_t totalRequested, emitterCount, i, emissionAllowed;
		TParticleEmitterList::iterator itEmit, iEmitEnd;
		TActiveEmittedEmitterList::iterator itActiveEmit;
		iEmitEnd = mEmitters.end();
		emitterCount = mEmitters.size();
		emissionAllowed = mFreeParticles.size();
		totalRequested = 0;

		// Count up total requested emissions for regular emitters (and exclude the ones that are used as
		// a template for emitted emitters)
		for (itEmit = mEmitters.begin(), i = 0; itEmit != iEmitEnd; ++itEmit, ++i)
		{
			if (!(*itEmit)->isEmitted())
			{
				requested[i] = (*itEmit)->_getEmissionCount(timeElapsed);
				totalRequested += requested[i];
			}
		}

		// Add up total requested emissions for (active) emitted emitters
		for (itActiveEmit = mActiveEmittedEmitters.begin(); itActiveEmit != mActiveEmittedEmitters.end(); ++itActiveEmit)
		{
			totalRequested += (*itActiveEmit)->_getEmissionCount(timeElapsed);
		}

		// Check if the quota will be exceeded, if so reduce demand
		real ratio =  1.0f;
		if (totalRequested > emissionAllowed)
		{
			// Apportion down requested values to allotted values
			ratio =  (real)emissionAllowed / (real)totalRequested;
			for (i = 0; i < emitterCount; ++i)
			{
				requested[i] = static_cast<unsigned>(requested[i] * ratio);
			}
		}

		// Emit
		// For each emission, apply a subset of the motion for the frame
		// this ensures an even distribution of particles when many are
		// emitted in a single frame
		for (itEmit = mEmitters.begin(), i = 0; itEmit != iEmitEnd; ++itEmit, ++i)
		{
			// Trigger the emitters, but exclude the emitters that are already in the emitted emitters list; 
			// they are handled in a separate loop
			if (!(*itEmit)->isEmitted())
				_executeTriggerEmitters (*itEmit, static_cast<unsigned>(requested[i]), timeElapsed);
		}

		// Do the same with all active emitted emitters
		for (itActiveEmit = mActiveEmittedEmitters.begin(), i = 0; itActiveEmit != mActiveEmittedEmitters.end(); ++itActiveEmit, ++i)
			_executeTriggerEmitters (*itActiveEmit, static_cast<unsigned>((*itActiveEmit)->_getEmissionCount(timeElapsed) * ratio), timeElapsed);
	}
	//-----------------------------------------------------------------------
	/** Helper function that actually performs the emission of particles
	*/
	void CGUIParticleSystem::_executeTriggerEmitters(CGUIParticleEmitter* emitter, unsigned requested, real timeElapsed)
	{
		TParticleAffectorList::iterator	itAff, itAffEnd;
		real timePoint = 0.0f;


		// avoid any divide by zero conditions
		if(!requested) 
			return;

		real timeInc = timeElapsed / requested;

		for (unsigned int j = 0; j < requested; ++j)
		{
			// Create a new particle & init using emitter
			// The particle is a visual particle if the emit_emitter property of the emitter isn't set 
			CGUIParticle* p = 0;
			CGUIString	emitterName = emitter->getEmittedEmitter();
			if (emitterName == StringUtil::BLANK)
				p = createParticle();
			else
				p = createEmitterParticle(emitterName);

			// Only continue if the particle was really created (not null)
			if (!p)
				return;

			emitter->_initParticle(p);

			// Translate position & direction into world space
			if (!mLocalSpace)
			{
				p->position  = 
					(mParentNode->_getDerivedOrientation() *
					(mParentNode->_getDerivedScale() * p->position))
					+ mParentNode->_getDerivedPosition();
				p->direction = 
					(mParentNode->_getDerivedOrientation() * p->direction);
			}

			// apply partial frame motion to this particle
			p->position += (p->direction * timePoint);

			// apply particle initialization by the affectors
			itAffEnd = mAffectors.end();
			for (itAff = mAffectors.begin(); itAff != itAffEnd; ++itAff)
				(*itAff)->_initParticle(p);

			// Increment time fragment
			timePoint += timeInc;

			if (p->particleType == CGUIParticle::Emitter)
			{
				// If the particle is an emitter, the position on the emitter side must also be initialised
				// Note, that position of the emitter becomes a position in worldspace if mLocalSpace is set 
				// to false (will this become a problem?)
				CGUIParticleEmitter* pParticleEmitter = static_cast<CGUIParticleEmitter*>(p);
				pParticleEmitter->setPosition(p->position);
			}

			// Notify renderer
			mRenderer->_notifyParticleEmitted(p);
		}
	}
	//-----------------------------------------------------------------------
	/** Updates existing particle based on their momentum. */
	void CGUIParticleSystem::_applyMotion(real timeElapsed)
	{
		TActiveParticleList::iterator i, itEnd;
		CGUIParticle* pParticle;
		CGUIParticleEmitter* pParticleEmitter;

		itEnd = mActiveParticles.end();
		for (i = mActiveParticles.begin(); i != itEnd; ++i)
		{
			pParticle = static_cast<CGUIParticle*>(*i);
			pParticle->position += (pParticle->direction * timeElapsed);

			if (pParticle->particleType == CGUIParticle::Emitter)
			{
				// If it is an emitter, the emitter position must also be updated
				// Note, that position of the emitter becomes a position in worldspace if mLocalSpace is set 
				// to false (will this become a problem?)
				pParticleEmitter = static_cast<CGUIParticleEmitter*>(*i);
				pParticleEmitter->setPosition(pParticle->position);
			}
		}

		// Notify renderer
		mRenderer->_notifyParticleMoved(mActiveParticles);
	}
	//-----------------------------------------------------------------------
	/** Applies the effects of affectors. */
	void CGUIParticleSystem::_triggerAffectors(real timeElapsed)
	{
		TParticleAffectorList::iterator i, itEnd;

		itEnd = mAffectors.end();
		for (i = mAffectors.begin(); i != itEnd; ++i)
		{
			(*i)->_affectParticles(this, timeElapsed);
		}

	}
	//-----------------------------------------------------------------------
	/** Resize the internal pool of particles. */
	void CGUIParticleSystem::increasePool(size_t size)
	{
		size_t oldSize = mParticlePool.size();

		// Increase size
		mParticlePool.reserve(size);
		mParticlePool.resize(size);

		// Create new particles
		for( size_t i = oldSize; i < size; i++ )
		{
			mParticlePool[i] = OGRE_NEW CGUIParticle();
		}

		if (mIsRendererConfigured)
		{
			createVisualParticles(oldSize, size);
		}


	}
	//-----------------------------------------------------------------------
	/** Returns an iterator for stepping through all particles in this system.
	@remarks
	This method is designed to be used by people providing new CGUIParticleAffector subclasses,
	this is the easiest way to step through all the particles in a system and apply the
	changes the affector wants to make.
	*/
	CGUIParticleIterator CGUIParticleSystem::_getIterator(void)
	{
		return CGUIParticleIterator(mActiveParticles.begin(), mActiveParticles.end());
	}
	//-----------------------------------------------------------------------
	/** Retrieve a particle from the system for manual tweaking.
	@remarks
	Normally you use an affector to alter particles in flight, but
	for small manually controlled particle systems you might want to use
	this method.
	*/
	CGUIParticle* CGUIParticleSystem::getParticle(size_t index) 
	{
		assert (index < mActiveParticles.size() && "Index out of bounds!");
		TActiveParticleList::iterator i = mActiveParticles.begin();
		std::advance(i, index);
		return *i;
	}
	//-----------------------------------------------------------------------
	/** Manually add a particle to the system. 
	@remarks
	Instead of using an emitter, you can manually add a particle to the system.
	You must initialise the returned particle instance immediately with the
	'emission' state.
	@note
	There is no corresponding 'destroyParticle' method - if you want to dispose of a
	particle manually (say, if you've used setSpeedFactor(0) to make particles live forever)
	you should use getParticle() and modify it's timeToLive to zero, meaning that it will
	get cleaned up in the next update.
	*/
	CGUIParticle* CGUIParticleSystem::createParticle(void)
	{
		CGUIParticle* p = 0;
		if (!mFreeParticles.empty())
		{
			// Fast creation (don't use superclass since emitter will init)
			p = mFreeParticles.front();
			mActiveParticles.splice(mActiveParticles.end(), mFreeParticles, mFreeParticles.begin());

			p->_notifyOwner(this);
		}

		return p;

	}
	//-----------------------------------------------------------------------
	/** Manually add an emitter particle to the system. 
	@remarks
	The purpose of a particle emitter is to emit particles. Besides visual particles, also other other
	particle types can be emitted, other emitters for example. The emitted emitters have a double role;
	they behave as particles and can be influenced by affectors, but they are still emitters and capable 
	to emit other particles (or emitters). It is possible to create a chain of emitters - emitters 
	emitting other emitters, which also emit emitters.
	@param emitterName The name of a particle emitter that must be emitted.
	*/
	CGUIParticle* CGUIParticleSystem::createEmitterParticle(const CGUIString& emitterName)
	{
		// Get the appropriate list and retrieve an emitter	
		CGUIParticle* p = 0;
		list<CGUIParticleEmitter*>::type* fee = findFreeEmittedEmitter(emitterName);
		if (fee && !fee->empty())
		{
			p = fee->front();
			p->particleType = CGUIParticle::Emitter;
			fee->pop_front();
			mActiveParticles.push_back(p);

			// Also add to mActiveEmittedEmitters. This is needed to traverse through all active emitters
			// that are emitted. Don't use mActiveParticles for that (although they are added to
			// mActiveParticles also), because it would take too long to traverse.
			mActiveEmittedEmitters.push_back(static_cast<CGUIParticleEmitter*>(p));

			p->_notifyOwner(this);
		}

		return p;
	}
	//-----------------------------------------------------------------------
	/** Overridden from MovableObject
	@see
	MovableObject
	*/
	//void CGUIParticleSystem::_updateRenderQueue(RenderQueue* queue)
	//{
	//	if (mRenderer)
	//	{
	//		mRenderer->_updateRenderQueue(queue, mActiveParticles, mCullIndividual);
	//	}
	//}
	//---------------------------------------------------------------------
	/// @copydoc MovableObject::visitRenderables
	//void CGUIParticleSystem::visitRenderables(Renderable::Visitor* visitor, 
	//	bool debugRenderables)
	//{
	//	if (mRenderer)
	//	{
	//		mRenderer->visitRenderables(visitor, debugRenderables);
	//	}
	//}
	//---------------------------------------------------------------------
	/** Internal method for initialising string interface. */
	void CGUIParticleSystem::initParameters(void)
	{
		if (createParamDictionary("CGUIParticleSystem"))
		{
			ParamDictionary* dict = getParamDictionary();

			dict->addParameter(ParameterDef("quota", 
				"The maximum number of particle allowed at once in this system.",
				PT_UNSIGNED_INT),
				&msQuotaCmd);

			dict->addParameter(ParameterDef("emit_emitter_quota", 
				"The maximum number of emitters to be emitted at once in this system.",
				PT_UNSIGNED_INT),
				&msEmittedEmitterQuotaCmd);

			dict->addParameter(ParameterDef("material", 
				"The name of the material to be used to render all particles in this system.",
				PT_STRING),
				&msMaterialCmd);

			dict->addParameter(ParameterDef("particle_width", 
				"The width of particles in world units.",
				PT_REAL),
				&msWidthCmd);

			dict->addParameter(ParameterDef("particle_height", 
				"The height of particles in world units.",
				PT_REAL),
				&msHeightCmd);

			dict->addParameter(ParameterDef("cull_each", 
				"If true, each particle is culled in it's own right. If false, the entire system is culled as a whole.",
				PT_BOOL),
				&msCullCmd);

			dict->addParameter(ParameterDef("renderer", 
				"Sets the particle system renderer to use (default 'billboard').",
				PT_STRING),
				&msRendererCmd);

			dict->addParameter(ParameterDef("sorted", 
				"Sets whether particles should be sorted relative to the camera. ",
				PT_BOOL),
				&msSortedCmd);

			dict->addParameter(ParameterDef("local_space", 
				"Sets whether particles should be kept in local space rather than "
				"emitted into world space. ",
				PT_BOOL),
				&msLocalSpaceCmd);

			dict->addParameter(ParameterDef("iteration_interval", 
				"Sets a fixed update interval for the system, or 0 for the frame rate. ",
				PT_REAL),
				&msIterationIntervalCmd);

			dict->addParameter(ParameterDef("nonvisible_update_timeout", 
				"Sets a timeout on updates to the system if the system is not visible "
				"for the given number of seconds (0 to always update)",
				PT_REAL),
				&msNonvisibleTimeoutCmd);

		}
	}
	//-----------------------------------------------------------------------
	/** Internal method for updating the bounds of the particle system.
	@remarks
	This is called automatically for a period of time after the system's
	creation (10 seconds by default, settable by setBoundsAutoUpdated) 
	to increase (and only increase) the bounds of the system according 
	to the emitted and affected particles. After this period, the 
	system is assumed to achieved its maximum size, and the bounds are
	no longer computed for efficiency. You can tweak the behaviour by 
	either setting the bounds manually (setBounds, preferred), or 
	changing the time over which the bounds are updated (performance cost).
	You can also call this method manually if you need to update the 
	bounds on an ad-hoc basis.
	*/
	void CGUIParticleSystem::_updateBounds()
	{

		//if (mParentNode && (mBoundsAutoUpdate || mBoundsUpdateTime > 0.0f))
		//{
		//	if (mActiveParticles.empty())
		//	{
		//		// No particles, reset to null if auto update bounds
		//		if (mBoundsAutoUpdate)
		//		{
		//			mWorldAABB.setNull();
		//		}
		//	}
		//	else
		//	{
		//		CGUIVector3 min;
		//		CGUIVector3 max;
		//		if (!mBoundsAutoUpdate && mWorldAABB.isFinite())
		//		{
		//			// We're on a limit, grow rather than reset each time
		//			// so that we pick up the worst case scenario
		//			min = mWorldAABB.getMinimum();
		//			max = mWorldAABB.getMaximum();
		//		}
		//		else
		//		{
		//			min.x = min.y = min.z = Math::POS_INFINITY;
		//			max.x = max.y = max.z = Math::NEG_INFINITY;
		//		}
		//		TActiveParticleList::iterator p;
		//		CGUIVector3 halfScale = CGUIVector3::UNIT_SCALE * 0.5;
		//		CGUIVector3 defaultPadding = 
		//			halfScale * std::max(mDefaultHeight, mDefaultWidth);
		//		for (p = mActiveParticles.begin(); p != mActiveParticles.end(); ++p)
		//		{
		//			if ((*p)->mOwnDimensions)
		//			{
		//				CGUIVector3 padding = 
		//					halfScale * std::max((*p)->mWidth, (*p)->mHeight);
		//				min.makeFloor((*p)->position - padding);
		//				max.makeCeil((*p)->position + padding);
		//			}
		//			else
		//			{
		//				min.makeFloor((*p)->position - defaultPadding);
		//				max.makeCeil((*p)->position + defaultPadding);
		//			}
		//		}
		//		mWorldAABB.setExtents(min, max);
		//	}


		//	if (mLocalSpace)
		//	{
		//		// Merge calculated box with current AABB to preserve any user-set AABB
		//		mAABB.merge(mWorldAABB);
		//	}
		//	else
		//	{
		//		// We've already put particles in world space to decouple them from the
		//		// node transform, so reverse transform back since we're expected to 
		//		// provide a local AABB
		//		AxisAlignedBox newAABB(mWorldAABB);
		//		newAABB.transformAffine(mParentNode->_getFullTransform().inverseAffine());

		//		// Merge calculated box with current AABB to preserve any user-set AABB
		//		mAABB.merge(newAABB);
		//	}

		//	mParentNode->needUpdate();
		//}
	}
	//-----------------------------------------------------------------------
	/** Fast-forwards this system by the required number of seconds.
	@remarks
	This method allows you to fast-forward a system so that it effectively looks like
	it has already been running for the time you specify. This is useful to avoid the
	'startup sequence' of a system, when you want the system to be fully populated right
	from the start.
	@param
	time The number of seconds to fast-forward by.
	@param
	interval The sampling interval used to generate particles, apply affectors etc. The lower this
	is the more realistic the fast-forward, but it takes more iterations to do it.
	*/
	void CGUIParticleSystem::fastForward(real time, real interval)
	{
		// First make sure all transforms are up to date

		for (real ftime = 0; ftime < time; ftime += interval)
		{
			_update(interval);
		}
	}
	//-----------------------------------------------------------------------
	/** Sets a 'speed factor' on this particle system, which means it scales the elapsed
	real time which has passed by this factor before passing it to the emitters, affectors,
	and the particle life calculation.
	@remarks
	An interesting side effect - if you want to create a completely manual particle system
	where you control the emission and life of particles yourself, you can set the speed
	factor to 0.0f, thus disabling normal particle emission, alteration, and death.
	*/
	void CGUIParticleSystem::setSpeedFactor(real speedFactor) 
	{ 
		mSpeedFactor = speedFactor; 
	}
	//-----------------------------------------------------------------------
	/** Gets the 'speed factor' on this particle system.
	*/
	real CGUIParticleSystem::getSpeedFactor(void) const
	{ 
		return mSpeedFactor; 
	}
	//-----------------------------------------------------------------------
	/** This is used to turn on or off particle emission for this system.
	@remarks
	By default particle system is always emitting particles (if a emitters exists)
	and this can be used to stop the emission for all emitters. To turn it on again, 
	call it passing true.

	Note that this does not detach the particle system from the scene node, it will 
	still use some CPU.
	*/
	void CGUIParticleSystem::setEmitting(bool v)
	{
		mIsEmitting = v;
	}
	//-----------------------------------------------------------------------
	/** Returns true if the particle system emitting flag is turned on.
	@remarks
	This function will not actually return whether the particles are being emitted.
	It only returns the value of emitting flag.
	*/
	bool CGUIParticleSystem::getEmitting() const
	{
		return mIsEmitting;
	}
	//-----------------------------------------------------------------------
	/** Overridden from MovableObject */
	const CGUIString& CGUIParticleSystem::getMovableType(void) const
	{
		return CGUIParticleSystemFactory::FACTORY_TYPE_NAME;
	}
	//-----------------------------------------------------------------------
	/** Internal callback used by Particles to notify their parent that they have been resized.
	*/
	void CGUIParticleSystem::_notifyParticleResized(void)
	{
		if (mRenderer)
		{
			mRenderer->_notifyParticleResized();
		}
	}
	//-----------------------------------------------------------------------
	/** Internal callback used by Particles to notify their parent that they have been rotated.
	*/
	void CGUIParticleSystem::_notifyParticleRotated(void)
	{
		if (mRenderer)
		{
			mRenderer->_notifyParticleRotated();
		}
	}
	//-----------------------------------------------------------------------
	/** Sets the default dimensions of the particles in this set.
	@remarks
	All particles in a set are created with these default dimensions. The set will render most efficiently if
	all the particles in the set are the default size. It is possible to alter the size of individual
	particles at the expense of extra calculation. See the CGUIParticle class for more info.
	@param width
	The new default width for the particles in this set.
	@param height
	The new default height for the particles in this set.
	*/
	void CGUIParticleSystem::setDefaultDimensions( real width, real height )
	{
		mDefaultWidth = width;
		mDefaultHeight = height;
		if (mRenderer)
		{
			mRenderer->_notifyDefaultDimensions(width, height);
		}
	}
	//-----------------------------------------------------------------------
	/** See setDefaultDimensions - this sets 1 component individually. */
	void CGUIParticleSystem::setDefaultWidth(real width)
	{
		mDefaultWidth = width;
		if (mRenderer)
		{
			mRenderer->_notifyDefaultDimensions(mDefaultWidth, mDefaultHeight);
		}
	}
	//-----------------------------------------------------------------------
	/** See setDefaultDimensions - this gets 1 component individually. */
	real CGUIParticleSystem::getDefaultWidth(void) const
	{
		return mDefaultWidth;
	}
	//-----------------------------------------------------------------------
	/** See setDefaultDimensions - this sets 1 component individually. */
	void CGUIParticleSystem::setDefaultHeight(real height)
	{
		mDefaultHeight = height;
		if (mRenderer)
		{
			mRenderer->_notifyDefaultDimensions(mDefaultWidth, mDefaultHeight);
		}
	}
	//-----------------------------------------------------------------------
	/** See setDefaultDimensions - this gets 1 component individually. */
	real CGUIParticleSystem::getDefaultHeight(void) const
	{
		return mDefaultHeight;
	}
	//-----------------------------------------------------------------------
	/** Overridden from MovableObject
	@see
	MovableObject
	*/
	//void CGUIParticleSystem::_notifyCurrentCamera(Camera* cam)
	//{
	//	MovableObject::_notifyCurrentCamera(cam);

	//	// Record visible
	//	if (isVisible())
	//	{			
	//		mLastVisibleFrame = Root::getSingleton().getNextFrameNumber();
	//		mTimeSinceLastVisible = 0.0f;

	//		if (mSorted)
	//		{
	//			_sortParticles(cam);
	//		}

	//		if (mRenderer)
	//		{
	//			if (!mIsRendererConfigured)
	//				configureRenderer();

	//			mRenderer->_notifyCurrentCamera(cam);
	//		}
	//	}
	//}
	//-----------------------------------------------------------------------
	/** Overridden from MovableObject
	@see
	MovableObject
	*/
	//const AxisAlignedBox& CGUIParticleSystem::getBoundingBox(void) const 
	//{
	//	return mAABB;
	//}

	//-----------------------------------------------------------------------
	/** Overridden from MovableObject
	@see
	MovableObject
	*/
	//real CGUIParticleSystem::getBoundingRadius(void) const 
	//{
	//	return mBoundingRadius; 
	//}
	//-----------------------------------------------------------------------
	/** Overridden from MovableObject
	@see
	MovableObject
	*/
	//void CGUIParticleSystem::_notifyAttached(Node* parent, bool isTagPoint)
	//{
	//	MovableObject::_notifyAttached(parent, isTagPoint);
	//	if (mRenderer && mIsRendererConfigured)
	//	{
	//		mRenderer->_notifyAttached(parent, isTagPoint);
	//	}

	//	if (parent && !mTimeController)
	//	{
	//		// Assume visible
	//		mTimeSinceLastVisible = 0;
	//		mLastVisibleFrame = Root::getSingleton().getNextFrameNumber();

	//		// Create time controller when attached
	//		ControllerManager& mgr = ControllerManager::getSingleton(); 
	//		ControllerValueRealPtr updValue(OGRE_NEW ParticleSystemUpdateValue(this));
	//		mTimeController = mgr.createFrameTimePassthroughController(updValue);
	//	}
	//	else if (!parent && mTimeController)
	//	{
	//		// Destroy controller
	//		ControllerManager::getSingleton().destroyController(mTimeController);
	//		mTimeController = 0;
	//	}
	//}
	//-----------------------------------------------------------------------
	/** Sets the name of the material to be used for this billboard set.
	@param
	name The new name of the material to use for this set.
	*/
	void CGUIParticleSystem::setMaterialName( const CGUIString& name )
	{
		mMaterialName = name;
		//if (mIsRendererConfigured)
		//{
		//	MaterialPtr mat = MaterialManager::getSingleton().load(
		//		mMaterialName, mResourceGroupName);
		//	mRenderer->_setMaterial(mat);
		//}
	}
	//-----------------------------------------------------------------------
	/** Sets the name of the material to be used for this billboard set.
	@returns The name of the material that is used for this set.
	*/
	const CGUIString& CGUIParticleSystem::getMaterialName(void) const
	{
		return mMaterialName;
	}
	//-----------------------------------------------------------------------
	/** Empties this set of all particles.
	*/
	void CGUIParticleSystem::clear()
	{
		// Notify renderer if exists
		if (mRenderer)
		{
			mRenderer->_notifyParticleCleared(mActiveParticles);
		}

		// Move actives to free list
		mFreeParticles.splice(mFreeParticles.end(), mActiveParticles);

		// Add active emitted emitters to free list
		addActiveEmittedEmittersToFreeList();

		// Remove all active emitted emitter instances
		mActiveEmittedEmitters.clear();

		// Reset update remain time
		mUpdateRemainTime = 0;
	}
	//-----------------------------------------------------------------------
	/** Sets the ParticleRenderer to be used to render this particle system.
	@remarks
	The main CGUIParticleSystem just manages the creation and movement of 
	particles; they are rendered using functions in ParticleRenderer
	and the ParticleVisual instances they create.
	@param typeName CGUIString identifying the type of renderer to use; a new 
	instance of this type will be created; a factory must have been registered
	with CGUIParticleSystemManager.
	*/
	void CGUIParticleSystem::setRenderer(const CGUIString& rendererName)
	{
		if (mRenderer)
		{
			// Destroy existing
			destroyVisualParticles(0, mParticlePool.size());
			CGUIParticleSystemManager::getSingleton()._destroyRenderer(mRenderer);
			mRenderer = 0;
		}

		if (!rendererName.empty())
		{
			mRenderer = CGUIParticleSystemManager::getSingleton()._createRenderer(rendererName);
			mIsRendererConfigured = false;
		}
	}
	//-----------------------------------------------------------------------
	/** Internal method to configure the renderer. */
	void CGUIParticleSystem::configureRenderer(void)
	{
		//// Actual allocate particles
		//size_t currSize = mParticlePool.size();
		//size_t size = mPoolSize;
		//if( currSize < size )
		//{
		//	this->increasePool(size);

		//	for( size_t i = currSize; i < size; ++i )
		//	{
		//		// Add new items to the queue
		//		mFreeParticles.push_back( mParticlePool[i] );
		//	}

		//	// Tell the renderer, if already configured
		//	if (mRenderer && mIsRendererConfigured)
		//	{
		//		mRenderer->_notifyParticleQuota(size);
		//	}
		//}

		//if (mRenderer && !mIsRendererConfigured)
		//{
		//	mRenderer->_notifyParticleQuota(mParticlePool.size());
		//	mRenderer->_notifyAttached(mParentNode, mParentIsTagPoint);
		//	mRenderer->_notifyDefaultDimensions(mDefaultWidth, mDefaultHeight);
		//	createVisualParticles(0, mParticlePool.size());
		//	MaterialPtr mat = MaterialManager::getSingleton().load(
		//		mMaterialName, mResourceGroupName);
		//	mRenderer->_setMaterial(mat);
		//	if (mRenderQueueIDSet)
		//		mRenderer->setRenderQueueGroup(mRenderQueueID);
		//	mRenderer->setKeepParticlesInLocalSpace(mLocalSpace);
		//	mIsRendererConfigured = true;
		//}
	}
	//-----------------------------------------------------------------------
	/** Gets the ParticleRenderer to be used to render this particle system. */
	CGUIParticleSystemRenderer* CGUIParticleSystem::getRenderer(void) const
	{
		return mRenderer;
	}
	//-----------------------------------------------------------------------
	/** Gets the name of the ParticleRenderer to be used to render this particle system. */
	const CGUIString& CGUIParticleSystem::getRendererName(void) const
	{
		if (mRenderer)
		{
			return mRenderer->getType();
		}
		else
		{
			return StringUtil::BLANK;
		}
	}
	//-----------------------------------------------------------------------
	/** Returns whether or not particles in this are tested individually for culling. */
	bool CGUIParticleSystem::getCullIndividually(void) const
	{
		return mCullIndividual;
	}
	//-----------------------------------------------------------------------
	/** Sets whether culling tests particles in this individually as well as in a group.
	@remarks
	Particle sets are always culled as a whole group, based on a bounding box which 
	encloses all particles in the set. For fairly localised sets, this is enough. However, you
	can optionally tell the set to also cull individual particles in the set, i.e. to test
	each individual particle before rendering. The default is not to do this.
	@par
	This is useful when you have a large, fairly distributed set of particles, like maybe 
	trees on a landscape. You probably still want to group them into more than one
	set (maybe one set per section of landscape), which will be culled coarsely, but you also
	want to cull the particles individually because they are spread out. Whilst you could have
	lots of single-tree sets which are culled separately, this would be inefficient to render
	because each tree would be issued as it's own rendering operation.
	@par
	By calling this method with a parameter of true, you can have large particle sets which 
	are spaced out and so get the benefit of batch rendering and coarse culling, but also have
	fine-grained culling so unnecessary rendering is avoided.
	@param cullIndividual If true, each particle is tested before being sent to the pipeline as well 
	as the whole set having to pass the coarse group bounding test.
	*/
	void CGUIParticleSystem::setCullIndividually(bool cullIndividual)
	{
		mCullIndividual = cullIndividual;
	}
	//-----------------------------------------------------------------------
	/** Get the origin of this particle system, e.g. a script file name.
	@remarks
	This property will only contain something if the creator of
	this particle system chose to populate it. Script loaders are advised
	to populate it.
	*/
	const CGUIString& CGUIParticleSystem::getOrigin(void) const 
	{
		return mOrigin; 
	}
	//-----------------------------------------------------------------------
	/// Notify this particle system of it's origin
	void CGUIParticleSystem::_notifyOrigin(const CGUIString& origin)
	{
		mOrigin = origin; 
	}
	//-----------------------------------------------------------------------
	/// Internal method for creating ParticleVisualData instances for the pool
	void CGUIParticleSystem::createVisualParticles(size_t poolstart, size_t poolend)
	{
		TParticlePool::iterator i = mParticlePool.begin();
		TParticlePool::iterator iend = mParticlePool.begin();
		std::advance(i, poolstart);
		std::advance(iend, poolend);
		for (; i != iend; ++i)
		{
			(*i)->_notifyVisualData(
				mRenderer->_createVisualData());
		}
	}
	//-----------------------------------------------------------------------
	/// Internal method for destroying ParticleVisualData instances for the pool
	void CGUIParticleSystem::destroyVisualParticles(size_t poolstart, size_t poolend)
	{
		TParticlePool::iterator i = mParticlePool.begin();
		TParticlePool::iterator iend = mParticlePool.begin();
		std::advance(i, poolstart);
		std::advance(iend, poolend);
		for (; i != iend; ++i)
		{
			mRenderer->_destroyVisualData((*i)->getVisualData());
			(*i)->_notifyVisualData(0);
		}
	}
	//-----------------------------------------------------------------------
	/** Set the (initial) bounds of the particle system manually. 
	@remarks
	If you can, set the bounds of a particle system up-front and 
	call setBoundsAutoUpdated(false); this is the most efficient way to
	organise it. Otherwise, set an initial bounds and let the bounds increase
	for a little while (the default is 5 seconds), after which time the 
	AABB is fixed to save time.
	@param aabb Bounds in local space.
	*/
	//void CGUIParticleSystem::setBounds(const AxisAlignedBox& aabb)
	//{
	//	mAABB = aabb;
	//	mBoundingRadius = Math::boundingRadiusFromAABB(mAABB);

	//}
	//-----------------------------------------------------------------------
	/** Sets whether the bounds will be automatically updated
	for the life of the particle system
	@remarks
	If you have a stationary particle system, it would be a good idea to
	call this method and set the value to 'false', since the maximum
	bounds of the particle system will eventually be static. If you do
	this, you can either set the bounds manually using the setBounds()
	method, or set the second parameter of this method to a positive
	number of seconds, so that the bounds are calculated for a few
	seconds and then frozen.
	@param autoUpdate If true (the default), the particle system will
	update it's bounds every frame. If false, the bounds update will 
	cease after the 'stopIn' number of seconds have passed.
	@param stopIn Only applicable if the first parameter is true, this is the
	number of seconds after which the automatic update will cease.
	*/
	//void CGUIParticleSystem::setBoundsAutoUpdated(bool autoUpdate, real stopIn)
	//{
	//	mBoundsAutoUpdate = autoUpdate;
	//	mBoundsUpdateTime = stopIn;
	//}
	//-----------------------------------------------------------------------
	/** @copydoc MovableObject::setRenderQueueGroup */
	void CGUIParticleSystem::setRenderQueueGroup(uint8 queueID)
	{
		MovableObject::setRenderQueueGroup(queueID);
		if (mRenderer)
		{
			mRenderer->setRenderQueueGroup(queueID);
		}
	}
	//-----------------------------------------------------------------------
	/** Set whether or not particles are sorted according to the camera.
	@remarks
	Enabling sorting alters the order particles are sent to the renderer.
	When enabled, particles are sent to the renderer in order of 
	furthest distance from the camera.
	*/
	void CGUIParticleSystem::setSortingEnabled(bool enabled) 
	{ 
		mSorted = enabled; 
	}
	//-----------------------------------------------------------------------
	/// Gets whether particles are sorted relative to the camera.
	bool CGUIParticleSystem::getSortingEnabled(void) const
	{
		return mSorted; 
	}
	//-----------------------------------------------------------------------
	/** Set whether or not particles are sorted according to the camera.
	@remarks
	Enabling sorting alters the order particles are sent to the renderer.
	When enabled, particles are sent to the renderer in order of 
	furthest distance from the camera.
	*/
	void CGUIParticleSystem::setSortingEnabled(bool enabled) 
	{
		mSorted = enabled; 
	}
	//-----------------------------------------------------------------------
	/// Gets whether particles are sorted relative to the camera.
	bool CGUIParticleSystem::getSortingEnabled(void) const 
	{ 
		return mSorted; 
	}
	//-----------------------------------------------------------------------
	/** Sets whether particles (and any affector effects) remain relative 
	to the node the particle system is attached to.
	@remarks
	By default particles are in world space once emitted, so they are not
	affected by movement in the parent node of the particle system. This
	makes the most sense when dealing with completely independent particles, 
	but if you want to constrain them to follow local motion too, you
	can set this to true.
	*/
	void CGUIParticleSystem::setKeepParticlesInLocalSpace(bool keepLocal)
	{
		mLocalSpace = keepLocal;
		if (mRenderer)
		{
			mRenderer->setKeepParticlesInLocalSpace(keepLocal);
		}
	}
	//-----------------------------------------------------------------------
	/** Gets whether particles (and any affector effects) remain relative 
	to the node the particle system is attached to.
	*/
	bool CGUIParticleSystem::getKeepParticlesInLocalSpace(void) const 
	{
		return mLocalSpace;
	}
	//-----------------------------------------------------------------------
	/** Sort the particles in the system **/
	//void CGUIParticleSystem::_sortParticles(Camera* cam)
	//{
	//	if (mRenderer)
	//	{
	//		SortMode sortMode = mRenderer->_getSortMode();
	//		if (sortMode == SM_DIRECTION)
	//		{
	//			CGUIVector3 camDir = cam->getDerivedDirection();
	//			if (mLocalSpace)
	//			{
	//				// transform the camera direction into local space
	//				camDir = mParentNode->_getDerivedOrientation().UnitInverse() * camDir;
	//			}
	//			mRadixSorter.sort(mActiveParticles, SortByDirectionFunctor(- camDir));
	//		}
	//		else if (sortMode == SM_DISTANCE)
	//		{
	//			CGUIVector3 camPos = cam->getDerivedPosition();
	//			if (mLocalSpace)
	//			{
	//				// transform the camera position into local space
	//				camPos = mParentNode->_getDerivedOrientation().UnitInverse() *
	//					(camPos - mParentNode->_getDerivedPosition()) / mParentNode->_getDerivedScale();
	//			}
	//			mRadixSorter.sort(mActiveParticles, SortByDistanceFunctor(camPos));
	//		}
	//	}
	//}
	//-----------------------------------------------------------------------
	CGUIParticleSystem::SortByDirectionFunctor::SortByDirectionFunctor(const CGUIVector3& dir)
		: sortDir(dir)
	{
	}
	//-----------------------------------------------------------------------
	float CGUIParticleSystem::SortByDirectionFunctor::operator()(CGUIParticle* p) const
	{
		return sortDir.dotProduct(p->position);
	}
	//-----------------------------------------------------------------------
	CGUIParticleSystem::SortByDistanceFunctor::SortByDistanceFunctor(const CGUIVector3& pos)
		: sortPos(pos)
	{
	}
	//-----------------------------------------------------------------------
	float CGUIParticleSystem::SortByDistanceFunctor::operator()(CGUIParticle* p) const
	{
		// Sort descending by squared distance
		return - (sortPos - p->position).squaredLength();
	}
	//-----------------------------------------------------------------------
	/** Create a pool of emitted emitters and assign them to the free emitter list.
	@remarks
	The emitters in the pool are grouped by name. This name is the name of the base emitter in the
	main list with particle emitters, which forms the template of the created emitted emitters.
	*/
	void CGUIParticleSystem::initialiseEmittedEmitters(void)
	{
		// Initialise the pool if needed
		size_t currSize = 0;
		if (mEmittedEmitterPool.empty())
		{
			if (mEmittedEmitterPoolInitialised)
			{
				// It was already initialised, but apparently no emitted emitters were used
				return;
			}
			else
			{
				initialiseEmittedEmitterPool();
			}
		}
		else
		{
			TEmittedEmitterPool::iterator i;
			for (i = mEmittedEmitterPool.begin(); i != mEmittedEmitterPool.end(); ++i)
			{
				currSize += i->second.size();
			}
		}

		size_t size = mEmittedEmitterPoolSize;
		if( currSize < size && !mEmittedEmitterPool.empty())
		{
			// Increase the pool. Equally distribute over all vectors in the map
			increaseEmittedEmitterPool(size);

			// Add new items to the free list
			addFreeEmittedEmitters();
		}
	}

	//-----------------------------------------------------------------------
	/** Determine which emitters in the Particle Systems main emitter become a template for creating an
	pool of emitters that can be emitted.
	*/
	void CGUIParticleSystem::initialiseEmittedEmitterPool(void)
	{
		if (mEmittedEmitterPoolInitialised)
			return;

		// Run through mEmitters and add keys to the pool
		TParticleEmitterList::iterator emitterIterator;
		TParticleEmitterList::iterator emitterIteratorInner;
		CGUIParticleEmitter* emitter = 0;
		CGUIParticleEmitter* emitterInner = 0;
		for (emitterIterator = mEmitters.begin(); emitterIterator != mEmitters.end(); ++emitterIterator)
		{
			// Determine the names of all emitters that are emitted
			emitter = *emitterIterator ;
			if (emitter && emitter->getEmittedEmitter() != StringUtil::BLANK)
			{
				// This one will be emitted, register its name and leave the vector empty!
				TEmittedEmitterList empty;
				mEmittedEmitterPool.insert(make_pair(emitter->getEmittedEmitter(), empty));
			}

			// Determine whether the emitter itself will be emitted and set the 'mEmitted' attribute
			for (emitterIteratorInner = mEmitters.begin(); emitterIteratorInner != mEmitters.end(); ++emitterIteratorInner)
			{
				emitterInner = *emitterIteratorInner;
				if (emitter && 
					emitterInner && 
					emitter->getName() != StringUtil::BLANK && 
					emitter->getName() == emitterInner->getEmittedEmitter())
				{
					emitter->setEmitted(true);
					break;
				}
				else
				{
					// Set explicitly to 'false' although the default value is already 'false'
					emitter->setEmitted(false);
				}
			}
		}

		mEmittedEmitterPoolInitialised = true;
	}
	//-----------------------------------------------------------------------
	/** Resize the internal pool of emitted emitters.
	@remarks
	The pool consists of multiple vectors containing pointers to particle emitters. Increasing the 
	pool with size implies that the vectors are equally increased. The quota of emitted emitters is 
	defined on a particle system level and not on a particle emitter level. This is to prevent that
	the number of created emitters becomes too high; the quota is shared amongst the emitted emitters.
	*/
	void CGUIParticleSystem::increaseEmittedEmitterPool(size_t size)
	{
		// Don't proceed if the pool doesn't contain any keys of emitted emitters
		if (mEmittedEmitterPool.empty())
			return;

		TEmittedEmitterPool::iterator emittedEmitterPoolIterator;
		TParticleEmitterList::iterator emitterIterator;
		CGUIParticleEmitter* emitter = 0;
		CGUIParticleEmitter* clonedEmitter = 0;
		CGUIString name = StringUtil::BLANK;
		TEmittedEmitterList* e = 0;
		size_t maxNumberOfEmitters = size / mEmittedEmitterPool.size(); // equally distribute the number for each emitted emitter list
		size_t oldSize = 0;

		// Run through mEmittedEmitterPool and search for every key (=name) its corresponding emitter in mEmitters
		for (emittedEmitterPoolIterator = mEmittedEmitterPool.begin(); emittedEmitterPoolIterator != mEmittedEmitterPool.end(); ++emittedEmitterPoolIterator)
		{
			name = emittedEmitterPoolIterator->first;
			e = &emittedEmitterPoolIterator->second;

			// Search the correct emitter in the mEmitters vector
			emitter = 0;
			for (emitterIterator = mEmitters.begin(); emitterIterator != mEmitters.end(); ++emitterIterator)
			{
				emitter = *emitterIterator;
				if (emitter && 
					name != StringUtil::BLANK && 
					name == emitter->getName())
				{		
					// Found the right emitter, clone each emitter a number of times
					oldSize = e->size();
					for (size_t t = oldSize; t < maxNumberOfEmitters; ++t)
					{
						clonedEmitter = CGUIParticleSystemManager::getSingleton()._createEmitter(emitter->getType(), this);
						emitter->copyParametersTo(clonedEmitter);
						clonedEmitter->setEmitted(emitter->isEmitted()); // is always 'true' by the way, but just in case

						// Initially deactivate the emitted emitter if duration/repeat_delay are set
						if (clonedEmitter->getDuration() > 0.0f && 
							(clonedEmitter->getRepeatDelay() > 0.0f || clonedEmitter->getMinRepeatDelay() > 0.0f || clonedEmitter->getMinRepeatDelay() > 0.0f))
							clonedEmitter->setEnabled(false);

						// Add cloned emitters to the pool
						e->push_back(clonedEmitter);
					}
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	/** Add  emitters from the pool to the free emitted emitter queue. */
	void CGUIParticleSystem::addFreeEmittedEmitters(void)
	{
		// Don't proceed if the TEmittedEmitterPool is empty
		if (mEmittedEmitterPool.empty())
			return;

		// Copy all pooled emitters to the free list
		TEmittedEmitterPool::iterator emittedEmitterPoolIterator;
		TEmittedEmitterList::iterator emittedEmitterIterator;
		TEmittedEmitterList* emittedEmitters = 0;
		list<CGUIParticleEmitter*>::type* fee = 0;
		CGUIString name = StringUtil::BLANK;

		// Run through the emittedEmitterPool map
		for (emittedEmitterPoolIterator = mEmittedEmitterPool.begin(); emittedEmitterPoolIterator != mEmittedEmitterPool.end(); ++emittedEmitterPoolIterator)
		{
			name = emittedEmitterPoolIterator->first;
			emittedEmitters = &emittedEmitterPoolIterator->second;
			fee = findFreeEmittedEmitter(name);

			// If its not in the map, create an empty one
			if (!fee)
			{
				TFreeEmittedEmitterList empty;
				mFreeEmittedEmitters.insert(make_pair(name, empty));
				fee = findFreeEmittedEmitter(name);
			}

			// Check anyway if its ok now
			if (!fee)
				return; // forget it!

			// Add all emitted emitters from the pool to the free list
			for(emittedEmitterIterator = emittedEmitters->begin(); emittedEmitterIterator != emittedEmitters->end(); ++emittedEmitterIterator)
			{
				fee->push_back(*emittedEmitterIterator);
			}
		}
	}
	//-----------------------------------------------------------------------
	/** Removes all emitted emitters from this system.	*/
	void CGUIParticleSystem::removeAllEmittedEmitters(void)
	{
		TEmittedEmitterPool::iterator emittedEmitterPoolIterator;
		TEmittedEmitterList::iterator emittedEmitterListIterator;
		TEmittedEmitterList* e = 0;
		for (emittedEmitterPoolIterator = mEmittedEmitterPool.begin(); emittedEmitterPoolIterator != mEmittedEmitterPool.end(); ++emittedEmitterPoolIterator)
		{
			e = &emittedEmitterPoolIterator->second;
			for (emittedEmitterListIterator = e->begin(); emittedEmitterListIterator != e->end(); ++emittedEmitterListIterator)
			{
				CGUIParticleSystemManager::getSingleton()._destroyEmitter(*emittedEmitterListIterator);
			}
			e->clear();
		}

		// Dont leave any references behind
		mEmittedEmitterPool.clear();
		mFreeEmittedEmitters.clear();
		mActiveEmittedEmitters.clear();
	}
	//-----------------------------------------------------------------------
	/** Find the list with free emitted emitters.
	@param name The name that identifies the list with free emitted emitters.
	*/
	list<CGUIParticleEmitter*>::type* CGUIParticleSystem::findFreeEmittedEmitter (const CGUIString& name)
	{
		TFreeEmittedEmitterMap::iterator it;
		it = mFreeEmittedEmitters.find (name);
		if (it != mFreeEmittedEmitters.end())
		{
			// Found it
			return &it->second;
		}

		return 0;
	}
	//-----------------------------------------------------------------------
	/** Removes an emitter from the active emitted emitter list.
	@remarks
	The emitter will not be destroyed!
	@param emitter Pointer to a particle emitter.
	*/
	void CGUIParticleSystem::removeFromActiveEmittedEmitters (CGUIParticleEmitter* emitter)
	{
		assert(emitter && "Emitter to be removed is 0!");
		TActiveEmittedEmitterList::iterator itActiveEmit;
		for (itActiveEmit = mActiveEmittedEmitters.begin(); itActiveEmit != mActiveEmittedEmitters.end(); ++itActiveEmit)
		{
			if (emitter == (*itActiveEmit))
			{
				mActiveEmittedEmitters.erase(itActiveEmit);
				break;
			}
		}
	}
	//-----------------------------------------------------------------------
	/** Moves all emitted emitters from the active list to the free list
	@remarks
	The active emitted emitter list will not be cleared and still keeps references to the emitters!
	*/
	void CGUIParticleSystem::addActiveEmittedEmittersToFreeList (void)
	{
		TActiveEmittedEmitterList::iterator itActiveEmit;
		for (itActiveEmit = mActiveEmittedEmitters.begin(); itActiveEmit != mActiveEmittedEmitters.end(); ++itActiveEmit)
		{
			list<CGUIParticleEmitter*>::type* fee = findFreeEmittedEmitter ((*itActiveEmit)->getName());
			if (fee)
				fee->push_back(*itActiveEmit);
		}
	}
	//-----------------------------------------------------------------------
	/** This function clears all data structures that are used in combination with emitted emitters and
	sets the flag to indicate that the emitted emitter pool must be initialised again.
	@remarks
	This function should be called if new emitters are added to a CGUIParticleSystem or deleted from a
	CGUIParticleSystem. The emitted emitter data structures become out of sync and need to be build up
	again. The data structures are not reorganised in this function, but by setting a flag, 
	they are rebuild in the regular process flow.
	*/
	void CGUIParticleSystem::_notifyReorganiseEmittedEmitterData (void)
	{
		removeAllEmittedEmitters();
		mEmittedEmitterPoolInitialised = false; // Dont rearrange immediately; it will be performed in the regular flow
	}
}
