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
#include <libguiex_core\guiwidget.h>
#include <libguiex_core\guiwidgetgenerator.h>

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
		CGUIWgtEdit( const CGUIString& rName, const CGUIString& rProjectName );

		/// get result string
		const wchar_t*	GetResult() const;

		/// clear result string, usually call it after get result string
		void	ClearResult();

		/// open this widget
		virtual void	Open();

		/// close this widget
		virtual void	Close();

		/// update this widget
		virtual void	Update( real fDeltaTime );

		/// set position of cursor
		void	SetCursorPos( const CGUIVector2& rPos );

	protected:
		///initialize
		void InitEdit();

		/// render
		virtual void RenderSelf(IGUIInterfaceRender* pRender);


	protected:	//!< callback function

	protected:
		std::wstring		m_strResult;	///result string

		IGUIInterfaceIme	*	m_pIme;	///interface of ime

	private:
		static CGUIString	ms_strType;
	};

}//namespace guiex


#endif //__GUI_WGTEDIT_20060824_H__
