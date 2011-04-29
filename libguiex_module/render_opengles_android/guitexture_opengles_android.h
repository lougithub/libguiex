/** 
* @file guitexture_opengles.h
* @brief use opengl to render gui
* @author ken
* @date 2011-04-29
*/

#ifndef __GUI_TEXTURE_OPENGLES_ANDROID_20110429_H__
#define __GUI_TEXTURE_OPENGLES_ANDROID_20110429_H__

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

	class GUIEXPORT CGUITexture_opengles_android : public CGUITexture_opengl_base
	{
	public:
		/**
		* @brief destructor
		*/
		virtual ~CGUITexture_opengles_android();

	protected:
		friend class IGUIRender_opengles_android;
		/**
		* @brief constructor
		*/
		CGUITexture_opengles_android(IGUIInterfaceRender* pRender);
	};

}//namespace guiex

#endif //__GUI_TEXTURE_OPENGLES_ANDROID_20110429_H__
