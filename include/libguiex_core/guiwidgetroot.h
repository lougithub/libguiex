/** 
* @file guiwgtroot.h
* @brief base class, for root widget
* @author ken
* @date 2009-09-29
*/


#ifndef __GUI_WIDGET_ROOT_20090929_H__
#define __GUI_WIDGET_ROOT_20090929_H__

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
	* @class CGUIWgtStatic
	* @brief base class for static widget
	*/
	class GUIEXPORT CGUIWidgetRoot : public CGUIWidget
	{
	public:
		/**
		* @brief constructor
		*/
		CGUIWidgetRoot( const CGUIString& rName, const CGUIString& rSceneName );

	protected:
		///initialize root
		void	InitRoot();

		virtual void PushClipRect( IGUIInterfaceRender* pRender, const CGUIRect& rClipRect );
		virtual void PopClipRect( IGUIInterfaceRender* pRender );
		
		/// render
		virtual void RenderSelf(IGUIInterfaceRender* pRender);

	protected:
		static CGUIString ms_strType;


	};
	GUI_WIDGET_GENERATOR_DECLARE(CGUIWidgetRoot);

}//namespace guiex

#endif //__GUI_WIDGET_ROOT_20090929_H__
