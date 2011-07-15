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
	*		-bg_focus
	*		-bg
	*		-cursor
	*
	* scrollbar_v_background
	* scrollbar_v_arrow_0_normal
	* scrollbar_v_arrow_0_hover
	* scrollbar_v_arrow_0_disable
	* scrollbar_v_arrow_0_push
	* scrollbar_v_arrow_0_hover_overlay
	* scrollbar_v_arrow_0_push_overlay
	* scrollbar_v_arrow_1_normal
	* scrollbar_v_arrow_1_hover
	* scrollbar_v_arrow_1_disable
	* scrollbar_v_arrow_1_push
	* scrollbar_v_arrow_1_hover_overlay
	* scrollbar_v_arrow_1_push_overlay
	* scrollbar_v_slide_normal
	* scrollbar_v_slide_hover
	* scrollbar_v_slide_disable
	* scrollbar_v_slide_push
	* scrollbar_v_slide_hover_overlay
	* scrollbar_v_slide_push_overlay
	*
	* scrollbar_h_background
	* scrollbar_h_arrow_0_normal
	* scrollbar_h_arrow_0_hover
	* scrollbar_h_arrow_0_disable
	* scrollbar_h_arrow_0_push
	* scrollbar_h_arrow_0_hover_overlay
	* scrollbar_h_arrow_0_push_overlay
	* scrollbar_h_arrow_1_normal
	* scrollbar_h_arrow_1_hover
	* scrollbar_h_arrow_1_disable
	* scrollbar_h_arrow_1_push
	* scrollbar_h_arrow_1_hover_overlay
	* scrollbar_h_arrow_1_push_overlay
	* scrollbar_h_slide_normal
	* scrollbar_h_slide_hover
	* scrollbar_h_slide_disable
	* scrollbar_h_slide_push
	* scrollbar_h_slide_hover_overlay
	* scrollbar_h_slide_push_overlay
	*/
	class GUIEXPORT CGUIWgtMultiEditBox : public CGUIWgtScrollbarContainer
	{
	public:
		CGUIWgtMultiEditBox( const CGUIString& rName, const CGUIString& rSceneName );
		virtual ~CGUIWgtMultiEditBox( );

		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty);

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
		
		void SetReadOnly(bool bRead);
		bool IsReadOnly() const;

		void SetSelectedTextColor( const CGUIColor& rColor);

		void SetCursorSize( const CGUISize& rSize );
		const CGUISize& GetCursorSize() const;

		void SetMaxTextNum( uint32 num);
		uint32 GetMaxTextNum( ) const;

	protected:
		CGUIWgtMultiEditBox( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void UpdateSelf( real fDeltaTime );
		virtual void RefreshSelf();

		void InitMultiEditbox();

		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );
		virtual CGUISize GetDesiredVirtualClientSize( );

	protected://string related function
		CGUIVector2 GetCursorPos();
		CGUIRect GetCursorRect();
		void ClearSelection();
		uint32 GetSelectionLength(void) const;
		void EraseSelectedText( );
		void SetSelection(size_t start_pos, size_t end_pos);


		void InsertString( const CGUIStringW& rText );
		void DeleteString( int32 nBeginPos, int32 nEndPos);
		real GetStringWidth(int32 nBeginPos, int32 nEndPos) const;

		void SetCursorIndex( int32 nIdx );
		void UpdateCursorIndexByPos( const CGUIVector2& rPos);
		uint32 GetLineIndexByIndex(uint32 index) const;

		void FormatText();

	protected: //callback function
		virtual uint32 OnGetFocus( CGUIEventNotification* pEvent );
		virtual uint32 OnLostFocus( CGUIEventNotification* pEvent );
		virtual uint32 OnKeyPressed( CGUIEventKeyboard* pEvent );
		virtual uint32 OnMouseLeftDown( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseLeftUp( CGUIEventMouse* pEvent );
		virtual uint32 OnMouseMove( CGUIEventMouse* pEvent );
		virtual uint32 OnSizeChanged( CGUIEventSize* pEvent );

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

		uint32 m_nMaxString; //!< max number of string
		int32 m_nCursorIdx; //!< cursor's position in edited string, the first is 0.
		int32 m_nCursorLine; //!< the line where cursor is
		bool m_bReadOnly; //!< is text readonly
		ETextAlignmentHorz m_eTextAlignmentHorz;
		ETextAlignmentVert m_eTextAlignmentVert;

		uint32 m_nSelectionStart; //!< Start of selection area.
		uint32 m_nSelectionEnd; //!< End of selection area.
		bool m_bDraging; //!< true when a selection is being dragged.
		uint32 m_nDragAnchorIdx; //!< Selection index for drag selection anchor point.
		CGUIColor m_aSelectedTextColor; //!< selected text color

		CGUIImage* m_pBG; //!< bg image
		CGUIImage* m_pBGFocus; //!< bg image rendered when this widget is focusable
		CGUIImage* m_pCursor; //!< cursor 
		
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
		static wchar ms_wLineBreak; //!< Holds what we consider to be line break characters.
		CGUISize m_aTotalTextSize;
	
		CGUIStringRender m_strText; //!< text content
		std::vector<CGUISize> m_vecStringSize;//!< size of each string.
		TLineList m_aLineList; //!< line list

	protected:
		CGUIWgtEdit* m_pEdit; //!< edit control

	protected:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtMultiEditBox);
	};

}//namespace guiex

#endif //__GUI_WGTMULTIEDITBOX_20070627_H__

