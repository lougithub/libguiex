/** 
* @file guitexture_opengles.h
* @brief use opengl to render gui
* @author ken
* @date 2010-11-09
*/

#ifndef __GUI_TEXTURE_OPENGLES_20101109_H__
#define __GUI_TEXTURE_OPENGLES_20101109_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_module\render_opengl_base\guitexture_opengl_base.h>


//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class IGUIRender_opengles;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	class GUIEXPORT CGUITexture_opengles : public CGUITexture_opengl_base
	{
	public:
		/**
		* @brief destructor
		*/
		virtual ~CGUITexture_opengles();

	protected:
		friend class IGUIRender_opengles;
		/**
		* @brief constructor
		*/
		CGUITexture_opengles(IGUIInterfaceRender* pRender);
	};

}//namespace guiex

#endif //__GUI_TEXTURE_OPENGL_20101109_H__
