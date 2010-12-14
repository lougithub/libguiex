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
#include "guiinterfaceImageLoader.h"


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
		throw ::guiex::CGUIException( "Failed to initialize interface <%s>", pInterface->GetModuleName().c_str());	\
	}															\
	::guiex::CGUIInterfaceManager::Instance()->RegisterInterface(pInterface->GetModuleName(), pInterface);	\
}

#define GUI_UNREGISTER_INTERFACE_LIB(classname)	\
	::guiex::CGUIInterfaceManager::Instance()->UnregisterInterface(classname::StaticGetModuleName());	



#define GUI_REGISTER_INTERFACE_LIB_ARG( classname, arg )	\
{	\
	::guiex::classname* pInterface = new ::guiex::classname;	\
	if( 0 != pInterface->Initialize(arg))						\
	{															\
		throw ::guiex::CGUIException( "Failed to initialize interface <%s>", pInterface->GetModuleName().c_str());	\
	}															\
	::guiex::CGUIInterfaceManager::Instance()->RegisterInterface(pInterface->GetModuleName(), pInterface);	\
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
		CGUIInterfaceManager();
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
		IGUIInterfaceMouse* GetInterfaceMouse();
		IGUIInterfaceKeyboard* GetInterfaceKeyboard();
		IGUIInterfaceSound* GetInterfaceSound();
		IGUIInterfaceCommand* GetInterfaceCommand();
		IGUIInterfaceIme* GetInterfaceIme();
		IGUIInterfaceStringConv* GetInterfaceStringConv();
		IGUIInterfaceImageLoader* GetInterfaceImageLoader();

		template <class T> T* GetInterfaceRenderTyped();
		template <class T> T* GetInterfaceFontTyped();
		template <class T> T* GetInterfaceScriptTyped();
		template <class T> T* GetInterfaceConfigFileTyped();
		template <class T> T* GetInterfaceFileSysTyped();
		template <class T> T* GetInterfaceMouseTyped();
		template <class T> T* GetInterfaceKeyboardTyped();
		template <class T> T* GetInterfaceSoundTyped();
		template <class T> T* GetInterfaceCommandTyped();
		template <class T> T* GetInterfaceImeTyped();
		template <class T> T* GetInterfaceStringConvTyped();
		template <class T> T* GetInterfaceImageLoaderTyped();

	protected:
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
		IGUIInterfaceMouse* m_pInterfaceMouse;
		IGUIInterfaceKeyboard* m_pInterfaceKeyboard;
		IGUIInterfaceSound* m_pInterfaceSound;
		IGUIInterfaceCommand* m_pInterfaceCommand;
		IGUIInterfaceIme* m_pInterfaceIme;
		IGUIInterfaceStringConv* m_pInterfaceStringConv;
		IGUIInterfaceImageLoader* m_pInterfaceImageLoader;

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

	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceRenderTyped()
	{
		GUI_ASSERT( m_pInterfaceRender->GetModuleName() == T::StaticGetModuleName(), "wrong interface type" );
		return static_cast<T*>( m_pInterfaceRender );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceFontTyped()
	{
		GUI_ASSERT( m_pInterfaceFont->GetModuleName() == T::StaticGetModuleName(), "wrong interface type" );
		return static_cast<T*>( m_pInterfaceFont );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceScriptTyped()
	{
		GUI_ASSERT( m_pInterfaceScript->GetModuleName() == T::StaticGetModuleName(), "wrong interface type" );
		return static_cast<T*>( m_pInterfaceScript );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceConfigFileTyped()
	{
		GUI_ASSERT( m_pInterfaceConfigFile->GetModuleName() == T::StaticGetModuleName(), "wrong interface type" );
		return static_cast<T*>( m_pInterfaceConfigFile );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceFileSysTyped()
	{
		GUI_ASSERT( m_pInterfaceFileSys->GetModuleName() == T::StaticGetModuleName(), "wrong interface type" );
		return static_cast<T*>( m_pInterfaceFileSys );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceMouseTyped()
	{
		GUI_ASSERT( m_pInterfaceMouse->GetModuleName() == T::StaticGetModuleName(), "wrong interface type" );
		return static_cast<T*>( m_pInterfaceMouse );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceKeyboardTyped()
	{
		GUI_ASSERT( m_pInterfaceKeyboard->GetModuleName() == T::StaticGetModuleName(), "wrong interface type" );
		return static_cast<T*>( m_pInterfaceKeyboard );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceSoundTyped()
	{
		GUI_ASSERT( m_pInterfaceSound->GetModuleName() == T::StaticGetModuleName(), "wrong interface type" );
		return static_cast<T*>( m_pInterfaceSound );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceCommandTyped()
	{
		GUI_ASSERT( m_pInterfaceCommand->GetModuleName() == T::StaticGetModuleName(), "wrong interface type" );
		return static_cast<T*>( m_pInterfaceCommand );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceImeTyped()
	{
		GUI_ASSERT( m_pInterfaceIme->GetModuleName() == T::StaticGetModuleName(), "wrong interface type" );
		return static_cast<T*>( m_pInterfaceIme );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceStringConvTyped()
	{
		GUI_ASSERT( m_pInterfaceStringConv->GetModuleName() == T::StaticGetModuleName(), "wrong interface type" );
		return static_cast<T*>( m_pInterfaceStringConv );
	}
	template<class T> 
	inline T* CGUIInterfaceManager::GetInterfaceImageLoaderTyped()
	{
		GUI_ASSERT( m_pInterfaceImageLoader->GetModuleName() == T::StaticGetModuleName(), "wrong interface type" );
		return static_cast<T*>( m_pInterfaceImageLoader );
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
