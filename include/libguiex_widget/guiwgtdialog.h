/** 
* @file guiwgtdialog.h
* @brief dialog used in guiex system
* @author ken
* @date 2006-08-16
*/

#ifndef __GUI_WGTDIALOG_20060816_H__
#define __GUI_WGTDIALOG_20060816_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetgenerator.h>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	/**
	* @class CGUIWgtDialog
	* @brief dialog is a exclusive widget, which means all 
	* sibling of this dialog won't receive any input
	* used image name:
	*		-BGIMAGE
	*/
	class GUIEXPORT CGUIWgtDialog : public CGUIWidget
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWgtDialog( const CGUIString& rName, const CGUIString& rProjectName );

		/**
		* @brief create this widget
		*/
		virtual int32 Create();


	protected:
		/**
		* @brief constructor
		* for derived class
		*/
		CGUIWgtDialog( const CGUIString& rType, const CGUIString& rName, const CGUIString& rProjectName );

		///initialize
		void InitDialog();

		/// render
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

		/**
		* @brief override the OnSetImage function
		*/
		virtual void	OnSetImage( const CGUIString& rName,CGUIImage* pImage );

	protected:	//!< callback function
		uint32	OnOpen( CGUIEventNotification* pEvent );


	protected:
		CGUIImage*		m_pImageBG;

	private:
		static CGUIString	ms_strType;
	};



	GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtDialog);

}//namespace libguiex



#endif //__GUI_WGTDIALOG_20060816_H__
