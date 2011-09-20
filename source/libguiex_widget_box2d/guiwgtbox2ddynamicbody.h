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
#include "guiwgtbox2dbodybase.h"


//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtBox2DDynamicBody
	*/
	class GUIEXPORT CGUIWgtBox2DDynamicBody : public CGUIWgtBox2DBodyBase
	{
	public:
		CGUIWgtBox2DDynamicBody( const CGUIString& rName, const CGUIString& rSceneName );

		void SetVelocity( real fVelocityX, real fVelocityY );

	protected:
		CGUIWgtBox2DDynamicBody( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitBox2DDynamicBody();

		virtual void InitializeBox2D();
		virtual void UpdateBox2D();

	private:

	protected:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtBox2DDynamicBody);
	};


}//namespace guiex

#endif //__GUI_WGTBOX2DDYNAMICBODY_20101221_H__

