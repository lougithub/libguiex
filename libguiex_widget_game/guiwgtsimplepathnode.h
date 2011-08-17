/** 
* @file guiwgtsimplepathnode.h
* @brief path node which only connect one other node.
* @author ken
* @date 2011-08-16
*/


#ifndef __GUI_WGTSIMPLEPATHNODE_20110816_H__
#define __GUI_WGTSIMPLEPATHNODE_20110816_H__

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
	* @class CGUIWgtSimplePathNode
	* @brief used to get position info
	* used image name:
	*		-bg
	*/
	class GUIEXPORT CGUIWgtSimplePathNode: public CGUIWidget
	{
	public:
		CGUIWgtSimplePathNode( const CGUIString& rName, const CGUIString& rSceneName );
		
		virtual int32 GenerateProperty( CGUIProperty& rProperty );
		virtual void ProcessProperty( const CGUIProperty& rProperty);

		void SetNextNodeName( const CGUIString& rNodeName );
		const CGUIString& GetNextNodeName( ) const;

		void SetPreviousNode( CGUIWgtSimplePathNode* pNode );
		CGUIWgtSimplePathNode* GetPreviousNode( ) const;

		void SetNextNode( CGUIWgtSimplePathNode* pNode );
		CGUIWgtSimplePathNode* GetNextNode( ) const;

	protected:
		CGUIWgtSimplePathNode( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName );

		void InitSimplePathNode();

		virtual void RenderSelf(IGUIInterfaceRender* pRender);
		virtual void OnSetImage( const CGUIString& rName, CGUIImage* pImage );

	protected:	//!< callback function
		virtual uint32 OnParentChanged( CGUIEventRelativeChange* pEvent );

	protected:
		CGUIImage* m_pImageBG;

		CGUIString m_strNextNodeName;
		CGUIWgtSimplePathNode* m_pPreviousNode;
		CGUIWgtSimplePathNode* m_pNextNode;

		class CGUIWgtSimplePathNodeMgr* m_pNodeMgr;
		
	private:
		GUI_WIDGET_GENERATOR_DECLARE(CGUIWgtSimplePathNode);
	};


}//namespace guiex

#endif //__GUI_WGTSIMPLEPATHNODE_20110816_H__

