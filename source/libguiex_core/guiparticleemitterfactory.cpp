/** 
* @file guiparticleemitterfactory.cpp
* @brief 
* @author ken
* @date 2011-01-19
*/

//============================================================================//
// include
//============================================================================// 
#include "guiparticleemitterfactory.h"
#include "guiparticleemitter.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//-----------------------------------------------------------------------
	CGUIParticleEmitterFactory::CGUIParticleEmitterFactory() 
	{
	}
	//-----------------------------------------------------------------------
	CGUIParticleEmitterFactory::~CGUIParticleEmitterFactory()
	{
		// Destroy all emitters
		std::vector<CGUIParticleEmitter*>::iterator i;
		for (i = mEmitters.begin(); i != mEmitters.end(); ++i)
		{
			delete (*i);
		}

		mEmitters.clear();
	}
	//-----------------------------------------------------------------------
	void CGUIParticleEmitterFactory::destroyEmitter(CGUIParticleEmitter* e)        
	{
		std::vector<CGUIParticleEmitter*>::iterator i;
		for (i = mEmitters.begin(); i != mEmitters.end(); ++i)
		{
			if ((*i) == e)
			{
				mEmitters.erase(i);
				delete e;
				break;
			}
		}
	}
	//-----------------------------------------------------------------------
}
