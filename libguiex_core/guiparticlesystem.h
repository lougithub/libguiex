/** 
* @file guiparticlesystem.h
* @brief 
* @author ken
* @date 2011-01-18
*/

#ifndef __GUI_PARTICLESYSTEM_20110118_H__
#define __GUI_PARTICLESYSTEM_20110118_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guiparticleiterator.h"
#include "guivector3.h"
#include <vector>
#include <map>
#include <list>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIParticleEmitter;
	class CGUIParticleSystemRenderer;
	class CGUIParticleAffector;
	class CGUIParticle;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex 
{
   class CGUIParticleSystem
    {
		typedef std::list<CGUIParticleEmitter*> TFreeEmittedEmitterList;
		typedef std::list<CGUIParticleEmitter*> TActiveEmittedEmitterList;
		typedef std::vector<CGUIParticleEmitter*> TEmittedEmitterList;
		typedef std::map<CGUIString, TFreeEmittedEmitterList> TFreeEmittedEmitterMap;
		typedef std::map<CGUIString, TEmittedEmitterList> TEmittedEmitterPool;

    public:
        CGUIParticleSystem();
        CGUIParticleSystem(const CGUIString& name, const CGUIString& resourceGroupName);
        virtual ~CGUIParticleSystem();

        void setRenderer(const CGUIString& typeName);
        CGUIParticleSystemRenderer* getRenderer(void) const;
        const CGUIString& getRendererName(void) const;

        CGUIParticleEmitter* addEmitter(const CGUIString& emitterType);
        CGUIParticleEmitter* getEmitter(uint16 index) const;
        uint16 getNumEmitters(void) const;
        void removeEmitter(uint16 index);
        void removeAllEmitters(void);

        CGUIParticleAffector* addAffector(const CGUIString& affectorType);
        CGUIParticleAffector* getAffector(uint16 index) const;
        uint16 getNumAffectors(void) const;
        void removeAffector(uint16 index);
        void removeAllAffectors(void);

        void clear();

        size_t getNumParticles(void) const;
		CGUIParticle* createParticle(void);
		CGUIParticle* createEmitterParticle(const CGUIString& emitterName);
		CGUIParticle* getParticle(size_t index);

        size_t getParticleQuota(void) const;
        void setParticleQuota(size_t quota);

        size_t getEmittedEmitterQuota(void) const;
        void setEmittedEmitterQuota(size_t quota);

        CGUIParticleSystem& operator=(const CGUIParticleSystem& rhs);

        void _update(real timeElapsed);

        CGUIParticleIterator _getIterator(void);

        virtual void setMaterialName( const CGUIString& name );
        virtual const CGUIString& getMaterialName(void) const;
  //      virtual void _notifyCurrentCamera(Camera* cam);
  //      void _notifyAttached(Node* parent, bool isTagPoint = false);
  //      virtual const AxisAlignedBox& getBoundingBox(void) const;
  //      virtual real getBoundingRadius(void) const;
  //      virtual void _updateRenderQueue(RenderQueue* queue);
		//void visitRenderables( Renderable::Visitor* visitor, bool debugRenderables = false);
		
		//void setBounds(const AxisAlignedBox& aabb);
		//void setBoundsAutoUpdated(bool autoUpdate, real stopIn = 0.0f);

        void fastForward(real time, real interval = 0.1);
		void setSpeedFactor(real speedFactor);
		real getSpeedFactor(void) const;

        void setIterationInterval(real iterationInterval);
        real getIterationInterval(void) const;

        static void setDefaultIterationInterval(real iterationInterval);
        static real getDefaultIterationInterval(void);

		void setNonVisibleUpdateTimeout(real timeout);
		real getNonVisibleUpdateTimeout(void) const;
		static void setDefaultNonVisibleUpdateTimeout(real timeout) ;
		static real getDefaultNonVisibleUpdateTimeout(void);

        const CGUIString& getMovableType(void) const;

        virtual void _notifyParticleResized(void);
        virtual void _notifyParticleRotated(void);

        virtual void setDefaultDimensions(real width, real height);

        virtual void setDefaultWidth(real width);
        virtual real getDefaultWidth(void) const;
        virtual void setDefaultHeight(real height);
        virtual real getDefaultHeight(void) const;
        virtual bool getCullIndividually(void) const;
        virtual void setCullIndividually(bool cullIndividual);

		const CGUIString& getOrigin(void) const;
		void _notifyOrigin(const CGUIString& origin) { mOrigin = origin; }

		void setRenderQueueGroup(uint8 queueID);

		void setSortingEnabled(bool enabled);
		bool getSortingEnabled(void) const;

		void setKeepParticlesInLocalSpace(bool keepLocal);
		bool getKeepParticlesInLocalSpace(void) const;

        void _updateBounds(void);

		void setEmitting(bool v);
		bool getEmitting() const;

	protected:
        void _expire(real timeElapsed);

        void _triggerEmitters(real timeElapsed);

		void _executeTriggerEmitters(CGUIParticleEmitter* emitter, unsigned requested, real timeElapsed);

        void _applyMotion(real timeElapsed);

        void _triggerAffectors(real timeElapsed);

		//void _sortParticles(Camera* cam);

        void increasePool(size_t size);
		void increaseEmittedEmitterPool(size_t size);

        void initParameters(void);
        void configureRenderer(void);

		void createVisualParticles(size_t poolstart, size_t poolend);
		void destroyVisualParticles(size_t poolstart, size_t poolend);

		void initialiseEmittedEmitters(void);
		void initialiseEmittedEmitterPool(void);
		void addFreeEmittedEmitters(void);
		void removeAllEmittedEmitters(void);
		TFreeEmittedEmitterList* findFreeEmittedEmitter (const CGUIString& name);
		void removeFromActiveEmittedEmitters (CGUIParticleEmitter* emitter);
		void addActiveEmittedEmittersToFreeList (void);


		void _notifyReorganiseEmittedEmitterData (void);

    protected:
//        AxisAlignedBox mAABB;
        real mBoundingRadius;
        bool mBoundsAutoUpdate;
        real mBoundsUpdateTime;
        real mUpdateRemainTime;

        /// World AABB, only used to compare world-space positions to calc bounds
 //       AxisAlignedBox mWorldAABB;

        /// Name of the resource group to use to load materials
        //CGUIString mResourceGroupName;
        /// Name of the material to use
        CGUIString mMaterialName;
        /// Have we set the material etc on the renderer?
        bool mIsRendererConfigured;
        /// Pointer to the material to use
//        MaterialPtr mpMaterial;
        /// Default width of each particle
        real mDefaultWidth;
        /// Default height of each particle
        real mDefaultHeight;
		/// Speed factor
		real mSpeedFactor;
        /// Iteration interval
        real mIterationInterval;
        /// Iteration interval set? Otherwise track default
        bool mIterationIntervalSet;
		/// Particles sorted according to camera?
		bool mSorted;
		/// Particles in local space?
		bool mLocalSpace;
		/// Update timeout when nonvisible (0 for no timeout)
		real mNonvisibleTimeout;
		/// Update timeout when nonvisible set? Otherwise track default
		bool mNonvisibleTimeoutSet;
		/// Amount of time non-visible so far
		real mTimeSinceLastVisible;
		/// Last frame in which known to be visible
//		unsigned long mLastVisibleFrame;
		/// Controller for time update
//		Controller<real>* mTimeController;
        /// Indication whether the emitted emitter pool (= pool with particle emitters that are emitted) is initialised
		bool mEmittedEmitterPoolInitialised;
		/// Used to control if the particle system should emit particles or not.
		bool mIsEmitting;

        typedef std::list<CGUIParticle*> TActiveParticleList;
        typedef std::list<CGUIParticle*> TFreeParticleList;
        typedef std::vector<CGUIParticle*> TParticlePool;

        /** Sort by direction functor */
        struct SortByDirectionFunctor
        {
            /// Direction to sort in
            CGUIVector3 sortDir;

            SortByDirectionFunctor(const CGUIVector3& dir);
            float operator()(CGUIParticle* p) const;
        };

        /** Sort by distance functor */
        struct SortByDistanceFunctor
        {
            /// Position to sort in
            CGUIVector3 sortPos;

            SortByDistanceFunctor(const CGUIVector3& pos);
            float operator()(CGUIParticle* p) const;
        };

//		static RadixSort<TActiveParticleList, CGUIParticle*, float> mRadixSorter;

		/** Active particle list.
            @remarks
                This is a linked list of pointers to particles in the particle pool.
            @par
                This allows very fast insertions and deletions from anywhere in 
                the list to activate / deactivate particles as well as reuse of 
                Particle instances in the pool without construction & destruction 
                which avoids memory thrashing.
        */
        TActiveParticleList mActiveParticles;

        /** Free particle queue.
            @remarks
                This contains a list of the particles free for use as new instances
                as required by the set. Particle instances are preconstructed up 
                to the estimated size in the mParticlePool vector and are 
                referenced on this deque at startup. As they get used this list
                reduces, as they get released back to to the set they get added
				back to the list.
        */
        TFreeParticleList mFreeParticles;

        /** Pool of particle instances for use and reuse in the active particle list.
            @remarks
                This vector will be preallocated with the estimated size of the set,and will extend as required.
        */
        TParticlePool mParticlePool;


		/** Pool of emitted emitters for use and reuse in the active emitted emitter list.
        @remarks
			The emitters in this pool act as particles and as emitters. The pool is a map containing lists 
			of emitters, identified by their name.
        @par
            The emitters in this pool are cloned using emitters that are kept in the main emitter list
			of the CGUIParticleSystem.
        */
		TEmittedEmitterPool mEmittedEmitterPool;

        /** Free emitted emitter list.
            @remarks
                This contains a list of the emitters free for use as new instances as required by the set.
        */
        TFreeEmittedEmitterMap mFreeEmittedEmitters;

		/** Active emitted emitter list.
            @remarks
                This is a linked list of pointers to emitters in the emitted emitter pool.
				Emitters that are used are stored (their pointers) in both the list with active particles and in 
				the list with active emitted emitters.        */
        TActiveEmittedEmitterList mActiveEmittedEmitters;

		typedef std::vector<CGUIParticleEmitter*> TParticleEmitterList;
        typedef std::vector<CGUIParticleAffector*> TParticleAffectorList;
        
        /// List of particle emitters, ie sources of particles
        TParticleEmitterList mEmitters;
        /// List of particle affectors, ie modifiers of particles
        TParticleAffectorList mAffectors;

        /// The renderer used to render this particle system
        CGUIParticleSystemRenderer* mRenderer;

        /// Do we cull each particle individually?
        bool mCullIndividual;

        /// The name of the type of renderer used to render this system
        CGUIString mRendererType;
        
        /// The number of particles in the pool.
        size_t mPoolSize;

        /// The number of emitted emitters in the pool.
        size_t mEmittedEmitterPoolSize;

		/// Optional origin of this particle system (eg script name)
		CGUIString mOrigin;

        /// Default iteration interval
        static real msDefaultIterationInterval;
        /// Default nonvisible update timeout
        static real msDefaultNonvisibleTimeout;
    };
}

#endif	//__GUI_PARTICLESYSTEM_20110118_H__

