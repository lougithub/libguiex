/** 
* @file guiparticlesystemfactory.h
* @brief 
* @author ken
* @date 2011-01-19
*/

#ifndef __GUI_PARTICLESYSTEMFACTORY_20110119_H__
#define __GUI_PARTICLESYSTEMFACTORY_20110119_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"

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
	class CGUIParticleSystemFactory
	{
	public:
		CGUIParticleSystemFactory();
		virtual ~CGUIParticleSystemFactory();

		static CGUIString FACTORY_TYPE_NAME;

		const CGUIString& getType(void) const;
		CGUIParticleSystem* createInstance( const CGUIString& name, SceneManager* manager, const NameValuePairList* params );
		void destroyInstance( CGUIParticleSystem* obj);  
	
	protected:
		CGUIParticleSystem* createInstanceImpl(const CGUIString& name, const NameValuePairList* params);
	};
}

#endif	//__GUI_PARTICLESYSTEMFACTORY_20110119_H__

