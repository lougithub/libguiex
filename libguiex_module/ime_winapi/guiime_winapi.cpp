/** 
* @file guiime_winapi.cpp
* @brief use windows ime
* @author ken
* @date 2006-08-25
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module\ime_winapi\guiime_winapi.h>
#include <libguiex_core/guiex.h>


//============================================================================//
// define
//============================================================================// 
namespace guiex
{
#define WINIME_SHOW_MSG(eventname)	\
	GUI_TRACE( eventname##"\n");
}


//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	GUI_INTERFACE_IMPLEMENT(IGUIIme_winapi);
	//------------------------------------------------------------------------------
	const char* IGUIIme_winapi::StaticGetModuleName()
	{
		return "IGUIIme_winapi";
	}
	//------------------------------------------------------------------------------
	IGUIIme_winapi::IGUIIme_winapi()
		:IGUIInterfaceIme( StaticGetModuleName() )
		,m_hWnd(NULL)
		,m_pIMC(NULL)
		,m_bIsOpen(false)
	{
	}
	//------------------------------------------------------------------------------
	IGUIIme_winapi::~IGUIIme_winapi()
	{
	}
	//------------------------------------------------------------------------------
	int IGUIIme_winapi::DoInitialize(void* pData )
	{
		m_hWnd = HWND(pData);
		m_pIMC = ::ImmCreateContext();
		if (NULL == m_pIMC)
		{
			return -1;
		}
		::ImmAssociateContext(m_hWnd, m_pIMC);
		return 0;
	}
	//------------------------------------------------------------------------------
	void IGUIIme_winapi::DoDestroy()
	{
		if( m_pIMC )
		{
			::ImmDestroyContext( m_pIMC);
			m_pIMC = NULL;
		}
	}		
	//------------------------------------------------------------------------------
	void IGUIIme_winapi::DeleteSelf()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	void IGUIIme_winapi::OpenIme()
	{
		HKL hKL = GetKeyboardLayout( 0 );
		if(ImmIsIME(hKL) == TRUE)
		{
			HIMC hIMC = ImmGetContext( m_hWnd );
			if(hIMC)
			{
				ImmSetOpenStatus(hIMC, TRUE);
				//ImmSetConversionStatus(hIMC, dwConvMode & ~IME_CMODE_NATIVE, dwSentMode & ~0);
				ImmReleaseContext( m_hWnd, hIMC );
			}
		}

		ClearResultString();

		m_bIsOpen = true;
	}
	//------------------------------------------------------------------------------
	void IGUIIme_winapi::CloseIme()
	{
		HKL hKL = GetKeyboardLayout( 0 );
		if(ImmIsIME(hKL) == TRUE)
		{
			HIMC hIMC = ImmGetContext( m_hWnd );
			if(hIMC)
			{
				ImmSetOpenStatus(hIMC, FALSE);
				//ImmSetConversionStatus(hIMC, dwConvMode & ~IME_CMODE_NATIVE, dwSentMode & ~0);
				ImmReleaseContext( m_hWnd, hIMC );
			}
		}

		ClearResultString();

		m_bIsOpen = false;
	}
	//------------------------------------------------------------------------------
	bool IGUIIme_winapi::IsOpen()
	{
		return m_bIsOpen;
	}
	//------------------------------------------------------------------------------
	const wchar* IGUIIme_winapi::GetResultString() const
	{
		if( m_strResult.empty() )
		{
			return NULL;
		}
		else
		{
			return m_strResult.c_str();
		}
	}
	//------------------------------------------------------------------------------
	void IGUIIme_winapi::AddResultString( const wchar* pString )
	{
		m_strResult += pString;
	}
	//------------------------------------------------------------------------------
	void IGUIIme_winapi::ClearResultString()
	{
		m_strResult.clear();
	}
	//------------------------------------------------------------------------------
	void IGUIIme_winapi::SetCursorPos( const CGUIVector2& rPos )
	{
		m_aPos = rPos;
	}
	//------------------------------------------------------------------------------
	const CGUIVector2&  IGUIIme_winapi::GetCursorPos() const
	{
		return m_aPos;
	}
	//------------------------------------------------------------------------------
	bool IGUIIme_winapi::ProcessWindowMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if( IsOpen() == false )
		{
			return false;
		}

		switch(message)
		{
			//IME Message
		case WM_CHAR:
			return On_WM_CHAR( wParam );

			//	case WM_IME_CHAR:
			//		return On_WM_CHAR( wParam );

		case WM_IME_SETCONTEXT:
			return   OnWM_IME_SETCONTEXT();

		case WM_INPUTLANGCHANGEREQUEST:
			return  OnWM_INPUTLANGCHANGEREQUEST();

		case WM_INPUTLANGCHANGE:
			return OnWM_INPUTLANGCHANGE();

		case WM_IME_STARTCOMPOSITION:
			return OnWM_IME_STARTCOMPOSITION();

		case WM_IME_ENDCOMPOSITION:
			return  OnWM_IME_ENDCOMPOSITION();

		case WM_IME_NOTIFY:
			return OnWM_IME_NOTIFY(message,wParam, lParam );

		case WM_IME_COMPOSITION:
			return OnWM_IME_COMPOSITION( message, wParam, lParam);
		}

		return false;
	}
	//------------------------------------------------------------------------------
	bool IGUIIme_winapi::OnWM_IME_SETCONTEXT()
	{
		/*
		The WM_IME_SETCONTEXT message is sent to an application when a window 
		of the application is activated. If the application has created an IME
		window, it should call the ImmIsUIMessage function. Otherwise, it should 
		pass this message to the DefWindowProc function.
		*/

		WINIME_SHOW_MSG("OnWM_IME_SETCONTEXT");
		return false;
	}
	//------------------------------------------------------------------------------
	bool IGUIIme_winapi::OnWM_INPUTLANGCHANGEREQUEST()
	{
		/*
		The WM_INPUTLANGCHANGEREQUEST message is posted to the window 
		with the focus when the user chooses a new input language, either
		with the hotkey (specified in the Keyboard control panel application) 
		or from the indicator on the system taskbar. An application can accept 
		the change by passing the message to the DefWindowProc function or reject 
		the change (and prevent it from taking place) by returning immediately. 
		*/

		if( !IsOpen() )
		{
			//ignore it if the ime is closed
			return true;
		}

		WINIME_SHOW_MSG("OnWM_INPUTLANGCHANGEREQUEST");
		return false;
	}
	//------------------------------------------------------------------------------
	bool IGUIIme_winapi::OnWM_INPUTLANGCHANGE()
	{
		/*
		The WM_INPUTLANGCHANGE message is sent to the topmost affected window 
		after an application's input language has been changed. You should make 
		any application-specific settings and pass the message to the DefWindowProc
		function, which passes the message to all first-level child windows. These 
		child windows can pass the message to DefWindowProc to have it pass the message 
		to their child windows, and so on. 
		*/

		if( !IsOpen() )
		{
			//ignore it if the ime is closed
			return false;
		}

		WINIME_SHOW_MSG("OnWM_INPUTLANGCHANGE");
		return false;
	}
	//------------------------------------------------------------------------------
	bool IGUIIme_winapi::OnWM_IME_STARTCOMPOSITION()
	{
		/*
		The WM_IME_STARTCOMPOSITION message is sent immediately before the IME generates
		the composition string as a result of a keystroke. The message is a notification 
		to an IME window to open its composition window. An application should process 
		this message if it displays composition characters itself.
		*/
		WINIME_SHOW_MSG("OnWM_IME_STARTCOMPOSITION");


		HIMC hIMC = ImmGetContext(m_hWnd);
		if( hIMC )
		{
			//set composition window's position
			COMPOSITIONFORM Composition;
			Composition.dwStyle = CFS_POINT;
			Composition.ptCurrentPos.x = LONG(GetCursorPos().x);
			Composition.ptCurrentPos.y = LONG(GetCursorPos().y);
			ImmSetCompositionWindow(hIMC,&Composition);

			//set composition window's font
			LOGFONT logfont;
			ImmGetCompositionFont( hIMC, &logfont );
			logfont.lfHeight = 0-static_cast<int>(20);
			ImmSetCompositionFont( hIMC, &logfont );

			ImmReleaseContext(m_hWnd,hIMC);
		}

		//make key be take over
		//SetTakeOverKey(true);

		return false;
	}
	//------------------------------------------------------------------------------
	bool IGUIIme_winapi::OnWM_IME_ENDCOMPOSITION()
	{
		/*
		The WM_IME_ENDCOMPOSITION message is sent to an application when the IME ends 
		composition. An application should process this message if it displays composition 
		characters itself.
		*/
		WINIME_SHOW_MSG("OnWM_IME_ENDCOMPOSITION");

		//make key not be take over
		//SetTakeOverKey(false);

		return false;
	}
	//------------------------------------------------------------------------------
	bool IGUIIme_winapi::OnWM_IME_NOTIFY(UINT message, WPARAM wParam, LPARAM lParam)
	{
		/*
		The WM_IME_NOTIFY message is sent to an application to notify it of changes to 
		the IME window. An application processes this message if it is responsible for
		managing the IME window.
		*/

		WINIME_SHOW_MSG("OnWM_IME_NOTIFY");

		switch(wParam)
		{
		case IMN_CHANGECANDIDATE:	
			/*
			The IMN_CHANGECANDIDATE command is sent to the application when an IME 
			is about to change the content of the candidate window.An application 
			should process this command if it displays candidates itself. 
			*/
			WINIME_SHOW_MSG("\tIMN_CHANGECANDIDATE");
			break;

		case IMN_CLOSECANDIDATE:
			/*
			The IMN_CLOSECANDIDATE command is sent to the application when an IME is 
			about to close the candidate window. An application should process this 
			command if it displays candidates itself.
			*/
			WINIME_SHOW_MSG("\tIMN_CLOSECANDIDATE");
			break;

		case IMN_CLOSESTATUSWINDOW:
			/*
			The IMN_CLOSESTATUSWINDOW command is sent to the application when an IME 
			is about to close the status window. An application should process this 
			command if it displays the status window for the IME. 
			*/
			WINIME_SHOW_MSG("\tIMN_CLOSESTATUSWINDOW");
			break;

		case IMN_GUIDELINE:
			/*
			The IMN_GUIDELINE command is sent when an IME is about to show an error message
			or other information. An application processes this command by calling the 
			ImmGetGuideLine function to retrieve the error message or information from the IME. 
			*/
			WINIME_SHOW_MSG("\tIMN_GUIDELINE");
			break;

		case IMN_OPENCANDIDATE:
			/*
			The IMN_OPENCANDIDATE command is sent to the application when an IME is about to 
			open the candidate window. An application should process this command if it displays 
			candidates itself. The application can retrieve a list of candidates to display by
			using the ImmGetCandidateList function. 
			*/
			WINIME_SHOW_MSG("\tIMN_OPENCANDIDATE");
			break;

		case IMN_OPENSTATUSWINDOW:
			/*
			The IMN_OPENSTATUSWINDOW command is sent when an IME is about to create the status window.
			An application processes this command to display the status window for the IME by itself. 
			*/
			WINIME_SHOW_MSG("\tIMN_OPENSTATUSWINDOW");
			break;

		case IMN_SETCANDIDATEPOS:
			/*
			The IMN_SETCANDIDATEPOS command is sent when candidate processing has finished and the IME 
			is about to move the candidate window. An application should process this command if it 
			displays the candidate window itself. 
			*/
			WINIME_SHOW_MSG("\tIMN_SETCANDIDATEPOS");
			break;

		case IMN_SETCOMPOSITIONFONT:
			/*
			The IMN_SETCOMPOSITIONFONT command is sent when the font of the input context is updated. 
			*/
			WINIME_SHOW_MSG("\tIMN_SETCOMPOSITIONFONT");
			break;

		case IMN_SETCOMPOSITIONWINDOW:
			/*
			The IMN_SETCOMPOSITIONWINDOW command is sent when the style or position of the composition 
			window is updated. 
			*/
			WINIME_SHOW_MSG("\tIMN_SETCOMPOSITIONWINDOW");
			break;

		case IMN_SETCONVERSIONMODE:
			/*
			The IMN_SETCONVERSIONMODE command is sent when the conversion mode of the input context is updated. 
			*/
			WINIME_SHOW_MSG("\tIMN_SETCONVERSIONMODE");
			break;

		case IMN_SETOPENSTATUS:
			/*
			The IMN_SETOPENSTATUS command is sent when the open status of the input context is updated. 
			*/
			WINIME_SHOW_MSG("\tIMN_SETOPENSTATUS");
			break;

		case IMN_SETSENTENCEMODE:
			/*
			The IMN_SETSENTENCEMODE command is sent when the sentence mode of the input context is updated. 
			*/
			WINIME_SHOW_MSG("\tIMN_SETSENTENCEMODE");
			break;

		case IMN_SETSTATUSWINDOWPOS :
			/*
			The IMN_SETSTATUSWINDOWPOS command is sent when the status window position in the input context is updated.
			*/
			WINIME_SHOW_MSG("\tIMN_SETSTATUSWINDOWPOS");
			break;

		default:
			WINIME_SHOW_MSG("\tUNKNOW NOTIFY");
			break;
		}

		return false;
	}
	//------------------------------------------------------------------------------
	bool IGUIIme_winapi::OnWM_IME_COMPOSITION(UINT message, WPARAM wParam, LPARAM lParam)
	{
		/*
		The WM_IME_COMPOSITION message is sent to an application when the IME changes 
		composition status as a result of a keystroke. An application should process 
		this message if it displays composition characters itself. Otherwise, it should
		send the message to the IME window.
		*/

		WINIME_SHOW_MSG("OnWM_IME_COMPOSITION");


		bool bRet = false;

		if( lParam & GCS_COMPATTR)
		{
			/*
			Retrieves or updates the attribute of the composition string.
			*/
			WINIME_SHOW_MSG("\tGCS_COMPATTR");
		}

		if( lParam & GCS_COMPCLAUSE)
		{
			/*
			Retrieves or updates clause information of the composition string.
			*/
			WINIME_SHOW_MSG("\tGCS_COMPCLAUSE");
		}

		if( lParam & GCS_COMPREADSTR)
		{
			/*
			Retrieves or updates the attributes of the reading string of the current composition.
			*/
			WINIME_SHOW_MSG("\tGCS_COMPREADSTR");
		}

		if( lParam & GCS_COMPREADATTR)
		{
			/*
			Retrieves or updates the attributes of the reading string of the current composition.
			*/
			WINIME_SHOW_MSG("\tGCS_COMPREADATTR");
		}

		if( lParam & GCS_COMPREADCLAUSE)
		{
			/*
			Retrieves or updates the clause information of the reading string of the composition string.
			*/
			WINIME_SHOW_MSG("\tGCS_COMPREADCLAUSE");
		}

		if( lParam & GCS_COMPSTR)
		{
			/*
			Retrieves or updates the reading string of the current composition. 
			*/
			WINIME_SHOW_MSG("\tGCS_COMPREADCLAUSE");
		}

		if( lParam & GCS_CURSORPOS)
		{
			/*
			Retrieves or updates the cursor position in composition string.
			*/
			WINIME_SHOW_MSG("\tGCS_CURSORPOS");
		}

		if( lParam & GCS_DELTASTART)
		{
			/*
			Retrieves or updates the starting position of any changes in composition string.
			*/
			WINIME_SHOW_MSG("\tGCS_DELTASTART");
		}

		if( lParam & GCS_RESULTCLAUSE)
		{
			/*
			Retrieves or updates clause information of the result string. 
			*/
			WINIME_SHOW_MSG("\tGCS_RESULTCLAUSE");
		}

		if( lParam & GCS_RESULTREADCLAUSE)
		{
			/*
			Retrieves or updates clause information of the reading string. 
			*/
			WINIME_SHOW_MSG("\tGCS_RESULTREADCLAUSE");
		}

		if( lParam & GCS_RESULTREADSTR)
		{
			/*
			Retrieves or updates the reading string. 
			*/
			WINIME_SHOW_MSG("\tGCS_RESULTREADSTR");
		}

		if( lParam & GCS_RESULTSTR )
		{
			/*
			Retrieves or updates the string of the composition result. 
			*/
			WINIME_SHOW_MSG("\tGCS_RESULTSTR");

			HIMC hIMC = ImmGetContext( m_hWnd );
			if( !hIMC )
			{
				return false;
			}

			// Get the size of the result string.
			DWORD dwSize = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, NULL, 0);
			// Increase buffer size for NULL terminator; 
			dwSize += sizeof(wchar);
			wchar* lpStr = new wchar[dwSize];
			lpStr[(dwSize/2)-1] = 0;

			// Get the result string that is generated by IME into lpstr.
			ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, lpStr, dwSize);
			ImmReleaseContext(m_hWnd, hIMC);

			AddResultString( lpStr );

			//free buffer
			delete[] lpStr;

			bRet = true;
		}

		return bRet;
	}
	//------------------------------------------------------------------------------
	bool IGUIIme_winapi::On_WM_CHAR( WPARAM wParam )
	{
		if( !IsOpen() )
		{
			//ignore it if the ime is closed
			return true;
		}

		wchar buf[2] = {0,0};
		buf[0] = (unsigned short)((unsigned int)(wParam) & 0xffff);
		if(buf[0]>=32 && buf[0] <=127)
		{
			AddResultString(buf);
		}

		return true;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
