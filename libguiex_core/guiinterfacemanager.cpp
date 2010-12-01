/** 
* @file guiinterfacemanager.cpp
* @brief interface manager
* @author ken
* @date 2006-06-10
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterface.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guilogmsgmanager.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUIEXPORT int RegisterInterface(const CGUIString& rInterface, 
		const CGUIString& rModuleName )
	{
		return CGUIInterfaceManager::Instance()->RegisterInterface( rInterface, rModuleName );
	}
	//------------------------------------------------------------------------------
	GUIEXPORT int UnregisterInterface(const CGUIString& rInterface )
	{
		return CGUIInterfaceManager::Instance()->UnregisterInterface( rInterface);
	}
	//------------------------------------------------------------------------------
	GUIEXPORT IGUIInterface* GetInterface(const CGUIString& rInterface )
	{
		return CGUIInterfaceManager::Instance()->GetInterface(rInterface);
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	CGUIInterfaceManager * CGUIInterfaceManager::m_pSingleton = NULL; 
	//------------------------------------------------------------------------------
	CGUIInterfaceManager::CGUIInterfaceManager()
		:m_pInterfaceRender(NULL)
		,m_pInterfaceFont(NULL)
		,m_pInterfaceScript(NULL)
		,m_pInterfaceConfigFile(NULL)
		,m_pInterfaceFileSys(NULL)
		,m_pInterfaceMouse(NULL)
		,m_pInterfaceKeyboard(NULL)
		,m_pInterfaceSound(NULL)
		,m_pInterfaceCommand(NULL)
		,m_pInterfaceIme(NULL)
		,m_pInterfaceStringConv(NULL)
	{
		GUI_ASSERT( !m_pSingleton, "[CGUIInterfaceManager::CGUIInterfaceManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUIInterfaceManager::~CGUIInterfaceManager()
	{
		//unregister all interface
		UnregisterAllInterface();
		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	CGUIInterfaceManager* CGUIInterfaceManager::Instance()
	{
		return m_pSingleton; 
	}
	//------------------------------------------------------------------------------
	int32	CGUIInterfaceManager::DoRegisterInterface( const CGUIString& rInterface, const SInterface& rInterfaceData )
	{
		if( m_mapInterface.find( rInterface) != m_mapInterface.end())
		{
			throw CGUIException(
				"[CGUIInterfaceManager::DoRegisterInterface] the interface <%s> has been registed",
				rInterface.c_str());
			return -1;
		}

		m_mapInterface.insert(std::make_pair( rInterface, rInterfaceData ));

		//add it to fast retrieve function
		if( rInterface == "IGUIRender" )
		{
			m_pInterfaceRender = (IGUIInterfaceRender*)rInterfaceData.m_pInterface;
		}
		else if( rInterface == "IGUIFont" )
		{
			m_pInterfaceFont = (IGUIInterfaceFont*)rInterfaceData.m_pInterface;
		}
		else if( rInterface == "IGUIScript" )
		{
			m_pInterfaceScript = (IGUIInterfaceScript*)rInterfaceData.m_pInterface;
		}
		else if( rInterface == "IGUIConfigFile" )
		{
			m_pInterfaceConfigFile = (IGUIInterfaceConfigFile*)rInterfaceData.m_pInterface;
		}
		else if( rInterface == "IGUIMouse" )
		{
			m_pInterfaceMouse = (IGUIInterfaceMouse*)rInterfaceData.m_pInterface;
		}
		else if( rInterface == "IGUIKeyboard" )
		{
			m_pInterfaceKeyboard = (IGUIInterfaceKeyboard*)rInterfaceData.m_pInterface;
		}
		else if( rInterface == "IGUIFileSys" )
		{
			m_pInterfaceFileSys = (IGUIInterfaceFileSys*)rInterfaceData.m_pInterface;
		}
		else if( rInterface == "IGUISound" )
		{
			m_pInterfaceSound = (IGUIInterfaceSound*)rInterfaceData.m_pInterface;
		}
		else if( rInterface == "IGUICommand" )
		{
			m_pInterfaceCommand = (IGUIInterfaceCommand*)rInterfaceData.m_pInterface;
		}
		else if( rInterface == "IGUIIme" )
		{
			m_pInterfaceIme = (IGUIInterfaceIme*)rInterfaceData.m_pInterface;
		}
		else if( rInterface == "IGUIStringConv" )
		{
			m_pInterfaceStringConv = (IGUIInterfaceStringConv*)rInterfaceData.m_pInterface;
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief register interface
	* this function is located in the dll,if the value is NULL,the default
	* name is GetGenerator_<rInterface>
	* @return 
	*		- 0 for success
	*		- -1 for failed
	* @exception CGUIException
	*/
	int	CGUIInterfaceManager::RegisterInterface(const CGUIString& rInterface, 
		const CGUIString& rModuleName, 
		void* pUserData )
	{
		GUI_TRACE( GUI_FORMAT("[CGUIInterfaceManager::RegisterInterface]:\n    Register interface <%s> from module <%s>\n\n",rInterface.c_str(), rModuleName.c_str()));

		if( m_mapInterface.find( rInterface) == m_mapInterface.end())
		{
			//register module
			GUI_DYNLIB_HANDLE d_handle = GUI_DYNLIB_LOAD(rModuleName.c_str());
			if (d_handle == NULL)
			{
				throw CGUIException(
					"[CGUIInterfaceManager::RegisterInterface] failed to load module <%s>",
					rModuleName.c_str());
				return -1;
			}

			//get function
			FunCreateInterface pFunc = (FunCreateInterface)GUI_DYNLIB_GETSYM(d_handle, "GetInterfaceInstance");
			if( !pFunc )
			{
				GUI_DYNLIB_UNLOAD(d_handle);
				throw CGUIException(
					"[CGUIInterfaceManager::RegisterInterface] failed to get function <GetInterfaceInstance> for interface <%s> from module <%s>",
					rInterface.c_str(),
					rModuleName.c_str());
				return -1;
			}

			//get interface
			IGUIInterface* pInterface = pFunc(pUserData);
			if( !pInterface )
			{
				GUI_DYNLIB_UNLOAD(d_handle);
				throw CGUIException(
					"[CGUIInterfaceManager::RegisterInterface] failed to get interface <GetInterfaceInstance> for interface <%s> from module <%s>",
					rInterface.c_str(),
					rModuleName.c_str());
				return -1;
			}

			SInterface aInterface;
			aInterface.m_hHandle = d_handle;
			aInterface.m_pInterface = pInterface;
			return DoRegisterInterface( rInterface, aInterface );
		}
		else
		{
			throw CGUIException(
				"[CGUIInterfaceManager::RegisterInterface] the interface <%s> has been registed",
				rInterface.c_str());
			return -1;
		}		

		return -1;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief register interface
	* @exception CGUIException
	*/
	int	CGUIInterfaceManager::RegisterInterface( const CGUIString& rInterface, IGUIInterface* pInterface )
	{
		GUI_TRACE( GUI_FORMAT( "[CGUIInterfaceManager::RegisterInterface]:\n    Register interface <%s>\n\n",
			rInterface.c_str()));

		GUI_ASSERT(pInterface,"wrong parameter");

		SInterface aInterface;
		aInterface.m_hHandle = NULL;
		aInterface.m_pInterface = pInterface;
		return DoRegisterInterface( rInterface, aInterface );
	}
	//------------------------------------------------------------------------------
	/**
	* @brief ungister a interface
	* @param rName the name of a interface
	* @return 
	*		- 0 for success
	*		- -1 for failed
	* @exception CGUIException
	*/
	int CGUIInterfaceManager::UnregisterInterface(const CGUIString& rInterface )
	{
		GUI_TRACE( GUI_FORMAT( "[CGUIInterfaceManager::UnregisterInterface]:\n    Unregister interface <%s>\n\n",
			rInterface.c_str()));

		TMapInterface::iterator itor = m_mapInterface.find( rInterface );
		if( itor == m_mapInterface.end() )
		{
			throw CGUIException(
				"[CGUIInterfaceManager::UnregisterInterface] the interface <%s> has been unregisted",
				rInterface.c_str());
			return -1;
		}
		else
		{
			//destroy interface
			(*itor).second.m_pInterface->Destroy();
			(*itor).second.m_pInterface->DeleteSelf();
			if( (*itor).second.m_hHandle )
			{
				GUI_DYNLIB_UNLOAD((*itor).second.m_hHandle);
			}
			m_mapInterface.erase( itor );

			//add it to fast retrieve function
			if( rInterface == "IGUIRender" )
			{
				m_pInterfaceRender = NULL;
			}
			else if( rInterface == "IGUIFont" )
			{
				m_pInterfaceFont = NULL;
			}
			else if( rInterface == "IGUIScript" )
			{
				m_pInterfaceScript = NULL;
			}
			else if( rInterface == "IGUIConfigFile" )
			{
				m_pInterfaceConfigFile = NULL;
			}
			else if( rInterface == "IGUIMouse" )
			{
				m_pInterfaceMouse = NULL;
			}
			else if( rInterface == "IGUIKeyboard" )
			{
				m_pInterfaceKeyboard = NULL;
			}
			else if( rInterface == "IGUIFileSys" )
			{
				m_pInterfaceFileSys = NULL;
			}
			else if( rInterface == "IGUISound" )
			{
				m_pInterfaceSound = NULL;
			}
			else if( rInterface == "IGUICommand" )
			{
				m_pInterfaceCommand = NULL;
			}
			else if( rInterface == "IGUIIme" )
			{
				m_pInterfaceIme = NULL;
			}
			return 0;
		}
		return -1;
	}
	//------------------------------------------------------------------------------

	/**
	* @brief unregister all interface
	*/
	void CGUIInterfaceManager::UnregisterAllInterface()
	{
		TMapInterface::iterator itorEnd2 = m_mapInterface.end();
		for( TMapInterface::iterator itor = m_mapInterface.begin();
			itor != itorEnd2;
			itor++)
		{
			SInterface&		rInterface = itor->second;
			rInterface.m_pInterface->Destroy();
			rInterface.m_pInterface->DeleteSelf();
			if( rInterface.m_hHandle )
			{
				GUI_DYNLIB_UNLOAD(rInterface.m_hHandle);
			}
		}
		m_mapInterface.clear();

		m_pInterfaceRender = NULL;
		m_pInterfaceFont = NULL;
		m_pInterfaceScript = NULL;
		m_pInterfaceConfigFile = NULL;
		m_pInterfaceFileSys = NULL;
		m_pInterfaceMouse = NULL;
		m_pInterfaceKeyboard = NULL;
		m_pInterfaceSound = NULL;
		m_pInterfaceCommand = NULL;
		m_pInterfaceIme = NULL;
	}
	//------------------------------------------------------------------------------

	/**
	* @brief get pointer of interface
	* @return NULL for failed
	* @exception CGUIException
	*/
	IGUIInterface* CGUIInterfaceManager::GetInterface(const CGUIString& rInterface )
	{
		TMapInterface::iterator itor = m_mapInterface.find( rInterface );
		if( itor == m_mapInterface.end() )
		{
			throw CGUIException(
				"[CGUIInterfaceManager::GetInterface] the interface <%s> hasn't been unregisted",
				rInterface.c_str());
			return NULL;
		}
		else
		{
			return (*itor).second.m_pInterface;
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief judge wheather has this kind of interface
	* @return true for has this interface, false for havn't this interface
	*/
	bool CGUIInterfaceManager::HasInterface(const CGUIString& rInterface )
	{
		if( m_mapInterface.find( rInterface ) == m_mapInterface.end() )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	//------------------------------------------------------------------------------

}//namespace guiex
