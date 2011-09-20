/** 
* @file guiinterfacemanager.h
* @brief interface manager
* @author ken
* @date 2006-06-10
*/

#ifndef __GUI_INTERFACEMANAGER_H_20060610__
#define __GUI_INTERFACEMANAGER_H_20060610__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include "guidynlib.h"
#include <map>
#include "guiinterface.h"
#include "guiinterfacerender.h"
#include "guiinterfacefont.h"
#include "guiinterfacesound.h"
#include "guiinterfacescript.h"
#include "guiinterfacefilesys.h"
#include "guiinterfaceconfigfile.h"
#include "guiinterfacecommand.h"
#include "guiinterfaceime.h"
#include "guiinterfacestringconv.h"
#include "guiinterfacekeyboard.h"
#include "guiinterfacemouse.h"
#include "guiinterfaceimageLoader.h"
#include "guiinterfacelocalizationloader.h"
#include "guiinterfacephysics.h"


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	typedef IGUIInterface* (*FunCreateInterface)(void*); 
}


//============================================================================//
// define
//============================================================================// 

#define GUI_REGISTER_INTERFACE_LIB(classname)	\
{	\
	::guiex::classname* pInterface = new ::guiex::classname;	\
	if( 0 != pInterface->Initialize(NULL))						\
	{															\
	GUI_THROW( GUI_FORMAT( "Failed to initialize interface <%s>", pInterface->GetModuleType().c_str()));	\
	}															\
	::guiex::CGUIInterfaceManager::Instance()->RegisterInterface(pInterface->GetModuleType(), pInterface);	\
}

#define GUI_UNREGISTER_INTERFACE_LIB(classname)	\
	::guiex::CGUIInterfaceManager::Instance()->UnregisterInterface(classname::StaticGetModuleType());	



#define GUI_REGISTER_INTERFACE_LIB_ARG( classname, arg )	\
{	\
	::guiex::classname* pInterface = new ::guiex::classname;	\
	if( 0 != pInterface->Initialize(arg))						\
	{															\
		GUI_THROW( GUI_FORMAT( "Failed to initialize interface <%s>", pInterface->GetModuleType().c_str()));	\
	}															\
	::guiex::CGUIInterfaceManager::Instance()->RegisterInterface(pInterface->GetModuleType(), pInterface);	\
}

#define GUI_REGISTER_INTERFACE_DLL(interfacename, dllname)	\
	::guiex::CGUIInterfaceManager::Instance()->RegisterInterface(interfacename, dllname)

#define GUI_REGISTER_INTERFACE_DLL_ARG(interfacename, dllname, arg)	\
	::guiex::CGUIInterfaceManager::Instance()->RegisterInterface(interfacename, dllname, arg)

#define GUI_HAS_INTERFACE(interfacetype)	\
	::guiex::CGUIInterfaceManager::Instance()->HasInterface(interfacetype)


//============================================================================//
// class
//============================================================================// 

namespace guiex
{
	/**
	* @class CGUIInterfaceManager
	* @brief interface manager, use it to register 
	* and unregister interface
	*/
	class GUIEXPORT CGUIInterfaceManager
	{
	public:
		~CGUIInterfaceManager();

		static CGUIInterfaceManager* Instance();

		int	RegisterInterface(const CGUIString& rInterface, const CGUIString& rModuleName, void* pUserData = NULL);
		int	RegisterInterface(const CGUIString& rInterface, IGUIInterface* pInterface );
		int UnregisterInterface( IGUIInterface* pInterface );
		int UnregisterInterface(const CGUIString& rInterface );
		void UnregisterAllInterface();
		IGUIInterface* GetInterface(const CGUIString& rInterface );
		bool HasInterface(const CGUIString& rInterface );

	public:
		IGUIInterfaceRender* GetInterfaceRender();
		IGUIInterfaceFont* GetInterfaceFont();
		IGUIInterfaceScript* GetInterfaceScript();
		IGUIInterfaceConfigFile* GetInterfaceConfigFile();
		IGUIInterfaceFileSys* GetInterfaceFileSys();
		IGUIInterfaceLocalizationLoader* GetInterfaceLocalizationLoader();
		IGUIInterfaceMouse* GetInterfaceMouse();
		IGUIInterfaceKeyboard* GetInterfaceKeyboard();
		IGUIInterfaceSound* GetInterfaceSound();
		IGUIInterfaceCommand* GetInterfaceCommand();
		IGUIInterfaceIme* GetInterfaceIme();
		IGUIInterfaceStringConv* GetInterfaceStringConv();
		IGUIInterfaceImageLoader* GetInterfaceImageLoader();
		IGUIInterfacePhysics* GetInterfacePhysics();

		template <class T> T* GetInterfaceRenderWithTypeCheck();
		template <class T> T* GetInterfaceFontWithTypeCheck();
		template <class T> T* GetInterfaceScriptWithTypeCheck();
		template <class T> T* GetInterfaceConfigFileWithTypeCheck();
		template <class T> T* GetInterfaceFileSysWithTypeCheck();
		template <class T> T* GetInterfaceLocalizationLoaderWithTypeCheck();
		template <class T> T* GetInterfaceMouseWithTypeCheck();
		template <class T> T* GetInterfaceKeyboardWithTypeCheck();
		template <class T> T* GetInterfaceSoundWithTypeCheck();
		template <class T> T* GetInterfaceCommandWithTypeCheck();
		template <class T> T* GetInterfaceImeWithTypeCheck();
		template <class T> T* GetInterfaceStringConvWithTypeCheck();
		template <class T> T* GetInterfaceImageLoaderWithTypeCheck();
		template <class T> T* GetInterfacePhysicsWithTypeCheck();

	protected:
		friend class CGUISystem;
		CGUIInterfaceManager();
		CGUIInterfaceManager( const CGUIInterfaceManager& );

		struct SInterface
		{
			IGUIInterface*		m_pInterface;
			GUI_DYNLIB_HANDLE	m_hHandle;
		};
		int32 DoRegisterInterface( const CGUIString& rInterface, const SInterface& rInterfaceData );

	private:

		//map for interface
		typedef std::map<CGUIString, SInterface> TMapInterface;
		TMapInterface m_mapInterface;

		IGUIInterfaceRender* m_pInterfaceRender;
		IGUIInterfaceFont* m_pInterfaceFont;
		IGUIInterfaceScript* m_pInterfaceScript;
		IGUIInterfaceConfigFile* m_pInterfaceConfigFile;
		IGUIInterfaceFileSys* m_pInterfaceFileSys;
		IGUIInterfaceLocalizationLoader* m_pInterfaceLocalizationLoader;
		IGUIInterfaceMouse* m_pInterfaceMouse;
		IGUIInterfaceKeyboard* m_pInterfaceKeyboard;
		IGUIInterfaceSound* m_pInterfaceSound;
		IGUIInterfaceCommand* m_pInterfaceCommand;
		IGUIInterfaceIme* m_pInterfaceIme;
		IGUIInterfaceStringConv* m_pInterfaceStringConv;
		IGUIInterfaceImageLoader* m_pInterfaceImageLoader;
		IGUIInterfacePhysics* m_pInterfacePhysics;

	private:
		static CGUIInterfaceManager* m_pSingleton;
	};

	inline IGUIInterfaceRender* CGUIInterfaceManager::GetInterfaceRender()
	{
		return m_pInterfaceRender;
	}
	inline IGUIInterfaceFont* CGUIInterfaceManager::GetInterfaceFont()
	{
		return m_pInterfaceFont;
	}
	inline IGUIInterfaceScript* CGUIInterfaceManager::GetInterfaceScript()
	{
		return m_pInterfaceScript;
	}
	inline IGUIInterfaceConfigFile* CGUIInterfaceManager::GetInterfaceConfigFile()
	{
		return m_pInterfaceConfigFile;
	}
	inline IGUIInterfaceFileSys* CGUIInterfaceManager::GetInterfaceFileSys()
	{
		return m_pInterfaceFileSys;
	}
	inline IGUIInterfaceLocalizationLoader* CGUIInterfaceManager::GetInterfaceLocalizationLoader()
	{
		return m_pInterfaceLocalizationLoader;
	}
	inline IGUIInterfaceMouse* CGUIInterfaceManager::GetInterfaceMouse()
	{
		return m_pInterfaceMouse;
	}
	inline IGUIInterfaceKeyboard* CGUIInterfaceManager::GetInterfaceKeyboard()
	{
		return m_pInterfaceKeyboard;
	}
	inline IGUIInterfaceSound* CGUIInterfaceManager::GetInterfaceSound()
	{
		return m_pInterfaceSound;
	}
	inline IGUIInterfaceCommand* CGUIInterfaceManager::GetInterfaceCommand()
	{
		return m_pInterfaceCommand;
	}
	inline IGUIInterfaceIme* CGUIInterfaceManager::GetInterfaceIme()
	{
		return m_pInterfaceIme;
	}
	inline IGUIInterfaceStringConv* CGUIInterfaceManager::GetInterfaceStringConv()
	{
		return m_pInterfaceStringConv;
	}
	inline IGUIInterfaceImageLoader* CGUIInterfaceManager::GetInterfaceImageLoader()
	{
		return m_pInterfaceImageLoader;
	}
	inline IGUIInterfacePhysics* CGUIInterfaceManager::GetInterfacePhysics()
	{
		return m_pInterfacePhysics;
	}

	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceRenderWithTypeCheck()
	{
		GUI_ASSERT( m_pInterfaceRender->GetModuleType() == T::StaticGetModuleType(), "wrong interface type" );
		GUI_ASSERT( m_pInterfaceRender->GetModuleName() == T::StaticGetModuleName(), "wrong interface name" );
		return static_cast<T*>( m_pInterfaceRender );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceFontWithTypeCheck()
	{
		GUI_ASSERT( m_pInterfaceFont->GetModuleType() == T::StaticGetModuleType(), "wrong interface type" );
		GUI_ASSERT( m_pInterfaceFont->GetModuleName() == T::StaticGetModuleName(), "wrong interface name" );
		return static_cast<T*>( m_pInterfaceFont );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceScriptWithTypeCheck()
	{
		GUI_ASSERT( m_pInterfaceScript->GetModuleType() == T::StaticGetModuleType(), "wrong interface type" );
		GUI_ASSERT( m_pInterfaceScript->GetModuleName() == T::StaticGetModuleName(), "wrong interface name" );
		return static_cast<T*>( m_pInterfaceScript );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceConfigFileWithTypeCheck()
	{
		GUI_ASSERT( m_pInterfaceConfigFile->GetModuleType() == T::StaticGetModuleType(), "wrong interface type" );
		GUI_ASSERT( m_pInterfaceConfigFile->GetModuleName() == T::StaticGetModuleName(), "wrong interface name" );
		return static_cast<T*>( m_pInterfaceConfigFile );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceFileSysWithTypeCheck()
	{
		GUI_ASSERT( m_pInterfaceFileSys->GetModuleType() == T::StaticGetModuleType(), "wrong interface type" );
		GUI_ASSERT( m_pInterfaceFileSys->GetModuleName() == T::StaticGetModuleName(), "wrong interface name" );
		return static_cast<T*>( m_pInterfaceFileSys );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceLocalizationLoaderWithTypeCheck()
	{
		GUI_ASSERT( m_pInterfaceLocalizationLoader->GetModuleType() == T::StaticGetModuleType(), "wrong interface type" );
		GUI_ASSERT( m_pInterfaceLocalizationLoader->GetModuleName() == T::StaticGetModuleName(), "wrong interface name" );
		return static_cast<T*>( m_pInterfaceLocalizationLoader );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceMouseWithTypeCheck()
	{
		GUI_ASSERT( m_pInterfaceMouse->GetModuleType() == T::StaticGetModuleType(), "wrong interface type" );
		GUI_ASSERT( m_pInterfaceMouse->GetModuleName() == T::StaticGetModuleName(), "wrong interface name" );
		return static_cast<T*>( m_pInterfaceMouse );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceKeyboardWithTypeCheck()
	{
		GUI_ASSERT( m_pInterfaceKeyboard->GetModuleType() == T::StaticGetModuleType(), "wrong interface type" );
		GUI_ASSERT( m_pInterfaceKeyboard->GetModuleName() == T::StaticGetModuleName(), "wrong interface name" );
		return static_cast<T*>( m_pInterfaceKeyboard );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceSoundWithTypeCheck()
	{
		GUI_ASSERT( m_pInterfaceSound->GetModuleType() == T::StaticGetModuleType(), "wrong interface type" );
		GUI_ASSERT( m_pInterfaceSound->GetModuleName() == T::StaticGetModuleName(), "wrong interface name" );
		return static_cast<T*>( m_pInterfaceSound );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceCommandWithTypeCheck()
	{
		GUI_ASSERT( m_pInterfaceCommand->GetModuleType() == T::StaticGetModuleType(), "wrong interface type" );
		GUI_ASSERT( m_pInterfaceCommand->GetModuleName() == T::StaticGetModuleName(), "wrong interface name" );
		return static_cast<T*>( m_pInterfaceCommand );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceImeWithTypeCheck()
	{
		GUI_ASSERT( m_pInterfaceIme->GetModuleType() == T::StaticGetModuleType(), "wrong interface type" );
		GUI_ASSERT( m_pInterfaceIme->GetModuleName() == T::StaticGetModuleName(), "wrong interface name" );
		return static_cast<T*>( m_pInterfaceIme );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceStringConvWithTypeCheck()
	{
		GUI_ASSERT( m_pInterfaceStringConv->GetModuleType() == T::StaticGetModuleType(), "wrong interface type" );
		GUI_ASSERT( m_pInterfaceStringConv->GetModuleName() == T::StaticGetModuleName(), "wrong interface name" );
		return static_cast<T*>( m_pInterfaceStringConv );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceImageLoaderWithTypeCheck()
	{
		GUI_ASSERT( m_pInterfaceImageLoader->GetModuleType() == T::StaticGetModuleType(), "wrong interface type" );
		GUI_ASSERT( m_pInterfaceImageLoader->GetModuleName() == T::StaticGetModuleName(), "wrong interface name" );
		return static_cast<T*>( m_pInterfaceImageLoader );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfacePhysicsWithTypeCheck()
	{
		GUI_ASSERT( m_pInterfacePhysics->GetModuleType() == T::StaticGetModuleType(), "wrong interface type" );
		GUI_ASSERT( m_pInterfacePhysics->GetModuleName() == T::StaticGetModuleName(), "wrong interface name" );
		return static_cast<T*>( m_pInterfacePhysics );
	}
}//namespace guiex



//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	/**
	* @brief register interface with out parameter
	*/
	GUIEXPORT int RegisterInterface( const CGUIString& rInterface, const CGUIString& rModuleName );

	/**
	* @brief unregister interface
	*/
	GUIEXPORT int UnregisterInterface( const CGUIString& rInterface );

	/**
	* @brief get interface
	*/
	GUIEXPORT IGUIInterface* GetInterface( const CGUIString& rInterface );

}//namespace guiex

#endif //__GUI_INTERFACEMANAGER_H_20060610__
