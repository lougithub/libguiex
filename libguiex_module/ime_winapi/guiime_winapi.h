/** 
* @file guiime_winapi.h
* @brief use windows ime
* @author ken
* @date 2006-08-25
*/

#ifndef __GUI_IME_WINAPI_20060825_H__
#define __GUI_IME_WINAPI_20060825_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiinterfaceime.h>
#include <windows.h>
#include <string>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class IGUIIme_winapi
	* @brief use winapi to implement ime.
	* the argument of Initialize() function is HWND.
	*/
	class GUIEXPORT IGUIIme_winapi : public IGUIInterfaceIme
	{
	public:
		IGUIIme_winapi();
		virtual ~IGUIIme_winapi();

		virtual void DeleteSelf();


	public:
		///open ime, that's means the ime could receive the input
		virtual void OpenIme();

		///close ime, that's means the ime won't receive any input
		virtual void CloseIme();

		///get Result string
		virtual const wchar_t* GetResultString() const;

		/// add result string
		void AddResultString( const wchar_t* pString );

		///clear Result string
		virtual void ClearResultString();

		///set cursor's position, the position is window's position
		virtual void SetCursorPos( const CGUIVector2& rPos );

		///get cursor's position
		virtual const CGUIVector2& GetCursorPos() const;

		/**
		* @brief process ime's message
		* @return 
		*	-true for the ime will process this message, the window needn't process it
		*	-false for make windows process it by DefWindowProc(hWnd, message, wParam, lParam)
		*/
		bool ProcessWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	protected:
		virtual int DoInitialize(void* pData);
		virtual void DoDestroy();

		/**
		* @brief is this ime opened;
		*/
		bool IsOpen();


		bool OnWM_IME_SETCONTEXT();
		bool OnWM_INPUTLANGCHANGEREQUEST();
		bool OnWM_INPUTLANGCHANGE();
		bool OnWM_IME_STARTCOMPOSITION();
		bool OnWM_IME_ENDCOMPOSITION();
		bool OnWM_IME_NOTIFY(UINT message, WPARAM wParam, LPARAM lParam);
		bool OnWM_IME_COMPOSITION(UINT message, WPARAM wParam, LPARAM lParam);
		bool On_WM_CHAR( WPARAM wParam );	

	protected:
		CGUIVector2 m_aPos; /// position of cursor
		std::wstring m_strResult; /// result string of ime

		HWND m_hWnd; /// handle of the windows
		bool m_bIsOpen;	/// flag, whether ime is opened

	public: 
		static const char* StaticGetModuleName();
	};



	GUI_INTERFACE_DECLARE();

}//namespace guiex

#endif //__GUI_IME_WINAPI_20060825_H__

