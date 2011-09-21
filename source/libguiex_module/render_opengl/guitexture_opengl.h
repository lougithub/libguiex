/** 
* @file guitexture_opengl.h
* @brief use opengl to render gui
* @author ken
* @date 2006-07-06
*/

#ifndef __GUI_TEXTURE_OPENGL_20060706_H__
#define __GUI_TEXTURE_OPENGL_20060706_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module/render_opengl_base/guitexture_opengl_base.h>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class IGUIRender_opengl;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUITexture_opengl : public CGUITexture_opengl_base
	{
	protected:
		friend class IGUIRender_opengl;

		CGUITexture_opengl(IGUIInterfaceRender* pRender);
	};
	
}//namespace guiex

#endif //__GUI_TEXTURE_OPENGL_20060706_H__
