/** 
* @file guiwgtwgtbox2djoint.h
* @brief use physic engine box2d
* @author ken
* @date 2010-12-21
*/

#ifndef __GUI_WGTBOX2DJOINT_20101221_H__
#define __GUI_WGTBOX2DJOINT_20101221_H__

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
	* @class CGUIWgtBox2DJoint
	*/
	class GUIEXPORT CGUIWgtBox2DJoint : public CGUIWgtBox2DBase
	{
	public:
		CGUIWgtBox2DJoint( const CGUIString& rName, const CGUIString& rSceneName );

	protected:
		CGUIWgtBox2DJoint( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitBox2DJoint();

	protected:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtBox2DJoint);
	};


}//namespace guiex

#endif //__GUI_WGTBOX2DJOINT_20101221_H__

