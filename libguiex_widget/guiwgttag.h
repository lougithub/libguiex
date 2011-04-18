/** 
 * @file guiwgttag.cpp
 * @brief used to get position info
 * @author ken
 * @date 2011-04-15
 */




#ifndef __GUI_WGTTAG_20110415_H__
#define __GUI_WGTTAG_20110415_H__

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
	* @class CGUIWgtTag
	* @brief used to get position info
	*/
	class GUIEXPORT CGUIWgtTag: public CGUIWidget
	{
	public:
		CGUIWgtTag( const CGUIString& rName, const CGUIString& rSceneName );
		
		virtual void Render( IGUIInterfaceRender* pRender );
		virtual void RenderExtraInfo(IGUIInterfaceRender* pRender);
		virtual void Update( real fDeltaTime );
		

	protected:
		CGUIWgtTag( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		void InitTag();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );
		
	protected:
		CGUIImage* m_pImageBG;		
		
	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtTag);
	};


}//namespace guiex

#endif //__GUI_WGTTAG_20110415_H__

