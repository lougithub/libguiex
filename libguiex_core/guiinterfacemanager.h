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

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{

	class IGUIInterface;

	typedef IGUIInterface* (*FunCreateInterface)(void*); 

	class IGUIInterfaceRender;
	class IGUIInterfaceFont;
	class IGUIInterfaceScript;
	class IGUIInterfaceConfigFile;
	class IGUIInterfaceFileSys;
	class IGUIInterfaceMouse;
	class IGUIInterfaceKeyboard;
	class IGUIInterfaceSound;
	class IGUIInterfaceCommand;
	class IGUIInterfaceIme;
	class IGUIInterfaceStringConv;
	
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
		//for singleton use
		CGUIInterfaceManager();
		~CGUIInterfaceManager();

		static CGUIInterfaceManager* Instance();

		int	RegisterInterface(const CGUIString& rInterface, const CGUIString& rModuleName, void* pUserData = NULL);
		int	RegisterInterface(const CGUIString& rInterface, IGUIInterface* pInterface );
		int UnregisterInterface(const CGUIString& rInterface );
		void UnregisterAllInterface();
		IGUIInterface* GetInterface(const CGUIString& rInterface );
		bool HasInterface(const CGUIString& rInterface );

	public:
		//utility functions
		IGUIInterfaceRender* GetInterfaceRender()
		{
			return m_pInterfaceRender;
		}
		IGUIInterfaceFont* GetInterfaceFont()
		{
			return m_pInterfaceFont;
		}
		IGUIInterfaceScript* GetInterfaceScript()
		{
			return m_pInterfaceScript;
		}
		IGUIInterfaceConfigFile* GetInterfaceConfigFile()
		{
			return m_pInterfaceConfigFile;
		}
		IGUIInterfaceFileSys* GetInterfaceFileSys()
		{
			return m_pInterfaceFileSys;
		}
		IGUIInterfaceMouse* GetInterfaceMouse()
		{
			return m_pInterfaceMouse;
		}
		IGUIInterfaceKeyboard* GetInterfaceKeyboard()
		{
			return m_pInterfaceKeyboard;
		}
		IGUIInterfaceSound* GetInterfaceSound()
		{
			return m_pInterfaceSound;
		}
		IGUIInterfaceCommand* GetInterfaceCommand()
		{
			return m_pInterfaceCommand;
		}
		IGUIInterfaceIme* GetInterfaceIme()
		{
			return m_pInterfaceIme;
		}
		IGUIInterfaceStringConv* GetInterfaceStringConv()
		{
			return m_pInterfaceStringConv;
		}

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
		TMapInterface	m_mapInterface;

		IGUIInterfaceRender*		m_pInterfaceRender;
		IGUIInterfaceFont*			m_pInterfaceFont;
		IGUIInterfaceScript*		m_pInterfaceScript;
		IGUIInterfaceConfigFile*	m_pInterfaceConfigFile;
		IGUIInterfaceFileSys*		m_pInterfaceFileSys;
		IGUIInterfaceMouse*			m_pInterfaceMouse;
		IGUIInterfaceKeyboard*		m_pInterfaceKeyboard;
		IGUIInterfaceSound*			m_pInterfaceSound;
		IGUIInterfaceCommand*		m_pInterfaceCommand;
		IGUIInterfaceIme*			m_pInterfaceIme;
		IGUIInterfaceStringConv*	m_pInterfaceStringConv;

	private:
		static CGUIInterfaceManager* m_pSingleton;
	};

}//namespace guiex



//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	/**
	* @brief register interface with out parameter
	*/
	GUIEXPORT int	RegisterInterface(const CGUIString& rInterface, 
		const CGUIString& rModuleName );

	/**
	* @brief unregister interface
	*/
	GUIEXPORT int UnregisterInterface(const CGUIString& rInterface );

	/**
	* @brief get interface
	*/
	GUIEXPORT IGUIInterface* GetInterface(const CGUIString& rInterface );

}//namespace guiex

#endif //__GUI_INTERFACEMANAGER_H_20060610__
