/** 
* @file editorid.h
* @brief 
* @author ken
* @date 2011-09-09
*/

#ifndef __KEN_EDITOR_ID_20110909_H__
#define __KEN_EDITOR_ID_20110909_H__

#include "editorwidgetid.h"
enum
{
	ID_Begin = EditorWidgetID_End,

	ID_ResourceFileCtrl, 
	ID_ResourceItemCtrl, 
	ID_NoteBookCtrl,

	//file menu
	ID_Exit,
	ID_Open,
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

	//tool menu
	ID_SetDefaultEditor,

	//about menu
	ID_About,
};


#endif //__KEN_EDITOR_ID_20110909_H__
