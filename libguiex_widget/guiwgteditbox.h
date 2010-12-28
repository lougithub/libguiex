/** 
* @file guiwgteditbox.h
* @brief editbox used in this system
* @author ken
* @date 2006-08-07
*/

#ifndef __GUI_WGTEDITBOX_20060807_H__
#define __GUI_WGTEDITBOX_20060807_H__


//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetgenerator.h>
#include "guiwgtedit.h"
#include <vector>


//============================================================================//
// class
//============================================================================// 

namespace guiex
{
	/**
	* @class CGUIWgtEditBox
	* @brief edit box, used to edit test
	* used image name:
	*		-EDIT_BGFOCUS
	*		-EDIT_BG
	*		-EDIT_CURSOR
	*/
	class GUIEXPORT CGUIWgtEditBox : public CGUIWidget
	{
	public:
		CGUIWgtEditBox( const CGUIString& rName, const CGUIString& rSceneName );
		virtual ~CGUIWgtEditBox( );

		virtual void SetTextContent(const CGUIStringW& rText);
		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty );

		void SetCursorIndex(int32 nPos);

		void SetTextMasked(bool bMask);
		bool IsTextMasked() const;

		void SetMaskCode(wchar_t wMaskCode);
		wchar_t GetTextMasked() const;

		void SetReadOnly(bool bRead);
		bool IsReadOnly() const;


		void SetStringAreaRatio(const CGUIRect& rStringAreaRatio);

		const CGUIRect& GetStringAreaRatio( ) const;

		void SetSelectedTextColor( const CGUIColor& rColor);

		void SetCursorSize( const CGUISize& rSize );
		const CGUISize& GetCursorSize() const;

		void SetMaxTextNum( uint32 num);
		uint32 GetMaxTextNum( ) const;

	protected:
		CGUIWgtEditBox( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitEditbox();

		virtual void RefreshSelf( );
		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void UpdateSelf( real fDeltaTime );
		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );


	protected://string related function
		/// get position of cursor 
		CGUIVector2	GetCursorPos();

		/// get rect of cursor necessary for rendering
		CGUIRect GetCursorRect();

		/// get text index from given global pos
		uint32 GetTextIndexFromPos( const CGUIVector2& rPos);

		/// clear selection-area
		void ClearSelection();

		///return the length of the current selection .
		uint32 GetSelectionLength(void) const;

		/// erase selected string
		void EraseSelectedText( );

		/**
		 * @brief Define the current selection for the Editbox
		*/
		void SetSelection(size_t start_pos, size_t end_pos);


		/// insert a string
		void InsertString( const CGUIStringW& rText );

		/// delete string
		void DeleteString( int32 nBeginPos, int32 nEndPos);

		/// update string position
		void UpdateStringPos();

		/// get string width
		real GetStringWidth(int32 nBeginPos, int32 nEndPos) const;




	protected:	//callback function
		virtual	uint32 OnGetFocus( CGUIEventNotification* pEvent );
		virtual	uint32 OnLostFocus( CGUIEventNotification* pEvent );
		virtual	uint32 OnKeyPressed( CGUIEventKeyboard* pEvent );
		virtual	uint32 OnMouseLeftDown( CGUIEventMouse* pEvent );
		virtual	uint32 OnMouseLeftUp( CGUIEventMouse* pEvent );
		virtual	uint32 OnMouseMove( CGUIEventMouse* pEvent );


		//keyboard event
		void OnKeyPressed_Left(CGUIEventKeyboard* pEvent);
		void OnKeyPressed_Right(CGUIEventKeyboard* pEvent);
		void OnKeyPressed_Delete(CGUIEventKeyboard* pEvent);
		void OnKeyPressed_Back(CGUIEventKeyboard* pEvent);
		void OnKeyPressed_Home(CGUIEventKeyboard* pEvent);
		void OnKeyPressed_End(CGUIEventKeyboard* pEvent);

	protected:
		//---------------------------------------------------
		//string
		uint32 m_nMaxString; //!< max number of string
		uint32 m_nCursorIdx; //!< cursor's position in edited string, the first is 0.
		real m_fTextWidthRel; //!< start position of text, it's a relative position
		std::vector<uint32>	m_vecStringWidth; //!< width of each string.
		bool m_bReadOnly; //!< is text readonly

		//for mask
		bool m_bMaskText; //!< True if the editbox text should be rendered masked.
		wchar_t m_wMaskCodePoint; //!< Code point to use when rendering masked text.
		CGUIStringEx m_strMaskText; //!< mask text

		//---------------------------------------------------
		//select state
		uint32 m_nSelectionStart; //!< Start of selection area.
		uint32 m_nSelectionEnd; //!< End of selection area.
		bool m_bDraging; //!< true when a selection is being dragged.
		uint32 m_nDragAnchorIdx; //!< Selection index for drag selection anchor point.
		CGUIColor m_aSelectedTextColor; //!< selected text color

		//---------------------------------------------------
		//image
		const CGUIImage* m_pBG; //!< bg image
		const CGUIImage* m_pBGFocus; //!< bg image rendered when this widget is focusable
		const CGUIImage* m_pCursor; //!< cursor 

		//---------------------------------------------------
		CGUIRect m_aStringAreaRatio; //!< the ratio of string area, the (0,0,1,1) equal whole client area

		CGUIRect m_aStringAreaRect;

	protected:
		CGUIWgtEdit* m_pEdit; //!< edit control

	protected:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtEditBox);
	};


}//namespace guiex

#endif //__GUI_WGTEDITBOX_20060807_H__

