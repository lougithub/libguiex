/** 
* @file guiwgtsimplepathnode.cpp
* @brief path node which only connect one other node.
* @author ken
* @date 2011-08-16
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiwgtsimplepathnode.h"
#include "guiwgtsimplepathnodemgr.h"
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
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtSimplePathNode);
	//------------------------------------------------------------------------------
	CGUIWgtSimplePathNode::CGUIWgtSimplePathNode(const CGUIString& rName, const CGUIString& rSceneName)
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitSimplePathNode();
	}
	//------------------------------------------------------------------------------
	CGUIWgtSimplePathNode::CGUIWgtSimplePathNode( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitSimplePathNode();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtSimplePathNode::InitSimplePathNode()
	{
		m_pImageBG = NULL;
		m_pPreviousNode = NULL;
		m_pNextNode = NULL;
		m_pNodeMgr = NULL;
		
		SetPositionType( eScreenValue_Percentage );
		SetPosition( 0.5f, 0.5f );
		SetAnchorPoint( 0.5f, 0.5f );
		SetPixelSize(17.0f, 17.0f );
		SetMouseConsumed(false);
		SetHitable( false );				
		SetGenerateParentChildEvent( true );
	}
	//------------------------------------------------------------------------------
	void CGUIWgtSimplePathNode::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		if( rName == "bg")
		{
			m_pImageBG = pImage;
			if( GetSize().IsEqualZero() && m_pImageBG )
			{
				SetPixelSize(m_pImageBG->GetSize());
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWgtSimplePathNode::RenderSelf(IGUIInterfaceRender* pRender)
	{
		DrawImage( pRender, m_pImageBG, GetBoundArea( ));
	}
	//------------------------------------------------------------------------------
	void CGUIWgtSimplePathNode::SetNextNodeName( const CGUIString& rNodeName )
	{
		if( !m_pNodeMgr )
		{
			return;
		}
		m_strNextNodeName = rNodeName;
		if( !rNodeName.empty() )
		{
			CGUIWgtSimplePathNode* pNode = m_pNodeMgr->FindPathNode( rNodeName );
			if( pNode )
			{
				SetNextNode( pNode );
				pNode->SetPreviousNode( this );
			}
			else
			{
				SetNextNode(NULL);
			}
		}

		if( GSystem->IsEditorMode() )
		{
			m_pNodeMgr->RelinkPathNode();
		}
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIWgtSimplePathNode::GetNextNodeName( ) const
	{
		return m_strNextNodeName;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtSimplePathNode::SetPreviousNode( CGUIWgtSimplePathNode* pNode )
	{
		m_pPreviousNode = pNode;
	}
	//------------------------------------------------------------------------------
	CGUIWgtSimplePathNode* CGUIWgtSimplePathNode::GetPreviousNode( ) const
	{
		return m_pPreviousNode;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtSimplePathNode::SetNextNode( CGUIWgtSimplePathNode* pNode )
	{
		m_pNextNode = pNode;
	}
	//------------------------------------------------------------------------------
	CGUIWgtSimplePathNode* CGUIWgtSimplePathNode::GetNextNode( ) const
	{
		return m_pNextNode;
	}
	//------------------------------------------------------------------------------
	int32 CGUIWgtSimplePathNode::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_String && rProperty.GetName() == "next_node" )
		{
			rProperty.SetValue( GetNextNodeName() );
		}
		else
		{
			return CGUIWidget::GenerateProperty( rProperty );
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtSimplePathNode::ProcessProperty( const CGUIProperty& rProperty)
	{
		if( rProperty.GetType() == ePropertyType_String && rProperty.GetName() == "next_node")
		{
			SetNextNodeName( rProperty.GetValue() );
		}
		else
		{
			CGUIWidget::ProcessProperty( rProperty );
		}
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWgtSimplePathNode::OnParentChanged( CGUIEventRelativeChange* pEvent )
	{
		if( GetParent() && GetParent()->GetType() == CGUIWgtSimplePathNodeMgr::StaticGetType())
		{
			m_pNodeMgr = static_cast<CGUIWgtSimplePathNodeMgr*>(GetParent());
		}
		else
		{
			m_pNodeMgr = NULL;
		}
		return CGUIWidget::OnParentChanged(pEvent);
	}
	//------------------------------------------------------------------------------

}//namespace guiex

