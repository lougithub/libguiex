/** 
* @file guiwgtstaticimage.h
* @brief used to show a static image
* @author ken
* @date 2006-07-19
*/

#ifndef __GUI_WGTSTATICIMAGE_20060719_H__
#define __GUI_WGTSTATICIMAGE_20060719_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_widget/guiwgtstatic.h>
#include <libguiex_core/guiwidgetgenerator.h>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtStaticImage
	* @brief to show a static image
	* used image name:
	*		-BGIMAGE
	*/
	class GUIEXPORT CGUIWgtStaticImage : public CGUIWgtStatic
	{
	public:
		CGUIWgtStaticImage( const CGUIString& rName, const CGUIString& rSceneName );

	protected:
		CGUIWgtStaticImage( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitStaticImage();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );

	protected:
		const CGUIImage* m_pImageBG;

	protected:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtStaticImage);
	};


}//namespace guiex

#endif //__GUI_WGTSTATICIMAGE_20060719_H__

