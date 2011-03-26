
/** 
* @file guiwgttiledmap.h
* @brief used to render tile map
* @author ken
* @date 2011-01-26
*/

#ifndef __GUI_WGTTILEDMAP_20110126_H__
#define __GUI_WGTTILEDMAP_20110126_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guiwidgetgenerator.h>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUITiledMap;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtTiledMap
	*/
	class GUIEXPORT CGUIWgtTiledMap : public CGUIWidget
	{
	public:
		typedef std::vector<std::vector<uint32> > TTiledMapData; //row<column>

	public:
		CGUIWgtTiledMap( const CGUIString& rName, const CGUIString& rSceneName );
		virtual ~CGUIWgtTiledMap();

		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty);


	protected:
		CGUIWgtTiledMap( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitTiledMap();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void OnDestroy();

		void SetTiledMap( const CGUIString& rTiledMapName );
		void SetTiledMap( class CGUITiledMap* pTiledMap );

	protected:
		CGUITiledMap* m_pTiledMap;

	protected:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtTiledMap);
	};


}//namespace guiex

#endif //__GUI_WGTTILEDMAP_20110126_H__

