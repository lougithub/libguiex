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
#include <libguiex_core/guiwidgetmanager.h>
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
// declare
//============================================================================// 
namespace guiex
{
	//global function called in lua
	static int ExactType(lua_State* L);
}

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	static int ExactType(lua_State* L)
	{
		int SWIG_arg = 0;
		guiex::CGUIWidget *pWidget = 0 ;

		//get widget info
		swig_module_info* head_module=SWIG_Lua_GetModule(L);
		if( !head_module )
		{
			lua_pop(L,1);
			GUI_SCRIPT_THROW( "(ExactType) Unable to find swig module\n" );
		}
		swig_type_info* pWidgetInfo = SWIG_TypeQueryModule( head_module, head_module, "guiex::CGUIWidget *");
		if( !pWidgetInfo )
		{
			lua_pop(L,1);
			GUI_SCRIPT_THROW( GUI_FORMAT( "(ExactType) Unable to find swig_type_info for <%s>\n","guiex::CGUIWidget *" ));
		}

		SWIG_check_num_args("ExactType",1,1);
		if(!SWIG_isptrtype(L,1)) SWIG_fail_arg("ExactType",1,"guiex::CGUIWidget *");

		if (!SWIG_IsOK(SWIG_ConvertPtr(L,1,(void**)&pWidget,pWidgetInfo,0))){
			SWIG_fail_ptr("ExactType",1,pWidgetInfo);
		}

		if( pWidget )
		{
			CGUIString strWidgetType = "guiex::" + pWidget->GetType() + " *";
			pWidgetInfo = SWIG_TypeQueryModule( head_module, head_module, strWidgetType.c_str());
			if( !pWidgetInfo )
			{
				lua_pop(L,1);
				GUI_SCRIPT_THROW( GUI_FORMAT( "(ExactType) Unable to find swig_type_info for <%s>\n",strWidgetType.c_str() ));
			}
		}

		SWIG_NewPointerObj( L, (void*)pWidget, pWidgetInfo, 0 );	

		SWIG_arg++; 
		return SWIG_arg;

		if(0) SWIG_fail;

fail:
		lua_error(L);
		return SWIG_arg;
	}
	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIScript_lua);
	//------------------------------------------------------------------------------
	const char* IGUIScript_lua::StaticGetModuleName()
	{
		return "IGUIScript_lua";
	}
	//------------------------------------------------------------------------------
	IGUIScript_lua::IGUIScript_lua()
		:IGUIInterfaceScript(StaticGetModuleName())
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
	void IGUIScript_lua::RegisterGlobalFunction( void* pLuaState, const CGUIString& rSceneName )
	{
		lua_State *L = (lua_State *)pLuaState;

		//register global function
		lua_pushvalue(L,LUA_GLOBALSINDEX);
		lua_pushstring(L,"guiex");
		lua_rawget(L,-2);

		SWIG_Lua_add_function(L,"ExactType",ExactType);

		lua_pop(L, 2);


		std::string strGlobalFunc = std::string("guiex.GetSceneName = function() return\"") + rSceneName +"\"; end";
		int error =	luaL_loadbuffer(L,strGlobalFunc.c_str(),strGlobalFunc.length(),"script from string") || lua_pcall(L,0,0,0);
		if ( error )
		{
			CGUIString msg = CGUIString("(IGUIScript_lua::RegisterGlobalFunction) Unable to execute script string: ")+strGlobalFunc+"\n"+lua_tostring(L,-1)+"\n";
			lua_pop(L,1);
			GUI_SCRIPT_THROW( msg.c_str() );
		}
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
			GUI_SCRIPT_THROW( GUI_FORMAT("[IGUIScript_lua::CreateScript]: failed to create script for scene <%s>!", rSceneName.c_str() ));
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

		OnRegisterLuaFunction( pState );

		RegisterGlobalFunction( pState, rSceneName );

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
			GUI_SCRIPT_THROW( GUI_FORMAT( "[IGUIScript_lua::GetLuaState]: failed to get script by scene name <%s>!", rSceneName.c_str() ));
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
			GUI_SCRIPT_THROW( GUI_FORMAT( "[IGUIScript_lua::ExecuteFile]: failed to open file <%s>!", filename.c_str() ));
		}

		// load code into lua and call it
		int error =	luaL_loadbuffer(L,(const char*)aDataChunk.GetDataPtr(),aDataChunk.GetSize(),filename.c_str()) || lua_pcall(L,0,0,0);


		// handle errors
		if ( error )
		{
			CGUIString msg = CGUIString("(IGUIScript_lua::ExecuteFile) Unable to execute script file: ")+"\n"+lua_tostring(L,-1)+"\n";
			lua_pop(L,1);
			GUI_SCRIPT_THROW( msg.c_str() );
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
			GUI_SCRIPT_THROW(  msg.c_str() );
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
			GUI_SCRIPT_THROW( msg.c_str() );
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
		}

		// call it
		int error = lua_pcall(L,0,0,0);

		// handle errors
		if ( error )
		{
			CGUIString msg = CGUIString("(IGUIScript_lua::ExecuteFunction) Unable to execute script function: ")+
				pFunName+"\n"+lua_tostring(L,-1)+"\n";
			lua_pop(L,1);
			GUI_SCRIPT_THROW( msg.c_str() );
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

		SWIG_NewPointerObj( L,pPara, pInfo, 0 );
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
			GUI_SCRIPT_THROW( GUI_FORMAT( "(IGUIScript_lua::ExecuteFunction) Unable to find function in script <%s>\n",pFunName.c_str()));
		}

		if( PushParToLua(pPara1, rParaType1, pLuaState) != true )
		{
			lua_pop(L,1);
			GUI_SCRIPT_THROW( GUI_FORMAT( "(IGUIScript_lua::ExecuteFunction) Unable to push parameter <%s>\n",rParaType1.c_str() ));
		}

		// call it
		int error = lua_pcall(L,1,0,0);

		// handle errors
		if ( error )
		{
			CGUIString msg = CGUIString("(IGUIScript_lua::ExecuteFunction) Unable to execute script function: ")+
				pFunName+"\n"+lua_tostring(L,-1)+"\n";
			lua_pop(L,1);
			GUI_SCRIPT_THROW(  msg.c_str() );
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
			GUI_SCRIPT_THROW( GUI_FORMAT( "(IGUIScript_lua::ExecuteFunction) Unable to find function in script <%s>\n",pFunName.c_str() ));
		}

		if( PushParToLua(pPara1, rParaType1, pLuaState) != true )
		{
			lua_pop(L,1);
			GUI_SCRIPT_THROW( GUI_FORMAT( "(IGUIScript_lua::ExecuteFunction) Unable to push parameter <%s>\n",rParaType1.c_str() ));
		}
		if( PushParToLua(pPara2, rParaType2, pLuaState) != true )
		{
			lua_pop(L,2);
			GUI_SCRIPT_THROW( GUI_FORMAT( "(IGUIScript_lua::ExecuteFunction) Unable to push parameter <%s>\n",rParaType2.c_str() ));
		}

		// call it
		int error = lua_pcall(L,2,0,0);

		// handle errors
		if ( error )
		{
			CGUIString msg = CGUIString("(IGUIScript_lua::ExecuteFunction) Unable to execute script function: ")+
				pFunName+"\n"+lua_tostring(L,-1)+"\n";
			lua_pop(L,1);
			GUI_SCRIPT_THROW( msg.c_str() );
		}

		int nNewTop = lua_gettop(L);
		GUI_ASSERT( nOldTop == nNewTop, "error in script");
	}
	//------------------------------------------------------------------------------
	void IGUIScript_lua::RegisterWidget( const CGUIWidget* pWidget )
	{
#if 0
		lua_State* L = (lua_State*)GetLuaState(pWidget->GetWorkingSceneName());

		lua_pushstring( L, pWidget->GetName().c_str());

		swig_module_info* head_module=SWIG_Lua_GetModule(L);
		if( !head_module )
		{
			lua_pop(L,1);
			GUI_SCRIPT_THROW( "(IGUIScript_lua::RegisterWidget) Unable to find swig module\n" );
		}
		CGUIString strWidgetType = "guiex::" + pWidget->GetType() + " *";
		swig_type_info* pInfo = SWIG_TypeQueryModule( head_module, head_module, strWidgetType.c_str());
		if( !pInfo )
		{
			lua_pop(L,1);
			GUI_SCRIPT_THROW( GUI_FORMAT( "(IGUIScript_lua::RegisterWidget) Unable to find swig_type_info for <%s>\n",strWidgetType.c_str() ));
		}

		SWIG_NewPointerObj( L, (void*)pWidget, pInfo, 0 );
		lua_rawset( L, LUA_GLOBALSINDEX );
#endif
	}
	//------------------------------------------------------------------------------

}//namespace guiex
