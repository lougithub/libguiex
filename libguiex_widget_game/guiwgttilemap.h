
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

		void SetTileImageCacheCapacity( uint32 nCapacity );
		uint32 GetTileImageCacheCapacity( ) const;
		void ClearTileImageCache();

		void SetTileImageSize( const CGUISize& rTileImageSize );
		const CGUISize& GetTileImageSize( ) const;

		void SetTileImageCache( uint32 nTileID, CGUIImage* pImage );
		const CGUIImage* GetTileImageCache( uint32 nTileID) const;

		void ClearTileMapData();
		void SetTileMapData( const TTileMapData& rTileMapData );
		const TTileMapData& GetTileMapData() const;

		void SetTileMapSize( uint32 nRow, uint32 nColumn );
		void SetTileID( uint32 nRow, uint32 nColumn, uint32 nTileID );
		uint32 GetTileID( uint32 nRow, uint32 nColumn );

	protected:
		CGUIWgtTileMap( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );
		void InitTileMap();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);

		void UpdateTileMapSize( );

	protected:
		typedef std::vector<CGUIImage*> TTileImageCache;
		TTileImageCache m_vecTileImageCache;

		TTileMapData m_aTileMapData;
		uint32 m_nRows;
		uint32 m_nColumns;

		CGUISize m_aTileImageSize;

	protected:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtTileMap);
	};


}//namespace guiex

#endif //__GUI_WGTTILEMAP_20110126_H__

