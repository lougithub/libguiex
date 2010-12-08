/** 
* @file guiscript_lua.cpp
* @brief script module with lua.
* implement lua script with tolua++.
* tolua++ version is 1.0.91
* lua version is 5.1
*
* @author ken
* @date 2006-04-04
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/script_lua/guiscript_lua.h>


#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacefilesys.h>
#include <libguiex_core/guievent.h>
#include <libguiex_core/guiwidget.h>
#include <string.h>
#include <cassert>
#include <libguiex_script_wrapper/guiex_wrapper.h>

extern "C" int luaopen_guiex(lua_State* L);

#ifdef __cplusplus
extern "C" {
#endif //#ifdef __cplusplus
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	GUI_INTERFACE_IMPLEMENT(IGUIScript_lua);

	//------------------------------------------------------------------------------
	IGUIScript_lua::IGUIScript_lua()
	{
	}
	//------------------------------------------------------------------------------
	IGUIScript_lua::~IGUIScript_lua()
	{
		Destroy();
	}
	//------------------------------------------------------------------------------
	int IGUIScript_lua::DoInitialize( void* )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIScript_lua::DoDestroy()
	{
		DestroyAllScript();
	}
	//------------------------------------------------------------------------------
	void IGUIScript_lua::CreateScript( const CGUIString& rSceneName )
	{
		if( HasScript( rSceneName ))
		{
			return;
		}

		lua_State *pState = lua_open();
		if( !pState )
		{
			throw CGUIException_Script( "[IGUIScript_lua::CreateScript]: failed to create script for scene <%s>!", rSceneName.c_str() );
		}

		// init all standard libraries
		luaL_Reg lualibs[] = 
		{
			{"",				luaopen_base},
			{LUA_LOADLIBNAME,	luaopen_package},
			{LUA_TABLIBNAME,	luaopen_table},
			{LUA_IOLIBNAME,		luaopen_io},
			{LUA_OSLIBNAME,		luaopen_os},
			{LUA_STRLIBNAME,	luaopen_string},
			{LUA_MATHLIBNAME,	luaopen_math},
			{LUA_DBLIBNAME,		luaopen_debug},
			{NULL,				NULL}
		};

		const luaL_Reg *lib = lualibs;
		for (; lib->func; lib++) 
		{
			lua_pushcfunction(pState, lib->func);
			lua_pushstring(pState, lib->name);
			lua_call(pState, 1, 0);
		}
		
		//init others
		for( std::list<FuncScriptLoadModule>::iterator itor = m_listModules.begin();
			itor != m_listModules.end();
			++itor )
		{
			(*itor)( pState );
		}

		//register swig.
		luaopen_guiex( pState );

		m_mapLuaState.insert( std::make_pair( rSceneName, pState ));
	}
	//------------------------------------------------------------------------------
	void IGUIScript_lua::DestroyScript( const CGUIString& rSceneName )
	{
		std::map<CGUIString, void*>::iterator itor =  m_mapLuaState.find( rSceneName );
		if( itor != m_mapLuaState.end())
		{
			lua_close((lua_State*)(itor->second));
			m_mapLuaState.erase( itor );
		}
	}
	//------------------------------------------------------------------------------
	void IGUIScript_lua::DestroyAllScript( )
	{
		for( std::map<CGUIString, void*>::iterator itor = m_mapLuaState.begin();
			itor != m_mapLuaState.end();
			++itor )
		{
			lua_close((lua_State*)(itor->second));
		}
		m_mapLuaState.clear();
	}
	//------------------------------------------------------------------------------
	bool IGUIScript_lua::HasScript( const CGUIString& rSceneName )
	{
		return m_mapLuaState.find( rSceneName ) != m_mapLuaState.end();
	}
	//------------------------------------------------------------------------------
	void	IGUIScript_lua::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	void IGUIScript_lua::AddScriptModule( void * pScriptModule )
	{
		GUI_ASSERT( pScriptModule, "invalid parameter" );
		m_listModules.push_back( FuncScriptLoadModule(pScriptModule) );
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get lua_State used in this module
	* @return lua_State
	*/
	void* IGUIScript_lua::GetLuaState(const CGUIString& rSceneName)
	{
		std::map<CGUIString, void*>::iterator itor =  m_mapLuaState.find( rSceneName );
		if( itor != m_mapLuaState.end())
		{
			return itor->second;
		}
		else
		{
			throw CGUIException_Script( "[IGUIScript_lua::GetLuaState]: failed to get script by scene name <%s>!", rSceneName.c_str() );
			return NULL;
		}
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief execute a script file
	* 
	* @param filename filename of the script file
	*/
	void IGUIScript_lua::ExecuteFile(const CGUIString& filename, const CGUIString& rSceneName)
	{
		lua_State* L = (lua_State*)GetLuaState(rSceneName);

		IGUIInterfaceFileSys* pFileSys = CGUIInterfaceManager::Instance()->GetInterfaceFileSys();
		CGUIDataChunk aDataChunk;
		if( 0 != pFileSys->ReadFile( filename, aDataChunk ))
		{
			//failed
			throw CGUIException_Script( "[IGUIScript_lua::ExecuteFile]: failed to open file <%s>!", filename.c_str() );
		}

		// load code into lua and call it
		int error =	luaL_loadbuffer(L,(const char*)aDataChunk.GetDataPtr(),aDataChunk.GetSize(),filename.c_str()) || lua_pcall(L,0,0,0);


		// handle errors
		if ( error )
		{
			CGUIString msg = CGUIString("(IGUIScript_lua::ExecuteFile) Unable to execute script file: ")+"\n"+lua_tostring(L,-1)+"\n";
			lua_pop(L,1);
			throw CGUIException_Script( msg.c_str() );
		}
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief execute a memory buffer which contain script
	* 
	*/
	void IGUIScript_lua::ExecuteBuffer(void * pBuffer, int32 nBufferSize, const CGUIString& rSceneName)
	{
		lua_State* L = (lua_State*)GetLuaState(rSceneName);

		// load code into lua and call it
		int error =	luaL_loadbuffer(L,(const char*)pBuffer,nBufferSize,"script from buffer") || lua_pcall(L,0,0,0);

		// handle errors
		if ( error )
		{
			CGUIString msg = CGUIString("(IGUIScript_lua::ExecuteBuffer) Unable to execute script buffer: ")+"\n"+lua_tostring(L,-1)+"\n";
			lua_pop(L,1);
			throw CGUIException_Script( msg.c_str() );
		}
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief execute script from string
	*/
	void IGUIScript_lua::ExecuteString(const char * pString, const CGUIString& rSceneName)
	{
		lua_State* L = (lua_State*)GetLuaState(rSceneName);

		// load code into lua and call it
		int error =	luaL_loadbuffer(L,pString,strlen(pString),"script from string") || lua_pcall(L,0,0,0);

		// handle errors
		if ( error )
		{
			CGUIString msg = CGUIString("(IGUIScript_lua::ExecuteString) Unable to execute script string: ")+pString+"\n"+lua_tostring(L,-1)+"\n";
			lua_pop(L,1);
			throw CGUIException_Script( msg.c_str() );
		}
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief execute a event handle
	*/
	void IGUIScript_lua::ExecuteEventHandler(
		const CGUIString& rEventName, 
		CGUIEvent* pEvent,
		const CGUIString& rSceneName)
	{
		lua_State* L = (lua_State*)GetLuaState(rSceneName);
		ExecuteFunction( 
			rEventName, 
			pEvent->GetReceiver(),  "guiex::" + pEvent->GetReceiver()->GetType() + " *",
			pEvent, "guiex::" + pEvent->GetEventType() + " *",
			L); 
	}
	//------------------------------------------------------------------------------
	/**
	* @brief execute script function with given parameter.
	* @param pFunName function name.
	*/
	void IGUIScript_lua::ExecuteFunction(const CGUIString& pFunName, void* pLuaState)
	{
		lua_State* L = (lua_State*)pLuaState;

		// get the function from lua
		lua_getglobal(L, pFunName.c_str());

		// is it a function
		if ( !lua_isfunction(L,-1) )
		{
			return;
			//throw String( "name does not represent a Lua function" );
		}

		// call it
		int error = lua_pcall(L,0,0,0);

		// handle errors
		if ( error )
		{
			CGUIString msg = CGUIString("(IGUIScript_lua::ExecuteFunction) Unable to execute script function: ")+
				pFunName+"\n"+lua_tostring(L,-1)+"\n";
			lua_pop(L,1);
			throw CGUIException_Script( msg.c_str() );
		}
	}
	//------------------------------------------------------------------------------
	bool IGUIScript_lua::PushParToLua( void* pPara, const CGUIString& rParaType, void* pLuaState)
	{
		lua_State* L = (lua_State*)pLuaState;
		swig_module_info* head_module=SWIG_Lua_GetModule(L);
		if( !head_module )
		{
			return false;
		}
		swig_type_info* pInfo = SWIG_TypeQueryModule( head_module, head_module, rParaType.c_str());
		if( !pInfo )
		{
			return false;
		}

		SWIG_Lua_NewPointerObj( L,pPara, pInfo, 0 );
		return true;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief execute script function with given parameter.
	* @param pFunName function name.
	* @param pPara1 parameter
	* @param pParaType1 type of parameter.
	*/
	void IGUIScript_lua::ExecuteFunction(
		const CGUIString& pFunName, 
		void* pPara1, const CGUIString& rParaType1,
		void* pLuaState)
	{
		lua_State* L = (lua_State*)pLuaState;

		int nOldTop = lua_gettop(L);

		// get the function from lua
		lua_getglobal(L, pFunName.c_str());

		// is it a function
		if ( !lua_isfunction(L,-1) )
		{
			lua_pop(L,1);
			throw CGUIException_Script( "(IGUIScript_lua::ExecuteFunction) Unable to find function in script <%s>\n",pFunName.c_str() );
		}

		if( PushParToLua(pPara1, rParaType1, pLuaState) != true )
		{
			lua_pop(L,1);
			throw CGUIException_Script( "(IGUIScript_lua::ExecuteFunction) Unable to push parameter <%s>\n",rParaType1.c_str() );
		}

		// call it
		int error = lua_pcall(L,1,0,0);

		// handle errors
		if ( error )
		{
			CGUIString msg = CGUIString("(IGUIScript_lua::ExecuteFunction) Unable to execute script function: ")+
				pFunName+"\n"+lua_tostring(L,-1)+"\n";
			lua_pop(L,1);
			throw CGUIException_Script( msg.c_str() );
		}

		int nNewTop = lua_gettop(L);
		GUI_ASSERT( nOldTop == nNewTop, "error in script");
	}
	//------------------------------------------------------------------------------
	void IGUIScript_lua::ExecuteFunction(
		const CGUIString& pFunName, 
		void* pPara1, const CGUIString& rParaType1,
		void* pPara2, const CGUIString& rParaType2,
		void* pLuaState)
	{
		lua_State* L = (lua_State*)pLuaState;

		int nOldTop = lua_gettop(L);

		// get the function from lua
		lua_getglobal(L, pFunName.c_str());

		// is it a function
		if ( !lua_isfunction(L,-1) )
		{
			lua_pop(L,1);
			throw CGUIException_Script( "(IGUIScript_lua::ExecuteFunction) Unable to find function in script <%s>\n",pFunName.c_str() );
		}

		if( PushParToLua(pPara1, rParaType1, pLuaState) != true )
		{
			lua_pop(L,1);
			throw CGUIException_Script( "(IGUIScript_lua::ExecuteFunction) Unable to push parameter <%s>\n",rParaType1.c_str() );
		}
		if( PushParToLua(pPara2, rParaType2, pLuaState) != true )
		{
			lua_pop(L,2);
			throw CGUIException_Script( "(IGUIScript_lua::ExecuteFunction) Unable to push parameter <%s>\n",rParaType2.c_str() );
		}

		// call it
		int error = lua_pcall(L,2,0,0);

		// handle errors
		if ( error )
		{
			CGUIString msg = CGUIString("(IGUIScript_lua::ExecuteFunction) Unable to execute script function: ")+
				pFunName+"\n"+lua_tostring(L,-1)+"\n";
			lua_pop(L,1);
			throw CGUIException_Script( msg.c_str() );
		}

		int nNewTop = lua_gettop(L);
		GUI_ASSERT( nOldTop == nNewTop, "error in script");
	}
	//------------------------------------------------------------------------------
	void IGUIScript_lua::RegisterWidget( const CGUIWidget* pWidget )
	{
		lua_State* L = (lua_State*)GetLuaState(pWidget->GetSceneName());

		lua_pushstring( L, pWidget->GetName().c_str());

		swig_module_info* head_module=SWIG_Lua_GetModule(L);
		if( !head_module )
		{
			lua_pop(L,1);
			throw CGUIException_Script( "(IGUIScript_lua::RegisterWidget) Unable to find swig module\n" );
		}
		CGUIString strWidgetType = "guiex::" + pWidget->GetType() + " *";
		swig_type_info* pInfo = SWIG_TypeQueryModule( head_module, head_module, strWidgetType.c_str());
		if( !pInfo )
		{
			lua_pop(L,1);
			throw CGUIException_Script( "(IGUIScript_lua::RegisterWidget) Unable to find swig_type_info for <%s>\n",strWidgetType.c_str() );
		}

		SWIG_Lua_NewPointerObj( L, (void*)pWidget, pInfo, 0 );
		lua_rawset( L, LUA_GLOBALSINDEX );
	}
	//------------------------------------------------------------------------------

}//namespace guiex
