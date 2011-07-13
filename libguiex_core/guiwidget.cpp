/** 
* @file guiwidget.cpp
* @brief base class, define most operation of widget
* @author ken
* @date 2006-06-07
*/

//============================================================================//
// include
//============================================================================// 
#include "guiwidget.h"
#include "guisystem.h"
#include "guievent.h"
#include "guirect.h"
#include "guiexception.h"
#include "guipropertymanager.h"
#include "guilogmsgmanager.h"
#include "guistringconvertor.h"
#include "guiperfmonitor.h"
#include "guipropertyconvertor.h"

#include "guiinterfacemanager.h"
#include "guiinterfacerender.h"
#include "guiinterfacescript.h"
#include "guiinterfacefont.h"

#include "guiimage.h"
#include "guianimation.h"
#include "guias.h"
#include "guisounddata.h"

#include "guiimagemanager.h"
#include "guianimationmanager.h"
#include "guiasmanager.h"
#include "guisoundmanager.h"
#include "guiwidgetmanager.h"

#include "guisceneeffect.h"

#include <algorithm>

//------------------------------------------------------------------------------


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	/**
	* @brief default constructor
	*/
	CGUIWidget::CGUIWidget( const CGUIString& rType, 
		const CGUIString& rName, 
		const CGUIString& rSceneName )
		:CGUINode()
		,m_strName(rName)
		,m_strOwnerSceneName(rSceneName)
		,m_strWorkingSceneName(rSceneName)
		,m_pParent(NULL)
		,m_pChild(NULL)
		,m_pNextSibling(NULL)
		,m_pPrevSibling(NULL)
		,m_pWidgetGenerator(NULL)
		,m_aParamScale(CGUISize(1.0f,1.0f))
		,m_aParamAlpha(1.0f)
		,m_aParamDisable(false)
		,m_aParamVisible(true)
		,m_aParamActivable(false)
		,m_strType(rType)
		,m_bIsOpen( false )
		,m_bIsCreate( false )
		,m_aColor(1.0f,1.0f,1.0f,1.0f)
		,m_vRotation(0.0f,0.0f,0.0f)
		,m_bOpenWithParent(true)
		,m_bInheritAlpha(true)
		,m_bIsFocusAgency(false)
		,m_bIsFocusable(false)
		,m_bIsMovable(false)
		,m_bIsHitable(true)
		,m_bIsMouseConsumed(true)
		,m_bForceHitTest(false)
		,m_bIsGenerateUpdateEvent(false)
		,m_bIsGenerateParentSizeChangeEvent(false)
		,m_bIsGenerateParentChildEvent(false )
		,m_bIsGenerateClickEvent(false)
		,m_bIsGenerateLoadEvent(false)
		,m_bIsGenerateDBClickEvent(false)
		,m_bIsGenerateMultiClickEvent(false)
		,m_bIsAutoPlayAs(false)
		,m_bIsClipChildren(false)
		,m_pSceneEffect( NULL )
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief default destructor
	*/
	CGUIWidget::~CGUIWidget( )
	{
		DestroyAllResource();

		//delete child
		CGUIWidget* pChild = GetChild();
		CGUIWidget* pChildTmp = NULL;
		while( pChild )
		{
			pChildTmp = pChild;
			pChild = pChild->GetNextSibling();

			if( CGUIWidgetManager::Instance()->HasPage( pChildTmp))
			{
				//it's child is another page
				pChildTmp->SetParent(NULL);
				continue;
			}

			//destroy it
			CGUIWidgetManager::Instance()->DestroyWidget(pChildTmp);		
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief create this widget
	* @return return 0 for success.
	*/
	void CGUIWidget::Create()
	{
		//create self
		if( IsCreate())
		{
			GUI_THROW( GUI_FORMAT(
				"[CGUIWidget::Create]: the widget has been created. TYPE<%s>  NAME<%s>",
				GetType().c_str(), 
				GetName().c_str()));
		}
		m_bIsCreate = true;

		Refresh();

		//register to script
		if( !CGUIWidgetManager::Instance()->IsInternalName( GetName()) )
		{
			bool bHasScript = false;
			guiex::IGUIInterfaceScript* pInterfaceScript = CGUIInterfaceManager::Instance()->GetInterfaceScript();
			if( pInterfaceScript && !GSystem->IsEditorMode())
			{
				bHasScript = pInterfaceScript->HasScript( GetSceneName() );
			}
			if( bHasScript )
			{
				pInterfaceScript->RegisterWidget( this );
			}
		}


		OnCreate();
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::NotifyLoaded()
	{
		if( IsGenerateLoadEvent() )
		{
			//notify self
			CGUIEventNotification aEvent;
			aEvent.SetEventId(eEVENT_LOAD);
			aEvent.SetReceiver(this);
			GSystem->SendEvent( &aEvent);
		}

		//children's
		CGUIWidget*	pWidget = GetChild();
		while(pWidget)
		{
			pWidget->NotifyLoaded();
			pWidget = pWidget->GetNextSibling();
		}	
	}
	//------------------------------------------------------------------------------
	/**
	* @brief open the widget
	*/
	void CGUIWidget::Open()
	{
		//open self
		if( IsOpen())
		{
			return;
			//GUI_THROW( GUI_FORMAT("[CGUIWidget::Open]: the widget isn't in closed state. TYPE<%s>  NAME<%s>",
			//	GetType().c_str(), GetName().c_str()));
		}

		m_bIsOpen = true;
		CGUIEventNotification aEvent;
		aEvent.SetEventId(eEVENT_OPEN);
		aEvent.SetReceiver(this);
		GSystem->SendEvent( &aEvent);

		//open child if it is set to "open with parent"
		CGUIWidget*	pWidget = GetChild();
		while(pWidget)
		{
			if( pWidget->IsOpenWithParent() && pWidget->IsOpen()==false)
			{
				pWidget->Open();
			}
			pWidget = pWidget->GetNextSibling();
		}	
	}
	//------------------------------------------------------------------------------
	/**
	* @brief close the widget
	* now just send a close event and make this widget invisible
	*/
	void CGUIWidget::Close()
	{
		//close self
		if( !IsOpen())
		{
			return;
		}

		m_bIsOpen = false;
		//send event
		CGUIEventNotification aEvent;
		aEvent.SetEventId(eEVENT_CLOSE);
		aEvent.SetReceiver(this);
		GSystem->SendEvent( &aEvent);

		//close child
		CGUIWidget* pWidget = GetChild();
		while( pWidget )
		{
			if( pWidget->IsOpen() )
			{
				pWidget->Close();
			}
			pWidget = pWidget->GetNextSibling();
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief open the widget
	*/
	bool CGUIWidget::IsOpen() const
	{
		return m_bIsOpen;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsCreate() const
	{
		return m_bIsCreate;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetUserData(void*	pData)
	{
		m_pUserData = pData;
	}
	//------------------------------------------------------------------------------
	void* CGUIWidget::GetUserData() const
	{
		return m_pUserData;
	}
	//------------------------------------------------------------------------------
	sigslot::signal1<CGUIWidget*>& CGUIWidget::GetOnWidgetDestroyedSignal()
	{
		return OnWidgetDestroyed;
	}
	//------------------------------------------------------------------------------
	CGUIStringRender* CGUIWidget::GetTooltipText(void) const
	{
		return NULL;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetTooltipText(const CGUIStringRender& /*rText*/)
	{
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::HasTooltips( ) const
	{
		return GetTooltipText() !=  NULL;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetGenerator( const CGUIWidgetGenerator* pGenerator)
	{
		m_pWidgetGenerator = pGenerator;
	}
	//------------------------------------------------------------------------------
	const CGUIWidgetGenerator* CGUIWidget::GetGenerator() const
	{
		return m_pWidgetGenerator;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetParentImpl( CGUIWidget* pParent )
	{
		if( pParent == m_pParent )
		{
			return;
		}

		if( m_pParent )
		{
			//for parameter
			m_pParent->m_aParamScale.RemoveChild(&(m_aParamScale));
			m_pParent->m_aParamAlpha.RemoveChild(&(m_aParamAlpha));
			m_pParent->m_aParamDisable.RemoveChild(&(m_aParamDisable));
			m_pParent->m_aParamActivable.RemoveChild(&(m_aParamActivable));
			m_pParent->m_aParamVisible.RemoveChild(&(m_aParamVisible));
		}
		m_pParent = pParent;
		if( m_pParent )
		{
			m_pParent->m_aParamScale.AddChild(&(m_aParamScale));
			m_pParent->m_aParamAlpha.AddChild(&(m_aParamAlpha));
			m_pParent->m_aParamDisable.AddChild(&(m_aParamDisable));
			m_pParent->m_aParamActivable.AddChild(&(m_aParamActivable));
			m_pParent->m_aParamVisible.AddChild(&(m_aParamVisible));

			//send event for add child
			if( m_pParent->IsGenerateParentChildEvent() )
			{
				CGUIEventRelativeChange aEvent;
				aEvent.SetEventId(eEVENT_ADD_CHILD);
				aEvent.SetRelative(this);
				aEvent.SetReceiver(m_pParent);
				GSystem->SendEvent( &aEvent);
			}
		}

		//send event for change parent
		if( IsGenerateParentChildEvent() )
		{
			CGUIEventRelativeChange aEvent;
			aEvent.SetEventId(eEVENT_PARENT_CHANGED);
			aEvent.SetRelative(pParent);
			aEvent.SetReceiver(this);
			GSystem->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	/// set child
	void CGUIWidget::SetChildImpl( CGUIWidget* pChild )
	{
		m_pChild = pChild;
		if( m_pChild )
		{
			m_pChild->m_pPrevSibling = NULL;
		}
	}
	//-----------------------------------------------------------------------	
	/// set next sibling
	void CGUIWidget::SetNextSiblingImpl( CGUIWidget* pNextSibling )
	{
		m_pNextSibling = pNextSibling;
		if( m_pNextSibling )
		{
			m_pNextSibling->m_pPrevSibling = this;
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set parent
	*/
	void CGUIWidget::SetParent(CGUIWidget *pParent)
	{
		if( GetParent() == pParent )
		{
			return;
		}

		//remove from old parent
		if( GetParent() )
		{
			GetParent()->RemoveChild(this);
		}

		//add to current parent
		if( pParent )
		{
			pParent->AddChild(this);
		}
		else
		{
			SetParentImpl( NULL );
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get parent
	*/
	CGUIWidget* CGUIWidget::GetParent() const
	{
		return m_pParent;
	}
	//------------------------------------------------------------------------------
	CGUINode* CGUIWidget::NodeGetParent()
	{
		return m_pParent;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get child
	*/
	CGUIWidget*	CGUIWidget::GetChild( ) const
	{
		return m_pChild;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get child by given name
	*/
	CGUIWidget*	CGUIWidget::GetChild( const CGUIString& rChildName ) const
	{
		CGUIWidget* pWidget = GetChild();
		while( pWidget )
		{
			if( pWidget->GetName() == rChildName )
			{
				return pWidget;
			}
			pWidget = pWidget->GetNextSibling();
		}
		return NULL;
	}
	//------------------------------------------------------------------------------
	/// get next sibling
	CGUIWidget*	CGUIWidget::GetNextSibling( ) const
	{
		return m_pNextSibling;
	}
	//------------------------------------------------------------------------------
	CGUIWidget* CGUIWidget::GetPrevSibling( ) const
	{
		return m_pPrevSibling;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetWorkingSceneName(const CGUIString& rWorkingProjName)
	{
		m_strWorkingSceneName = rWorkingProjName;

		CGUIWidget* pWidget = GetChild();
		while( pWidget )
		{
			pWidget->SetWorkingSceneName( rWorkingProjName );
			pWidget = pWidget->GetNextSibling();
		}
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIWidget::GetWorkingSceneName( ) const
	{
		return m_strWorkingSceneName;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief move down this widget between his sibling
	*/
	void CGUIWidget::MoveDown()
	{
		if( GetParent() )
		{
			//check for only one child, or have been last one
			if( !GetParent()->GetChild()->GetNextSibling() ||
				GetParent()->GetChild() == this )
			{
				return;
			}

			//check for second one
			if( this == GetParent()->GetChild()->GetNextSibling() )
			{
				GetParent()->GetChild()->SetNextSiblingImpl( GetNextSibling() );
				SetNextSiblingImpl( GetParent()->GetChild() );
				GetParent()->SetChildImpl( this );
			}
			else
			{
				//find my brother's brother
				CGUIWidget* pWidget = GetParent()->GetChild();
				while( pWidget && pWidget->GetNextSibling())
				{
					if( pWidget->GetNextSibling()->GetNextSibling() == this )
					{
						break;
					}
					pWidget = pWidget->GetNextSibling();
				}
				GUI_ASSERT( pWidget, "error");
				pWidget->GetNextSibling()->SetNextSiblingImpl( GetNextSibling( ));
				SetNextSiblingImpl( pWidget->GetNextSibling());
				pWidget->SetNextSiblingImpl( this );
			}
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief move up this widget between his sibling
	*/
	void	CGUIWidget::MoveUp()
	{
		if( GetParent() )
		{
			//check for only one child, or have topest
			if( !GetParent()->GetChild()->GetNextSibling() ||
				!GetNextSibling() )
			{
				return;
			}

			//check for first
			if( this == GetParent()->GetChild() )
			{
				GetParent()->SetChildImpl( GetParent()->GetChild()->GetNextSibling());
				SetNextSiblingImpl( GetParent()->GetChild()->GetNextSibling() );
				GetParent()->GetChild()->SetNextSiblingImpl( this );
			}
			else
			{
				//find my brother
				CGUIWidget* pWidget = GetParent()->GetChild();
				while( pWidget && pWidget->GetNextSibling())
				{
					if( pWidget->GetNextSibling() == this )
					{
						break;
					}
					pWidget = pWidget->GetNextSibling();
				}
				GUI_ASSERT( pWidget, "error");
				pWidget->SetNextSiblingImpl( GetNextSibling( ));
				SetNextSiblingImpl( pWidget->GetNextSibling()->GetNextSibling());
				pWidget->GetNextSibling()->SetNextSiblingImpl( this );

			}
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief move this widget to the top between
	* his sibling.
	*/
	void CGUIWidget::MoveToTop()
	{
		if( GetParent() )
		{
			//check for only one child
			if( !GetParent()->GetChild()->GetNextSibling())
			{
				return;
			}

			//check for first
			if( this == GetParent()->GetChild() )
			{
				GetParent()->SetChildImpl( GetParent()->GetChild()->GetNextSibling());
				SetNextSiblingImpl( NULL );
			}

			//find this
			CGUIWidget* pWidget = GetParent()->GetChild();

			while( pWidget && pWidget->GetNextSibling())
			{
				if( pWidget->GetNextSibling() == this )
				{
					break;
				}
				pWidget = pWidget->GetNextSibling();
			}

			if( !pWidget->GetNextSibling() )
			{
				//add to last
				pWidget->SetNextSiblingImpl( this );
			}
			else
			{
				//remove it first
				pWidget->SetNextSiblingImpl( GetNextSibling() );
				SetNextSiblingImpl( NULL );

				//add to last
				while( pWidget )
				{
					if( !pWidget->GetNextSibling() )
					{
						pWidget->SetNextSiblingImpl( this );
						return;
					}
					pWidget = pWidget->GetNextSibling();
				}
			}
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief remove child 
	*/
	void CGUIWidget::RemoveChild( CGUIWidget* pChild )
	{
		GUI_ASSERT( pChild, "invalid parameter");
		GUI_ASSERT( pChild->GetParent() == this, "the child's parent isn't this!");

		if( pChild == GetChild() )
		{
			SetChildImpl( GetChild()->GetNextSibling());
		}
		else
		{
			bool bFind = false;
			CGUIWidget* pParam = GetChild();
			while(pParam && pParam->GetNextSibling())
			{
				if( pParam->GetNextSibling() == pChild )
				{
					pParam->SetNextSiblingImpl( pParam->GetNextSibling()->GetNextSibling() );
					bFind = true;
					break;
				}
				else
				{
					pParam = pParam->GetNextSibling();
				}
			}
			if( !bFind ) 
			{
				GUI_THROW( GUI_FORMAT("[CGUIWidget::RemoveChild]: failed to find widget, type <%s>, name<%s>!",
					pChild->GetType().c_str(), pChild->GetName().c_str()));
			}
		}
		pChild->SetParentImpl( NULL );
		pChild->SetNextSiblingImpl( NULL );

		//send event for remove child
		if( IsGenerateParentChildEvent() )
		{
			CGUIEventRelativeChange aEvent;
			aEvent.SetEventId(eEVENT_REMOVE_CHILD);
			aEvent.SetRelative(pChild);
			aEvent.SetReceiver(this);
			GSystem->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	CGUIWidget*	CGUIWidget::GetLastChild( )
	{
		CGUIWidget* pLastChild = GetChild();
		while( pLastChild )
		{
			if( pLastChild->GetNextSibling() )
			{
				pLastChild = pLastChild->GetNextSibling();
			}
			else
			{
				return pLastChild;
			}
		}
		return NULL;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief add child 
	*/
	void CGUIWidget::AddChild( CGUIWidget* pChild )
	{
		GUI_ASSERT( pChild, "invalid parameter");
		GUI_ASSERT( pChild->GetParent() == NULL, "the child has haven a parent!");

		if( IsAddChildToTail() )
		{
			CGUIWidget* pLastChild = GetLastChild();
			if( pLastChild )
			{
				pLastChild->SetNextSiblingImpl( pChild );
			}
			else
			{
				SetChildImpl( pChild );
			}
			pChild->SetNextSiblingImpl( NULL );
		}
		else
		{
			CGUIWidget* pFirstChild = GetChild();
			if( pFirstChild )
			{
				SetChildImpl( pChild );
				pChild->SetNextSiblingImpl( pFirstChild );
			}
			else
			{
				SetChildImpl( pChild );
			}
		}

		//set parent
		pChild->SetParentImpl( this );
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::InsertChild( CGUIWidget* pWhere, CGUIWidget* pChild )
	{	
		GUI_ASSERT( pChild, "invalid parameter");
		GUI_ASSERT( pChild->GetParent() == NULL, "the child has haven a parent!");
		
		if( !pWhere )
		{
			AddChild( pChild );
		}
		else
		{
			CGUIWidget* pOldTmpChild = NULL;
			for( CGUIWidget* pTmpChild = GetChild(); pTmpChild != NULL; pTmpChild = pTmpChild->GetNextSibling() )
			{
				if( pTmpChild == pWhere )
				{
					pChild->SetNextSiblingImpl( pWhere );
					if( pOldTmpChild )
					{
						pOldTmpChild->SetNextSiblingImpl( pChild );
					}	
					//set parent
					pChild->SetParentImpl( this );

					return;
				}
				else
				{
					pOldTmpChild = pTmpChild;
				}
			}

			GUI_THROW( GUI_FORMAT("[CGUIWidget::InsertChild]: failed to find where to insert child! TYPE<%s>  NAME<%s>",
				pWhere->GetType().c_str(),pWhere->GetName().c_str()));
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetActivable(bool bActivable)
	{
		m_aParamActivable.SetSelfValue(bActivable);
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsActivable( ) const
	{
		return m_aParamActivable.GetSelfValue();
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsDerivedActivable()
	{
		return m_aParamActivable.GetFinalValue();
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetFocus(bool bFocus)
	{
		if( bFocus && IsFocusAgency())
		{
			//is a focus agency
			if( GetParent() )
			{
				GetParent()->SetFocus( bFocus);
				return;
			}
			GUI_THROW( GUI_FORMAT("[CGUIWidget::SetFocus]: lack of parent! TYPE<%s>  NAME<%s>",
				GetType().c_str(),GetName().c_str()));
		}

		if( !bFocus )
		{
			/// lost focus
			if( GSystem->GetFocusWidget()  == this)
			{
				CGUIEventNotification aEvent;
				aEvent.SetEventId(eEVENT_FOCUS_LOST);
				aEvent.SetReceiver(this);
				GSystem->SendEvent( &aEvent);
			}
			return;
		}


		if( GSystem->GetFocusWidget()  == this)
		{
			return;
		}
		//remove old widget
		if( GSystem->GetFocusWidget() )
		{
			CGUIEventNotification aEvent;
			aEvent.SetEventId(eEVENT_FOCUS_LOST);
			aEvent.SetReceiver(GSystem->GetFocusWidget());
			GSystem->SendEvent( &aEvent);
		}

		//add this widget 
		if( IsFocusable())
		{
			CGUIEventNotification aEvent;
			aEvent.SetEventId(eEVENT_FOCUS_GET);
			aEvent.SetReceiver(this);
			GSystem->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidget::IsFocus() const
	{
		if( IsFocusAgency() )
		{
			//is a focus agency
			if( GetParent() )
			{
				return GetParent()->IsFocus( );
			}
			GUI_THROW( GUI_FORMAT("[CGUIWidget::IsFocus]: lack of parent! TYPE<%s>  NAME<%s>",
				GetType().c_str(),GetName().c_str()));
		}

		return GSystem->GetFocusWidget() == this;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetVisible(bool bVisible)
	{
		if( bVisible != IsVisible() )
		{
			return m_aParamVisible.SetSelfValue(bVisible);

			CGUIEventNotification aEvent;
			if( bVisible )
			{
				aEvent.SetEventId(eEVENT_VISIBLE);
			}
			else
			{
				aEvent.SetEventId(eEVENT_INVISIBLE);
			}
			aEvent.SetReceiver(this);
			GSystem->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidget::IsVisible() const
	{
		return m_aParamVisible.GetSelfValue();
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidget::IsDerivedVisible()
	{
		return m_aParamVisible.GetFinalValue();
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetAutoPlayAs( bool bEnable )
	{
		m_bIsAutoPlayAs = bEnable;

		//if( m_bIsAutoPlayAs )
		//{
		//	for( TMapAs::iterator itor = m_aMapAs.begin();
		//		itor != m_aMapAs.end();
		//		++itor )
		//	{
		//		CGUIAs* pAs = itor->second;
		//		if( IsAsPlaying( pAs ) == false )
		//		{
		//			PlayAs( pAs );
		//		}
		//	}
		//}
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsAutoPlayAs( ) const
	{
		return m_bIsAutoPlayAs;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetClipChildren( bool bClip )
	{
		m_bIsClipChildren = bClip;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsClipChildren( ) const
	{
		return m_bIsClipChildren;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetDisable(bool bDisable)
	{
		if( bDisable != IsDisable() )
		{
			CGUIEventNotification aEvent;
			if( bDisable )
			{
				aEvent.SetEventId(eEVENT_DISABLE);
			}
			else
			{
				aEvent.SetEventId(eEVENT_ENABLE);
			}
			aEvent.SetReceiver(this);
			GSystem->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsDisable( ) const
	{
		return m_aParamDisable.GetSelfValue();
	}
	//------------------------------------------------------------------------------
	/// is this widget disable, the value will be affect by the parent's state
	bool CGUIWidget::IsDerivedDisable()
	{
		return m_aParamDisable.GetFinalValue();
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::LocalToWorld( const CGUIRect& rRect, CGUIRenderRect& rRenderRect )
	{
		CGUIVector2 aTmpPos;
		//top-left
		aTmpPos.x = rRect.m_fLeft;
		aTmpPos.y = rRect.m_fTop;
		LocalToWorld(aTmpPos);
		rRenderRect.m_vecVertex[0].m_aVector.x = aTmpPos.x;
		rRenderRect.m_vecVertex[0].m_aVector.y = aTmpPos.y;
		//top-right
		aTmpPos.x = rRect.m_fRight;
		aTmpPos.y = rRect.m_fTop;
		LocalToWorld(aTmpPos);
		rRenderRect.m_vecVertex[1].m_aVector.x = aTmpPos.x;
		rRenderRect.m_vecVertex[1].m_aVector.y = aTmpPos.y;
		//bottom-right
		aTmpPos.x = rRect.m_fRight;
		aTmpPos.y = rRect.m_fBottom;
		LocalToWorld(aTmpPos);
		rRenderRect.m_vecVertex[2].m_aVector.x = aTmpPos.x;
		rRenderRect.m_vecVertex[2].m_aVector.y = aTmpPos.y;
		//bottom-left
		aTmpPos.x = rRect.m_fLeft;
		aTmpPos.y = rRect.m_fBottom;
		LocalToWorld(aTmpPos);
		rRenderRect.m_vecVertex[3].m_aVector.x = aTmpPos.x;
		rRenderRect.m_vecVertex[3].m_aVector.y = aTmpPos.y;
	}
	//------------------------------------------------------------------------------
	const CGUIMatrix4& CGUIWidget::GetFullTransform(void)
	{
		return NodeGetFullTransform();
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::LocalToWorld( CGUIVector2& rPos )
	{
		const CGUIMatrix4& rWorldMatrix = NodeGetFullTransform();
		CGUIVector3	aPos(rPos.x,rPos.y,0.0f);
		aPos = rWorldMatrix*aPos;
		rPos.x = aPos.x;
		rPos.y = aPos.y;	
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::WorldToLocal( CGUIVector2& rPos )
	{
		const CGUIMatrix4& rWorldInverseMatrix = NodeGetFullInverseTransform();
		CGUIVector3	aPos(rPos.x,rPos.y,0.0f);
		aPos = rWorldInverseMatrix*aPos;
		rPos.x = aPos.x;
		rPos.y = aPos.y;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::LocalToWorld( CGUIVector3& rPos )
	{
		const CGUIMatrix4& rWorldMatrix = NodeGetFullTransform();
		rPos = rWorldMatrix*rPos;

	}
	//------------------------------------------------------------------------------
	void CGUIWidget::WorldToLocal( CGUIVector3& rPos )
	{
		const CGUIMatrix4& rWorldInverseMatrix = NodeGetFullInverseTransform();
		rPos = rWorldInverseMatrix*rPos;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::ParentToWorld( CGUIVector2& rPos )
	{
		if(GetParent())
		{
			const CGUIMatrix4& rWorldMatrix = GetParent()->NodeGetFullTransform();
			CGUIVector3	aPos(rPos.x,rPos.y,0.0f);
			aPos = rWorldMatrix*aPos;
			rPos.x = aPos.x;
			rPos.y = aPos.y;	
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::WorldToParent( CGUIVector2& rPos )
	{
		if(GetParent())
		{
			const CGUIMatrix4& rWorldInverseMatrix = GetParent()->NodeGetFullInverseTransform();
			CGUIVector3	aPos(rPos.x,rPos.y,0.0f);
			aPos = rWorldInverseMatrix*aPos;
			rPos.x = aPos.x;
			rPos.y = aPos.y;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::ParentToLocal( CGUIVector2& rPos )
	{
		if(GetParent())
		{
			ParentToWorld( rPos );
			WorldToLocal( rPos );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::LocalToParent( CGUIVector2& rPos )
	{
		if(GetParent())
		{
			LocalToWorld( rPos );
			WorldToParent( rPos );
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief give the child which is under given point
	*/
	CGUIWidget* CGUIWidget::GetWidgetAtPoint(const CGUIVector2& rPos)
	{
		if( !IsOpen() || !IsDerivedVisible() ) 
		{
			return NULL;
		}

		bool bHitSelf = HitTest( rPos );
		if( !bHitSelf && !IsForceHitTest())
		{
			return NULL;
		}

		if( GetChild() )
		{
			CGUIWidget* pSibling = GetChild();

			//find last one
			while( pSibling->GetNextSibling() )
			{
				pSibling = pSibling->GetNextSibling();
			}

			while( pSibling )
			{
				CGUIWidget* pHitedWidget = pSibling->GetWidgetAtPoint( rPos );
				if( pHitedWidget )
				{
					return pHitedWidget;
				}
				pSibling = pSibling->GetPrevSibling();
			}
		}
		
		if( bHitSelf && (IsHitable() || GSystem->IsEditorMode()))
		{
			return this;
		}
		else
		{
			return NULL;
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief test whether the point is in this widget
	* @param rPos in pixel format
	*/
	bool CGUIWidget::HitTest( const CGUIVector2& rPos)
	{
		return m_aHitTestArea.IsPointInRect(rPos);
	}
	//------------------------------------------------------------------------------
	CGUISoundData* CGUIWidget::RegisterSound( const CGUIString& strEventName, const CGUIString& rSoundName )
	{
		UnregisterSound( strEventName );
		if( rSoundName.empty() )
		{
			return NULL;
		}

		CGUISoundData* pSound = CGUISoundManager::Instance()->AllocateResource( rSoundName );
		if( !pSound )
		{
			GUI_THROW( GUI_FORMAT( "failed to get sound by name <%s>", rSoundName.c_str()));
			return NULL;
		};
		m_mapEventSound.insert( std::make_pair( strEventName, pSound ) );

		return pSound;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::UnregisterSound( const CGUIString& strEventName )
	{
		TMapSound::iterator itor = m_mapEventSound.find(strEventName);
		if( itor != m_mapEventSound.end())
		{
			itor->second->RefRelease();
			m_mapEventSound.erase(itor);
		}
	}
	//------------------------------------------------------------------------------
	CGUISoundData* CGUIWidget::GetSound( const CGUIString& strEventName )
	{
		TMapSound::iterator itor = m_mapEventSound.find(strEventName);
		if( itor != m_mapEventSound.end())
		{
			return itor->second;
		}
		return NULL;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::RegisterScriptCallbackFunc( 
		const CGUIString& strEventName, 
		const CGUIString& strFunc )
	{
		m_mapScriptFunc.insert( std::make_pair(strEventName, strFunc));
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::UnregisterScriptCallbackFunc(const CGUIString& strEventName)
	{
		TMapScriptFunc::iterator itor = m_mapScriptFunc.find(strEventName);
		if( itor != m_mapScriptFunc.end())
		{
			m_mapScriptFunc.erase(itor);
		}
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidget::HasScriptCallbackFunc(const CGUIString& strEventName) const
	{
		return m_mapScriptFunc.find(strEventName) != m_mapScriptFunc.end();
	}
	//------------------------------------------------------------------------------
	const CGUIString&CGUIWidget::GetScriptCallbackFunc(const CGUIString& strEventName) const
	{
		TMapScriptFunc::const_iterator itor = m_mapScriptFunc.find(strEventName);
		if( itor == m_mapScriptFunc.end())
		{
			GUI_THROW( GUI_FORMAT("[CGUIWidget::GetScriptCallbackFunc]: failed to find event function by name <%s>!", strEventName.c_str()));
		}
		return itor->second;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::UnregisterTimer(const CGUIString& strEventName)
	{
		for( TVecTimer::iterator itor = m_arrayTimer.begin();
			itor != m_arrayTimer.end();
			++itor )
		{
			if( (*itor).m_strTimerName == strEventName )
			{
				m_arrayTimer.erase( itor );
				return;
			}
		}
		GUI_THROW( GUI_FORMAT("[CGUIWidget::UnregisterTimer]: failed to find event <%s>!", strEventName.c_str()));
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::RegisterTimer( const CGUIString& strEventName, real rWaitingTime )
	{
		for( TVecTimer::iterator itor = m_arrayTimer.begin();
			itor != m_arrayTimer.end();
			++itor )
		{
			if( (*itor).m_strTimerName == strEventName )
			{
				m_arrayTimer.erase( itor );
				GUI_THROW( GUI_FORMAT("[CGUIWidget::RegisterTimer]: multiple register event <%s>!", strEventName.c_str()));
				return;
			}
		}
		m_arrayTimer.push_back( STimer(strEventName, rWaitingTime));
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::PlaySound(const CGUIString& strEventName, CGUIEvent* pEvent)
	{
		//play sound
		TMapSound::iterator itor = m_mapEventSound.find( strEventName );
		if( itor != m_mapEventSound.end())
		{
			IGUIInterfaceSound* pInterfaceSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
			if( pInterfaceSound )
			{
				pInterfaceSound->PlayEffect( itor->second );
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::CallGlobalFunction(const CGUIString& strEventName, CGUIEvent* pEvent)
	{
		TMapGlobalFunc::iterator itor = m_mapNativeFunc.find(strEventName);
		if( itor != m_mapNativeFunc.end())
		{
			(*(itor->second))(pEvent);
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::CallScriptFunction(const CGUIString& strEventName, CGUIEvent* pEvent)
	{
		if( !GSystem->IsEditorMode() )
		{

			TMapScriptFunc::iterator itor = m_mapScriptFunc.find(strEventName);
			if( itor != m_mapScriptFunc.end())
			{
				//get script interface
				guiex::IGUIInterfaceScript* pScript = CGUIInterfaceManager::Instance()->GetInterfaceScript();
				pScript->ExecuteEventHandler( itor->second, pEvent, GetWorkingSceneName());
			}
		}

	}
	//------------------------------------------------------------------------------
	void CGUIWidget::CallbackFunction(const CGUIString& strEventName, CGUIEvent* pEvent)
	{		
		//play sound
		PlaySound( strEventName, pEvent );

		//call global function
		CallGlobalFunction( strEventName, pEvent );

		//call script function
		CallScriptFunction( strEventName, pEvent );
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetScale( const CGUISize& rSize )
	{
		if(m_aParamScale.GetSelfValue() != rSize )
		{
			CGUISize aSize( rSize );
			if( aSize.m_fHeight <= 0.0f )
			{
				aSize.m_fHeight = 0.001f;
			}
			if( aSize.m_fWidth <= 0.0f )
			{
				aSize.m_fWidth = 0.001f;
			}

			m_aParamScale.SetSelfValue(aSize);

			//send event
			if( IsGenerateScaleChangeEvent())
			{
				CGUIEventNotification aEvent;
				aEvent.SetEventId(eEVENT_SCALE_CHANGE);
				aEvent.SetReceiver(this);
				GSystem->SendEvent( &aEvent );
			}
		}
	}
	//------------------------------------------------------------------------------
	const CGUISize& CGUIWidget::GetScale( ) const
	{
		return m_aParamScale.GetSelfValue();
	}
	//------------------------------------------------------------------------------
	const CGUISize & CGUIWidget::GetDerivedScale()
	{
		return m_aParamScale.GetFinalValue();
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetAlpha(real fAlpha)
	{
		if( GetAlpha() != fAlpha )
		{
			m_aParamAlpha.SetSelfValue(fAlpha);

			CGUIEventAlpha aEvent;
			aEvent.SetEventId(eEVENT_CHANGE_ALPHA);
			aEvent.SetAlpha(fAlpha);
			aEvent.SetReceiver(this);
			GSystem->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	real CGUIWidget::GetAlpha()  const
	{
		return m_aParamAlpha.GetSelfValue();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get the alpha which used to render this widget.
	* this value will be affected by both this widget's alpha and inheritable flag.
	*/
	real CGUIWidget::GetDerivedAlpha() 
	{
		if( IsInheritAlpha() && GetParent() )
		{
			return m_aParamAlpha.GetFinalValue();
		}
		else
		{
			return m_aParamAlpha.GetSelfValue();
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief callback of set the image of widget.
	*/
	void CGUIWidget::OnSetImage( const CGUIString& rName, CGUIImage* pImage )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::OnSetAnimation( const CGUIString& rName, CGUIAnimation* pAnimation )
	{

	}
	//------------------------------------------------------------------------------
	void CGUIWidget::OnCreate()
	{
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::OnUpdate( real fDeltaTime )
	{
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::OnDestroy()
	{
		OnWidgetDestroyed( this );
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		//clear old one
		TMapImage::iterator itor = m_aMapImage.find(rName );
		if( itor != m_aMapImage.end())
		{
			itor->second->RefRelease();
			m_aMapImage.erase( itor );
		}

		OnSetImage( rName, pImage );
		if( pImage )
		{
			pImage->RefRetain();
			m_aMapImage.insert( std::make_pair( rName, pImage));
		}
	}
	//------------------------------------------------------------------------------
	CGUIImage*	CGUIWidget::SetImage( const CGUIString& rName, const CGUIString& rImageName )
	{
		//find image
		CGUIImage* pImage = CGUIImageManager::Instance()->AllocateResource( rImageName );
		if( !pImage )
		{
			GUI_THROW( GUI_FORMAT( "failed to get image by name <%s>", rImageName.c_str()));
			return NULL;
		};
		SetImage(rName, pImage);
		pImage->RefRelease();
		return pImage;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetAnimation( const CGUIString& rName, CGUIAnimation* pAnimation )
	{
		TMapAnimation::iterator itor = m_aMapAnimation.find(rName);
		if( itor != m_aMapAnimation.end())
		{
			//remove old one
			itor->second->RefRelease();
			m_aMapAnimation.erase(itor);
		}

		OnSetAnimation( rName, pAnimation );
		if( pAnimation )
		{
			pAnimation->RefRetain();
			m_aMapAnimation.insert(std::make_pair(rName,pAnimation));
		}
	}
	//------------------------------------------------------------------------------
	CGUIAnimation* CGUIWidget::SetAnimation( const CGUIString& rName, const CGUIString& rAnimationName )
	{
		//find animation
		CGUIAnimation* pAnimation = CGUIAnimationManager::Instance()->AllocateResource( rAnimationName );
		if( !pAnimation )
		{
			GUI_THROW( GUI_FORMAT( "failed to get animation by name <%s>", rAnimationName.c_str()));
			return NULL;
		};
		SetAnimation(rName, pAnimation);
		pAnimation->RefRelease();
		return pAnimation;
	}
	//------------------------------------------------------------------------------
	CGUIAnimation* CGUIWidget::GetAnimation( const CGUIString& rAnimationName)
	{
		TMapAnimation::iterator itor = m_aMapAnimation.find(rAnimationName);
		if( itor != m_aMapAnimation.end())
		{
			return itor->second;
		}
		else
		{
			return NULL;
		}
	}
	//------------------------------------------------------------------------------
	CGUIAs* CGUIWidget::SetAs( const CGUIString& rName, const CGUIString& rAsName )
	{
		//find as
		CGUIAs* pAs = CGUIAsManager::Instance()->AllocateResource( rAsName );
		if( !pAs )
		{
			GUI_THROW( GUI_FORMAT( "failed to get as by name <%s>", rAsName.c_str()));
			return NULL;
		};
		pAs->SetReceiver( this );
		SetAs( rName, pAs );
		pAs->RefRelease();
		return pAs;
	}
	//------------------------------------------------------------------------------
	///< this will retain the reference count of image
	void CGUIWidget::SetAs( const CGUIString& rName, CGUIAs* pAs )
	{
		TMapAs::iterator itor = m_aMapAs.find(rName);
		if( itor != m_aMapAs.end())
		{
			//remove old one
			itor->second->RefRelease();
			m_aMapAs.erase(itor);
		}
		if( pAs )
		{
			pAs->RefRetain();
			m_aMapAs.insert(std::make_pair(rName,pAs));

			if( m_bIsAutoPlayAs )
			{
				PlayAs( pAs );
			}
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief has the as been add to this widget.
	*/
	bool CGUIWidget::HasAs( const CGUIString& rName )
	{
		return m_aMapAs.find(rName ) != m_aMapAs.end();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get as
	* @return NULL for failed to find as by given name
	*/
	CGUIAs* CGUIWidget::GetAs( const CGUIString& rName )
	{
		TMapAs::iterator itor = m_aMapAs.find(rName);
		if( itor != m_aMapAs.end())
		{
			return itor->second;
		}
		else
		{
			return NULL;
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::PlayAs( const CGUIString& rName )
	{
		CGUIAs* pAs = GetAs( rName );
		if( !pAs )
		{
			GUI_THROW( GUI_FORMAT(
				"[CGUIWidget::PlayAs]: failed to play as <%s>",
				rName.c_str()));
			return;
		}
		PlayAs( pAs );
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsAsPlaying( const CGUIString& rName )
	{
		CGUIAs* pAs = GetAs( rName );
		if( !pAs )
		{
			GUI_THROW( GUI_FORMAT(
				"[CGUIWidget::IsAsPlaying]: failed to check whether as <%s> is playing",
				rName.c_str()));
			return false;
		}
		return IsAsPlaying(pAs);
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::StopAs( const CGUIString& rName )
	{
		CGUIAs* pAs = GetAs( rName );
		if( !pAs )
		{
			GUI_THROW( GUI_FORMAT(
				"[CGUIWidget::StopAs]: failed to stop as <%s>",
				rName.c_str()));
			return;
		}
		StopAs( pAs );
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::StopAllAs( )
	{
		for( TListAs::iterator itor = m_listAsPlaying.begin();
			itor != m_listAsPlaying.end();
			++itor )
		{
			(*itor)->RefRelease();
		}
		m_listAsPlaying.clear();
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::PlayAs( CGUIAs* pAs )
	{
		TListAs::iterator itor = std::find( m_listAsPlaying.begin(), m_listAsPlaying.end(), pAs );
		if( itor == m_listAsPlaying.end() )
		{
			pAs->Reset( );
			pAs->RefRetain();
			m_listAsPlaying.push_back( pAs );

			pAs->Update(0.0f);
		}
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsAsPlaying( CGUIAs* pAs )
	{
		TListAs::iterator itor = std::find( m_listAsPlaying.begin(), m_listAsPlaying.end(), pAs );
		if( itor == m_listAsPlaying.end() )
		{
			return false;
		}
		return true;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::StopAs( CGUIAs* pAs )
	{
		TListAs::iterator itor = std::find( m_listAsPlaying.begin(), m_listAsPlaying.end(), pAs );
		if( itor != m_listAsPlaying.end() )
		{
			pAs->RefRelease();
			m_listAsPlaying.erase( itor );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::UpdateAsSelf( real fDeltaTime )
	{
		if( GSystem->IsPlayingAs() == false )
		{
			return;
		}

		TListAs	listAsBuffer;
		bool bIsAsPlaying = !m_listAsPlaying.empty();

		while( !m_listAsPlaying.empty())
		{
			//pop as
			CGUIAs *pAs = m_listAsPlaying.back();
			m_listAsPlaying.pop_back();

			//process event
			pAs->Update( fDeltaTime );

			if( pAs->IsRetired())
			{
				//the as has retired, push his successor
				for( uint32 i=0; i<pAs->GetSuccessorNum(); ++i )
				{
					CGUIAs* pSuccessor = pAs->GetSuccessor( i );
					pSuccessor->GetReceiver()->PlayAs(pSuccessor);
				}
				pAs->RefRelease();
			}
			else
			{
				//process this event again
				listAsBuffer.push_back(pAs);
			}
		}

		//swap list
		m_listAsPlaying.swap(listAsBuffer);

		if( bIsAsPlaying )
		{
			Refresh();
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetSceneEffect( CGUISceneEffect* pSceneEffect )
	{
		if( m_pSceneEffect )
		{
			m_pSceneEffect->RefRelease();
		}
		m_pSceneEffect = pSceneEffect;
		if( m_pSceneEffect )
		{
			m_pSceneEffect->RefRetain();
		}
	}
	//------------------------------------------------------------------------------
	CGUISceneEffect* CGUIWidget::GetSceneEffect() const
	{
		return m_pSceneEffect;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::HasAnimation( const CGUIString& rName )
	{
		return m_aMapAnimation.find(rName ) != m_aMapAnimation.end();
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::HasImage( const CGUIString& rName )
	{
		return m_aMapImage.find(rName ) != m_aMapImage.end();
	}
	//------------------------------------------------------------------------------
	CGUIImage* CGUIWidget::GetImage( const CGUIString& rName )
	{
		TMapImage::iterator itor=  m_aMapImage.find(rName);
		if( itor == m_aMapImage.end())
		{
			return NULL;
		}
		else
		{
			return itor->second;
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief destroy all image and animation
	*/
	void CGUIWidget::DestroyAllResource( )
	{
		//release image
		for( TMapImage::iterator itor = m_aMapImage.begin();
			itor != m_aMapImage.end();
			++itor)
		{
			itor->second->RefRelease();
		}
		m_aMapImage.clear();

		//release animation
		for( TMapAnimation::iterator itor = m_aMapAnimation.begin();
			itor != m_aMapAnimation.end();
			++itor)
		{
			itor->second->RefRelease();
		}
		m_aMapAnimation.clear();

		//release sound
		for( TMapSound::iterator itor = m_mapEventSound.begin();
			itor != m_mapEventSound.end();
			++itor)
		{
			CGUISoundManager::Instance()->DeallocateResource( itor->second );
		}
		m_mapEventSound.clear();

		//release as
		for( TMapAs::iterator itor = m_aMapAs.begin();
			itor != m_aMapAs.end();
			++itor)
		{
			itor->second->RefRelease();
		}
		m_aMapAs.clear();
		for( TListAs::iterator itor = m_listAsPlaying.begin();
			itor != m_listAsPlaying.end();
			++itor)
		{
			(*itor)->RefRelease();
		}
		m_listAsPlaying.clear();

		//clear scene capture
		if( m_pSceneEffect )
		{
			m_pSceneEffect->RefRelease();
			m_pSceneEffect = NULL;
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set property set of this widget, if this widget has gotten the 
	* property, the old property will be destroyed.
	*/
	void CGUIWidget::SetProperty( const CGUIProperty& rProperty)
	{
		ClearProperty();

		m_aPropertySet = rProperty;
		CGUIString strTempName;
		if( m_aPropertySet.HasDuplicatedNames( &strTempName ) )
		{
			GUI_THROW( GUI_FORMAT(
				"[CGUIWidget::SetProperty]: widget <%s> find duplicated name <%s> in property", 
				GetName().c_str(), 
				strTempName.c_str()));
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set property of this widget, if this widget has exist, reset it.
	* if,this widget hasn't this property, add it
	*/
	void CGUIWidget::InsertProperty( const CGUIProperty& rProperty)
	{
		CGUIProperty* pProp = m_aPropertySet.GetProperty(rProperty.GetName(), rProperty.GetType());
		if( pProp )
		{
			*pProp = rProperty;
		}
		else
		{
			m_aPropertySet.AddProperty(rProperty);
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get property of this widget
	*/
	const CGUIProperty&	CGUIWidget::GetProperty() const
	{
		return m_aPropertySet;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief clear current property
	*/
	void CGUIWidget::ClearProperty()
	{
		m_aPropertySet.Clear();
	}
	//------------------------------------------------------------------------------
	int32 CGUIWidget::GenerateProperty( CGUIProperty& rProperty )
	{
		if( rProperty.GetType() == ePropertyType_WidgetPosition && rProperty.GetName() == "position")
		{
			ValueToProperty( m_aWidgetPosition, rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_WidgetSize && rProperty.GetName() == "size")
		{
			ValueToProperty( m_aWidgetSize, rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_Color && rProperty.GetName() == "color")
		{
			ValueToProperty( m_aColor, rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_Vector2 && rProperty.GetName() == "anchor" )
		{
			ValueToProperty( m_aWidgetAnchorPoint, rProperty );
		}
		else if(  rProperty.GetType()== ePropertyType_Size && rProperty.GetName() == "scale" )
		{
			ValueToProperty( GetScale(), rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_Size && rProperty.GetName() == "max_size" )
		{
			ValueToProperty( m_aMaxSize, rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_Size && rProperty.GetName() == "min_size" )
		{
			ValueToProperty( m_aMinSize, rProperty );
		}
		else if( rProperty.GetType() == ePropertyType_Image  )
		{
			CGUIImage* pImage = GetImage(rProperty.GetName());
			if( pImage )
			{
				rProperty.SetValue( pImage->GetName() );
			}
			else
			{
				rProperty.SetValue( "" );
			}
		}
		else if( rProperty.GetType() == ePropertyType_Sound  )
		{
			CGUISoundData* pSound = GetSound( rProperty.GetName() );
			if( pSound )
			{
				rProperty.SetValue( pSound->GetName() );
			}
			else
			{
				rProperty.SetValue( "" );
			}
		}
		else if( rProperty.GetType() == ePropertyType_Event  )
		{
			if( HasScriptCallbackFunc(rProperty.GetName()) )
			{
				rProperty.SetValue( GetScriptCallbackFunc(rProperty.GetName()) );
			}
			else
			{
				rProperty.SetValue( "" );
			}
		}
		else if( rProperty.GetType() == ePropertyType_As  )
		{
			CGUIAs* pAs = GetAs(rProperty.GetName());
			if( pAs )
			{
				rProperty.SetValue( pAs->GetName() );
			}
			else
			{
				rProperty.SetValue( "" );
			}
		}
		else if( rProperty.GetType() == ePropertyType_Real && rProperty.GetName() == "alpha" )
		{
			ValueToProperty( GetAlpha(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Vector3 && rProperty.GetName() == "rotation" )
		{
			ValueToProperty( GetRotation(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "activable" )
		{
			ValueToProperty( IsActivable(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "disable" )
		{
			ValueToProperty( IsDisable(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "auto_play_as" )
		{
			ValueToProperty( IsAutoPlayAs(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "open_with_parent" )
		{
			ValueToProperty( IsOpenWithParent(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "generate_update_event" )
		{
			ValueToProperty( IsGenerateUpdateEvent(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "generate_click_event" )
		{
			ValueToProperty( IsGenerateClickEvent(), rProperty);
		}		
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "generate_load_event" )
		{
			ValueToProperty( IsGenerateLoadEvent(), rProperty);
		}		
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "generate_parentsizechange_event" )
		{
			ValueToProperty( IsGenerateParentSizeChangeEvent(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "movable" )
		{
			ValueToProperty( IsMovable(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "visible" )
		{
			ValueToProperty( IsVisible(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "disable" )
		{
			ValueToProperty( IsDisable(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "force_hittest" )
		{
			ValueToProperty( IsForceHitTest(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "hitable" )
		{
			ValueToProperty( IsHitable(), rProperty);
		}
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName()=="focusable" )
		{
			ValueToProperty( IsFocusable(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "clip_children" )
		{
			ValueToProperty( IsClipChildren(), rProperty);
		}
		else if( rProperty.GetType() == ePropertyType_String && rProperty.GetName() == "parent" )
		{
			if( GetParent() && !CGUIWidgetManager::Instance()->IsInternalName( GetParent()->GetName()) )
			{
				rProperty.SetValue(GetParent()->GetName());
			}
			else
			{
				rProperty.SetValue("");
			}
		}
		else
		{
			//failed
			GUI_THROW( GUI_FORMAT("[CGUIWidget::GenerateProperty]: failed to generate property: name=[%s] type=[%s]!", 
				rProperty.GetName().c_str(),
				rProperty.GetTypeAsString().c_str()));

			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::ProcessProperty( const CGUIProperty& rProperty )
	{
		if(rProperty.GetType() == ePropertyType_String && rProperty.GetName() == "parent")
		{
			CGUIWidget* pParent = NULL;
			if( !rProperty.GetValue().empty() )
			{
				pParent = CGUIWidgetManager::Instance()->GetWidget( rProperty.GetValue(), GetSceneName());
			}
			SetParent( pParent);
		}
		else if( rProperty.GetType() == ePropertyType_WidgetPosition && rProperty.GetName() == "position")
		{
			PropertyToValue( rProperty, m_aWidgetPosition);
		}
		else if( rProperty.GetType() == ePropertyType_WidgetSize && rProperty.GetName() == "size")
		{
			PropertyToValue( rProperty, m_aWidgetSize);
		}
		else if( rProperty.GetType() == ePropertyType_Color && rProperty.GetName() == "color")
		{
			PropertyToValue( rProperty, m_aColor );
		}
		else if(  rProperty.GetType()== ePropertyType_Sound)
		{
			RegisterSound( rProperty.GetName(), rProperty.GetValue());
		}
		else if( rProperty.GetType() == ePropertyType_Image )
		{
			if(!rProperty.GetValue().empty())
			{
				SetImage( rProperty.GetName(), rProperty.GetValue());
			}
			else
			{
				//clear image
				SetImage( rProperty.GetName(), NULL);
			}
		}
		else if( rProperty.GetType() == ePropertyType_As )
		{
			if( !rProperty.GetValue().empty() )
			{
				SetAs( rProperty.GetName(), rProperty.GetValue());
			}
			else
			{
				//clear as
				SetAs( rProperty.GetName(), NULL);
			}
		}
		else if( rProperty.GetType()== ePropertyType_Event )
		{
			if( rProperty.GetValue().empty())
			{
				UnregisterScriptCallbackFunc(rProperty.GetName());
			}
			else
			{
				RegisterScriptCallbackFunc( rProperty.GetName(), rProperty.GetValue());
			}
		}
		else if(  rProperty.GetType()== ePropertyType_Vector2 && rProperty.GetName() == "anchor" )
		{
			CGUIVector2 aValue;
			PropertyToValue( rProperty, aValue );
			SetAnchorPoint( aValue );
		}
		else if(  rProperty.GetType()== ePropertyType_Size && rProperty.GetName() == "scale" )
		{
			CGUISize aValue;
			PropertyToValue( rProperty, aValue );
			SetScale( aValue );
		}
		else if(  rProperty.GetType()== ePropertyType_Size && rProperty.GetName() == "max_size" )
		{
			CGUISize aSize;
			PropertyToValue( rProperty, aSize );
			SetMaximumSize(aSize);
		}
		else if( rProperty.GetType()== ePropertyType_Size && rProperty.GetName() == "min_size" )
		{
			CGUISize aSize;
			PropertyToValue( rProperty, aSize );
			SetMaximumSize( aSize );
		}
		else if(  rProperty.GetType()== ePropertyType_Real && rProperty.GetName()=="alpha" )
		{
			real fAlpha = 0.f;
			PropertyToValue( rProperty, fAlpha );
			SetAlpha( fAlpha );
		}
		else if(  rProperty.GetType()== ePropertyType_Vector3 && rProperty.GetName()=="rotation" )
		{
			CGUIVector3 vRotation;
			PropertyToValue( rProperty, vRotation );
			SetRotation( vRotation );
		}
		else if(  rProperty.GetType()== ePropertyType_Bool &&  rProperty.GetName()=="movable" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetMovable(bValue);
		}
		else if(  rProperty.GetType()== ePropertyType_Bool &&  rProperty.GetName()=="visible" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetVisible( bValue );
		}
		else if(  rProperty.GetType()== ePropertyType_Bool &&  rProperty.GetName()=="disable" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetDisable( bValue );
		}
		else if( rProperty.GetType()== ePropertyType_Bool &&  rProperty.GetName()=="auto_play_as" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetAutoPlayAs( bValue );
		}
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "force_hittest" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetForceHitTest( bValue );
		}
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName()=="hitable" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetHitable( bValue );
		}
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName()=="focusable" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetFocusable( bValue );
		}
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName()=="clip_children" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetClipChildren( bValue );
		}
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName()=="activable" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetActivable( bValue );
		}
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName()=="open_with_parent" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetOpenWithParent( bValue );
		}
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName()=="generate_update_event" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetGenerateUpdateEvent(bValue );
		}
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName()=="generate_click_event" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetGenerateClickEvent(bValue );
		}
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName()=="generate_load_event" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetGenerateLoadEvent(bValue );
		}
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName()=="generate_parentsizechange_event" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetGenerateParentSizeChangeEvent(bValue );
		}		
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName()=="generate_dbclick_event" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetGenerateDBClickEvent( bValue );
		}
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName() == "DISABLE" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetDisable(bValue);
		}
		else
		{ 
			GUI_THROW( GUI_FORMAT("[CGUIWidget::ProcessProperty]: widget <%s> failed to process property: name=[%s] type=[%s] value=[%s]!", 
				GetName().c_str(),
				rProperty.GetName().c_str(),
				rProperty.GetTypeAsString().c_str(),
				rProperty.GetValue().c_str()));
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::LoadFromProperty( const CGUIProperty& rProperty )
	{
		//set parent first
		const CGUIProperty* pPropertyParent = rProperty.GetProperty( "parent", "CGUIString" );
		if( pPropertyParent )
		{
			ProcessProperty( *pPropertyParent );
		}

		uint32 nSize = rProperty.GetPropertyNum();
		for( uint32 nIdx = 0; nIdx<nSize;++nIdx)
		{
			const CGUIProperty* pProperty = rProperty.GetProperty(nIdx);
			ProcessProperty( *pProperty );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::LoadFromProperty()
	{
		LoadFromProperty( m_aPropertySet );
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::DumpToProperty()
	{
		uint32 nSize = m_aPropertySet.GetPropertyNum();
		for( uint32 nIdx = 0; nIdx<nSize;++nIdx)
		{
			CGUIProperty* pProperty = m_aPropertySet.GetProperty(nIdx);
			GenerateProperty( *pProperty );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::RenderExtraInfo(IGUIInterfaceRender* pRender)
	{
		// don't do anything if window is not visible or closed
		if ( IsOpen()==false ) 
		{
			return;
		}

		pRender->PushMatrix();
		pRender->MultMatrix( NodeGetTransform() );

		// perform render for 'this' Window
		RenderExtraSelfInfo(pRender);

		// render any child windows
		CGUIWidget* pWidget = GetChild();
		while( pWidget )
		{
			pWidget->RenderExtraInfo(pRender);
			pWidget = pWidget->GetNextSibling();
		}

		pRender->PopMatrix();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief render this widget, 
	* it will render this widget and all children.
	* @param pRender current render
	*/
	void CGUIWidget::Render(IGUIInterfaceRender* pRender)
	{
		// don't do anything if window is not visible or closed
		if (IsOpen()==false || !IsDerivedVisible()) 
		{
			return;
		}

		pRender->PushMatrix();
		pRender->MultMatrix( NodeGetTransform() );

		if( m_pSceneEffect )
		{
			m_pSceneEffect->BeforeRender( pRender );
		}

		// perform render for 'this' Window
		RenderSelf(pRender);

		PushClipRect( pRender );

		// render any child windows
		CGUIWidget* pWidget = GetChild();
		while( pWidget )
		{
			if( pWidget->IsIgnoreParentClipRect() )
			{
				PopClipRect( pRender );
			}
			
			pWidget->Render(pRender);

			if( pWidget->IsIgnoreParentClipRect() )
			{
				PushClipRect( pRender );
			}
			pWidget = pWidget->GetNextSibling();
		}

		PopClipRect( pRender );

		if( m_pSceneEffect )
		{
			m_pSceneEffect->AfterRender( pRender );
		}

		pRender->PopMatrix();
	}
	//------------------------------------------------------------------------------

	/**
	* @brief update this widget, 
	* it will update this widget and all children.
	* @param pRender current render
	*/
	void CGUIWidget::Update( real fDeltaTime )
	{
		// don't do anything if window is closed
		if (IsOpen()==false ) 
		{
			return;
		}

		// perform update for 'this' Window
		UpdateSelf( fDeltaTime );

		// render any child windows
		CGUIWidget* pWidget = GetChild();
		while( pWidget )
		{
			pWidget->Update( fDeltaTime );
			pWidget = pWidget->GetNextSibling();
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief render this widget only
	*/
	void CGUIWidget::RenderSelf(IGUIInterfaceRender* pRender)
	{
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::RenderExtraSelfInfo(IGUIInterfaceRender* pRender)
	{
		if( IsFocus() )
		{
			pRender->DrawRect( GetBoundArea(), 7.0f, 0.0f, CGUIColor( 1.f,1.f,1.f, 0.8f) );
		}

		//draw bound
		if( IsFocusable() )
		{
			pRender->DrawRect( GetBoundArea(), 3.0f, 0.0f, CGUIColor( 0.f,1.f,0.f,0.8f) );
		}
		else
		{
			pRender->DrawRect( GetBoundArea(), 3.0f, 0.0f, CGUIColor( 0.f,0.f,1.f,0.8f) );
		}

		//draw client area
		pRender->DrawRect( GetBoundArea(), 1.0f, 0.0f, CGUIColor( 1.f,0.f,0.f,0.8f) );
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::PushClipRect( IGUIInterfaceRender* pRender )
	{
		if( IsClipChildren() && GetClipArea() )
		{
			pRender->PushClipRect( *GetClipArea() );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::PopClipRect( IGUIInterfaceRender* pRender )
	{
		if( IsClipChildren() && GetClipArea() )
		{
			pRender->PopClipRect( );
		}
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsIgnoreParentClipRect() const
	{
		return false;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsAddChildToTail() const
	{
		return true;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief update this widget only
	*/
	void CGUIWidget::UpdateSelf( real fDeltaTime )
	{
		//update action sequence
		UpdateAsSelf( fDeltaTime );

		//call update event
		if( IsGenerateUpdateEvent() )
		{
			CGUIEventNotification aEvent;
			aEvent.SetEventId(eEVENT_UPDATE);
			aEvent.SetReceiver(this);
			GSystem->SendEvent( &aEvent);
		}

		//for timer event
		if( !m_arrayTimer.empty())
		{
			for( TVecTimer::iterator itor = m_arrayTimer.begin();
				itor != m_arrayTimer.end();
				++itor )
			{
				STimer& rCurrentTimer = *itor;
				rCurrentTimer.m_fTimeLeft -= fDeltaTime;
				if( rCurrentTimer.m_fTimeLeft <= 0.0f )
				{
					rCurrentTimer.m_fTimeLeft = rCurrentTimer.m_fTimeWaiting;

					//call function
					CGUIEventTimer aEvent;
					aEvent.SetEventId(eEVENT_TIMER);
					aEvent.SetReceiver(this);
					aEvent.SetTimerName(rCurrentTimer.m_strTimerName);
					aEvent.SetDuration( rCurrentTimer.m_fTimeWaiting );
					GSystem->SendEvent( &aEvent);
				}
			}
		}

		OnUpdate( fDeltaTime );
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::DrawCharacter(IGUIInterfaceRender* pRender, 
		wchar charCode, 
		const CGUIStringRenderInfo& rInfo,
		const CGUIVector2& rPos)
	{
		pRender->GetFontRender()->DrawCharacter(pRender, charCode, rInfo, rPos, GetDerivedAlpha());
	}
	//------------------------------------------------------------------------------
	/**
	* @brief draw a string
	*/
	void CGUIWidget::DrawString(
		IGUIInterfaceRender* pRender, 
		const CGUIStringRender& strText, 
		const CGUIRect& rDrawRect,
		ETextAlignmentHorz uTextAlignmentHorz,
		ETextAlignmentVert uTextAlignmentVert,
		int32 nStartPos,
		int32 nEndPos)
	{
		pRender->GetFontRender()->DrawString(pRender, strText, rDrawRect, uTextAlignmentHorz, uTextAlignmentVert, GetDerivedAlpha(), nStartPos, nEndPos);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::DrawString(
		IGUIInterfaceRender* pRender, 
		const CGUIStringRender& strText, 
		const CGUIVector2& rPos,
		int32 nStartPos,
		int32 nEndPos)
	{
		pRender->GetFontRender()->DrawString(pRender, strText,rPos, GetDerivedAlpha(), nStartPos,nEndPos);
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::DrawImage(IGUIInterfaceRender* pRender, 
		CGUIImage* pImage, 
		const CGUIRect& rDestRect)
	{
		if( pImage )
		{
			pImage->Draw( pRender, rDestRect,0,m_aColor,GetDerivedAlpha() );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::DrawRect( IGUIInterfaceRender* pRender,
		const CGUIRect& rDestRect, 
		real fLineWidth,
		const CGUIColor& rColor )
	{
		pRender->DrawRect( rDestRect, fLineWidth, 0, rColor );
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::DrawAnimation(IGUIInterfaceRender* pRender, 
		CGUIAnimation* pAnimation, 
		const CGUIRect& rDestRect)
	{
		if( pAnimation )
		{
			pAnimation->Draw( pRender, rDestRect,0,GetDerivedAlpha() );
		}
	}
	//------------------------------------------------------------------------------
	const CGUIRect*	CGUIWidget::GetClipArea() const
	{
		return &m_aClientArea;
	}
	//------------------------------------------------------------------------------
	const CGUIRect&	CGUIWidget::GetClientArea() const
	{
		return m_aClientArea;
	}
	//------------------------------------------------------------------------------
	const CGUIRect&	CGUIWidget::GetBoundArea() const
	{
		return m_aBoundArea;
	}
	//------------------------------------------------------------------------------
	const CGUIVector2&	CGUIWidget::GetAnchorPoint()
	{
		return m_aWidgetAnchorPoint;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetAnchorPoint(const CGUIVector2&rAnchorPoint)
	{
		m_aWidgetAnchorPoint = rAnchorPoint;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetAnchorPoint(real x, real y)
	{
		m_aWidgetAnchorPoint.x = x;
		m_aWidgetAnchorPoint.y = y;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetMaximumSize(const CGUISize& rSize)
	{
		m_aMaxSize = rSize;
	}
	//------------------------------------------------------------------------------
	const CGUISize&		CGUIWidget::GetMaximumSize() const			
	{
		return m_aMaxSize;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetMinimumSize(const CGUISize& rSize)
	{
		m_aMinSize = rSize;
	}
	//------------------------------------------------------------------------------
	const CGUISize&		CGUIWidget::GetMinimumSize() const
	{
		return m_aMinSize;
	}
	//------------------------------------------------------------------------------
	CGUISize CGUIWidget::GetParentSize() const
	{
		if(GetParent())
		{
			//has parent
			return GetParent()->GetClientArea().GetSize();
		}
		else
		{
			//no parent
			return GSystem->GetScreenSize();
		}
	}
	//------------------------------------------------------------------------------
	//set widget position relative to parent, from anchor point to parent's top-left
	void CGUIWidget::SetPosition( real x, real y )
	{
		m_aWidgetPosition.m_aValue.x = x;
		m_aWidgetPosition.m_aValue.y = y;

		switch( m_aWidgetSize.m_eType )
		{
		case eScreenValue_Pixel:
			m_aWidgetPosition.m_aPixelValue = m_aWidgetPosition.m_aValue;
			break;
		case eScreenValue_Percentage:
			{
				CGUISize aParentPixelSize = GetParentSize();
				m_aWidgetPosition.m_aPixelValue = m_aWidgetPosition.m_aValue * aParentPixelSize;
			}
			break;
		default:
			GUI_FORCE_ASSERT( "unknown widget size type");
			break;
		}
	};
	//------------------------------------------------------------------------------
	//set widget position relative to parent, from anchor point to parent's top-left
	void CGUIWidget::SetPosition( const CGUIVector2& rPos )
	{
		SetPosition( rPos.x, rPos.y );
	}
	//------------------------------------------------------------------------------
	//get widget position in given value relative to parent
	const CGUIVector2&	CGUIWidget::GetPosition() const
	{
		return m_aWidgetPosition.m_aValue;
	}
	//------------------------------------------------------------------------------
	//set widget position in pixel format
	void CGUIWidget::SetPixelPosition( real x, real y )
	{
		m_aWidgetPosition.m_aPixelValue.x = x;
		m_aWidgetPosition.m_aPixelValue.y = y;
		switch( m_aWidgetPosition.m_eType )
		{
		case eScreenValue_Pixel:
			m_aWidgetPosition.m_aValue = m_aWidgetPosition.m_aPixelValue;
			break;
		case eScreenValue_Percentage:
			{
				CGUISize aParentPixelSize = GetParentSize();
				m_aWidgetPosition.m_aValue.x = m_aWidgetPosition.m_aPixelValue.x / aParentPixelSize.m_fWidth;
				m_aWidgetPosition.m_aValue.y = m_aWidgetPosition.m_aPixelValue.y / aParentPixelSize.m_fHeight;
			}
			break;
		default:
			GUI_FORCE_ASSERT( "unknown widget position type");
			break;
		}
	}
	//------------------------------------------------------------------------------
	//set widget position in pixel format
	void CGUIWidget::SetPixelPosition( const CGUIVector2& rPixelPos)
	{
		SetPixelPosition( rPixelPos.x, rPixelPos.y );
	}
	//------------------------------------------------------------------------------
	//get widget position in pixel format
	const CGUIVector2& CGUIWidget::GetPixelPosition() const
	{
		return m_aWidgetPosition.m_aPixelValue;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetGlobalPixelPosition( const CGUIVector2& rPos )
	{
		if( GetParent() )
		{
			CGUIVector3 aLocalPoint( rPos.x, rPos.y, 0.0f );
			GetParent()->WorldToLocal(aLocalPoint);
			SetPixelPosition( CGUIVector2( aLocalPoint.x, aLocalPoint.y ) );
		}
		else
		{
			SetPixelPosition( rPos );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetGlobalPixelPosition( real x, real y )
	{
		if( GetParent() )
		{
			CGUIVector3 aLocalPoint( x, y, 0.0f );
			GetParent()->WorldToLocal(aLocalPoint);
			SetPixelPosition( CGUIVector2(aLocalPoint.x, aLocalPoint.y ) );
		}
		else
		{
			SetPixelPosition( x, y );
		}
	}
	//------------------------------------------------------------------------------
	CGUIVector2 CGUIWidget::GetGlobalPixelPosition( ) const
	{
		const CGUIVector2& rLocalPos = GetPixelPosition();
		if( GetParent() )
		{
			CGUIVector3 aWorldPoint( rLocalPos.x, rLocalPos.y, 0.0f );
			GetParent()->LocalToWorld(aWorldPoint);
			return CGUIVector2( aWorldPoint.x, aWorldPoint.y );
		}
		else
		{
			return rLocalPos;
		}
	}
	//------------------------------------------------------------------------------
	//set position type
	void CGUIWidget::SetPositionType( EScreenValue eValueType )
	{
		if( eValueType != m_aWidgetPosition.m_eType )
		{
			m_aWidgetPosition.m_eType = eValueType;
		}
	}
	//------------------------------------------------------------------------------
	//get position type
	EScreenValue CGUIWidget::GetPositionType( ) const
	{
		return m_aWidgetPosition.m_eType;
	}
	//------------------------------------------------------------------------------
	//set widget size, according size value type
	void CGUIWidget::SetSize( real width, real height )
	{
		m_aWidgetSize.m_aValue.m_fWidth = width;
		m_aWidgetSize.m_aValue.m_fHeight = height;

		switch( m_aWidgetSize.m_eType )
		{
		case eScreenValue_Pixel:
			m_aWidgetSize.m_aPixelValue = m_aWidgetSize.m_aValue;
			break;
		case eScreenValue_Percentage:
			{
				CGUISize aParentPixelSize = GetParentSize();
				m_aWidgetSize.m_aPixelValue = m_aWidgetSize.m_aValue * aParentPixelSize;
			}
			break;
		default:
			GUI_FORCE_ASSERT( "unknown widget size type");
			break;
		}

		CGUIEventSize aEvent;
		aEvent.SetEventId(eEVENT_SIZE_CHANGE);
		aEvent.SetReceiver(this);
		GSystem->SendEvent( &aEvent);
	}
	//------------------------------------------------------------------------------
	//set widget size, according size value type
	void CGUIWidget::SetSize( const CGUISize& rSize )
	{
		SetSize(rSize.m_fWidth, rSize.m_fHeight);
	}
	//------------------------------------------------------------------------------
	//get widget size by given size type
	const CGUISize&	CGUIWidget::GetSize() const
	{
		return m_aWidgetSize.m_aValue;
	}
	//------------------------------------------------------------------------------
	//set widget size in pixel.
	void CGUIWidget::SetPixelSize( real width, real height )
	{
		m_aWidgetSize.m_aPixelValue.m_fWidth = width;
		m_aWidgetSize.m_aPixelValue.m_fHeight = height;

		switch( m_aWidgetSize.m_eType )
		{
		case eScreenValue_Pixel:
			m_aWidgetSize.m_aValue = m_aWidgetSize.m_aPixelValue;
			break;
		case eScreenValue_Percentage:
			{
				CGUISize aParentPixelSize = GetParentSize();
				m_aWidgetSize.m_aValue.m_fWidth = m_aWidgetSize.m_aPixelValue.m_fWidth / aParentPixelSize.m_fWidth;
				m_aWidgetSize.m_aValue.m_fHeight = m_aWidgetSize.m_aPixelValue.m_fHeight / aParentPixelSize.m_fHeight;
			}
			break;
		default:
			GUI_FORCE_ASSERT( "unknown widget size type");
			break;
		}

		CGUIEventSize aEvent;
		aEvent.SetEventId(eEVENT_SIZE_CHANGE);
		aEvent.SetReceiver(this);
		GSystem->SendEvent( &aEvent);
	}
	//------------------------------------------------------------------------------
	//set widget size in pixel.
	void CGUIWidget::SetPixelSize( const CGUISize& rPixelSize )
	{
		SetPixelSize( rPixelSize.m_fWidth, rPixelSize.m_fHeight );
	}
	//------------------------------------------------------------------------------
	//get widget size in pixel.
	const CGUISize&	CGUIWidget::GetPixelSize() const
	{
		return m_aWidgetSize.m_aPixelValue;
	}
	//------------------------------------------------------------------------------
	//set size type
	void CGUIWidget::SetSizeType( EScreenValue eValueType )
	{
		if( eValueType != m_aWidgetSize.m_eType )
		{
			m_aWidgetSize.m_eType = eValueType;
		}
	}
	//------------------------------------------------------------------------------
	//get size type
	EScreenValue CGUIWidget::GetSizeType( ) const
	{
		return m_aWidgetSize.m_eType;
	}
	//------------------------------------------------------------------------------
	/**
	* in degree
	*/
	void CGUIWidget::SetRotation(real x, real y, real z)
	{
		m_vRotation.x = CGUIDegree(x).valueRadians();
		m_vRotation.y = CGUIDegree(y).valueRadians();
		m_vRotation.z = CGUIDegree(z).valueRadians();
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::Roll( real x )
	{
		m_vRotation.x = CGUIDegree(x).valueRadians();
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::Pitch( real y )
	{
		m_vRotation.y = CGUIDegree(y).valueRadians();
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::Yaw( real z )
	{
		m_vRotation.z = CGUIDegree(z).valueRadians();
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetRotation(const CGUIVector3& rRotation)
	{
		SetRotation(rRotation.x, rRotation.y, rRotation.z);
	}
	//------------------------------------------------------------------------------
	CGUIVector3 CGUIWidget::GetRotation( ) const
	{
		return CGUIVector3(
			CGUIRadian(m_vRotation.x).valueDegrees(),
			CGUIRadian(m_vRotation.y).valueDegrees(),
			CGUIRadian(m_vRotation.z).valueDegrees());
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::Refresh( )
	{
		RefreshSelf();

		CGUIWidget*	pWidget = GetChild();
		while(pWidget)
		{
			pWidget->Refresh();
			pWidget = pWidget->GetNextSibling();
		}	
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::RefreshSelf()
	{
		//get parent info
		CGUISize aParentPixelSize = GetParentSize();

		//refresh position
		switch( m_aWidgetPosition.m_eType )
		{
		case eScreenValue_Pixel:
			m_aWidgetPosition.m_aPixelValue = m_aWidgetPosition.m_aValue;
			break;
		case eScreenValue_Percentage:
			m_aWidgetPosition.m_aPixelValue = m_aWidgetPosition.m_aValue * aParentPixelSize;
			break;
		default:
			GUI_FORCE_ASSERT( "unknown widget position type");
			break;
		}

		//refresh size
		switch( m_aWidgetSize.m_eType )
		{
		case eScreenValue_Pixel:
			m_aWidgetSize.m_aPixelValue = m_aWidgetSize.m_aValue;
			break;
		case eScreenValue_Percentage:
			m_aWidgetSize.m_aPixelValue = aParentPixelSize * m_aWidgetSize.m_aValue;
			break;
		default:
			GUI_FORCE_ASSERT( "unknown widget size type");
			break;
		}

		//refresh widget rect
		CGUIVector2 aOffsetPos( -GetPixelSize().m_fWidth*m_aWidgetAnchorPoint.x, -GetPixelSize().m_fHeight*m_aWidgetAnchorPoint.y );
		m_aBoundArea.SetRect( aOffsetPos, m_aWidgetSize.m_aPixelValue );
		m_aClientArea = m_aBoundArea;

		//refresh node
		CGUIVector2 aParentOffsetPos( 0.0f, 0.0f );
		if( GetParent() )
		{
			aParentOffsetPos.x = GetParent()->m_aWidgetSize.m_aPixelValue.m_fWidth*GetParent()->m_aWidgetAnchorPoint.x;
			aParentOffsetPos.y = GetParent()->m_aWidgetSize.m_aPixelValue.m_fHeight*GetParent()->m_aWidgetAnchorPoint.y;

			aParentOffsetPos.x -= (GetParent()->GetClientArea().m_fLeft - GetParent()->GetBoundArea().m_fLeft);
			aParentOffsetPos.y -= (GetParent()->GetClientArea().m_fTop - GetParent()->GetBoundArea().m_fTop);
		}
		CGUIVector3 aPos( m_aWidgetPosition.m_aPixelValue.x - aParentOffsetPos.x,
			m_aWidgetPosition.m_aPixelValue.y - aParentOffsetPos.y,
			0.0f);
		const CGUISize& rScale = m_aParamScale.GetSelfValue( );
		
		//pos
		NodeSetPosition( aPos );
		//scale
		NodeSetScale( rScale.m_fWidth, rScale.m_fHeight, 1.0f );
		//rotation
		CGUIMatrix3 aRotMat;
		aRotMat.FromEulerAnglesXYZ( m_vRotation.x, m_vRotation.y, m_vRotation.z );
		NodeSetOrientation( CGUIQuaternion( aRotMat) );

		NodeUpdateFromParent( );

		//refresh bound rect for hittest
		LocalToWorld( m_aBoundArea, m_aHitTestArea );
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetOpenWithParent( bool bFlag )
	{
		m_bOpenWithParent = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsOpenWithParent( ) const
	{
		return m_bOpenWithParent;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetInheritAlpha( bool bFlag )
	{
		m_bInheritAlpha = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsInheritAlpha( ) const
	{
		return m_bInheritAlpha;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetFocusAgency( bool bFlag )
	{
		m_bIsFocusAgency = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsFocusAgency( ) const
	{
		return m_bIsFocusAgency;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetFocusable( bool bFlag )
	{
		m_bIsFocusable = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsFocusable( ) const
	{
		return m_bIsFocusable;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsDerivedFocusable() const
	{
		if( IsFocusAgency())
		{
			//is a focus agency
			if( GetParent() )
			{
				return GetParent()->IsDerivedFocusable( );
			}

			GUI_THROW( GUI_FORMAT("[CGUIWidget::IsDerivedFocusable]: lack of parent! TYPE<%s>  NAME<%s>",
				GetType().c_str(),GetName().c_str()));
		}

		return IsFocusable();
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetMovable( bool bFlag )
	{
		m_bIsMovable = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsMovable( ) const
	{
		return m_bIsMovable;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetHitable( bool bFlag )
	{
		m_bIsHitable = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsHitable( ) const
	{
		return m_bIsHitable;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetMouseConsumed( bool bFlag )
	{
		m_bIsMouseConsumed = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsMouseConsumed( ) const
	{
		return m_bIsMouseConsumed;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetForceHitTest( bool bFlag )
	{
		m_bForceHitTest = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsForceHitTest( ) const
	{
		return m_bForceHitTest;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetGenerateUpdateEvent( bool bFlag )
	{
		m_bIsGenerateUpdateEvent = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsGenerateUpdateEvent( ) const
	{
		return m_bIsGenerateUpdateEvent;
	}	
	//------------------------------------------------------------------------------
	void CGUIWidget::SetGenerateScaleChangeEvent( bool bFlag )
	{
		m_bIsGenerateScaleChangeEvent = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsGenerateScaleChangeEvent( ) const
	{
		return m_bIsGenerateScaleChangeEvent;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetGenerateParentSizeChangeEvent( bool bFlag )
	{
		m_bIsGenerateParentSizeChangeEvent = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsGenerateParentSizeChangeEvent( ) const
	{
		return m_bIsGenerateParentSizeChangeEvent;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetGenerateParentChildEvent( bool bFlag )
	{
		m_bIsGenerateParentChildEvent = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsGenerateParentChildEvent( ) const
	{
		return m_bIsGenerateParentChildEvent;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetGenerateClickEvent( bool bFlag )
	{
		m_bIsGenerateClickEvent = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsGenerateClickEvent( ) const
	{
		return m_bIsGenerateClickEvent;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetGenerateLoadEvent( bool bFlag )
	{
		m_bIsGenerateLoadEvent = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsGenerateLoadEvent( ) const
	{
		return m_bIsGenerateLoadEvent;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetGenerateDBClickEvent( bool bFlag )
	{
		m_bIsGenerateDBClickEvent = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsGenerateDBClickEvent( ) const
	{
		return m_bIsGenerateDBClickEvent;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetGenerateMultiClickEvent( bool bFlag )
	{
		m_bIsGenerateMultiClickEvent = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsGenerateMultiClickEvent( ) const
	{
		return m_bIsGenerateMultiClickEvent; 
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetTextContentUTF8( const CGUIString& /*rString*/)
	{
	}
	//------------------------------------------------------------------------------
	CGUIString CGUIWidget::GetTextContentUTF8( ) const
	{
		return CGUIString();
	}
	//------------------------------------------------------------------------------
	const CGUIColor& CGUIWidget::GetColor() const
	{
		return m_aColor;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetColor(const CGUIColor& rColor )
	{
		m_aColor = rColor;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIWidget::GetName() const
	{
		return m_strName;
	}
	//------------------------------------------------------------------------------
	//!< get scene name
	const CGUIString& CGUIWidget::GetSceneName( ) const
	{
		return m_strOwnerSceneName;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUIWidget::GetType() const
	{
		return m_strType;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
