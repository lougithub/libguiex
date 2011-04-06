/** 
* @file wxeditorid.h
* @brief 
* @author ken
* @date 2010-12-28
*/

#ifndef __KEN_EDITOR_ID_20101228_H__
#define __KEN_EDITOR_ID_20101228_H__

enum
{
	ID_Begin = wxID_HIGHEST,

	//==================================================
	//main frame
	ID_Exit,
	ID_About,
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
	//edit
	ID_CreateWidget,
	ID_DeleteWidget,
	ID_WidgetUp,
	ID_WidgetDown,
	//view
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
	ID_ITEM_Image_Edit,

	WIDGET_ID_PG, //id of grid manager
	WIDGET_ID_TreeCtrl_File, //id of tree ctrl
	WIDGET_ID_TreeCtrl_Widget, //id of tree ctrl
	WIDGET_ID_NoteBook_Canvas,	
	WIDGET_ID_NoteBook_Config,	


	//==================================================
	//image select dialog
	ID_ImageSelect_BTN_OK,
	ID_ImageSelect_BTN_CANCEL,
	ID_ImageListID,

	//==================================================
	//sound select dialog
	ID_SoundSelect_BTN_OK,
	ID_SoundSelect_BTN_CANCEL,
	ID_SoundSelect_BTN_PLAY,
	ID_SoundListID,

	//==================================================
	//localization select dialog
	ID_LocalizationSelect_BTN_OK,
	ID_LocalizationSelect_BTN_CANCEL,
	ID_LocalizationListID,

	//==================================================
	//save file dialog
	ID_SaveFile_BTN_OK,
	ID_SaveFile_BTN_NO,
	ID_SaveFile_BTN_CANCEL,
};


#endif //__KEN_EDITOR_ID_20101228_H__
