/** 
* @file guiscript_lua.h
* @brief script module with lua.
* implement lua script with tolua++.
* tolua++ version is 1.0.91
* lua version is 5.1
*
* @author ken
* @date 2006-04-04
*/

#ifndef __GUI_SCRIPT_LUA_20060404_H__
#define __GUI_SCRIPT_LUA_20060404_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacescript.h>
#include <map>


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	typedef void (*FuncInitScript)( void* );
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUIScript_lua : public IGUIInterfaceScript
	{
	public:
		/** 
		* @brief constructor
		*/
		IGUIScript_lua();

		/** 
		* @brief destructor
		*/
		virtual ~IGUIScript_lua();

		virtual void	SetExternalData(void* pData);

		virtual void CreateScript( const CGUIString& rProjectName );
		virtual bool HasScript( const CGUIString& rProjectName );
		virtual void DestroyScript( const CGUIString& rProjectName );
		virtual void DestroyAllScript( );

		/** 
		* @brief execute a script file
		* 
		* @param filename filename of the script file
		*/
		virtual	void	ExecuteFile(const CGUIString& filename, const CGUIString& rProjectName);

		/** 
		* @brief execute a memory buffer which contain script
		* 
		*/
		virtual	void	ExecuteBuffer(void * pBuffer, int32 nBufferSize, const CGUIString& rProjectName);

		/** 
		* @brief execute script from string
		*/
		virtual	void	ExecuteString(const char * pString, const CGUIString& rProjectName);

		/** 
		* @brief execute a event handle
		*/
		virtual void 	ExecuteEventHandler(const CGUIString& rEventName, CGUIEvent* pEvent, const CGUIString& rProjectName);

		virtual void	RegisterWidget( const CGUIWidget* pWidget );

		/**
		* @brief delete this interface
		*/
		virtual void	DeleteSelf();

	public:
		/**
		* @brief get lua_State used in this module
		* @return lua_State
		*/
		void*		GetLuaState( const CGUIString& rProjectName );

		/**
		* @brief execute script function with given parameter.
		* @param pFunName function name.
		*/
		void			ExecuteFunction(const CGUIString& pFunName, void* pLuaState);

		/**
		* @brief execute script function with given parameter.
		* @param pFunName function name.
		* @param pPara1 parameter
		* @param pParaType1 type of parameter.
		*/
		void			ExecuteFunction(
			const CGUIString& pFunName, 
			void* pPara1, const CGUIString& rParaType1, 
			void* pLuaState);

		void			ExecuteFunction(
			const CGUIString& pFunName, 
			void* pPara1, const CGUIString& rParaType1,
			void* pPara2, const CGUIString& rParaType2,
			void* pLuaState);


	protected:
		/** 
		* @brief initialize lua and tolua
		* @return 0 for success
		*/
		virtual int DoInitialize(void* );

		/** 
		* @brief destroy lua and tolua
		* @return 0 for success
		*/
		virtual void DoDestroy();

		bool	PushParToLua( void* pPara, const CGUIString& rParaType, void* pLuaState);

	private:
		std::map<CGUIString, void*>			m_mapLuaState;///< The lua_State used by this module
		FuncInitScript						m_funcInitScript;
	};

	GUI_INTERFACE_DECLARE();

}//namesace guiex

#endif //__GUI_SCRIPT_LUA_20060404_H__

