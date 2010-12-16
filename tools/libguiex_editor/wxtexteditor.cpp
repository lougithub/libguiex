/** 
* @file texteditor.cpp
* @brief text editor, use scintilla library
* @author ken
* @date 2007-09-18
*/

//============================================================================//
// include
//============================================================================// 
#include "libguiex_editor.h"


#include <richedit.h>


//============================================================================//
// declare
//============================================================================// 
static const int MARGIN_SCRIPT_FOLD_INDEX = 1;





//============================================================================//
// function
//============================================================================// 
//------------------------------------------------------------------------------
IMPLEMENT_ABSTRACT_CLASS(WxTextEditor, wxPanel)

BEGIN_EVENT_TABLE(WxTextEditor, wxPanel)
EVT_SIZE(WxTextEditor::OnSize)
END_EVENT_TABLE()
//------------------------------------------------------------------------------
WxTextEditor::WxTextEditor(wxWindow *parent,const std::string& rFileName,wxWindowID winid,const wxPoint& pos,const wxSize& size,long style,const wxString& name)
:wxPanel(parent, winid, pos,size, style, name )
,CSaveFileBase(rFileName)
{
	wordCharacters = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	autoCompleteIgnoreCase = false;
}
//------------------------------------------------------------------------------
WxTextEditor::~WxTextEditor()
{
	::DestroyWindow(m_hScintillaWnd);
}
//------------------------------------------------------------------------------
int WxTextEditor::InitEditor(const std::string& rFileName, EFileType eFileType )
{
	m_strFilename = rFileName;

	m_hScintillaWnd = CreateWindowEx(
		0,
		_T("Scintilla"),
		_T("source"), 
		WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0,
		0,
		this->GetSize().GetWidth(),
		this->GetSize().GetHeight(), 
		HWND(this->GetHandle()),
		NULL, 
		wxGetInstance(),
		NULL);

	//get function ptr
	fnEditor = reinterpret_cast<SciFnDirect>(::SendMessage(m_hScintillaWnd, SCI_GETDIRECTFUNCTION, 0, 0));
	ptrEditor = ::SendMessage(m_hScintillaWnd,SCI_GETDIRECTPOINTER, 0, 0);
	if (!fnEditor || !ptrEditor)
	{
		return -1;
	}

	::ShowWindow( m_hScintillaWnd, SW_SHOWNOACTIVATE );
	SendEditor(SCI_USEPOPUP, 0);
	::SendMessage(m_hScintillaWnd, SCI_GRABFOCUS, 0, 0 );
	::SetFocus(m_hScintillaWnd);

	InitialiseEditor( eFileType );

	if( OpenFile() != 0 )
	{
		return -1;
	}

	return 0;
}
//------------------------------------------------------------------------------
sptr_t WxTextEditor::SendEditor(unsigned int msg, uptr_t wParam, sptr_t lParam)
{
	sptr_t retVal = fnEditor(ptrEditor, msg, wParam, lParam);
	sptr_t status = fnEditor(ptrEditor, SCI_GETSTATUS, 0, 0);
	if (status > 0)
	{
		wxASSERT(0);
	}
	return retVal;
}
//------------------------------------------------------------------------------
sptr_t WxTextEditor::SendEditorString(unsigned int msg, uptr_t wParam, const char *s)
{
	return SendEditor(msg, wParam, reinterpret_cast<sptr_t>(s));
}
//------------------------------------------------------------------------------
int	WxTextEditor::OpenFile()
{
	const int blockSize = 128 * 1024;

	SendEditor(SCI_CLEARALL);
	SendEditor(EM_EMPTYUNDOBUFFER);
	SendEditor(SCI_CANCEL);
	SendEditor(SCI_SETUNDOCOLLECTION, 0);


	if( m_strFilename.empty() == false)
	{
		FILE *fp = fopen(m_strFilename.c_str(), "rb");
		if (fp) 
		{
			char data[blockSize];
			unsigned int lenFile = fread(data, 1, sizeof(data), fp);
			while (lenFile > 0) 
			{
				SendEditor(SCI_ADDTEXT, lenFile,reinterpret_cast<LPARAM>(static_cast<char *>(data)));
				lenFile = fread(data, 1, sizeof(data), fp);
			}
			fclose(fp);
		} 
		else 
		{
			std::string rErr = std::string("failed to open file: ") + m_strFilename;
			wxMessageBox(Gui2wxString(rErr), wxT("error"), wxOK|wxCENTER|wxICON_ERROR);
			return -1;
		}
	}
	else
	{
		//a new file
	}

	SendEditor(SCI_SETUNDOCOLLECTION, 1);
	SendEditor(EM_EMPTYUNDOBUFFER);
	SendEditor(SCI_SETSAVEPOINT);
	SendEditor(SCI_GOTOPOS, 0);
	return 0;
}
//------------------------------------------------------------------------------
int WxTextEditor::SaveFileAs(const std::string& rNewFileName)
{
	wxASSERT( m_strFilename.empty() == false);

	const int blockSize = 128 * 1024;
	FILE *fp= fopen(rNewFileName.c_str(), "wb");
	if (fp) 
	{
		char data[blockSize + 1];
		int lengthDoc = SendEditor(SCI_GETLENGTH);
		for (int i = 0; i < lengthDoc; i += blockSize)
		{
			int grabSize = lengthDoc - i;
			if (grabSize > blockSize)
				grabSize = blockSize;
			GetRange(i, i + grabSize, data);
			fwrite(data, grabSize, 1, fp);
		}
		fclose(fp);
		SendEditor(SCI_SETSAVEPOINT);
	} 
	else 
	{
		wxMessageBox(wxT("failed to save file!"),wxT("error"), wxOK|wxCENTER|wxICON_ERROR);
		return -1;
	}

	return 0;
}
//------------------------------------------------------------------------------
void WxTextEditor::OnSize(wxSizeEvent& event)
{
	if( m_hScintillaWnd )
	{
		wxRect aRect = GetClientRect();
		::SetWindowPos(m_hScintillaWnd, 0, aRect.GetLeft(), aRect.GetTop(), aRect.GetWidth(), aRect.GetHeight(), 0);
	}
}
//------------------------------------------------------------------------------
WXLRESULT WxTextEditor::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
	switch (nMsg) 
	{
	case WM_NOTIFY:
		OnNotify(reinterpret_cast<SCNotification *>(lParam));
		break;

	default:
		//Platform::DebugPrintf("default wnd proc %x %d %d\n",iMessage, wParam, lParam);
		return wxPanel::MSWWindowProc(nMsg,wParam, lParam);
	}
	return 0l;
}
//------------------------------------------------------------------------------
void WxTextEditor::OnNotify(SCNotification *notification) 
{
	switch (notification->nmhdr.code) 
	{
	case SCN_MARGINCLICK:
		{
			OnMarginClick(notification);
		}
		break;

	case SCN_CHARADDED:
		{
			if( notification->ch == '\r' ||notification->ch == '\n' )
			{
				AutomaticIndentation(notification->ch);
			}
			else if( notification->ch <= 31 )
			{
				SendEditor(SCI_UNDO);

				return;
			}
			else
			{
				AutomaticIndentation(notification->ch);
			}

			//StartAutoCompleteWord(false);
		}
		break;

	case SCN_SAVEPOINTREACHED:
		{
			SetSaveFlag(false);
		}
		break;

	case SCN_SAVEPOINTLEFT:
		{
			SetSaveFlag(true);
		}
		break;

	default:     	// Scintilla notification, use default treatment
		//SciTEBase::Notify(notification);
		break;
	}
}
//------------------------------------------------------------------------------
void WxTextEditor::OnMarginClick( SCNotification *notification ) 
{
	if( notification->margin != MARGIN_SCRIPT_FOLD_INDEX )
	{
		return;
	}
	
	int position = notification->position;
	int modifiers = notification->modifiers;

	int lineClick = SendEditor(SCI_LINEFROMPOSITION, position);
	if ((modifiers & SCMOD_SHIFT) && (modifiers & SCMOD_CTRL)) 
	{
		FoldAll();
	} 
	else 
	{
		int levelClick = SendEditor(SCI_GETFOLDLEVEL, lineClick);
		if (levelClick & SC_FOLDLEVELHEADERFLAG) 
		{
			if (modifiers & SCMOD_SHIFT) 
			{
				EnsureAllChildrenVisible(lineClick, levelClick);
			} 
			else if (modifiers & SCMOD_CTRL)
			{
				ToggleFoldRecursive(lineClick, levelClick);
			} 
			else 
			{
				// Toggle this line
				SendEditor(SCI_TOGGLEFOLD, lineClick);
			}
		}
	}
}
//------------------------------------------------------------------------------
const char sparkKeyWords[] = 
"xml version "
"public !doctype xml flash";
const char gcfKeyWords[] = 
"xml version"
"config"
"name duration sparkfile image comment"
"catalogs catalog type" ;

struct SStyle
{
	COLORREF fore;
	COLORREF back;
} g_pStype[] = 
{
	//# Default
	{0x000000,0xFFFFFF},
	//# Tags
	{0x800000,0xFFFFFF},
	//# Unknown Tags
	{0x800000,0xFFFFFF},
	//# Attributes
	{0x808000,0xFFFFFF},
	//# Unknown Attributes
	{0x808000,0xFFFFFF},
	//# Numbers
	{0x0000FF,0xFFFFFF},
	//# Double quoted strings
	{0x800000,0xFFFFFF},
	//# Single quoted strings
	{0x800000,0xFFFFFF},
	//# Other inside tag
	{0x800080,0xFFFFFF},
	//# Comment
	{0x008080,0xFFFFFF},
	//# Entities
	{0x800080,0xFFFFFF},
	//# XML style tag ends '/>'
	{0x800000,0xFFFFFF},
	//# XML identifier start '<?'
	{0x800080,0xFFFFFF},//bold",
	//# XML identifier end '?>'
	{0x800080,0xFFFFFF},//bold",
	//# CDATA
	{0x000080,0xFFF0F0},
	//# Question
	{0x000080,0xFFFFFF},
	//# Unquoted Value
	{0x608060,0xFFFFFF},
	//# SGML tags <! ... >
	{0x800000,0xFFFFFF},
	//# SGML command
	{0x800000,0xFFFFFF},//bold",
	//# SGML 1st param
	{0x006600,0xFFFFFF},
	//# SGML double string
	{0x000080,0xFFFFFF},
	//# SGML single string
	{0x003399,0xFFFFFF},
	//# SGML error
	{0x000080,0xFF6666},
	//# SGML special (#xxxx type)
	{0xFF6633,0xFFFFFF},
	//# SGML entity
	{0x333333,0xFFFFFF},
	//# SGML comment
	{0x008080,0xFFFFFF},
	//# SGML block
	{0x660000,0xCCCCE0}
};

void WxTextEditor::InitialiseEditor( EFileType eFileType )
{		
	const COLORREF black = RGB(0,0,0);
	const COLORREF white = RGB(0xff,0xff,0xff);
	const COLORREF red = RGB(0xFF, 0, 0);
	const COLORREF offWhite = RGB(0xFF, 0xFB, 0xF0);
	const COLORREF darkGreen = RGB(0, 0x80, 0);
	const COLORREF darkBlue = RGB(0, 0, 0x80);
	const COLORREF yellow = RGB(0xFF, 0xFF, 0);
	const COLORREF lightBlue = RGB(0xA6, 0xCA, 0xF0);

	switch( eFileType )
	{
	case EFT_IMAGE:
	case EFT_WIDGET:
		SendEditor(SCI_SETLEXER, SCLEX_XML );
		break;

	case EFT_SCRIPT:
		SendEditor(SCI_SETLEXER, SCLEX_LUA);
		break;
	
	default:
		wxASSERT(0);
		break;
	}

	SendEditor(SCI_SETSTYLEBITS, 5);

	//******************************************************************
	// Set up the global default style. These attributes are used wherever no 
	//explicit choices are made.
	SetAStyle(STYLE_DEFAULT, black, white, 11, "Times New Roman");
	SendEditor(SCI_STYLECLEARALL);	// Copies global style to all others
	SetAStyle( STYLE_LINENUMBER,darkGreen,RGB(0xF6,0xF4,0xEC), 11, "Times New Roman");
	SendEditor(SCI_STYLESETBOLD, 1);	
	SetAStyle( STYLE_INDENTGUIDE,red,yellow, 11, "Times New Roman");


	//******************************************************************
	//SendEditor(SCI_SETLEXER, SCLEX_CONTAINER);
	SendEditor(SCI_SETVSCROLLBAR, 1);
	SendEditor(SCI_SETHSCROLLBAR, 1);


	//******************************************************************
	//set indent
	SendEditor(SCI_SETTABWIDTH, 8);
	SendEditor(SCI_SETUSETABS, 1);
	SendEditor(SCI_SETINDENT, 8);
	SendEditor(SCI_SETTABINDENTS, 1);
	SendEditor(SCI_SETBACKSPACEUNINDENTS, 1);
	SendEditor(SCI_SETINDENTATIONGUIDES, 1);
	SendEditor(SCI_SETHIGHLIGHTGUIDE, 1);


	//******************************************************************
	//set fold
	//Set some properties so the lexer knows what we want
	SendEditor(SCI_SETPROPERTY, (WPARAM)"fold", (LPARAM)"1");
	SendEditor(SCI_SETPROPERTY, (WPARAM)"fold.compact", (LPARAM)"0");

	SendEditor(SCI_SETPROPERTY, (WPARAM)"fold.comment", (LPARAM)"1");
	SendEditor(SCI_SETPROPERTY, (WPARAM)"fold.preprocessor", (LPARAM)"1");

	SendEditor(SCI_SETPROPERTY, (WPARAM)"fold.at.else", (LPARAM)"0");
	SendEditor(SCI_SETPROPERTY, (WPARAM)"fold.comment.yaml", (LPARAM)"0");
	SendEditor(SCI_SETPROPERTY, (WPARAM)"fold.directive", (LPARAM)"0");
	SendEditor(SCI_SETPROPERTY, (WPARAM)"fold.html", (LPARAM)"1");
	SendEditor(SCI_SETPROPERTY, (WPARAM)"fold.preprocessor", (LPARAM)"1");
	SendEditor(SCI_SETPROPERTY, (WPARAM)"fold.verilog.flags", (LPARAM)"0");


	SendEditor(SCI_SETPROPERTY, (WPARAM)"fold.preprocessor", (LPARAM)"1");


	//set margin
	SendEditor(SCI_SETMARGINWIDTHN, MARGIN_SCRIPT_FOLD_INDEX, 0);
	SendEditor(SCI_SETMARGINTYPEN,  MARGIN_SCRIPT_FOLD_INDEX, SC_MARGIN_SYMBOL);
	SendEditor(SCI_SETMARGINMASKN, MARGIN_SCRIPT_FOLD_INDEX, SC_MASK_FOLDERS);
	SendEditor(SCI_SETMARGINWIDTHN, MARGIN_SCRIPT_FOLD_INDEX, 20);

	//Set some visual preferences
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_PLUS);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY);

	SendEditor(SCI_SETFOLDFLAGS, 16, 0); // 16  	Draw line below if not expanded

	//Tell scintilla we want to be notified about mouse clicks in the margin
	SendEditor(SCI_SETMARGINSENSITIVEN, MARGIN_SCRIPT_FOLD_INDEX, 1);

}
//------------------------------------------------------------------------------
void WxTextEditor::SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char *face) 
{
	SendEditor(SCI_STYLESETFORE, style, fore);
	SendEditor(SCI_STYLESETBACK, style, back);
	if (size >= 1)
	{
		SendEditor(SCI_STYLESETSIZE, style, size);
	}
	if (face) 
	{
		SendEditor(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(face));
	}
}
//------------------------------------------------------------------------------
void WxTextEditor::AutomaticIndentation(char ch) 
{
	int curLine = SendEditor(SCI_LINEFROMPOSITION,SendEditor(SCI_GETCURRENTPOS));
	int indentBlock = IndentOfBlock(curLine - 1);

	if ((ch == '\r' || ch == '\n'))
	{
		SetLineIndentation(curLine, indentBlock);
	}
}
//------------------------------------------------------------------------------
static bool iswordcharforsel(char ch) {
	return !strchr("\t\n\r !\"#$%&'()*+,-./:;<=>?@[\\]^`{|}~", ch);
}
int Minimum(int a, int b) {
	if (a < b)
		return a;
	else
		return b;
}
//bool WxTextEditor::StartAutoCompleteWord(bool onlyOneWord)
//{
//	std::string line = GetLine();
//	int current = GetCaretInLine();
//	//~ 	if (current >= line.size())
//	//~ 		return false;
//
//	int startword = current;
//	// Autocompletion of pure numbers is mostly an annoyance
//	bool allNumber = true;
//	while (startword > 0 &&  wordCharacters.find(line[startword - 1]) != std::string::npos)
//	{
//		startword--;
//		if (line[startword] < '0' || line[startword] > '9')
//		{
//			allNumber = false;
//		}
//	}
//	if (startword == current || allNumber)
//		return true;
//	std::string root = line.substr(startword, current - startword);
//	int doclen = SendEditor(SCI_GETLENGTH);
//	TextToFind ft = {{0, 0}, 0, {0, 0}};
//	ft.lpstrText = const_cast<char*>(root.c_str());
//	ft.chrg.cpMin = 0;
//	ft.chrgText.cpMin = 0;
//	ft.chrgText.cpMax = 0;
//	int flags = SCFIND_WORDSTART | (autoCompleteIgnoreCase ? 0 : SCFIND_MATCHCASE);
//	unsigned int posCurrentWord = SendEditor(SCI_GETCURRENTPOS) - root.length();
//	unsigned int minWordLength = 0;
//	unsigned int nwords = 0;
//
//	// wordsNear contains a list of words separated by single spaces and with a space
//	// at the start and end. This makes it easy to search for words.
//	std::string wordsNear;
//	wordsNear.reserve(1000);
//	wordsNear += " ";
//
//	for (;;) 
//	{	// search all the document
//		ft.chrg.cpMax = doclen;
//		int posFind = SendEditorString(SCI_FINDTEXT, flags, reinterpret_cast<char *>(&ft));
//		if (posFind == -1 || posFind >= doclen)
//		{
//			break;
//		}
//		if (posFind == posCurrentWord) 
//		{
//			ft.chrg.cpMin = posFind + root.length();
//			continue;
//		}
//		// Grab the word and put spaces around it
//		const unsigned int wordMaxSize = 800;
//		char wordstart[wordMaxSize];
//		wordstart[0] = ' ';
//		GetRange(posFind, Minimum(posFind + wordMaxSize - 3, doclen), wordstart + 1);
//		char *wordend = wordstart + 1 + root.length();
//		while (iswordcharforsel(*wordend))
//		{
//			wordend++;
//		}
//		*wordend++ = ' ';
//		*wordend = '\0';
//		unsigned int wordlen = wordend - wordstart - 2;
//		if (wordlen > root.length()) 
//		{
//			if (wordsNear.find(wordstart) == std::string::npos)
//			{	// add a new entry
//				wordsNear += wordstart + 1;
//				if (minWordLength < wordlen)
//					minWordLength = wordlen;
//
//				nwords++;
//				if (onlyOneWord && nwords > 1)
//				{
//					return true;
//				}
//			}
//		}
//		ft.chrg.cpMin = posFind + wordlen;
//	}
//	size_t length = wordsNear.length();
//	if ((length > 2) && (!onlyOneWord || (minWordLength > root.length()))) 
//	{
//		WordList wl;
//		wl.Set(wordsNear.c_str());
//		char *words = wl.GetNearestWords("", 0, autoCompleteIgnoreCase);
//		SendEditorString(SCI_AUTOCSHOW, root.length(), words);
//		delete []words;
//	}
//	else 
//	{
//		SendEditor(SCI_AUTOCCANCEL);
//	}
//	return true;
//}
//------------------------------------------------------------------------------
void WxTextEditor::FoldAll() 
{
	SendEditor(SCI_COLOURISE, 0, -1);
	int maxLine = SendEditor(SCI_GETLINECOUNT);
	bool expanding = true;
	for (int lineSeek = 0; lineSeek < maxLine; lineSeek++) 
	{
		if (SendEditor(SCI_GETFOLDLEVEL, lineSeek) & SC_FOLDLEVELHEADERFLAG)
		{
			expanding = !SendEditor(SCI_GETFOLDEXPANDED, lineSeek);
			break;
		}
	}
	for (int line = 0; line < maxLine; line++) 
	{
		int level = SendEditor(SCI_GETFOLDLEVEL, line);
		if ((level & SC_FOLDLEVELHEADERFLAG) && (SC_FOLDLEVELBASE == (level & SC_FOLDLEVELNUMBERMASK))) 
		{
			if (expanding) 
			{
				SendEditor(SCI_SETFOLDEXPANDED, line, 1);
				Expand(line, true, false, 0, level);
				line--;
			} 
			else
			{
				int lineMaxSubord = SendEditor(SCI_GETLASTCHILD, line, -1);
				SendEditor(SCI_SETFOLDEXPANDED, line, 0);
				if (lineMaxSubord > line)
				{
					SendEditor(SCI_HIDELINES, line + 1, lineMaxSubord);
				}
			}
		}
	}
}
//------------------------------------------------------------------------------
void WxTextEditor::EnsureAllChildrenVisible(int line, int level) 
{
	// Ensure all children visible
	SendEditor(SCI_SETFOLDEXPANDED, line, 1);
	Expand(line, true, true, 100, level);
}
//------------------------------------------------------------------------------
void WxTextEditor::ToggleFoldRecursive(int line, int level) 
{
	if (SendEditor(SCI_GETFOLDEXPANDED, line))
	{
		// Contract this line and all children
		SendEditor(SCI_SETFOLDEXPANDED, line, 0);
		Expand(line, false, true, 0, level);
	} 
	else
	{
		// Expand this line and all children
		SendEditor(SCI_SETFOLDEXPANDED, line, 1);
		Expand(line, true, true, 100, level);
	}
}
//------------------------------------------------------------------------------
void WxTextEditor::SetLineIndentation(int line, int indent) 
{
	if (indent < 0)
		return;
	CharacterRange crange = GetSelection();
	int posBefore = SendEditor(SCI_GETLINEINDENTPOSITION, line);
	SendEditor(SCI_SETLINEINDENTATION, line, indent);
	int posAfter = SendEditor(SCI_GETLINEINDENTPOSITION, line);
	int posDifference = posAfter - posBefore;
	if (posAfter > posBefore) 
	{
		// Move selection on
		if (crange.cpMin >= posBefore)
		{
			crange.cpMin += posDifference;
		}
		if (crange.cpMax >= posBefore) 
		{
			crange.cpMax += posDifference;
		}
	} 
	else if (posAfter < posBefore) 
	{
		// Move selection back
		if (crange.cpMin >= posAfter) 
		{
			if (crange.cpMin >= posBefore)
				crange.cpMin += posDifference;
			else
				crange.cpMin = posAfter;
		}
		if (crange.cpMax >= posAfter) 
		{
			if (crange.cpMax >= posBefore)
				crange.cpMax += posDifference;
			else
				crange.cpMax = posAfter;
		}
	}
	SendEditor(SCI_SETSEL, crange.cpMin, crange.cpMax);
}
//------------------------------------------------------------------------------
std::string WxTextEditor::GetLine(int line)
{
	int len;
	// Get needed buffer size
	if (line < 0) {
		len = SendEditor(SCI_GETCURLINE, 0, 0);
	} 
	else
	{
		len = SendEditor(SCI_GETLINE, line, 0);
	}
	// Allocate buffer
	char* buf = new char[len+1];
	buf[len] = '\0';

	// And get the line
	if (line < 0) 
	{
		SendEditor(SCI_GETCURLINE, len, LPARAM(buf));
	} else 
	{
		SendEditor(SCI_GETLINE, line, LPARAM(buf));
	}
	std::string text(buf);
	delete[] buf;
	return text;
}
//------------------------------------------------------------------------------
int WxTextEditor::GetCaretInLine() 
{
	int caret = SendEditor(SCI_GETCURRENTPOS);
	int line = SendEditor(SCI_LINEFROMPOSITION, caret);
	int lineStart = SendEditor(SCI_POSITIONFROMLINE, line);
	return caret - lineStart;
}
//------------------------------------------------------------------------------
void WxTextEditor::GetRange(int start, int end, char *text)
{
	Sci_TextRange tr;
	tr.chrg.cpMin = start;
	tr.chrg.cpMax = end;
	tr.lpstrText = text;
	SendEditor( SCI_GETTEXTRANGE, 0, reinterpret_cast<sptr_t>(&tr));
}
//------------------------------------------------------------------------------
void WxTextEditor::Expand(int &line, bool doExpand, bool force, int visLevels, int level) 
{
	int lineMaxSubord = SendEditor(SCI_GETLASTCHILD, line, level & SC_FOLDLEVELNUMBERMASK);
	line++;
	while (line <= lineMaxSubord)
	{
		if (force) 
		{
			if (visLevels > 0)
				SendEditor(SCI_SHOWLINES, line, line);
			else
				SendEditor(SCI_HIDELINES, line, line);
		} 
		else 
		{
			if (doExpand)
				SendEditor(SCI_SHOWLINES, line, line);
		}
		int levelLine = level;
		if (levelLine == -1)
			levelLine = SendEditor(SCI_GETFOLDLEVEL, line);
		if (levelLine & SC_FOLDLEVELHEADERFLAG)
		{
			if (force) 
			{
				if (visLevels > 1)
					SendEditor(SCI_SETFOLDEXPANDED, line, 1);
				else
					SendEditor(SCI_SETFOLDEXPANDED, line, 0);
				Expand(line, doExpand, force, visLevels - 1);
			} 
			else
			{
				if (doExpand) 
				{
					if (!SendEditor(SCI_GETFOLDEXPANDED, line))
						SendEditor(SCI_SETFOLDEXPANDED, line, 1);
					Expand(line, true, force, visLevels - 1);
				} else 
				{
					Expand(line, false, force, visLevels - 1);
				}
			}
		} 
		else 
		{
			line++;
		}
	}
}
//------------------------------------------------------------------------------
CharacterRange WxTextEditor::GetSelection()
{
	CharacterRange crange;
	crange.cpMin = SendEditor(SCI_GETSELECTIONSTART);
	crange.cpMax = SendEditor(SCI_GETSELECTIONEND);
	return crange;
}
//------------------------------------------------------------------------------
int WxTextEditor::IndentOfBlock(int line) 
{
	if (line < 0)
		return 0;
	int indentBlock = SendEditor(SCI_GETLINEINDENTATION, line);
	return indentBlock;
}

