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
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtEditBox( const CGUIString& rName, const CGUIString& rProjectName );

		/** 
		 * @brief destructor
		 */
		virtual ~CGUIWgtEditBox( );

		/** 
		* @brief set widget text
		*/
		virtual void SetTextContent(const wchar_t* pText);

		/**
		* @brief load widget config from property
		*/
		virtual CGUIProperty* GenerateProperty(const CGUIString& rName, const CGUIString& rType );

		/**
		* @brief process property
		*/
		virtual void ProcessProperty( const CGUIProperty* pProperty);

		/**
		* @brief Set the current position of the carat.
		* @param nPos New index for the insert cursor relative to the start of the text.  
		* If the value specified is greater than the number of characters in the Editbox or is negative, 
		* the cursor is positioned at the end of the text.
		*/
		void SetCursorIndex(int32 nPos);

		/** 
		* @brief set text masked
		*/
		void SetTextMasked(bool bMask);

		/** 
		* @brief is text masked
		*/
		bool IsTextMasked() const;


		/** 
		* @brief is text masked
		*/
		void SetMaskCode(wchar_t wMaskCode);

		/** 
		* @brief is text masked
		*/
		wchar_t GetTextMasked() const;


		/** 
		* @brief set text readonly
		*/
		void SetReadOnly(bool bRead);

		/** 
		* @brief is text readonly
		*/
		bool IsReadOnly() const;

		/** 
		* @brief set string area ratio
		*/
		void SetStringAreaRatio(const CGUIRect& rStringAreaRatio);

		/** 
		* @brief get string area ratio
		*/
		const CGUIRect& GetStringAreaRatio( ) const;

		/** 
		* @brief set selected text color
		*/
		void SetSelectedTextColor( const CGUIColor& rColor);

		void SetCursorSize( const CGUISize& rSize );
		const CGUISize& GetCursorSize() const;

		void SetMaxTextNum( uint32 num);
		uint32 GetMaxTextNum( ) const;

	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtEditBox( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName );
	
		virtual void RefreshSelf( );

		/// render
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

		/**
		* @brief update this widget, 
		*/
		virtual void UpdateSelf( real fDeltaTime );

		/// initialize editbox
		void InitEditbox();

		/**
		* @brief override the OnSetImage function
		*/
		virtual void OnSetImage( const CGUIString& rName,CGUIImage* pImage );

	protected://string related function
		/// get position of cursor 
		CGUIVector2	GetCursorPos();

		/// get rect of cursor necessary for rendering
		CGUIRect	GetCursorRect();

		/// get text index from given global pos
		uint32		GetTextIndexFromPos( const CGUIVector2& rPos);

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


		/// insert a string
		void				InsertString(const wchar_t* pChar);

		/// delete string
		void				DeleteString( int32 nBeginPos, int32 nEndPos);

		/// update string position
		void				UpdateStringPos();

		/// get string width
		real				GetStringWidth(int32 nBeginPos, int32 nEndPos) const;




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
		void				OnKeyPressed_Delete(CGUIEventKeyboard* pEvent);
		void				OnKeyPressed_Back(CGUIEventKeyboard* pEvent);
		void				OnKeyPressed_Home(CGUIEventKeyboard* pEvent);
		void				OnKeyPressed_End(CGUIEventKeyboard* pEvent);

	protected:
		//---------------------------------------------------
		//string
		uint32		m_nMaxString;				//!< max number of string
		uint32		m_nCursorIdx;				//!< cursor's position in edited string, the first is 0.
		real		m_fTextWidthRel;			//!< start position of text, it's a relative position
		std::vector<uint32>	m_vecStringWidth;	//!< width of each string.
		bool		m_bReadOnly;				//!< is text readonly

		//for mask
		bool			m_bMaskText;			//!< True if the editbox text should be rendered masked.
		wchar_t			m_wMaskCodePoint;		//!< Code point to use when rendering masked text.
		CGUIStringEx	m_strMaskText;			//!< mask text

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

		//---------------------------------------------------
		CGUIRect	m_aStringAreaRatio;			//!< the ratio of string area, the (0,0,1,1) equal whole client area

		CGUIRect	m_aStringAreaRect;

	protected:
		CGUIWgtEdit*	m_pEdit;			//!< edit control

	protected:
		static CGUIString ms_strType;

	};

	GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtEditBox);

}//namespace guiex

#endif //__GUI_WGTEDITBOX_20060807_H__

