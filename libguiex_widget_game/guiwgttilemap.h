
/** 
* @file guiwgttilemap.h
* @brief used to render tile map
* @author ken
* @date 2011-01-26
*/

#ifndef __GUI_WGTTILEMAP_20110126_H__
#define __GUI_WGTTILEMAP_20110126_H__

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
	class CGUITileMap;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUIWgtTileMap
	*/
	class GUIEXPORT CGUIWgtTileMap : public CGUIWidget
	{
	public:
		typedef std::vector<std::vector<uint32> > TTileMapData; //row<column>

	public:
		CGUIWgtTileMap( const CGUIString& rName, const CGUIString& rSceneName );
		virtual ~CGUIWgtTileMap();

		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty);


	protected:
		CGUIWgtTileMap( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitTileMap();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void OnDestroy();

		void SetTileMap( const CGUIString& rTileMapName );
		void SetTileMap( class CGUITileMap* pTileMap );

	protected:
		CGUITileMap* m_pTileMap;

	protected:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtTileMap);
	};


}//namespace guiex

#endif //__GUI_WGTTILEMAP_20110126_H__

