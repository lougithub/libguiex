/** 
* @file guiparticlesystemmanager.h
* @brief 
* @author ken
* @date 2011-01-19
*/

#ifndef __GUI_PARTICLESYSTEMMANAGER_20110119_H__
#define __GUI_PARTICLESYSTEMMANAGER_20110119_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include <map>
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
   class CGUIParticleSystemManager
    {
	public:
		typedef std::map<CGUIString, CGUIParticleSystem*> ParticleTemplateMap;
		typedef std::map<CGUIString, CGUIParticleAffectorFactory*> ParticleAffectorFactoryMap;
		typedef std::map<CGUIString, CGUIParticleEmitterFactory*> ParticleEmitterFactoryMap;
		typedef std::map<CGUIString, ParticleSystemRendererFactory*> ParticleSystemRendererFactoryMap;
 
		
    public:
        CGUIParticleSystemManager();
        virtual ~CGUIParticleSystemManager();

        void addEmitterFactory(CGUIParticleEmitterFactory* factory);
        void addAffectorFactory(CGUIParticleAffectorFactory* factory);
		void addRendererFactory(ParticleSystemRendererFactory* factory);

        void addTemplate(const CGUIString& name, CGUIParticleSystem* sysTemplate);
        void removeTemplate(const CGUIString& name, bool deleteTemplate = true);
        void removeAllTemplates(bool deleteTemplate = true);
        void removeTemplatesByResourceGroup(const CGUIString& resourceGroup);

        CGUIParticleSystem* createTemplate(const CGUIString& name, const CGUIString& resourceGroup);
        CGUIParticleSystem* getTemplate(const CGUIString& name);
 
        CGUIParticleEmitter* _createEmitter(const CGUIString& emitterType, CGUIParticleSystem* psys);
        void _destroyEmitter(CGUIParticleEmitter* emitter);

        CGUIParticleAffector* _createAffector(const CGUIString& affectorType, CGUIParticleSystem* psys);
        void _destroyAffector(CGUIParticleAffector* affector);

        ParticleSystemRenderer* _createRenderer(const CGUIString& rendererType);
        void _destroyRenderer(ParticleSystemRenderer* renderer);
      
		void _initialise(void);

        /// @copydoc ScriptLoader::getScriptPatterns
        const StringVector& getScriptPatterns(void) const;
        /// @copydoc ScriptLoader::parseScript
        void parseScript(DataStreamPtr& stream, const CGUIString& groupName);
        /// @copydoc ScriptLoader::getLoadingOrder
        real getLoadingOrder(void) const;

		typedef MapIterator<ParticleAffectorFactoryMap> ParticleAffectorFactoryIterator;
		typedef MapIterator<ParticleEmitterFactoryMap> ParticleEmitterFactoryIterator;
		typedef MapIterator<ParticleSystemRendererFactoryMap> ParticleRendererFactoryIterator;
		/** Return an iterator over the affector factories currently registered */
		ParticleAffectorFactoryIterator getAffectorFactoryIterator(void);
		/** Return an iterator over the emitter factories currently registered */
		ParticleEmitterFactoryIterator getEmitterFactoryIterator(void);
		/** Return an iterator over the renderer factories currently registered */
		ParticleRendererFactoryIterator getRendererFactoryIterator(void);


        typedef MapIterator<ParticleTemplateMap> ParticleSystemTemplateIterator;
        /** Gets an iterator over the list of particle system templates. */
        ParticleSystemTemplateIterator getTemplateIterator(void)
        {
            return ParticleSystemTemplateIterator(
                mSystemTemplates.begin(), mSystemTemplates.end());
        } 

        /** Get an instance of CGUIParticleSystemFactory (internal use). */
		CGUIParticleSystemFactory* _getFactory(void) { return mFactory; }
		
        static CGUIParticleSystemManager& getSingleton(void);
        static CGUIParticleSystemManager* getSingletonPtr(void);

	protected:
		friend class CGUIParticleSystemFactory;

		/// Templates based on scripts
		ParticleTemplateMap mSystemTemplates;

		/// Factories for named emitter types (can be extended using plugins)
		ParticleEmitterFactoryMap mEmitterFactories;

		/// Factories for named affector types (can be extended using plugins)
		ParticleAffectorFactoryMap mAffectorFactories;

		/// Map of renderer types to factories
		ParticleSystemRendererFactoryMap mRendererFactories;

		StringVector mScriptPatterns;

		// Factory instance
		CGUIParticleSystemFactory* mFactory;

		/** Internal script parsing method. */
		void parseNewEmitter(const CGUIString& type, DataStreamPtr& chunk, CGUIParticleSystem* sys);
		/** Internal script parsing method. */
		void parseNewAffector(const CGUIString& type, DataStreamPtr& chunk, CGUIParticleSystem* sys);
		/** Internal script parsing method. */
		void parseAttrib(const CGUIString& line, CGUIParticleSystem* sys);
		/** Internal script parsing method. */
		void parseEmitterAttrib(const CGUIString& line, CGUIParticleEmitter* sys);
		/** Internal script parsing method. */
		void parseAffectorAttrib(const CGUIString& line, CGUIParticleAffector* sys);
		/** Internal script parsing method. */
		void skipToNextCloseBrace(DataStreamPtr& chunk);
		/** Internal script parsing method. */
		void skipToNextOpenBrace(DataStreamPtr& chunk);

		/// Internal implementation of createSystem
		CGUIParticleSystem* createSystemImpl(const CGUIString& name, size_t quota, 
			const CGUIString& resourceGroup);
		/// Internal implementation of createSystem
		CGUIParticleSystem* createSystemImpl(const CGUIString& name, const CGUIString& templateName);
		/// Internal implementation of destroySystem
		void destroySystemImpl(CGUIParticleSystem* sys);

    };
}

#endif	//__GUI_PARTICLESYSTEMMANAGER_20110119_H__

