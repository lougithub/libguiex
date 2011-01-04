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
#include <list>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	typedef void (*FuncScriptLoadModule)( void* );
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUIScript_lua : public IGUIInterfaceScript
	{
	public:
		IGUIScript_lua();
		virtual ~IGUIScript_lua();

		virtual void CreateScript( const CGUIString& rSceneName );
		virtual bool HasScript( const CGUIString& rSceneName );
		virtual void DestroyScript( const CGUIString& rSceneName );
		virtual void DestroyAllScript( );

		virtual	void ExecuteFile(const CGUIString& filename, const CGUIString& rSceneName);
		virtual	void ExecuteBuffer(void * pBuffer, int32 nBufferSize, const CGUIString& rSceneName);
		virtual	void ExecuteString(const char * pString, const CGUIString& rSceneName);
		virtual void ExecuteEventHandler(const CGUIString& rEventName, CGUIEvent* pEvent, const CGUIString& rSceneName);

		virtual void RegisterWidget( const CGUIWidget* pWidget );

		virtual void DeleteSelf();

	public:
		void* GetLuaState( const CGUIString& rSceneName );

		void ExecuteFunction( const CGUIString& pFunName, void* pLuaState);
		void ExecuteFunction( const CGUIString& pFunName, void* pPara1, const CGUIString& rParaType1, void* pLuaState);
		void ExecuteFunction( const CGUIString& pFunName, void* pPara1, const CGUIString& rParaType1, void* pPara2, const CGUIString& rParaType2, void* pLuaState );

		void AddScriptModule( void* pScriptModule );

	protected:
		virtual int DoInitialize(void* );
		virtual void DoDestroy();

		bool PushParToLua( void* pPara, const CGUIString& rParaType, void* pLuaState);

	private:
		std::map<CGUIString, void*> m_mapLuaState;///< The lua_State used by this module
		std::list<FuncScriptLoadModule> m_listModules;

	public: 
		static const char* StaticGetModuleName();
	};

	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_SCRIPT_LUA_20060404_H__

