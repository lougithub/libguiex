/** 
* @file guiparticleemitterfactory.h
* @brief 
* @author ken
* @date 2011-01-19
*/

#ifndef __GUI_PARTICLEEMITTERFACTORY_20110119_H__
#define __GUI_PARTICLEEMITTERFACTORY_20110119_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guivector3.h"
#include <vector>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIParticleSystem;
	class CGUIParticleEmitter;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex 
{

	class CGUIParticleEmitterFactory
    {
    public:
        CGUIParticleEmitterFactory();
        virtual ~CGUIParticleEmitterFactory();

        /** Returns the name of the factory, the name which identifies the particle emitter type this factory creates. */
        virtual CGUIString getName() const = 0;

        /** Creates a new emitter instance.
        @remarks
            The subclass MUST add a pointer to the created instance to mEmitters.
        */
        virtual CGUIParticleEmitter* createEmitter(CGUIParticleSystem* psys) = 0;

        /** Destroys the emitter pointed to by the parameter (for early clean up if required). */
        virtual void destroyEmitter(CGUIParticleEmitter* e);

	protected:
		std::vector<CGUIParticleEmitter*> mEmitters;
	};
}

#endif	//__GUI_PARTICLEEMITTERFACTORY_20110119_H__

