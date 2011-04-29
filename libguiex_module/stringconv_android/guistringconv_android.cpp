/** 
* @file guistringconv_android.cpp
* @brief used to convert string between different code
* @author ken
* @date 2011-04-29
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/stringconv_android/guistringconv_android.h>
#include <jni.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIStringConv_android);
	//------------------------------------------------------------------------------
	const char* IGUIStringConv_android::StaticGetModuleName()
	{
		return "IGUIStringConv_android";
	}
	//------------------------------------------------------------------------------
	IGUIStringConv_android::IGUIStringConv_android()
		:IGUIInterfaceStringConv(StaticGetModuleName())
	{
	}
	//------------------------------------------------------------------------------
	IGUIStringConv_android::~IGUIStringConv_android()
	{
	}
	//------------------------------------------------------------------------------
	int	IGUIStringConv_android::DoInitialize(void* pUserData)
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIStringConv_android::DoDestroy()
	{

	}
	//------------------------------------------------------------------------------
	int IGUIStringConv_android::Utf8ToUtf16( const CGUIString& rSrc, CGUIStringW& rDst )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	int IGUIStringConv_android::Utf16ToUtf8( const CGUIStringW& rSrc, CGUIString& rDst )
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIStringConv_android::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
}
