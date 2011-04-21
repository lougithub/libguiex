/** 
* @file guiwgtmultieditbox.h
* @brief multi lineeditbox used in this system
* @author ken
* @date 2007-06-27
*/

#ifndef __GUI_WGTMULTIEDITBOX_20070627_H__
#define __GUI_WGTMULTIEDITBOX_20070627_H__


//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetgenerator.h>
#include "guiwgtscrollbarcontainer.h"
#include "guiwgtedit.h"
#include <vector>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtMultiEditBox
	* @brief edit box, used to edit test
	* used image name:
	*		-EDIT_BGFOCUS
	*		-EDIT_BG
	*		-EDIT_CURSOR
	*/
	class GUIEXPORT CGUIWgtMultiEditBox : public CGUIWgtScrollbarContainer
	{
	public:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtMultiEditBox( const CGUIString& rName, const CGUIString& rSceneName );

		/** 
		 * @brief destructor
		 */
		virtual ~CGUIWgtMultiEditBox( );

		void SetTextContent(const CGUIStringW& rText);
		const CGUIStringW& GetTextContent( ) const;

		void SetTextInfo(const CGUIStringRenderInfo& rInfo);
		const CGUIStringRenderInfo& GetTextInfo( ) const;

		void SetTextContentUTF8( const CGUIString& rString);
		CGUIString GetTextContentUTF8( ) const;

		void SetTextAlignmentVert( ETextAlignmentVert eAlignment );
		void SetTextAlignmentHorz( ETextAlignmentHorz eAlignment );
		ETextAlignmentHorz GetTextAlignmentHorz( ) const;
		ETextAlignmentVert GetTextAlignmentVert( ) const;

		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty);

		void SetReadOnly(bool bRead);
		bool IsReadOnly() const;

		void SetStringAreaRatio(const CGUIRect& rStringAreaRatio);
		void SetSelectedTextColor( const CGUIColor& rColor);

		const CGUISize& GetCursorSize() const;
		void SetCursorSize( const CGUISize& rSize );

	protected:
		CGUIWgtMultiEditBox( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void UpdateSelf( real fDeltaTime );
		virtual void RefreshSelf();
		virtual void UpdateClientArea(void);

		void InitMultiEditbox();

		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );

	protected://string related function
		CGUIVector2 GetCursorPos();
		CGUIRect GetCursorRect();
		void ClearSelection();
		uint32 GetSelectionLength(void) const;
		void EraseSelectedText( );
		void SetSelection(size_t start_pos, size_t end_pos);
		uint32 SetCursorIndexByPos( const CGUIVector2& rPos);

		uint32 GetLineNumberFromIndex(uint32 index) const;

		void InsertString( const CGUIStringW& rText );
		void DeleteString( int32 nBeginPos, int32 nEndPos);
		real GetStringWidth(int32 nBeginPos, int32 nEndPos) const;
		void SetCursorIndex( int32 nIdx, int32 nForceLineIdx = -1 );

		void FormatText_Imp();
		void FormatText();

	protected: //callback function
		virtual uint32 OnGetFocus( CGUIEventNotification* pEvent );
		virtual uint32 OnLostFocus( CGUIEventNotification* pEvent );
		virtual uint32 OnKeyPressed( CGUIEventKeyboard* pEvent );
		virtual uint32 OnMouseLeftDown( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseLeftUp( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseMove( CGUIEventMouse* pEvent );

		//keyboard event
		void OnKeyPressed_Left(CGUIEventKeyboard* pEvent);
		void OnKeyPressed_Right(CGUIEventKeyboard* pEvent);
		void OnKeyPressed_Up(CGUIEventKeyboard* pEvent);
		void OnKeyPressed_Down(CGUIEventKeyboard* pEvent);
		void OnKeyPressed_Delete(CGUIEventKeyboard* pEvent);
		void OnKeyPressed_Back(CGUIEventKeyboard* pEvent);
		void OnKeyPressed_Home(CGUIEventKeyboard* pEvent);
		void OnKeyPressed_End(CGUIEventKeyboard* pEvent);
		void OnKeyPressed_Enter(CGUIEventKeyboard* pEvent);

	protected:
		//---------------------------------------------------
		//string
		uint32 m_nMaxString; //!< max number of string
		int32 m_nCursorIdx; //!< cursor's position in edited string, the first is 0.
		int32 m_nCursorLine; //!< the line where cursor is
		std::vector<CGUISize> m_vecStringSize;//!< size of each string.
		bool m_bReadOnly; //!< is text readonly

		//---------------------------------------------------
		//select state
		uint32 m_nSelectionStart; //!< Start of selection area.
		uint32 m_nSelectionEnd; //!< End of selection area.
		bool m_bDraging; //!< true when a selection is being dragged.
		uint32 m_nDragAnchorIdx; //!< Selection index for drag selection anchor point.
		CGUIColor m_aSelectedTextColor; //!< selected text color

		//---------------------------------------------------
		//image
		CGUIImage* m_pBG; //!< bg image
		CGUIImage* m_pBGFocus; //!< bg image rendered when this widget is focusable
		CGUIImage* m_pCursor; //!< cursor 

		CGUIRect m_aStringAreaRatio; //!< the ratio of string area, the (0,0,1,1) equal whole client area

		//text
		CGUIStringRender m_strText;
		ETextAlignmentHorz m_eTextAlignmentHorz;
		ETextAlignmentVert m_eTextAlignmentVert;

		//line break
		/**
		* @brief struct used to store information about a formatted line within the paragraph.
		*/
		struct SLineInfo
		{
			uint32 m_nStartIdx;     //!< Starting index for this line.
			uint32 m_nLength;       //!< Code point length of this line.
			real m_fLineHeight;   //!< line height
		};
		typedef std::vector<SLineInfo>   TLineList;  //!< Type for collection of LineInfos.
		TLineList m_aLineList; //!< line list
		static wchar_t ms_wLineBreak; //!< Holds what we consider to be line break characters.

	protected:
		CGUIWgtEdit* m_pEdit; //!< edit control

	protected:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtMultiEditBox);
	};

}//namespace guiex

#endif //__GUI_WGTMULTIEDITBOX_20070627_H__

