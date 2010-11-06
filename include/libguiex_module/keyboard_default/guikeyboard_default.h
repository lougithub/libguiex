/** 
 * @file guikeyboard_default.h
 * @brief default keyboard interface, the user has to 
 * set keyboard value himself
 * @author ken
 * @date 2006-07-18
 */

#ifndef __GUI_KEYBOARD_DEFAULT_20060718_H__
#define __GUI_KEYBOARD_DEFAULT_20060718_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfacekeyboard.h>



namespace guiex
{

//============================================================================//
// class
//============================================================================// 
class GUIEXPORT IGUIKeyboard_default : public IGUIInterfaceKeyboard
{
public:
	/**
	 * @brief constructor
	 */
	IGUIKeyboard_default();

	/**
	 * @brief destructor
	 */
	virtual ~IGUIKeyboard_default();

	/**
	 * @brief used to delete this object
	 */
	virtual void	DeleteSelf();

protected:
	/** 
	 * @brief initialize keyboard
	 * @return 0 for success
	 */
	virtual int DoInitialize(void* );

	/** 
	 * @brief destroy keyboard
	 * @return 0 for success
	 */
	virtual void DoDestroy();

};



GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_KEYBOARD_DEFAULT_20060718_H__

