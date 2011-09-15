/** 
* @file wxeditorid.h
* @brief 
* @author ken
* @date 2010-12-28
*/

#ifndef __KEN_EDITOR_ID_20101228_H__
#define __KEN_EDITOR_ID_20101228_H__

#include "editorwidgetid.h"
enum
{
	ID_Begin = EditorWidgetID_End,

	//==================================================
	//main frame
	ID_Exit,
	ID_Open,
	ID_NewWidgetFile,
	ID_NewScriptFile,
	ID_AddImageFile,
	ID_Close,
	ID_Save,
	ID_SaveAs,
	ID_SaveAll,

	ID_RecentPaths,
	ID_RecentPathsBaseId,
	ID_RecentPathsEndId = ID_RecentPathsBaseId+10-1,
	ID_RecentScenes,
	ID_RecentScenesBaseId,
	ID_RecentScenesEndId = ID_RecentScenesBaseId+10-1,

	//about menu
	ID_About,

	//edit menu
	ID_CreateWidget,
	ID_DeleteWidget,
	ID_WidgetUp,
	ID_WidgetDown,
	ID_WidgetChangeParent,
	ID_WidgetCopy,
	ID_WidgetPaste,
	ID_ParseScript,

	ID_Localization_begin,
	ID_Localization_end = ID_Localization_begin+10,

	//tool menu
	ID_SetDefaultEditor,

	//view menu
	ID_VIEW_Fullscreen,
	ID_VIEW_800x600,
	ID_VIEW_1024x786,
	ID_VIEW_1280x800,
	ID_VIEW_Iphone480x320,
	ID_VIEW_Iphone320x480,
	ID_VIEW_Iphone960x640,
	ID_VIEW_Iphone640x960,
	ID_VIEW_Ipad1024x768,
	ID_VIEW_Ipad768x1024,

	ID_ToggleScissor,
	ID_ToggleWireframe,
	ID_TogglePlayingAs,
	ID_SetBGColor,
	ID_Refresh,
	ID_ToggleExtraInfo,
	
	ID_ITEM_Widget_Render,
	ID_ITEM_Widget_View,
	ID_ITEM_Widget_Edit,
	ID_ITEM_Edit_External,
	ID_ITEM_Script_Edit,
	ID_ITEM_Script_Check,
	ID_ITEM_Image_Edit,

	ID_GridManager, //id of grid manager
	ID_ID_TreeCtrl_File, //id of tree ctrl
	ID_TreeCtrl_Widget, //id of tree ctrl
	ID_NoteBookCtrl,	

	//==================================================
	//save file dialog
	ID_SaveFile_BTN_OK,
	ID_SaveFile_BTN_NO,
	ID_SaveFile_BTN_CANCEL,
};


#endif //__KEN_EDITOR_ID_20101228_H__
