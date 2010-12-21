/** 
* @file guiwgtbox2dstaticbody.h
* @brief use physic engine box2d
* @author ken
* @date 2010-12-21
*/

#ifndef __GUI_WGTBOX2DSTATICBODY_20101221_H__
#define __GUI_WGTBOX2DSTATICBODY_20101221_H__

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
	* @class CGUIWgtBox2DStaticBody
	*/
	class GUIEXPORT CGUIWgtBox2DStaticBody : public CGUIWgtBox2DBase
	{
	public:
		CGUIWgtBox2DStaticBody( const CGUIString& rName, const CGUIString& rSceneName );

	protected:
		CGUIWgtBox2DStaticBody( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitBox2DStaticBody();

		virtual void OnCreate();

	protected:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtBox2DStaticBody);
	};


}//namespace guiex

#endif //__GUI_WGTBOX2DSTATICBODY_20101221_H__

