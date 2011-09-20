/** 
* @file guikeyboard_default.cpp
* @brief default keyboard interface, the user has to 
* set keyboard value himself
* @author ken
* @date 2006-07-18
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module\keyboard_default\guikeyboard_default.h>



//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIKeyboard_default);
	//------------------------------------------------------------------------------
	const char* IGUIKeyboard_default::StaticGetModuleName()
	{
		return "IGUIKeyboard_default";
	}
	//------------------------------------------------------------------------------
	IGUIKeyboard_default::IGUIKeyboard_default()
		:IGUIInterfaceKeyboard( StaticGetModuleName() )
	{
	}
	//------------------------------------------------------------------------------
	IGUIKeyboard_default::~IGUIKeyboard_default()
	{
	}
	//------------------------------------------------------------------------------
	int IGUIKeyboard_default::DoInitialize(void* )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIKeyboard_default::DoDestroy()
	{
	}
	//------------------------------------------------------------------------------
	void	IGUIKeyboard_default::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------


}//namespace guiex
