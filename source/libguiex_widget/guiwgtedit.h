/** 
* @file guiwgtedit.h
* @brief internal use, for get string from ime
* @author ken
* @date 2006-08-24
*/


#ifndef __GUI_WGTEDIT_20060824_H__
#define __GUI_WGTEDIT_20060824_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetgenerator.h>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class IGUIInterfaceIme;
}


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtEdit
	* @brief edit, internal use by editbox
	*/ 
	class GUIEXPORT CGUIWgtEdit : public CGUIWidget
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtEdit( const CGUIString& rName, const CGUIString& rSceneName );

		/// get result string
		const CGUIStringW& GetResult() const;

		/// clear result string, usually call it after get result string
		void ClearResult();

		/// open this widget
		virtual void Open();

		/// close this widget
		virtual void Close();

		/// set position of cursor
		void SetCursorPos( const CGUIVector2& rPos );

		void SetCursorSize( const CGUISize& rSize );
		const CGUISize& GetCursorSize() const;

		bool IsShowCursor() const;
		void ResetShowCursor();

	protected:
		///initialize
		void InitEdit();

		/// render
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

		/// update this widget
		virtual void UpdateSelf( real fDeltaTime );

	protected:	//!< callback function

	protected:
		CGUIStringW m_strResult;	///result string

		IGUIInterfaceIme *m_pIme;	///interface of ime

		//---------------------------------------------------
		//cursor
		real		m_fBlinkSpeed;				//!< blink speed, in millisecond
		real		m_aCursorShowTime;			//!< used by cursor
		bool		m_bShowCursor;				//!< used by cursor
		CGUISize	m_aCursorSize;				//!< size of cursor

	private:
		static CGUIString	ms_strType;
	};

}//namespace guiex


#endif //__GUI_WGTEDIT_20060824_H__
