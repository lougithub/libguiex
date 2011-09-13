/** 
* @file editorwidgetid.h
* @brief 
* @author ken
* @date 2011-09-09
*/

#ifndef __KEN_EDITORWIDGET_ID_20110909_H__
#define __KEN_EDITORWIDGET_ID_20110909_H__

enum
{
	EditorWidgetID_Begin = wxID_HIGHEST,

	//resource select dialog
	ID_ResourceSelect_BTN_OK,
	ID_ResourceSelect_BTN_CANCEL,
	ID_ResourceListID,
	//sound select dialog
	ID_SoundSelect_BTN_PLAY,


	EditorWidgetID_End,
};

enum 
{
	TIMERID_PREVIEWCANVAS_RENDER = 100,

	TIMERID_HIGHEST,
};


#endif //__KEN_EDITORWIDGET_ID_20110909_H__
