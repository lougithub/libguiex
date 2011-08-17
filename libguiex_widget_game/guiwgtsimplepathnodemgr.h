/** 
* @file guiwgtsimplepathnodemgr.h
* @brief path node container which only connect one other node.
* @author ken
* @date 2011-08-16
*/


#ifndef __GUI_WGTSIMPLEPATHNODEMGR_20110816_H__
#define __GUI_WGTSIMPLEPATHNODEMGR_20110816_H__

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
	* @class CGUIWgtSimplePathNodeMgr
	*/
	class GUIEXPORT CGUIWgtSimplePathNodeMgr: public CGUIWidget
	{
	public:
		CGUIWgtSimplePathNodeMgr( const CGUIString& rName, const CGUIString& rSceneName );
		
		virtual void Render( IGUIInterfaceRender* pRender );
		virtual void RenderExtraInfo(IGUIInterfaceRender* pRender);
		
		class CGUIWgtSimplePathNode* FindPathNode( const CGUIString& rNodeName );
		void RelinkPathNode();

	protected:
		CGUIWgtSimplePathNodeMgr( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		void InitSimplePathNodeMgr();

		virtual void OnCreate();
		virtual void RenderSelf(IGUIInterfaceRender* pRender);
	
	protected:	//!< callback function
		virtual uint32 OnAddChild( CGUIEventRelativeChange* pEvent );
		virtual uint32 OnRemoveChild( CGUIEventRelativeChange* pEvent );

	protected:
		std::vector< class CGUIWgtSimplePathNode* > m_vecStartNode;
		std::map<CGUIString, class CGUIWgtSimplePathNode* > m_mapNodes;

	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtSimplePathNodeMgr);
	};


}//namespace guiex

#endif //__GUI_WGTSIMPLEPATHNODEMGR_20110816_H__

