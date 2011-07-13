/** 
* @file guiwidgetcallback.cpp
* @brief base class, define most operation of widget
* @author ken
* @date 2006-06-26
*/

//============================================================================//
// include
//============================================================================// 
#include "guiwidget.h"
#include "guilogmsgmanager.h"
#include "guievent.h"
#include "guisystem.h"

//------------------------------------------------------------------------------
/************ event list *******************
OnAlphaChange

************* event list *******************/
//------------------------------------------------------------------------------ 
//============================================================================//
// define
//============================================================================// 
namespace guiex
{
/*
	#define WIDGET_EVENT_TRACE(eventname)	\
		GUI_TRACE( GUI_FORMAT("[CGUIWidget::"eventname"]:\n    Widget type<%s>, name <%s>\n\n",m_strType.c_str(), m_strName.c_str()));
*/

#define WIDGET_EVENT_TRACE(eventname)


}//namespace guiex


//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnAlphaChange( CGUIEventAlpha* pEvent )
	{
		//WIDGET_EVENT_TRACE("OnAlphaChange");

		//call callback function
		CallbackFunction("OnAlphaChange", pEvent);

		return 0;	
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnParentSizeChange( CGUIEventSize* pEvent )
	{
		//WIDGET_EVENT_TRACE("OnParentSizeChange");

		//call callback function
		CallbackFunction("OnParentSizeChange", pEvent);

		return 0;	
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnChildSizeChange( CGUIEventSize* pEvent )
	{
		//WIDGET_EVENT_TRACE("OnChildSizeChange");

		//call callback function
		CallbackFunction("OnChildSizeChange", pEvent);

		return 0;	
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnSizeChanged( CGUIEventSize* pEvent )
	{
		//WIDGET_EVENT_TRACE("OnSizeChanged");

		//call callback function
		CallbackFunction("OnSizeChanged", pEvent);

		//send parent's change event to child
		if( IsGenerateParentSizeChangeEvent() )
		{
			CGUIWidget* pWidget = GetChild();
			while( pWidget )
			{
				if( pWidget->IsOpen() )
				{
					CGUIEventSize aEvent;
					aEvent.SetEventId(eEVENT_PARENT_SIZE_CHANGE);
					aEvent.SetSize(pEvent->GetSize());
					aEvent.SetReceiver(pWidget);
					GSystem->SendEvent( &aEvent );
				}
				pWidget = pWidget->GetNextSibling();
			}
		}

		if( IsGenerateChildSizeChangeEvent() )
		{
			if( GetParent() && GetParent()->IsOpen() )
			{
				CGUIEventSize aEvent;
				aEvent.SetEventId(eEVENT_CHILD_SIZE_CHANGE);
				aEvent.SetSize(pEvent->GetSize());
				aEvent.SetReceiver(GetParent());
				GSystem->SendEvent( &aEvent );
			}
		}

		return 0;	
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseEnter( CGUIEventMouse* pEvent )
	{
		//WIDGET_EVENT_TRACE("OnMouseEnter");

		//call callback function
		CallbackFunction("OnMouseEnter", pEvent);

		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseLeave( CGUIEventMouse* pEvent )
	{
		//WIDGET_EVENT_TRACE("OnMouseLeave");

		//call callback function
		CallbackFunction("OnMouseLeave", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseMove( CGUIEventMouse* pEvent )
	{
		//	WIDGET_EVENT_TRACE("OnMouseMove");

		//call callback function
		CallbackFunction("OnMouseMove", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseLeftDown( CGUIEventMouse* pEvent )
	{
		//WIDGET_EVENT_TRACE("OnMouseLeftDown");

		//call callback function
		CallbackFunction("OnMouseLeftDown", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseLeftUp( CGUIEventMouse* pEvent )
	{
		//WIDGET_EVENT_TRACE("OnMouseLeftUp");

		//call callback function
		CallbackFunction("OnMouseLeftUp", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseLeftClick( CGUIEventMouse* pEvent )
	{
		WIDGET_EVENT_TRACE("OnMouseLeftClick");

		//call callback function
		CallbackFunction("OnMouseLeftClick", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseLeftDbClick( CGUIEventMouse* pEvent )
	{
		WIDGET_EVENT_TRACE("OnMouseLeftDbClick");

		//call callback function
		CallbackFunction("OnMouseLeftDbClick", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseLeftMultiClick( CGUIEventMouse* pEvent )
	{
		WIDGET_EVENT_TRACE("OnMouseMultiClick");

		//call callback function
		CallbackFunction("OnMouseLeftMultiClick", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseRightDown( CGUIEventMouse* pEvent )
	{
		WIDGET_EVENT_TRACE("OnMouseRightDown");

		//call callback function
		CallbackFunction("OnMouseRightDown", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseRightUp( CGUIEventMouse* pEvent )
	{
		WIDGET_EVENT_TRACE("OnMouseRightUp");

		//call callback function
		CallbackFunction("OnMouseRightUp", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseRightClick( CGUIEventMouse* pEvent )
	{
		WIDGET_EVENT_TRACE("OnMouseRightClick");

		//call callback function
		CallbackFunction("OnMouseRightClick", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseRightDbClick( CGUIEventMouse* pEvent )
	{
		WIDGET_EVENT_TRACE("OnMouseRightDbClick");

		//call callback function
		CallbackFunction("OnMouseRightDbClick", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseRightMultiClick( CGUIEventMouse* pEvent )
	{
		WIDGET_EVENT_TRACE("OnMouseMultiClick");

		//call callback function
		CallbackFunction("OnMouseRightMultiClick", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseMiddleDown( CGUIEventMouse* pEvent )
	{
		WIDGET_EVENT_TRACE("OnMouseMiddleDown");

		//call callback function
		CallbackFunction("OnMouseMiddleDown", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseMiddleUp( CGUIEventMouse* pEvent )
	{
		WIDGET_EVENT_TRACE("OnMouseMiddleUp");

		//call callback function
		CallbackFunction("OnMouseMiddleUp", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseMiddleClick( CGUIEventMouse* pEvent )
	{
		WIDGET_EVENT_TRACE("OnMouseMiddleClick");

		//call callback function
		CallbackFunction("OnMouseMiddleClick", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseMiddleDbClick( CGUIEventMouse* pEvent )
	{
		WIDGET_EVENT_TRACE("OnMouseMiddleClick");

		//call callback function
		CallbackFunction("OnMouseMiddleDbClick", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseMiddleMultiClick( CGUIEventMouse* pEvent )
	{
		WIDGET_EVENT_TRACE("OnMouseMultiClick");

		//call callback function
		CallbackFunction("OnMouseMiddleMultiClick", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnMouseWheel( CGUIEventMouse* pEvent )
	{
		WIDGET_EVENT_TRACE("OnMouseWheel");

		//call callback function
		CallbackFunction("OnMouseWheel", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnActive( CGUIEventNotification* pEvent )
	{
		WIDGET_EVENT_TRACE("OnActive");

		//call callback function
		CallbackFunction("OnActive", pEvent);

		//set active
		CGUIWidget* pThis = this;
		CGUIWidget* pParent = pThis->GetParent();
		while( pThis && pParent )
		{
			if( pThis->IsActivable())
			{
				pThis->MoveToTop();
			}
			pThis = pParent;
			pParent = pThis->GetParent();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnInactive( CGUIEventNotification* pEvent )
	{
		WIDGET_EVENT_TRACE("OnInactive");

		//call callback function
		CallbackFunction("OnInactive", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnGetFocus( CGUIEventNotification* pEvent )
	{
		WIDGET_EVENT_TRACE("OnGetFocus");

		GSystem->SetFocusWidget(this);

		//call callback function
		CallbackFunction("OnGetFocus", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnLostFocus( CGUIEventNotification* pEvent )
	{
		WIDGET_EVENT_TRACE("OnLostFocus");

		GSystem->SetFocusWidget(NULL);

		//call callback function
		CallbackFunction("OnInactive", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnVisible( CGUIEventNotification* pEvent )
	{
		WIDGET_EVENT_TRACE("OnVisible");

		m_aParamVisible.SetSelfValue(true);

		//call callback function
		CallbackFunction("OnVisible", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnInvisible( CGUIEventNotification* pEvent )
	{
		WIDGET_EVENT_TRACE("OnInvisible");

		m_aParamVisible.SetSelfValue(false);

		//call callback function
		CallbackFunction("OnInvisible", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnScaleChange( CGUIEventNotification* pEvent )
	{
		WIDGET_EVENT_TRACE("OnScaleChange");

		//call callback function
		CallbackFunction("OnScaleChange", pEvent);


		//set child's scale
		CGUIWidget* pWidget = GetChild();
		while( pWidget )
		{
			if( pWidget->IsOpen() )
			{
				CGUIEventNotification aEvent;
				aEvent.SetEventId(eEVENT_SCALE_CHANGE);
				aEvent.SetReceiver(pWidget);
				GSystem->SendEvent( &aEvent );
			}
			pWidget = pWidget->GetNextSibling();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnEnable( CGUIEventNotification* pEvent )
	{
		WIDGET_EVENT_TRACE("OnEnable");

		m_aParamDisable.SetSelfValue(true);

		//call callback function
		CallbackFunction("OnEnable", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnDisable( CGUIEventNotification* pEvent )
	{
		WIDGET_EVENT_TRACE("OnDisable");

		m_aParamDisable.SetSelfValue(false);

		//call callback function
		CallbackFunction("OnDisable", pEvent);


		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnDragBegin( CGUIEventDrag* pEvent )
	{
		WIDGET_EVENT_TRACE("OnDragBegin");

		//call callback function
		CallbackFunction("OnDragBegin", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnDragProcess( CGUIEventDrag* pEvent )
	{
		//call callback function
		CallbackFunction("OnDragProcess", pEvent);

		if( pEvent->IsExpired() == false)
		{
			SetPixelPosition(pEvent->GetWidgetLocalPos());
			Refresh();
		}
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnDragEnd( CGUIEventDrag* pEvent )
	{
		WIDGET_EVENT_TRACE("OnDragEnd");

		//call callback function
		CallbackFunction("OnDragEnd", pEvent);
		if( IsMouseConsumed())
		{
			pEvent->Consume( true );
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnLoad( CGUIEventNotification* pEvent )
	{
		WIDGET_EVENT_TRACE("OnLoad");

		//call callback function
		CallbackFunction("OnLoad", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnOpen( CGUIEventNotification* pEvent )
	{
		WIDGET_EVENT_TRACE("OnOpen");

		//call callback function
		CallbackFunction("OnOpen", pEvent);

		//if( IsFocusable())
		//{
		//	SetFocus(true);
		//}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnClose( CGUIEventNotification* pEvent )
	{
		WIDGET_EVENT_TRACE("OnClose");

		//call callback function
		CallbackFunction("OnClose", pEvent);

		if( IsFocus())
		{
			SetFocus(false);
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnChecked( CGUIEventNotification* pEvent )
	{
		WIDGET_EVENT_TRACE("OnChecked");

		//call callback function
		CallbackFunction("OnChecked", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnUnchecked( CGUIEventNotification* pEvent )
	{
		WIDGET_EVENT_TRACE("OnUnchecked");

		//call callback function
		CallbackFunction("OnUnchecked", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIWidget::OnEventUpdate(CGUIEventNotification* pEvent )
	{
		//call callback function
		CallbackFunction("OnUpdate", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnScrollbarScroll( CGUIEventScrollbar* pEvent )
	{
		//WIDGET_EVENT_TRACE("OnScrollbarScroll");

		//call callback function
		CallbackFunction("OnScrollbarScroll", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnTimer( CGUIEventTimer* pEvent )
	{
		//call callback function
		CallbackFunction("OnTimer", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnKeyDown( CGUIEventKeyboard* pEvent )
	{
		WIDGET_EVENT_TRACE("OnKeyDown");

		//call callback function
		CallbackFunction("OnKeyDown", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnKeyUp( CGUIEventKeyboard* pEvent )
	{
		WIDGET_EVENT_TRACE("OnKeyUp");

		//call callback function
		CallbackFunction("OnKeyUp", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnKeyPressed( CGUIEventKeyboard* pEvent )
	{
		WIDGET_EVENT_TRACE("OnKeyPressed");

		//call callback function
		CallbackFunction("OnKeyPressed", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnKeyClicked( CGUIEventKeyboard* pEvent )
	{
		WIDGET_EVENT_TRACE("OnKeyClicked");

		//call callback function
		CallbackFunction("OnKeyClicked", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnListBoxSelected( CGUIEventListBox* pEvent )
	{
		WIDGET_EVENT_TRACE("OnListBoxSelected");

		//call callback function
		CallbackFunction("OnListBoxSelected", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnListBoxSelectChanged( CGUIEventListBox* pEvent )
	{
		WIDGET_EVENT_TRACE("OnListBoxSelecteChanged");

		//call callback function
		CallbackFunction("OnListBoxSelecteChanged", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnListBoxLeftDbClick( CGUIEventListBox* pEvent )
	{
		WIDGET_EVENT_TRACE("OnListBoxLeftDbClick");

		//call callback function
		CallbackFunction("OnListBoxLeftDbClick", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnListBoxRightDbClick( CGUIEventListBox* pEvent )
	{
		WIDGET_EVENT_TRACE("OnListBoxRightDbClick");

		//call callback function
		CallbackFunction("OnListBoxRightDbClick", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnListBoxLeftClick( CGUIEventListBox* pEvent )
	{
		WIDGET_EVENT_TRACE("OnListBoxLeftClick");

		//call callback function
		CallbackFunction("OnListBoxLeftClick", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnListBoxRightClick( CGUIEventListBox* pEvent )
	{
		WIDGET_EVENT_TRACE("OnListBoxRightClick");

		//call callback function
		CallbackFunction("OnListBoxRightClick", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnComboBoxSelected(CGUIEventComboBox* pEvent)
	{
		WIDGET_EVENT_TRACE("OnComboBoxSelected");

		//call callback function
		CallbackFunction("OnComboBoxSelected", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnUIEvent(CGUIEventUI* pEvent)
	{
		WIDGET_EVENT_TRACE("OnUIEvent");

		//call callback function
		CallbackFunction("OnUIEvent", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnAddChild( CGUIEventRelativeChange* pEvent )
	{
		WIDGET_EVENT_TRACE("OnAddChild");

		//call callback function
		CallbackFunction("OnAddChild", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnRemoveChild( CGUIEventRelativeChange* pEvent )
	{
		WIDGET_EVENT_TRACE("OnRemoveChild");

		//call callback function
		CallbackFunction("OnRemoveChild", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------
	uint32 CGUIWidget::OnParentChanged( CGUIEventRelativeChange* pEvent )
	{
		WIDGET_EVENT_TRACE("OnParentChanged");

		//call callback function
		CallbackFunction("OnParentChanged", pEvent);

		return 0;
	}
	//------------------------------------------------------------------------------

	//class CA 
	//{
	//	setSize()
	//	{
	//		SendSystemEvent(ChangSize, event,receive);		------	>	system list ->system call		-----> receive(onSize)
	//		SendUserEvent("killed", event,receive);			------	>	system list	->user call	-----> receive(onUserEvent)
	//	}
	//
	//	onUserEvent(event)
	//	{
	//		listFuncUser[event->userid]();
	//	}
	//
	//	onSize(event)
	//	{
	//		listFunc[ "onSizeBefore"](event);
	//		script->onEvent("onSizeBefore",event)
	//
	//		if(event.skip == false)
	//		{
	//			.....
	//			listFunc[ ChangeSizeAfter](event);
	//		}
	//	}
	//}
	//
	//class CB : public CA
	//{
	//	onSize(event)
	//	{
	//		....
	//		....
	//		CA::onSize(event);
	//
	//	}
	//	onkiller();
	//	
	//	onUserEvent(event)
	//	{
	//		if( find)
	//		{
	//			systemlistfun[event->userid]();
	//		}
	//		CA::onUserEvent();
	//	}
	//}




}//namespace guiex

