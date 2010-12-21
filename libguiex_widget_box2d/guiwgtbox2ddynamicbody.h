/** 
* @file guiwgtbox2ddynamicbody.h
* @brief use physic engine box2d
* @author ken
* @date 2010-12-21
*/

#ifndef __GUI_WGTBOX2DDYNAMICBODY_20101221_H__
#define __GUI_WGTBOX2DDYNAMICBODY_20101221_H__

//============================================================================//
// include
//============================================================================// 
#include "guiwgtbox2dbase.h"
#include <libguiex_core/guiwidgetgenerator.h>


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtBox2DDynamicBody
	*/
	class GUIEXPORT CGUIWgtBox2DDynamicBody : public CGUIWgtBox2DBase
	{
	public:
		CGUIWgtBox2DDynamicBody( const CGUIString& rName, const CGUIString& rSceneName );

	protected:
		CGUIWgtBox2DDynamicBody( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitBox2DDynamicBody();

		virtual void OnCreate();

	protected:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtBox2DDynamicBody);
	};


}//namespace guiex

#endif //__GUI_WGTBOX2DDYNAMICBODY_20101221_H__

