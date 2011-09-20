/** 
* @file guiparticlesystemmanager.cpp
* @brief 
* @author ken
* @date 2011-01-19
*/

//============================================================================//
// include
//============================================================================// 
#include "guiparticlesystemmanager.h"
#include "guiparticlesystem.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//-----------------------------------------------------------------------
	// Shortcut to set up billboard particle renderer
	BillboardParticleRendererFactory* mBillboardRendererFactory = 0;
	//-----------------------------------------------------------------------
	/** Override standard Singleton retrieval.
	@remarks
	Why do we do this? Well, it's because the Singleton
	implementation is in a .h file, which means it gets compiled
	into anybody who includes it. This is needed for the
	Singleton template to work, but we actually only want it
	compiled into the implementation of the class based on the
	Singleton, not all of them. If we don't change this, we get
	link errors when trying to use the Singleton-based class from
	an outside dll.
	@par
	This method just delegates to the template version anyway,
	but the implementation stays in this single compilation unit,
	preventing link errors.
	*/
	template<> CGUIParticleSystemManager* Singleton<CGUIParticleSystemManager>::ms_Singleton = 0;
	CGUIParticleSystemManager* CGUIParticleSystemManager::getSingletonPtr(void)
	{
		return ms_Singleton;
	}
	/** Override standard Singleton retrieval.
	@remarks
	Why do we do this? Well, it's because the Singleton
	implementation is in a .h file, which means it gets compiled
	into anybody who includes it. This is needed for the
	Singleton template to work, but we actually only want it
	compiled into the implementation of the class based on the
	Singleton, not all of them. If we don't change this, we get
	link errors when trying to use the Singleton-based class from
	an outside dll.
	@par
	This method just delegates to the template version anyway,
	but the implementation stays in this single compilation unit,
	preventing link errors.
	*/
	CGUIParticleSystemManager& CGUIParticleSystemManager::getSingleton(void)
	{  
		assert( ms_Singleton );  return ( *ms_Singleton );  
	}
	//-----------------------------------------------------------------------
	CGUIParticleSystemManager::CGUIParticleSystemManager()
	{
#if OGRE_USE_NEW_COMPILERS == 0
		mScriptPatterns.push_back("*.particle");
		ResourceGroupManager::getSingleton()._registerScriptLoader(this);
#endif
		mFactory = new CGUIParticleSystemFactory();
		Root::getSingleton().addMovableObjectFactory(mFactory);
	}
	//-----------------------------------------------------------------------
	CGUIParticleSystemManager::~CGUIParticleSystemManager()
	{
		// Destroy all templates
		ParticleTemplateMap::iterator t;
		for (t = mSystemTemplates.begin(); t != mSystemTemplates.end(); ++t)
		{
			delete t->second;
		}
		mSystemTemplates.clear();
		ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
		// delete billboard factory
		if (mBillboardRendererFactory)
		{
			delete mBillboardRendererFactory;
			mBillboardRendererFactory = 0;
		}

		if (mFactory)
		{
			// delete particle system factory
			Root::getSingleton().removeMovableObjectFactory(mFactory);
			delete mFactory;
			mFactory = 0;
		}

	}
	//-----------------------------------------------------------------------
	const StringVector& CGUIParticleSystemManager::getScriptPatterns(void) const
	{
		return mScriptPatterns;
	}
	//-----------------------------------------------------------------------
	real CGUIParticleSystemManager::getLoadingOrder(void) const
	{
		/// Load late
		return 1000.0f;
	}
	//-----------------------------------------------------------------------
	void CGUIParticleSystemManager::parseScript(DataStreamPtr& stream, const CGUIString& groupName)
	{
#if OGRE_USE_NEW_COMPILERS == 1
		ScriptCompilerManager::getSingleton().parseScript(stream, groupName);
#else // OGRE_USE_NEW_COMPILERS
		CGUIString line;
		CGUIParticleSystem* pSys;
		vector<CGUIString>::type vecparams;

		pSys = 0;

		while(!stream->eof())
		{
			line = stream->getLine();
			// Ignore comments & blanks
			if (!(line.length() == 0 || line.substr(0,2) == "//"))
			{
				if (pSys == 0)
				{
					// No current system
					// So first valid data should be a system name
					if (StringUtil::startsWith(line, "particle_system "))
					{
						// chop off the 'particle_system ' needed by new compilers
						line = line.substr(16);
					}
					pSys = createTemplate(line, groupName);
					pSys->_notifyOrigin(stream->getName());
					// Skip to and over next {
					skipToNextOpenBrace(stream);
				}
				else
				{
					// Already in a system
					if (line == "}")
					{
						// Finished system
						pSys = 0;
					}
					else if (line.substr(0,7) == "emitter")
					{
						// new emitter
						// Get typename
						vecparams = StringUtil::split(line, "\t ");
						if (vecparams.size() < 2)
						{
							// Oops, bad emitter
							LogManager::getSingleton().logMessage("Bad particle system emitter line: '"
								+ line + "' in " + pSys->getName());
							skipToNextCloseBrace(stream);

						}
						skipToNextOpenBrace(stream);
						parseNewEmitter(vecparams[1], stream, pSys);

					}
					else if (line.substr(0,8) == "affector")
					{
						// new affector
						// Get typename
						vecparams = StringUtil::split(line, "\t ");
						if (vecparams.size() < 2)
						{
							// Oops, bad affector
							LogManager::getSingleton().logMessage("Bad particle system affector line: '"
								+ line + "' in " + pSys->getName());
							skipToNextCloseBrace(stream);

						}
						skipToNextOpenBrace(stream);
						parseNewAffector(vecparams[1],stream, pSys);
					}
					else
					{
						// Attribute
						parseAttrib(line, pSys);
					}

				}

			}


		}
#endif // OGRE_USE_NEW_COMPILERS
	}
	//-----------------------------------------------------------------------
	/** Adds a new 'factory' object for emitters to the list of available emitter types.
	@remarks
	This method allows plugins etc to add new particle emitter types to Ogre. Particle emitters
	are sources of particles, and generate new particles with their start positions, colours and
	momentums appropriately. Plugins would create new subclasses of CGUIParticleEmitter which 
	emit particles a certain way, and register a subclass of CGUIParticleEmitterFactory to create them (since multiple 
	emitters can be created for different particle systems).
	@par
	All particle emitter factories have an assigned name which is used to identify the emitter
	type. This must be unique.
	@par
	Note that the object passed to this function will not be destroyed by the CGUIParticleSystemManager,
	since it may have been allocated on a different heap in the case of plugins. The caller must
	destroy the object later on, probably on plugin shutdown.
	@param
	factory Pointer to a CGUIParticleEmitterFactory subclass created by the plugin or application code.
	*/
	void CGUIParticleSystemManager::addEmitterFactory(CGUIParticleEmitterFactory* factory)
	{
		CGUIString name = factory->getName();
		mEmitterFactories[name] = factory;
		LogManager::getSingleton().logMessage("Particle Emitter Type '" + name + "' registered");
	}
	//-----------------------------------------------------------------------
	/** Adds a new 'factory' object for affectors to the list of available affector types.
	@remarks
	This method allows plugins etc to add new particle affector types to Ogre. Particle
	affectors modify the particles in a system a certain way such as affecting their direction
	or changing their colour, lifespan etc. Plugins would
	create new subclasses of CGUIParticleAffector which affect particles a certain way, and register
	a subclass of CGUIParticleAffectorFactory to create them.
	@par
	All particle affector factories have an assigned name which is used to identify the affector
	type. This must be unique.
	@par
	Note that the object passed to this function will not be destroyed by the CGUIParticleSystemManager,
	since it may have been allocated on a different heap in the case of plugins. The caller must
	destroy the object later on, probably on plugin shutdown.
	@param
	factory Pointer to a CGUIParticleAffectorFactory subclass created by the plugin or application code.
	*/
	void CGUIParticleSystemManager::addAffectorFactory(CGUIParticleAffectorFactory* factory)
	{
		CGUIString name = factory->getName();
		mAffectorFactories[name] = factory;
		LogManager::getSingleton().logMessage("Particle Affector Type '" + name + "' registered");
	}
	//-----------------------------------------------------------------------
	/** Registers a factory class for creating ParticleSystemRenderer instances. 
	@par
	Note that the object passed to this function will not be destroyed by the CGUIParticleSystemManager,
	since it may have been allocated on a different heap in the case of plugins. The caller must
	destroy the object later on, probably on plugin shutdown.
	@param
	factory Pointer to a ParticleSystemRendererFactory subclass created by the plugin or application code.
	*/
	void CGUIParticleSystemManager::addRendererFactory(ParticleSystemRendererFactory* factory)
	{
		CGUIString name = factory->getType();
		mRendererFactories[name] = factory;
		LogManager::getSingleton().logMessage("Particle Renderer Type '" + name + "' registered");
	}
	//-----------------------------------------------------------------------
	/** Adds a new particle system template to the list of available templates. 
	@remarks
	Instances of particle systems in a scene are not normally unique - often you want to place the
	same effect in many places. This method allows you to register a CGUIParticleSystem as a named template,
	which can subsequently be used to create instances using the createSystem method.
	@par
	Note that particle system templates can either be created programmatically by an application 
	and registered using this method, or they can be defined in a script file (*.particle) which is
	loaded by the engine at startup, very much like Material scripts.
	@param
	name The name of the template. Must be unique across all templates.
	@param
	sysTemplate A pointer to a particle system to be used as a template. The manager
	will take over ownership of this pointer.

	*/
	void CGUIParticleSystemManager::addTemplate(const CGUIString& name, CGUIParticleSystem* sysTemplate)
	{
		// check name
		if (mSystemTemplates.find(name) != mSystemTemplates.end())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"CGUIParticleSystem template with name '" + name + "' already exists.", 
				"CGUIParticleSystemManager::addTemplate");
		}

		mSystemTemplates[name] = sysTemplate;
	}
	//-----------------------------------------------------------------------
	/** Removes a specified template from the CGUIParticleSystemManager.
	@remarks
	This method removes a given template from the particle system manager, optionally deleting
	the template if the deleteTemplate method is called.  Throws an exception if the template
	could not be found.
	@param
	name The name of the template to remove.
	@param
	deleteTemplate Whether or not to delete the template before removing it.
	*/
	void CGUIParticleSystemManager::removeTemplate(const CGUIString& name, bool deleteTemplate)
	{
		ParticleTemplateMap::iterator itr = mSystemTemplates.find(name);
		if (itr == mSystemTemplates.end())
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
			"CGUIParticleSystem template with name '" + name + "' cannot be found.",
			"CGUIParticleSystemManager::removeTemplate");

		if (deleteTemplate)
			delete itr->second;

		mSystemTemplates.erase(itr);
	}
	//-----------------------------------------------------------------------
	/** Removes a specified template from the CGUIParticleSystemManager.
	@remarks
	This method removes all templates from the CGUIParticleSystemManager.
	@param
	deleteTemplate Whether or not to delete the templates before removing them.
	*/
	void CGUIParticleSystemManager::removeAllTemplates(bool deleteTemplate)
	{
		if (deleteTemplate)
		{
			ParticleTemplateMap::iterator itr;
			for (itr = mSystemTemplates.begin(); itr != mSystemTemplates.end(); ++itr)
				delete itr->second;
		}

		mSystemTemplates.clear();
	}
	//-----------------------------------------------------------------------
	/** Removes all templates that belong to a secific Resource Group from the CGUIParticleSystemManager.
	@remarks
	This method removes all templates that belong in a particular resource group from the CGUIParticleSystemManager.
	@param
	resourceGroup to delete templates for
	*/
	void CGUIParticleSystemManager::removeTemplatesByResourceGroup(const CGUIString& resourceGroup)
	{
		ParticleTemplateMap::iterator i = mSystemTemplates.begin();
		while (i != mSystemTemplates.end())
		{
			ParticleTemplateMap::iterator icur = i++;

			if(icur->second->getResourceGroupName() == resourceGroup)
			{
				delete icur->second;
				mSystemTemplates.erase(icur);
			}
		}    
	}
	//-----------------------------------------------------------------------
	/** Create a new particle system template. 
	@remarks
	This method is similar to the addTemplate method, except this just creates a new template
	and returns a pointer to it to be populated. Use this when you don't already have a system
	to add as a template and just want to create a new template which you will build up in-place.
	@param
	name The name of the template. Must be unique across all templates.
	@param
	resourceGroup The name of the resource group which will be used to 
	load any dependent resources.

	*/
	CGUIParticleSystem* CGUIParticleSystemManager::createTemplate(const CGUIString& name, 
		const CGUIString& resourceGroup)
	{
		// check name
		if (mSystemTemplates.find(name) != mSystemTemplates.end())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"CGUIParticleSystem template with name '" + name + "' already exists.", 
				"CGUIParticleSystemManager::createTemplate");
		}

		CGUIParticleSystem* tpl = new CGUIParticleSystem(name, resourceGroup);
		addTemplate(name, tpl);
		return tpl;

	}
	//-----------------------------------------------------------------------
	/** Retrieves a particle system template for possible modification. 
	@remarks
	Modifying a template does not affect the settings on any ParticleSystems already created
	from this template.
	*/
	CGUIParticleSystem* CGUIParticleSystemManager::getTemplate(const CGUIString& name)
	{
		ParticleTemplateMap::iterator i = mSystemTemplates.find(name);
		if (i != mSystemTemplates.end())
		{
			return i->second;
		}
		else
		{
			return 0;
		}
	}
	//-----------------------------------------------------------------------
	CGUIParticleSystem* CGUIParticleSystemManager::createSystemImpl(const CGUIString& name,
		size_t quota, const CGUIString& resourceGroup)
	{
		CGUIParticleSystem* sys = new CGUIParticleSystem(name, resourceGroup);
		sys->setParticleQuota(quota);
		return sys;
	}
	//-----------------------------------------------------------------------
	CGUIParticleSystem* CGUIParticleSystemManager::createSystemImpl(const CGUIString& name, 
		const CGUIString& templateName)
	{
		// Look up template
		CGUIParticleSystem* pTemplate = getTemplate(templateName);
		if (!pTemplate)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Cannot find required template '" + templateName + "'", "CGUIParticleSystemManager::createSystem");
		}

		CGUIParticleSystem* sys = createSystemImpl(name, pTemplate->getParticleQuota(), 
			pTemplate->getResourceGroupName());
		// Copy template settings
		*sys = *pTemplate;
		return sys;

	}
	//-----------------------------------------------------------------------
	void CGUIParticleSystemManager::destroySystemImpl(CGUIParticleSystem* sys)
	{
		delete sys;
	}
	//-----------------------------------------------------------------------
	/** Internal method for creating a new emitter from a factory.
	@remarks
	Used internally by the engine to create new CGUIParticleEmitter instances from named
	factories. Applications should use the CGUIParticleSystem::addEmitter method instead, 
	which calls this method to create an instance.
	@param
	emitterType CGUIString name of the emitter type to be created. A factory of this type must have been registered.
	@param 
	psys The particle system this is being created for
	*/
	CGUIParticleEmitter* CGUIParticleSystemManager::_createEmitter(
		const CGUIString& emitterType, CGUIParticleSystem* psys)
	{
		// Locate emitter type
		ParticleEmitterFactoryMap::iterator pFact = mEmitterFactories.find(emitterType);

		if (pFact == mEmitterFactories.end())
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Cannot find requested emitter type.", 
				"CGUIParticleSystemManager::_createEmitter");
		}

		return pFact->second->createEmitter(psys);
	}
	//-----------------------------------------------------------------------
	/** Internal method for destroying an emitter.
	@remarks
	Because emitters are created by factories which may allocate memory from separate heaps,
	the memory allocated must be freed from the same place. This method is used to ask the factory
	to destroy the instance passed in as a pointer.
	@param
	emitter Pointer to emitter to be destroyed. On return this pointer will point to invalid (freed) memory.
	*/
	void CGUIParticleSystemManager::_destroyEmitter(CGUIParticleEmitter* emitter)
	{
		// Destroy using the factory which created it
		ParticleEmitterFactoryMap::iterator pFact = mEmitterFactories.find(emitter->getType());

		if (pFact == mEmitterFactories.end())
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Cannot find emitter factory to destroy emitter.", 
				"CGUIParticleSystemManager::_destroyEmitter");
		}

		pFact->second->destroyEmitter(emitter);
	}
	//-----------------------------------------------------------------------
	/** Internal method for creating a new affector from a factory.
	@remarks
	Used internally by the engine to create new CGUIParticleAffector instances from named
	factories. Applications should use the CGUIParticleSystem::addAffector method instead, 
	which calls this method to create an instance.
	@param
	effectorType CGUIString name of the affector type to be created. A factory of this type must have been registered.
	@param
	psys The particle system it is being created for
	*/
	CGUIParticleAffector* CGUIParticleSystemManager::_createAffector(
		const CGUIString& affectorType, CGUIParticleSystem* psys)
	{
		// Locate affector type
		ParticleAffectorFactoryMap::iterator pFact = mAffectorFactories.find(affectorType);

		if (pFact == mAffectorFactories.end())
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Cannot find requested affector type.", 
				"CGUIParticleSystemManager::_createAffector");
		}

		return pFact->second->createAffector(psys);

	}
	//-----------------------------------------------------------------------
	/** Internal method for destroying an affector.
	@remarks
	Because affectors are created by factories which may allocate memory from separate heaps,
	the memory allocated must be freed from the same place. This method is used to ask the factory
	to destroy the instance passed in as a pointer.
	@param
	affector Pointer to affector to be destroyed. On return this pointer will point to invalid (freed) memory.
	*/
	void CGUIParticleSystemManager::_destroyAffector(CGUIParticleAffector* affector)
	{
		// Destroy using the factory which created it
		ParticleAffectorFactoryMap::iterator pFact = mAffectorFactories.find(affector->getType());

		if (pFact == mAffectorFactories.end())
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Cannot find affector factory to destroy affector.", 
				"CGUIParticleSystemManager::_destroyAffector");
		}

		pFact->second->destroyAffector(affector);
	}
	//-----------------------------------------------------------------------

	/** Internal method for creating a new renderer from a factory.
	@remarks
	Used internally by the engine to create new ParticleSystemRenderer instances from named
	factories. Applications should use the CGUIParticleSystem::setRenderer method instead, 
	which calls this method to create an instance.
	@param
	rendererType CGUIString name of the renderer type to be created. A factory of this type must have been registered.
	*/
	ParticleSystemRenderer* CGUIParticleSystemManager::_createRenderer(const CGUIString& rendererType)
	{
		// Locate affector type
		ParticleSystemRendererFactoryMap::iterator pFact = mRendererFactories.find(rendererType);

		if (pFact == mRendererFactories.end())
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Cannot find requested renderer type.", 
				"CGUIParticleSystemManager::_createRenderer");
		}

		return pFact->second->createInstance(rendererType);
	}
	//-----------------------------------------------------------------------
	/** Internal method for destroying a renderer.
	@remarks
	Because renderer are created by factories which may allocate memory from separate heaps,
	the memory allocated must be freed from the same place. This method is used to ask the factory
	to destroy the instance passed in as a pointer.
	@param
	renderer Pointer to renderer to be destroyed. On return this pointer will point to invalid (freed) memory.
	*/
	void CGUIParticleSystemManager::_destroyRenderer(ParticleSystemRenderer* renderer)
	{
		// Destroy using the factory which created it
		ParticleSystemRendererFactoryMap::iterator pFact = mRendererFactories.find(renderer->getType());

		if (pFact == mRendererFactories.end())
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Cannot find renderer factory to destroy renderer.", 
				"CGUIParticleSystemManager::_destroyRenderer");
		}

		pFact->second->destroyInstance(renderer);
	}
	//-----------------------------------------------------------------------
	/** Init method to be called by OGRE system.
	@remarks
	Due to dependencies between various objects certain initialisation tasks cannot be done
	on construction. OGRE will call this method when the rendering subsystem is initialised.
	*/
	void CGUIParticleSystemManager::_initialise(void)
	{
		// Create Billboard renderer factory
		mBillboardRendererFactory = new BillboardParticleRendererFactory();
		addRendererFactory(mBillboardRendererFactory);

	}
	//-----------------------------------------------------------------------
	void CGUIParticleSystemManager::parseNewEmitter(const CGUIString& type, DataStreamPtr& stream, CGUIParticleSystem* sys)
	{
		// Create new emitter
		CGUIParticleEmitter* pEmit = sys->addEmitter(type);
		// Parse emitter details
		CGUIString line;

		while(!stream->eof())
		{
			line = stream->getLine();
			// Ignore comments & blanks
			if (!(line.length() == 0 || line.substr(0,2) == "//"))
			{
				if (line == "}")
				{
					// Finished emitter
					break;
				}
				else
				{
					// Attribute
					StringUtil::toLowerCase(line);
					parseEmitterAttrib(line, pEmit);
				}
			}
		}



	}
	//-----------------------------------------------------------------------
	void CGUIParticleSystemManager::parseNewAffector(const CGUIString& type, DataStreamPtr& stream, CGUIParticleSystem* sys)
	{
		// Create new affector
		CGUIParticleAffector* pAff = sys->addAffector(type);
		// Parse affector details
		CGUIString line;

		while(!stream->eof())
		{
			line = stream->getLine();
			// Ignore comments & blanks
			if (!(line.length() == 0 || line.substr(0,2) == "//"))
			{
				if (line == "}")
				{
					// Finished affector
					break;
				}
				else
				{
					// Attribute
					StringUtil::toLowerCase(line);
					parseAffectorAttrib(line, pAff);
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	void CGUIParticleSystemManager::parseAttrib(const CGUIString& line, CGUIParticleSystem* sys)
	{
		// Split params on space
		vector<CGUIString>::type vecparams = StringUtil::split(line, "\t ", 1);

		// Look up first param (command setting)
		if (!sys->setParameter(vecparams[0], vecparams[1]))
		{
			// Attribute not supported by particle system, try the renderer
			ParticleSystemRenderer* renderer = sys->getRenderer();
			if (renderer)
			{
				if (!renderer->setParameter(vecparams[0], vecparams[1]))
				{
					LogManager::getSingleton().logMessage("Bad particle system attribute line: '"
						+ line + "' in " + sys->getName() + " (tried renderer)");
				}
			}
			else
			{
				// BAD command. BAD!
				LogManager::getSingleton().logMessage("Bad particle system attribute line: '"
					+ line + "' in " + sys->getName() + " (no renderer)");
			}
		}
	}
	//-----------------------------------------------------------------------
	void CGUIParticleSystemManager::parseEmitterAttrib(const CGUIString& line, CGUIParticleEmitter* emit)
	{
		// Split params on first space
		vector<CGUIString>::type vecparams = StringUtil::split(line, "\t ", 1);

		// Look up first param (command setting)
		if (!emit->setParameter(vecparams[0], vecparams[1]))
		{
			// BAD command. BAD!
			LogManager::getSingleton().logMessage("Bad particle emitter attribute line: '"
				+ line + "' for emitter " + emit->getType());
		}
	}
	//-----------------------------------------------------------------------
	void CGUIParticleSystemManager::parseAffectorAttrib(const CGUIString& line, CGUIParticleAffector* aff)
	{
		// Split params on space
		vector<CGUIString>::type vecparams = StringUtil::split(line, "\t ", 1);

		// Look up first param (command setting)
		if (!aff->setParameter(vecparams[0], vecparams[1]))
		{
			// BAD command. BAD!
			LogManager::getSingleton().logMessage("Bad particle affector attribute line: '"
				+ line + "' for affector " + aff->getType());
		}
	}
	//-----------------------------------------------------------------------
	void CGUIParticleSystemManager::skipToNextCloseBrace(DataStreamPtr& stream)
	{
		CGUIString line;
		while (!stream->eof() && line != "}")
		{
			line = stream->getLine();
		}

	}
	//-----------------------------------------------------------------------
	void CGUIParticleSystemManager::skipToNextOpenBrace(DataStreamPtr& stream)
	{
		CGUIString line;
		while (!stream->eof() && line != "{")
		{
			line = stream->getLine();
		}

	}
	//-----------------------------------------------------------------------
	CGUIParticleSystemManager::ParticleAffectorFactoryIterator 
		CGUIParticleSystemManager::getAffectorFactoryIterator(void)
	{
		return ParticleAffectorFactoryIterator(
			mAffectorFactories.begin(), mAffectorFactories.end());
	}
	//-----------------------------------------------------------------------
	CGUIParticleSystemManager::ParticleEmitterFactoryIterator 
		CGUIParticleSystemManager::getEmitterFactoryIterator(void)
	{
		return ParticleEmitterFactoryIterator(
			mEmitterFactories.begin(), mEmitterFactories.end());
	}
	//-----------------------------------------------------------------------
	CGUIParticleSystemManager::ParticleRendererFactoryIterator 
		CGUIParticleSystemManager::getRendererFactoryIterator(void)
	{
		return ParticleRendererFactoryIterator(
			mRendererFactories.begin(), mRendererFactories.end());
	}
	//-----------------------------------------------------------------------
}
