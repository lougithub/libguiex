/** 
* @file guiparticleaffectorfactory.cpp
* @brief 
* @author ken
* @date 2011-01-18
*/

//============================================================================//
// include
//============================================================================// 
#include "guiparticleaffectorfactory.h"
#include "guiparticleaffector.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//-----------------------------------------------------------------------
	CGUIParticleAffectorFactory::CGUIParticleAffectorFactory() 
	{
	}
	//-----------------------------------------------------------------------
	CGUIParticleAffectorFactory::~CGUIParticleAffectorFactory() 
	{
		// Destroy all affectors
		std::vector<CGUIParticleAffector*>::iterator i;
		for (i = mAffectors.begin(); i != mAffectors.end(); ++i)
		{
			delete (*i);
		}

		mAffectors.clear();
	}
	//-----------------------------------------------------------------------
	void CGUIParticleAffectorFactory::destroyAffector(CGUIParticleAffector* e)
	{
		std::vector<CGUIParticleAffector*>::iterator i;
		for (i = mAffectors.begin(); i != mAffectors.end(); ++i)
		{
			if ((*i) == e)
			{
				mAffectors.erase(i);
				delete e;
				break;
			}
		}
	}
	//-----------------------------------------------------------------------
}
