/** 
* @file guiwgtbox2dbase.h
* @brief use physic engine box2d
* @author ken
* @date 2010-12-21
*/

#ifndef __GUI_WGTBOX2DBASE_20101221_H__
#define __GUI_WGTBOX2DBASE_20101221_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetgenerator.h>
#include <Box2D/Box2D.h>

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtBox2DBase
	* @brief base class of box2d.
	*/
	class GUIEXPORT CGUIWgtBox2DBase : public CGUIWidget
	{
	public:

	protected:
		CGUIWgtBox2DBase( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitBox2DBase();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );

	protected:
		virtual void OnCreate();
		virtual void OnUpdate( real fDeltaTime );
		virtual void OnDestroy();

		virtual void InitializeBox2D();
		virtual void ReleaseBox2D();
		virtual void UpdateBox2D();

	protected:
		const CGUIImage* m_pImageBG;

	protected:
	};


}//namespace guiex

#endif //__GUI_WGTBOX2DBASE_20101221_H__

