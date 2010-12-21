/** 
* @file guiinterface.h
* @brief interface class
* @author ken
* @date 2006-04-04
*/

#ifndef __GUI_INTERFACE_20060404_H__
#define __GUI_INTERFACE_20060404_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"


//============================================================================//
// define
//============================================================================// 
namespace guiex
{

#if GUI_DLL	//using dll
#define GUI_INTERFACE_DECLARE()	\
	extern "C" GUIEXPORT guiex::IGUIInterface* GetInterfaceInstance(void*)

#define GUI_INTERFACE_IMPLEMENT(interfacename) \
	extern "C" guiex::IGUIInterface* GetInterfaceInstance(void* pUserData)	\
	{ \
		IGUIInterface * pInterface = new interfacename; \
		if( !pInterface ) \
		{ \
			return NULL; \
		} \
		if( pInterface->Initialize(pUserData) !=0) \
		{ \
			delete pInterface; \
			return NULL; \
		} \
		return pInterface; \
	}
#else	//#if GUI_DLL	
#	define GUI_INTERFACE_DECLARE()
#	define GUI_INTERFACE_IMPLEMENT(interfacename)
#endif	//#if GUI_DLL	

}//namespace guiex

//============================================================================//
// class
//============================================================================// 

namespace guiex
{
	/** 
	* @brief base class of interface
	*/
	class GUIEXPORT IGUIInterface
	{
	public:
		IGUIInterface( const char* szModuleType, const char* szModuleName );
		virtual ~IGUIInterface();

		/** 
		* @brief initialize interface.
		* @param pData user data used by interface,
		* @return 0 for successful
		*/
		int Initialize(void* pUserData = NULL);

		/** 
		* @brief destroy interface.
		*/
		void Destroy();

		/**
		* @brief delete self, used by CGUIInterfaceManager
		*/
		virtual void DeleteSelf() = 0;

		const CGUIString& GetModuleType( ) const;
		const CGUIString& GetModuleName( ) const;

	protected:
		/** 
		* @brief implement initialization
		* @return 0 for successful
		*/
		virtual int	DoInitialize(void* pUserData) = 0;

		/** 
		* @brief implement destruct
		*/
		virtual void DoDestroy() = 0;

	private:
		CGUIString m_strModuleType;
		CGUIString m_strModuleName;

	private:
		bool m_bInitialize;	///< flag: has this interface been initialized

	};

}//namespace guiex

#endif	//__GUI_INTERFACE_20060404_H__

