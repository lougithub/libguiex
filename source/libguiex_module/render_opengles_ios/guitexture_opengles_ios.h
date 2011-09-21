/** 
* @file guitexture_opengles_ios.h
* @brief use opengl to render gui
* @author ken
* @date 2010-11-09
*/

#ifndef __GUI_TEXTURE_OPENGLES_IOS_20101109_H__
#define __GUI_TEXTURE_OPENGLES_IOS_20101109_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/render_opengl_base/guitexture_opengl_base.h>


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class IGUIRender_opengles_ios;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	class GUIEXPORT CGUITexture_opengles_ios : public CGUITexture_opengl_base
	{
	protected:
		friend class IGUIRender_opengles_ios;
		/**
		* @brief constructor
		*/
		CGUITexture_opengles_ios(IGUIInterfaceRender* pRender);
	};

}//namespace guiex

#endif //__GUI_TEXTURE_OPENGLES_IOS_20101109_H__
