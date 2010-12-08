/** 
* @file guiinterfacescript.h
* @brief interface for script
* @author ken
* @date 2006-04-04
*/

#ifndef __GUI_INTERFACE_SCRIPT_20060404_H_
#define __GUI_INTERFACE_SCRIPT_20060404_H_

//============================================================================//
// include
//============================================================================// 
#include "guiinterface.h"
#include "guistring.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUIEvent;
	class CGUIWidget;
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUIInterfaceScript : public IGUIInterface
	{
	public:
		/** 
		* @brief constructor
		*/
		IGUIInterfaceScript();

		/** 
		* @brief destructor
		*/
		virtual ~IGUIInterfaceScript();

		virtual void AddScriptModule(void* pData) = 0;

		virtual void CreateScript( const CGUIString& rSceneName ) = 0;
		virtual bool HasScript( const CGUIString& rSceneName ) = 0;
		virtual void DestroyScript( const CGUIString& rSceneName ) = 0;
		virtual void DestroyAllScript( ) = 0;

		/** 
		* @brief execute a script file
		* 
		* @param filename filename of the script file
		*/
		virtual	void ExecuteFile(const CGUIString& filename, const CGUIString& rSceneName) = 0;

		/** 
		* @brief execute a memory buffer which contain script
		*/
		virtual	void ExecuteBuffer(void * pBuffer, int32 nBufferSize, const CGUIString& rSceneName) = 0;

		/** 
		* @brief execute a string which contain script
		*/
		virtual	void ExecuteString(const char * pString, const CGUIString& rSceneName) = 0;

		virtual void RegisterWidget( const CGUIWidget* pWidget) = 0;

		/** 
		* @brief execute a event handle
		* 
		* @param rEventName event name
		*/
		virtual void ExecuteEventHandler(const CGUIString& rEventName, CGUIEvent* pEvent, const CGUIString& rSceneName)=0;

	public: 
		static const char* StaticGetModuleName();
	};

}//namespace guiex


#endif __GUI_INTERFACE_SCRIPT_20060404_H_

