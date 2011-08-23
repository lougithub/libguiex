/** 
* @file guiwgtsimplepathnodemgr.cpp
* @brief path node manager.
* @author ken
* @date 2011-08-16
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiwgtsimplepathnodemgr.h"
#include "guiwgtsimplepathnode.h"
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiwidgetmanager.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiimage.h>
#include <libguiex_core/guipropertymanager.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtSimplePathNodeMgr);
	//------------------------------------------------------------------------------
	CGUIWgtSimplePathNodeMgr::CGUIWgtSimplePathNodeMgr(const CGUIString& rName, const CGUIString& rSceneName)
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitSimplePathNodeMgr();
	}
	//------------------------------------------------------------------------------
	CGUIWgtSimplePathNodeMgr::CGUIWgtSimplePathNodeMgr( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitSimplePathNodeMgr();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtSimplePathNodeMgr::InitSimplePathNodeMgr()
	{
		SetSizeType( eScreenValue_Percentage );
		SetSize( 1.0f, 1.0f );

		SetMouseConsumed(false);
		SetHitable( false );	
		SetGenerateParentChildEvent( true );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtSimplePathNodeMgr::Render( IGUIInterfaceRender* pRender )
	{
		if( GSystem->IsEditorMode() )
		{
			CGUIWidget::Render( pRender );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtSimplePathNodeMgr::RenderSelf(IGUIInterfaceRender* pRender)
	{
		for( std::map<CGUIString, CGUIWgtSimplePathNode* >::iterator itor = m_mapNodes.begin();
			itor != m_mapNodes.end();
			++itor )
		{
			CGUIWgtSimplePathNode* pNode = itor->second;
			if( !pNode->GetNextNodeName().empty())
			{
				CGUIWgtSimplePathNode* pNextNode = FindPathNode( pNode->GetNextNodeName() );
				if( pNextNode )
				{
					pRender->DrawLine( pNode->GetPixelPosition(), pNextNode->GetPixelPosition(), 3, 0, CGUIColor(0,0,0.5,0.8), CGUIColor(0,0,0.5,0.8) );
				}
			}
		}
	}
	//------------------------------------------------------------------------------	
	void CGUIWgtSimplePathNodeMgr::RenderExtraInfo(IGUIInterfaceRender* pRender)
	{
		if( GSystem->IsEditorMode() )
		{
			CGUIWidget::RenderExtraInfo( pRender );
		}		
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtSimplePathNodeMgr::OnAddChild( CGUIEventRelativeChange* pEvent )
	{
		if( pEvent->GetRelative()->GetType() != CGUIWgtSimplePathNode::StaticGetType())
		{
			GUI_THROW("[CGUIWgtSimplePathNodeMgr::OnAddChild]: don't accept child except CGUIWgtSimplePathNode.");
		}

		uint32 ret = CGUIWidget::OnAddChild(pEvent);

		m_mapNodes.insert( std::make_pair( pEvent->GetRelative()->GetName(), static_cast<CGUIWgtSimplePathNode*>( pEvent->GetRelative() ) ));

		return ret;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtSimplePathNodeMgr::OnRemoveChild( CGUIEventRelativeChange* pEvent )
	{
		if( pEvent->GetRelative()->GetType() != CGUIWgtSimplePathNode::StaticGetType())
		{
			GUI_THROW("[CGUIWgtSimplePathNodeMgr::OnRemoveChild]: don't accept child except CGUIWgtSimplePathNode.");
		}

		uint32 ret = CGUIWidget::OnRemoveChild(pEvent);

		m_mapNodes.erase( pEvent->GetRelative()->GetName() );

		return ret;
	}
	//------------------------------------------------------------------------------
	CGUIWgtSimplePathNode* CGUIWgtSimplePathNodeMgr::FindPathNode( const CGUIString& rNodeName )
	{
		std::map<CGUIString, class CGUIWgtSimplePathNode* >::iterator itor = m_mapNodes.find( rNodeName );
		if( itor != m_mapNodes.end())
		{
			return itor->second;
		}
		else
		{
			GUI_WARNING( GUI_FORMAT("[CGUIWgtSimplePathNodeMgr::FindPathNode]: failed to find pathnode by name %s", rNodeName.c_str()));
			return NULL;
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex

