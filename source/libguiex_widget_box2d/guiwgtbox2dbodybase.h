/** 
* @file guiwgtbox2dbodybase.h
* @brief use physic engine box2d
* @author ken
* @date 2010-12-21
*/

#ifndef __GUI_WGTBOX2DBODYBASE_20101221_H__
#define __GUI_WGTBOX2DBODYBASE_20101221_H__

//============================================================================//
// include
//============================================================================// 
#include "guiwgtbox2dbase.h"
#include <libguiex_core/guiwidgetgenerator.h>


//============================================================================//
// declare
//============================================================================// 
class b2Body;

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtBox2DBodyBase
	*/
	class GUIEXPORT CGUIWgtBox2DBodyBase : public CGUIWgtBox2DBase
	{
	public:

	protected:
		CGUIWgtBox2DBodyBase( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitBox2DBodyBase();

		virtual void RenderExtraSelfInfo(IGUIInterfaceRender* pRender);
		virtual void ReleaseBox2D();

		void GetBox2dPosition( CGUIVector2& rPos );
		void GetBox2dSize( CGUISize& rSize );
		void GetBox2dCenter( CGUIVector2& rCenter );
		void GetBox2dRot( real& rRot );

	protected:
		b2Body* m_pBody;

	protected:
	};


}//namespace guiex

#endif //__GUI_WGTBOX2DBODYBASE_20101221_H__

