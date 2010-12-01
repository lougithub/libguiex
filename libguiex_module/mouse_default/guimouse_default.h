/** 
* @file guimouse_default.h
* @brief default mouse interface, the user has to 
* set mouse value himself
* @author ken
* @date 2006-07-18
*/

#ifndef __GUI_MOUSE_DEFAULT_20060718_H__
#define __GUI_MOUSE_DEFAULT_20060718_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacemouse.h>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT IGUIMouse_default : public IGUIInterfaceMouse
	{
	public:
		/**
		* @brief constructor
		*/
		IGUIMouse_default();

		/**
		* @brief destructor
		*/
		virtual ~IGUIMouse_default();

		/**
		* @brief used to delete this object
		*/
		virtual void	DeleteSelf();

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

	};



	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_MOUSE_DEFAULT_20060718_H__

