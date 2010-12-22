/** 
 * @file guicamera.h
 * @brief camera used in system
 * @author ken
 * @date 2010-12-22
 */

#ifndef __GUI_CANVASLAYERMANAGER_20101222_H__
#define __GUI_CANVASLAYERMANAGER_20101222_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include <vector>

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUICanvasLayer;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUICanvasLayerManager
	{
	public:
		~CGUICanvasLayerManager();
		
		static CGUICanvasLayerManager* Instance();
	

		void Update( real fDeltaTime );
		void Render( class IGUIInterfaceRender* pRender );

		void PushCanvasLayer( CGUICanvasLayer* pLayer );
		CGUICanvasLayer* PopCanvasLayer( );
		void PopAndDestroyCanvasLayer( );

		CGUICanvasLayer* RemoveCanvasLayer( CGUICanvasLayer* pLayer );
		CGUICanvasLayer* DestroyAllCanvasLayer( );
		
		uint32 GetCanvasLayerNum() const;
		CGUICanvasLayer* GetCanvasLayer( uint32 nIndex );
		CGUICanvasLayer* GetCanvasLayer( const char* szLayerName );

	protected:
		friend class CGUISystem;
		CGUICanvasLayerManager();
		CGUICanvasLayerManager( const CGUICanvasLayerManager& );

	protected:
		typedef std::vector<CGUICanvasLayer*>	TArrayCanvasLayer;
		TArrayCanvasLayer m_arrayCanvasLayers;

	private:
		static CGUICanvasLayerManager* m_pSingleton;
	};

}//namespace guiex

#endif //__GUI_CANVASLAYERMANAGER_20101222_H__
