/** 
* @file guiparticleaffectorfactory.h
* @brief 
* @author ken
* @date 2011-01-18
*/

#ifndef __GUI_PARTICLEAFFECTORFACTORY_20110118_H__
#define __GUI_PARTICLEAFFECTORFACTORY_20110118_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include <vector>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIParticleAffector;
	class CGUIParticleSystem;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex 
{
	class CGUIParticleAffectorFactory
    {
    public:
        CGUIParticleAffectorFactory();
        virtual ~CGUIParticleAffectorFactory();
        
		/** Returns the name of the factory, the name which identifies the particle affector type this factory creates. */
        virtual CGUIString getName() const = 0;

        /** Creates a new affector instance.
        @remarks
            The subclass MUST add a pointer to the created instance to mAffectors.
        */
        virtual CGUIParticleAffector* createAffector(CGUIParticleSystem* psys) = 0;

        /** Destroys the affector pointed to by the parameter (for early clean up if required). */
        virtual void destroyAffector(CGUIParticleAffector* e);

	protected:
		std::vector<CGUIParticleAffector*> mAffectors;
    };
}

#endif	//__GUI_PARTICLEAFFECTORFACTORY_20110118_H__

