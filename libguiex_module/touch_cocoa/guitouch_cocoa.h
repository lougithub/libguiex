/** 
* @file guitouch_cocoa.h
* @brief mouse interface, use ios touch
* @author bxliu
* @date 2011-01-03
*/

#ifndef __GUI_TOUCH_COCOA_H__
#define __GUI_TOUCH_COCOA_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacemouse.h>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUITouch_cocoa : public IGUIInterfaceMouse
	{
	public:
		/**
		* @brief constructor
		*/
		IGUITouch_cocoa();

		/**
		* @brief destructor
		*/
		virtual ~IGUITouch_cocoa();

		/**
		* @brief used to delete this object
		*/
		virtual void DeleteSelf();

		

	protected:
		/** 
		* @brief initialize render
		* @return 0 for success
		*/
		virtual int DoInitialize(void* );

		/** 
		* @brief destroy render
		* @return 0 for success
		*/
		virtual void DoDestroy();


	public: 
		static const char* StaticGetModuleName();
	};

	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_TOUCH_COCOA_H__

