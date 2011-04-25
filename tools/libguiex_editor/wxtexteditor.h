/** 
* @file texteditor.h
* @brief text editor, use scintilla library
* @author ken
* @date 2007-09-18
*/

#ifndef __KEN_TEXT_EDITOR_20070918_H__
#define __KEN_TEXT_EDITOR_20070918_H__

//============================================================================//
// include
//============================================================================// 
#include "savefilebase.h"

#include <wx/wxprec.h>

//scinilla
#include <scintilla.h>
#include <Platform.h>
#include <ILexer.h>
#include <LexAccessor.h>
#include <Accessor.h>
#include <PropSetSimple.h>
#include <SciLexer.h>

//============================================================================//
// declare
//============================================================================// 
enum EFileType
{
	EFT_RESOURCE,
	EFT_WIDGET,
	EFT_SCRIPT
};


//============================================================================//
// class
//============================================================================// 
class WxTextEditor : public wxPanel, public CSaveFileBase
{
public:
	WxTextEditor(wxWindow *parent,const std::string& rFileName);

	virtual ~WxTextEditor();

	/** 
	 * @brief initialize editor
	 * @return 0 for success, vice versa
	 */
	int InitEditor(const std::string& rFileName, EFileType eFileType);

	//save file to a new place
	virtual int SaveFileAs(const std::string& rNewFileName);

	virtual WXLRESULT MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

	EFileType GetFileType() const;

	std::string GetFileContent();

protected:
	/** 
	 * @brief open file
	 * @return 0 for success, vice versa
	 */
	int	OpenFile();

	void InitialiseEditor( EFileType eFileType );

	void AutomaticIndentation(char ch);
	//bool StartAutoCompleteWord(bool onlyOneWord);

	int IndentOfBlock(int line) ;
	void SetLineIndentation(int line, int indent);

	std::string GetLine(int line=-1);
	int GetCaretInLine();
	void GetRange(int start, int end, char *text);
	CharacterRange GetSelection();
	
	sptr_t SendEditor(unsigned int msg, uptr_t wParam=0, sptr_t lParam=0);
	sptr_t SendEditorString(unsigned int msg, uptr_t wParam, const char *s);
	void SetAStyle(int style, COLORREF fore, COLORREF back, int size=-1, const char *face=0);

	//fold
	void FoldAll();
	void EnsureAllChildrenVisible(int line, int level);
	void ToggleFoldRecursive(int line, int level);
	void Expand(int &line, bool doExpand, bool force = false, int visLevels = 0, int level = -1);

	//callback
	void OnNotify(SCNotification *notification) ;
	void OnSize(wxSizeEvent& event);
	void OnMarginClick(SCNotification *notification);

protected:
	HWND m_hScintillaWnd;
	std::string m_strFilename;			//file name

	std::string wordCharacters;
	bool autoCompleteIgnoreCase;

	DECLARE_EVENT_TABLE()
	DECLARE_ABSTRACT_CLASS(WxTextEditor)

	//for scintilla
	SciFnDirect fnEditor;
	long ptrEditor;

	EFileType m_eFileType;
};


#endif //__KEN_TEXT_EDITOR_20070918_H__
