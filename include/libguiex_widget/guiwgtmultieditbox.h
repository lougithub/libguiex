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
#include <libguiex_core\guiwidget.h>
#include <libguiex_core\guiwidgetgenerator.h>
#include  <libguiex_core\guitimer.h>
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
		CGUIWgtMultiEditBox( const CGUIString& rName, const CGUIString& rProjectName );

		/** 
		 * @brief destructor
		 */
		virtual ~CGUIWgtMultiEditBox( );

		/**
		* @brief update this widget, 
		*/
		virtual void	Update( real fDeltaTime );

		/** 
		* @brief set widget text
		*/
		virtual void	SetTextContent(const wchar_t* pText);

		/**
		* @brief process property
		*/
		virtual void		ProcessProperty( const CGUIProperty* pProperty);

		/** 
		* @brief set text readonly
		*/
		void			SetReadOnly(bool bRead);

		/** 
		* @brief is text readonly
		*/
		bool			IsReadOnly() const;

		/** 
		* @brief set string area ratio
		*/
		void			SetStringAreaRatio(const CGUIRect& rStringAreaRatio);

		/** 
		* @brief set selected text color
		*/
		void			SetSelectedTextColor( const CGUIColor& rColor);

	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtMultiEditBox( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName );

		/// render
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

		/// initialize editbox
		void	InitMultiEditbox();

		/**
		* @brief override the OnSetImage function
		*/
		virtual void	OnSetImage( const CGUIString& rName,CGUIImage* pImage );

		//update rect, which may different for different widget
		virtual void		UpdateDirtyRect();	

	protected://string related function
		/// get position of cursor 
		CGUIVector2	GetCursorPos();

		/// get rect of cursor necessary for rendering
		CGUIRect	GetCursorRect();

		/// clear selection-area
		void		ClearSelection();

		///return the length of the current selection .
		uint32		GetSelectionLength(void) const;

		/// erase selected string
		void		EraseSelectedText( );

		/**
		* @brief Define the current selection for the Editbox
		*/
		void		SetSelection(size_t start_pos, size_t end_pos);


		/// set cursor index by pos and return it
		uint32		SetCursorIndexByPos( const CGUIVector2& rPos);

		/** 
		 * @brief get the line number from a given index
		 * @return the index of line, start from 0
		 */
		uint32		GetLineNumberFromIndex(uint32 index) const;

		/// insert a string
		void				InsertString(const wchar_t* pChar);

		/// delete string
		void				DeleteString( int32 nBeginPos, int32 nEndPos);

		/// get string width
		real				GetStringWidth(int32 nBeginPos, int32 nEndPos) const;

		/** 
		 * @brief set cursor index
		 * @param nForceLineNum if it isn't -1, means the cursor index will locate at this line
		 */
		void				SetCursorIndex( int32 nIdx, int32 nForceLineIdx = -1 );

		/// format text
		void				FormatText_Imp();
		void				FormatText();


	protected:	//callback function
		virtual	uint32		OnGetFocus( CGUIEventNotification* pEvent );
		virtual	uint32		OnLostFocus( CGUIEventNotification* pEvent );
		virtual	uint32		OnKeyPressed( CGUIEventKeyboard* pEvent );
		virtual	uint32		OnMouseLeftDown( CGUIEventMouse* pEvent );
		virtual	uint32		OnMouseLeftUp( CGUIEventMouse* pEvent );
		virtual	uint32		OnMouseMove( CGUIEventMouse* pEvent );

		//keyboard event
		void				OnKeyPressed_Left(CGUIEventKeyboard* pEvent);
		void				OnKeyPressed_Right(CGUIEventKeyboard* pEvent);
		void				OnKeyPressed_Up(CGUIEventKeyboard* pEvent);
		void				OnKeyPressed_Down(CGUIEventKeyboard* pEvent);
		void				OnKeyPressed_Delete(CGUIEventKeyboard* pEvent);
		void				OnKeyPressed_Back(CGUIEventKeyboard* pEvent);
		void				OnKeyPressed_Home(CGUIEventKeyboard* pEvent);
		void				OnKeyPressed_End(CGUIEventKeyboard* pEvent);
		void				OnKeyPressed_Enter(CGUIEventKeyboard* pEvent);

	protected:
		CGUIRect	m_aClientArea;				///< area which could used to show the text

		//---------------------------------------------------
		//string
		uint32		m_nMaxString;				//!< max number of string
		int32		m_nCursorIdx;				//!< cursor's position in edited string, the first is 0.
		int32		m_nCursorLine;				//!< the line where cursor is
		std::vector<CGUISize>	m_vecStringSize;//!< size of each string.
		bool		m_bReadOnly;				//!< is text readonly

		//---------------------------------------------------
		//cursor
		int32			m_nBlinkSpeed;			//!< blink speed, in millisecond
		CGUITimer		m_aCursorTimer;			//!< used by cursor
		bool			m_bShowCursor;			//!< used by cursor
		CGUISize		m_aCursorSize;			//!< size of cursor

		//---------------------------------------------------
		//select state
		uint32		m_nSelectionStart;			//!< Start of selection area.
		uint32		m_nSelectionEnd;			//!< End of selection area.
		bool		m_bDraging;					//!< true when a selection is being dragged.
		uint32		m_nDragAnchorIdx;			//!< Selection index for drag selection anchor point.
		CGUIColor	m_aSelectedTextColor;		//!< selected text color

		//---------------------------------------------------
		//image
		CGUIImage*	m_pBG;						//!< bg image
		CGUIImage*	m_pBGFocus;					//!< bg image rendered when this widget is focusable
		CGUIImage*	m_pCursor;					//!< cursor 

		CGUIRect	m_aStringAreaRatio;			//!< the ratio of string area, the (0,0,1,1) equal whole client area


		//line break
		/**
		* @brief struct used to store information about a formatted line within the paragraph.
		*/
		struct SLineInfo
		{
			uint32	m_nStartIdx;     //!< Starting index for this line.
			uint32	m_nLength;       //!< Code point length of this line.
			uint32	m_nLineHeight;   //!< line height
		};
		typedef std::vector<SLineInfo>   TLineList;  //!< Type for collection of LineInfos.
		TLineList			m_aLineList;			//!< line list
		static wchar_t		ms_wLineBreak;			//!< Holds what we consider to be line break characters.

	protected:
		CGUIWgtEdit*	m_pEdit;				//!< edit control

	protected:
		static CGUIString ms_strType;

	};

	GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtMultiEditBox);

}//namespace guiex

#endif //__GUI_WGTMULTIEDITBOX_20070627_H__

