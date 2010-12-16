/** 
* @file guiassert.h
* @brief Creates a Macro for Assert
* @author ken
* @date 2007-05-09
*/


#ifndef __GUI_ASSERT_H_20070509__
#define __GUI_ASSERT_H_20070509__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"



//============================================================================//
// define
//============================================================================// 
/**
* @brief define GUI_ASSERT for debug use
*/
#if GUI_ASSERT_ON

	#define GUI_ASSERT(X, error) \
	if (!(X)) \
	{ \
	::guiex::CGUIAssert::Assert(__FILE__, __LINE__, error, #X); \
	}

	#define GUI_FORCE_ASSERT(error)	::guiex::CGUIAssert::Assert(__FILE__, __LINE__, error, "");
	#define GUI_WARNING(msg) ::guiex::CGUIAssert::Warning(__FILE__, __LINE__,msg)
	#define GUI_WARNING_HOLD(msg) ::guiex::CGUIAssert::WarningHold(__FILE__, __LINE__,msg)

#else
	#define GUI_ASSERT(X, error)
	#define GUI_FORCE_ASSERT(error)
	#define STATUS_WARNING(msg)

#endif

#define GUI_FORMAT ::guiex::CGUIAssert::Format

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	typedef void (*FGUIAssertCB)(const char* file, int32 line, const char* message, const char* expression, void* user_data);
	typedef void (*FGUIWarningCB)(const char* message, void* user_data);
}//namespace guiex


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIAssert
	* @brief class for assert macro
	*/
	class GUIEXPORT CGUIAssert
	{
	public:
		//set callback function
		static void SetAssertCB(FGUIAssertCB cb, void* user_data);
		static void SetWarningCB(FGUIWarningCB cb, void* user_data);

		//operation
		static void Assert(const char* file, int32 line, const char* message, const char* expression);
		static void Warning(const char* file, int32 line,const char* message);
		static void WarningHold(const char* file, int32 line,const char* message);

		static const char* Format(const char* message, ...);


		//callback function and parameter
		static void*			ms_AssertUserData;
		static FGUIAssertCB		ms_AssertCB;

		static void*			ms_WarningUserData;
		static FGUIWarningCB	ms_WarningCB;

	protected:
		static char         ms_msg[1024];
	};




}//namespace guiex

#endif	//__GUI_ASSERT_H_20070509__
