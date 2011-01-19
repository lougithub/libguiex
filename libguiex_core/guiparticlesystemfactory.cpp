/** 
* @file guiparticle.cpp
* @brief 
* @author ken
* @date 2011-01-18
*/

//============================================================================//
// include
//============================================================================// 
#include "guiparticle.h"
#include "guiparticlesystem.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//-----------------------------------------------------------------------
	CGUIString CGUIParticleSystemFactory::FACTORY_TYPE_NAME = "ParticleSystem";
	//-----------------------------------------------------------------------
	CGUIParticleSystemFactory::CGUIParticleSystemFactory() 
	{

	}
	//-----------------------------------------------------------------------
	CGUIParticleSystemFactory::~CGUIParticleSystemFactory() 
	{
	}
	//-----------------------------------------------------------------------
	CGUIParticleSystem* CGUIParticleSystemFactory::createInstanceImpl( const CGUIString& name, const NameValuePairList* params)
	{
		if (params != 0)
		{
			NameValuePairList::const_iterator ni = params->find("templateName");
			if (ni != params->end())
			{
				CGUIString templateName = ni->second;
				// create using manager
				return CGUIParticleSystemManager::getSingleton().createSystemImpl(
					name, templateName);
			}
		}
		// Not template based, look for quota & resource name
		size_t quota = 500;
		CGUIString resourceGroup = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
		if (params != 0)
		{
			NameValuePairList::const_iterator ni = params->find("quota");
			if (ni != params->end())
			{
				quota = StringConverter::parseUnsignedInt(ni->second);
			}
			ni = params->find("resourceGroup");
			if (ni != params->end())
			{
				resourceGroup = ni->second;
			}
		}
		// create using manager
		return CGUIParticleSystemManager::getSingleton().createSystemImpl(name, quota, resourceGroup);
	}
	//-----------------------------------------------------------------------
	const CGUIString& CGUIParticleSystemFactory::getType(void) const
	{
		return FACTORY_TYPE_NAME;
	}
	//-----------------------------------------------------------------------
	CGUIParticleSystem* CGUIParticleSystemFactory::createInstance(
		const CGUIString& name, SceneManager* manager, const NameValuePairList* params)
	{
		CGUIParticleSystem* m = createInstanceImpl(name, params);
		m->_notifyCreator(this);
		m->_notifyManager(manager);
		return m;
	}
	//-----------------------------------------------------------------------
	void CGUIParticleSystemFactory::destroyInstance( CGUIParticleSystem* obj) 
	{
		// use manager
		CGUIParticleSystemManager::getSingleton().destroySystemImpl(static_cast<ParticleSystem*>(obj));

	}
	//-----------------------------------------------------------------------
}
