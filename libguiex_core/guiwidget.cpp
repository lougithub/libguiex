/** 
* @file guiwidget.cpp
* @brief base class, define most operation of widget
* @author ken
* @date 2006-06-07
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guiwidget.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guievent.h>
#include <libguiex_core/guirect.h>
#include <libguiex_core/guiimagemanager.h>
#include <libguiex_core/guianimationmanager.h>
#include <libguiex_core/guiimage.h>
#include <libguiex_core/guianimation.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guipropertymanager.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiinterfacesound.h>
#include <libguiex_core/guiinterfacescript.h>
#include <libguiex_core/guiinterfacefont.h>
#include <libguiex_core/guilogmsgmanager.h>
#include <libguiex_core/guistringconvertor.h>
#include <libguiex_core/guiperfmonitor.h>
#include <libguiex_core/guias.h>
#include <libguiex_core/guipropertyconvertor.h>
#include <libguiex_core/guiasmanager.h>
#include <libguiex_core/guiwidgetmanager.h>

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
		,m_pWidgetGenerator(NULL)
		,m_pExclusiveChild(NULL)
		,m_aParamScale(CGUISize(1.0f,1.0f))
		,m_aParamAlpha(1.0f)
		,m_aParamDisable(false)
		,m_aParamVisible(true)
		,m_aParamActivable(false)
		,m_strType(rType)
		,m_bIsOpen( false )
		,m_bIsCreate( false )
		,m_aColor(1.0f,1.0f,1.0f,1.0f)
		,m_uTextAlignment(GUI_TA_CENTER)
		,m_vRotation(0.0f,0.0f,0.0f)
		,m_bOpenWithParent(true)
		,m_bInheritAlpha(true)
		,m_bIsFocusAgency(false)
		,m_bIsFocusable(true)
		,m_bIsMovable(false)
		,m_bIsExclusive(false)
		,m_bIsHitable(true)
		,m_bIsMouseConsumed(true)
		,m_bIsResponseUpdateEvent(false)
		,m_bIsResponseParentSizeChangeEvent(false)
		,m_bIsGenerateClickEvent(false)
		,m_bIsGenerateDBClickEvent(false)
		,m_bIsGenerateMultiClickEvent(false)
		,m_bIsAutoPlayAs(false)
		,m_bIsClipChildren(false)
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
			throw CGUIException(
				"[CGUIWidget::Create]: the widget has been created. TYPE<%s>  NAME<%s>",
				GetType().c_str(), 
				GetName().c_str());
		}
		m_bIsCreate = true;

		Refresh();

		//register to script
		bool bHasScript = false;
		guiex::IGUIInterfaceScript* pInterfaceScript = CGUIInterfaceManager::Instance()->GetInterfaceScript();
		if( pInterfaceScript &&
			guiex::GSystem->ShouldRunScript())
		{
			bHasScript = pInterfaceScript->HasScript( GetSceneName() );
		}
		if( bHasScript )
		{
			pInterfaceScript->RegisterWidget( this );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::NotifyLoaded()
	{
		//notify self
		CGUIEventNotification aEvent;
		aEvent.SetEventId(eEVENT_LOAD);
		aEvent.SetReceiver(this);
		GSystem->SendEvent( &aEvent);

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
			throw CGUIException("[CGUIWidget::Open]: the widget isn't in closed state. TYPE<%s>  NAME<%s>",
				GetType().c_str(), GetName().c_str());
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
			throw CGUIException("[CGUIWidget::Close]: the widget isn't in opened state. TYPE<%s>  NAME<%s>",
				GetType().c_str(), GetName().c_str());
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
	const CGUIStringEx&	CGUIWidget::GetTooltipText(void) const
	{
		return m_strTooltipText;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetTooltipText(const CGUIStringEx& rText)
	{
		m_strTooltipText = rText;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetTextColor(const CGUIColor& rColor)
	{
		m_strText.m_aStringInfo.m_aColor = rColor;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetTextContent( const CGUIStringW& rStringW )
	{
		m_strText.m_strContent = rStringW;
	}
	//------------------------------------------------------------------------------
	const CGUIStringW& CGUIWidget::GetTextContent() const
	{
		return m_strText.m_strContent;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsTextContentEmpty( ) const
	{
		return m_strText.m_strContent.empty();
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetTextContentUTF8( const CGUIString& rString)
	{
		CGUIStringW strTemp;
		MultiByteToWideChar( rString, strTemp);
		SetTextContent( strTemp );
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUIWidget::GetTextContentUTF8() const
	{
		CGUIString aContentUTF8;
		WideByteToMultiChar( m_strText.m_strContent, aContentUTF8 );
		return aContentUTF8;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetTextAlignment( uint8 uAlignment)
	{
		m_uTextAlignment = uAlignment;
	}
	//------------------------------------------------------------------------------
	uint8  CGUIWidget::GetTextAlignment( ) const
	{
		return m_uTextAlignment;
	}
	//------------------------------------------------------------------------------
	const CGUIStringEx&	CGUIWidget::GetText() const
	{
		return m_strText;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetTextInfo(const CGUIStringInfo& rInfo)
	{
		m_strText.m_aStringInfo = rInfo;
	}
	//------------------------------------------------------------------------------
	const CGUIStringInfo& CGUIWidget::GetTextInfo( ) const
	{
		return m_strText.m_aStringInfo;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetGenerator( const CGUIWidgetGenerator* pGenerator)
	{
		m_pWidgetGenerator = pGenerator;
	}
	//------------------------------------------------------------------------------
	const	CGUIWidgetGenerator* CGUIWidget::GetGenerator() const
	{
		return m_pWidgetGenerator;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetParentImpl( CGUIWidget* pParent )
	{
		m_pParent = pParent;
		//send event for change parent
		{
			CGUIEventRelativeChange aEvent;
			aEvent.SetEventId(eEVENT_PARENT_CHANGED);
			aEvent.SetRelative(pParent);
			aEvent.SetReceiver(this);
			GSystem->SendEvent( &aEvent);
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
		return static_cast<CGUIWidget*>(m_pParent);
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get child
	*/
	CGUIWidget*	CGUIWidget::GetChild( ) const
	{
		return static_cast<CGUIWidget*>(m_pChild);
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
		return static_cast<CGUIWidget*>(m_pNextSibling);
	}
	//------------------------------------------------------------------------------
	//get exclusive child
	CGUIWidget*	CGUIWidget::GetExclusiveChild( ) const
	{
		return m_pExclusiveChild;
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
	void	CGUIWidget::MoveDown()
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
				GetParent()->GetChild()->SetNextSibling( GetNextSibling() );
				SetNextSibling( GetParent()->GetChild() );
				GetParent()->setChild( this );
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
				pWidget->GetNextSibling()->SetNextSibling( GetNextSibling( ));
				SetNextSibling( pWidget->GetNextSibling());
				pWidget->SetNextSibling( this );
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
				GetParent()->setChild( GetParent()->GetChild()->GetNextSibling());
				SetNextSibling( GetParent()->GetChild()->GetNextSibling() );
				GetParent()->GetChild()->SetNextSibling( this );
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
				pWidget->SetNextSibling( GetNextSibling( ));
				SetNextSibling( pWidget->GetNextSibling()->GetNextSibling());
				pWidget->GetNextSibling()->SetNextSibling( this );

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
				GetParent()->setChild( GetParent()->GetChild()->GetNextSibling());
				SetNextSibling( NULL );
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
				pWidget->SetNextSibling( this );
			}
			else
			{
				//remove it first
				pWidget->SetNextSibling( GetNextSibling() );
				SetNextSibling( NULL );

				//add to last
				while( pWidget )
				{
					if( !pWidget->GetNextSibling() )
					{
						pWidget->SetNextSibling( this );
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
			setChild( GetChild()->GetNextSibling());
		}
		else
		{
			bool bFind = false;
			CGUIWidget* pParam = GetChild();
			while(pParam && pParam->GetNextSibling())
			{
				if( pParam->GetNextSibling() == pChild )
				{
					pParam->SetNextSibling( pParam->GetNextSibling()->GetNextSibling() );
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
				throw CGUIException("[CGUIWidget::RemoveChild]: failed to find widget, type <%s>, name<%s>!",
					pChild->GetType().c_str(), pChild->GetName().c_str());
			}
		}
		pChild->SetParentImpl( NULL );
		pChild->SetNextSibling( NULL );

		//for exclusive child
		if( pChild->IsExclusive())
		{
			GUI_ASSERT( pChild == m_pExclusiveChild, "error for remove exclusive child");
			m_pExclusiveChild = NULL;
		}

		//for parameter
		m_aParamScale.RemoveChild(&(pChild->m_aParamScale));
		m_aParamAlpha.RemoveChild(&(pChild->m_aParamAlpha));
		m_aParamDisable.RemoveChild(&(pChild->m_aParamDisable));
		m_aParamActivable.RemoveChild(&(pChild->m_aParamActivable));
		m_aParamVisible.RemoveChild(&(pChild->m_aParamVisible));

		//send event for add child
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

		CGUIWidget* pLastChild = GetLastChild();
		if( pLastChild )
		{
			pLastChild->SetNextSibling( pChild );
		}
		else
		{
			setChild( pChild );
		}
		pChild->SetNextSibling( NULL );

		//set parent
		pChild->SetParentImpl( this );

		//for exclusive child
		if( pChild->IsExclusive())
		{
			GUI_ASSERT(m_pExclusiveChild==NULL, "there has been a exclusive child");
			m_pExclusiveChild = pChild;
		}

		//for parameter
		m_aParamScale.AddChild(&(pChild->m_aParamScale));
		m_aParamAlpha.AddChild(&(pChild->m_aParamAlpha));
		m_aParamDisable.AddChild(&(pChild->m_aParamDisable));
		m_aParamActivable.AddChild(&(pChild->m_aParamActivable));
		m_aParamVisible.AddChild(&(pChild->m_aParamVisible));

		//send event for add child
		{
			CGUIEventRelativeChange aEvent;
			aEvent.SetEventId(eEVENT_ADD_CHILD);
			aEvent.SetRelative(pChild);
			aEvent.SetReceiver(this);
			GSystem->SendEvent( &aEvent);
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetActivable(bool bActivable)
	{
		m_aParamActivable.SetSelfValue(bActivable);
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidget::IsActivable( ) const
	{
		return m_aParamActivable.GetSelfValue();
	}
	//------------------------------------------------------------------------------
	bool	CGUIWidget::IsDerivedActivable()
	{
		return m_aParamActivable.GetFinalValue();
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::SetFocus(bool bFocus)
	{
		if( bFocus && IsFocusAgency())
		{
			//is a focus agency
			if( GetParent() )
			{
				GetParent()->SetFocus( bFocus);
				return;
			}
			throw CGUIException("[CGUIWidget::SetFocus]: lack of parent! TYPE<%s>  NAME<%s>",
				GetType().c_str(),GetName().c_str());
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
			throw CGUIException("[CGUIWidget::IsFocus]: lack of parent! TYPE<%s>  NAME<%s>",
				GetType().c_str(),GetName().c_str());
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

		if( m_bIsAutoPlayAs )
		{
			for( TMapAs::iterator itor = m_aMapAs.begin();
				itor != m_aMapAs.end();
				++itor )
			{
				CGUIAs* pAs = itor->second;
				if( IsAsPlaying( pAs ) == false )
				{
					PlayAs( pAs );
				}
			}
		}
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
	bool	CGUIWidget::IsDisable( ) const
	{
		return m_aParamDisable.GetSelfValue();
	}
	//------------------------------------------------------------------------------
	/// is this widget disable, the value will be affect by the parent's state
	bool	CGUIWidget::IsDerivedDisable()
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
		return getFullTransform();
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::LocalToWorld( CGUIVector2& rPos )
	{
		const CGUIMatrix4& rWorldMatrix = getFullTransform();
		CGUIVector3	aPos(rPos.x,rPos.y,0.0f);
		aPos = rWorldMatrix*aPos;
		rPos.x = aPos.x;
		rPos.y = aPos.y;	
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::WorldToLocal( CGUIVector2& rPos )
	{
		const CGUIMatrix4& rWorldInverseMatrix = getFullInverseTransform();
		CGUIVector3	aPos(rPos.x,rPos.y,0.0f);
		aPos = rWorldInverseMatrix*aPos;
		rPos.x = aPos.x;
		rPos.y = aPos.y;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::LocalToWorld( CGUIVector3& rPos )
	{
		const CGUIMatrix4& rWorldMatrix = getFullTransform();
		rPos = rWorldMatrix*rPos;

	}
	//------------------------------------------------------------------------------
	void CGUIWidget::WorldToLocal( CGUIVector3& rPos )
	{
		const CGUIMatrix4& rWorldInverseMatrix = getFullInverseTransform();
		rPos = rWorldInverseMatrix*rPos;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::ParentToWorld( CGUIVector2& rPos )
	{
		if(GetParent())
		{
			const CGUIMatrix4& rWorldMatrix = GetParent()->getFullTransform();
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
			const CGUIMatrix4& rWorldInverseMatrix = GetParent()->getFullInverseTransform();
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
	* @brief give the child which is pinted
	*/
	CGUIWidget*		CGUIWidget::GetWidgetAtPoint(const CGUIVector2& rPos)
	{
		//check sibling
		if( GetNextSibling() && 
			!(IsExclusive() && IsDerivedDisable() == false && IsOpen() && IsDerivedVisible()))
		{
			CGUIWidget* pWidget = GetNextSibling()->GetWidgetAtPoint(rPos);
			if( pWidget &&(!pWidget->IsExclusive() || pWidget->HitTest(rPos)))
			{
				//find one
				return pWidget;
			}
		}

		if( IsDerivedDisable() == false && IsOpen() && IsDerivedVisible()) 
		{
			//check child
			bool bIsChildExclusive = false;
			if( GetChild() )
			{
				CGUIWidget* pWidget = NULL;
				if( m_pExclusiveChild &&  m_pExclusiveChild->IsDerivedDisable() == false && m_pExclusiveChild->IsOpen() && m_pExclusiveChild->IsDerivedVisible())
				{
					//check exclusive child
					bIsChildExclusive = true;
					pWidget = m_pExclusiveChild->GetWidgetAtPoint(rPos);
				}
				else
				{
					//check normal child
					pWidget = GetChild()->GetWidgetAtPoint(rPos);
				}
				if( pWidget &&(!pWidget->IsExclusive() || pWidget->HitTest(rPos)))
				{
					//find one
					return pWidget;
				}
			}

			//check self
			if( !bIsChildExclusive && HitTest(rPos) )
			{
				return this;
			}
		}

		return NULL;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief test whether the point is in this widget
	* @param rPos in pixel format
	*/
	bool CGUIWidget::HitTest( const CGUIVector2& rPos)
	{
		if( IsHitable() )
		{
			return m_aBound.IsPointInRect(rPos);
		}
		else
		{
			return false;
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::RegisterUIEvent( const CGUIString& rUIEventName )
	{
		GSystem->RegisterUIEvent( rUIEventName, this );
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::UnregisterUIEvent( const CGUIString& rUIEventName )
	{
		GSystem->UnregisterUIEvent( rUIEventName, this );
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::RegisterSound( const CGUIString& strEventName, int32 nSoundIdx )
	{
		m_mapEventSound.insert( std::make_pair(strEventName, nSoundIdx));
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::UnregisterSound( const CGUIString& strEventName )
	{
		TMapSound::iterator itor = m_mapEventSound.find(strEventName);
		if( itor != m_mapEventSound.end())
		{
			m_mapEventSound.erase(itor);
		}
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
			throw CGUIException("[CGUIWidget::GetScriptCallbackFunc]: failed to find event function by name <%s>!", strEventName.c_str());
		}
		return itor->second;
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::UnregisterNativeTimerFunc(const CGUIString& strEventName)
	{
		TMapTimer::iterator itor1 = m_aMapTimer.find(strEventName);
		TMapGlobalFunc::iterator itor2 = m_mapNativeFunc.find(strEventName);
		if( itor1 == m_aMapTimer.end() || itor2 == m_mapNativeFunc.end())
		{
			throw CGUIException("[CGUIWidget::UnregisterNativeTimerFunc]: failed to find event <%s>!", strEventName.c_str());
		}
		m_aMapTimer.erase(itor1);
		m_mapNativeFunc.erase(itor2);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::UnregisterScriptTimerFunc(const CGUIString& strEventName)
	{
		TMapTimer::iterator itor1 = m_aMapTimer.find(strEventName);
		TMapScriptFunc::iterator itor2 = m_mapScriptFunc.find(strEventName);
		if( itor1 == m_aMapTimer.end() || itor2 == m_mapScriptFunc.end())
		{
			throw CGUIException("[CGUIWidget::UnregisterScriptTimerFunc]: failed to find event <%s>!", strEventName.c_str());
		}
		m_aMapTimer.erase(itor1);
		m_mapScriptFunc.erase(itor2);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::RegisterScriptTimerFunc( 
		real rWaitingTime, 
		const CGUIString&strEventName, 
		const CGUIString& strFunc )
	{
		m_aMapTimer.insert( std::make_pair(strEventName, STimer(rWaitingTime)));
		m_mapScriptFunc.insert( std::make_pair(strEventName, strFunc));
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::RegisterNativeTimerFunc( 		
		real rWaitingTime, 
		const CGUIString& strEventName,  
		void (*pFunc)(CGUIEventTimer*) )
	{
		m_aMapTimer.insert( std::make_pair(strEventName, STimer(rWaitingTime)));
		m_mapNativeFunc.insert( std::make_pair(strEventName, reinterpret_cast<CallbackEventFunc>(pFunc)));
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::CallGlobalFunction(const CGUIString& strEventName, CGUIEvent* pEvent)
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
		if( GSystem->ShouldRunScript())
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
		TMapSound::iterator itor = m_mapEventSound.find( strEventName );
		if( itor != m_mapEventSound.end())
		{
			IGUIInterfaceSound* pSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
			pSound->PlayEffect(itor->second);
		}

		//call global function
		CallGlobalFunction(strEventName, pEvent);

		//call script function
		CallScriptFunction(strEventName, pEvent);
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetScale( const CGUISize& rSize )
	{
		if(m_aParamScale.GetSelfValue() != rSize )
		{
			if( rSize.m_fHeight <= 0.0f || rSize.m_fWidth <= 0.0f)
			{
				GUI_FORCE_ASSERT( GUI_FORMAT("can't set the zero scale to a widget <%s>", m_strName.c_str()));
				return;
			}

			m_aParamScale.SetSelfValue(rSize);

			//send event
			CGUIEventNotification aEvent;
			aEvent.SetEventId(eEVENT_SCALE_CHANGE);
			aEvent.SetReceiver(this);
			GSystem->SendEvent( &aEvent );
		}

	}
	//------------------------------------------------------------------------------
	const CGUISize & CGUIWidget::GetScale( ) const
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
	void	CGUIWidget::SetImage( const CGUIString& rName, CGUIImage* pImage )
	{
		//clear old one
		TMapImage::iterator itor = m_aMapImage.find(rName );
		if( itor != m_aMapImage.end())
		{
			CGUIImageManager::Instance()->DeallocateResource( itor->second );
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
			throw CGUIException( "failed to get image by name <%s>", rImageName.c_str());
			return NULL;
		};
		SetImage(rName, pImage);
		CGUIImageManager::Instance()->DeallocateResource( pImage );
		return pImage;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetAnimation( const CGUIString& rName, CGUIAnimation* pAnimation )
	{
		TMapAnimation::iterator itor = m_aMapAnimation.find(rName);
		if( itor != m_aMapAnimation.end())
		{
			//remove old one
			CGUIAnimationManager::Instance()->DeallocateResource( itor->second );
			m_aMapAnimation.erase(itor);
		}
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
			throw CGUIException( "failed to get animation by name <%s>", rAnimationName.c_str());
			return NULL;
		};
		SetAnimation(rName, pAnimation);
		CGUIAnimationManager::Instance()->DeallocateResource( pAnimation );
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
			throw CGUIException( "failed to get as by name <%s>", rAsName.c_str());
			return NULL;
		};
		pAs->SetReceiver( this );
		SetAs( rName, pAs );
		CGUIAsManager::Instance()->DeallocateResource( pAs );
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
			CGUIAsManager::Instance()->DeallocateResource( itor->second );
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
			throw CGUIException(
				"[CGUIWidget::PlayAs]: failed to play as <%s>",
				rName.c_str() );
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
			throw CGUIException(
				"[CGUIWidget::IsAsPlaying]: failed to check whether as <%s> is playing",
				rName.c_str() );
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
			throw CGUIException(
				"[CGUIWidget::StopAs]: failed to stop as <%s>",
				rName.c_str() );
			return;
		}
		StopAs( pAs );
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::StopAll( )
	{
		for( TListAs::iterator itor = m_listAsPlaying.begin();
			itor != m_listAsPlaying.end();
			++itor )
		{
			CGUIAsManager::Instance()->DeallocateResource( *itor );
		}
		m_listAsPlaying.clear();
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::PlayAs( CGUIAs* pAs )
	{
		TListAs::iterator itor = std::find( 
			m_listAsPlaying.begin(), 
			m_listAsPlaying.end(), 
			pAs );
		if( itor == m_listAsPlaying.end() )
		{
			pAs->SetElapsedTime( 0.0f );
			pAs->RefRetain();
			m_listAsPlaying.push_back( pAs );
		}
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsAsPlaying( CGUIAs* pAs )
	{
		TListAs::iterator itor = std::find( 
			m_listAsPlaying.begin(), 
			m_listAsPlaying.end(), 
			pAs );
		if( itor == m_listAsPlaying.end() )
		{
			return false;
		}
		return true;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::StopAs( CGUIAs* pAs )
	{
		TListAs::iterator itor = std::find( 
			m_listAsPlaying.begin(), 
			m_listAsPlaying.end(), 
			pAs );
		if( itor != m_listAsPlaying.end() )
		{
			CGUIAsManager::Instance()->DeallocateResource( pAs );
			m_listAsPlaying.erase( itor );
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::UpdateAsSelf( real fDeltaTime )
	{
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
				CGUIAs* pSuccessor = NULL;
				while( pSuccessor = pAs->PopSuccessor())
				{
					pSuccessor->GetReceiver()->PlayAs(pSuccessor);
				}
				CGUIAsManager::Instance()->DeallocateResource( pAs );
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
			CGUIImageManager::Instance()->DeallocateResource( itor->second );
		}
		m_aMapImage.clear();

		//release animation
		for( TMapAnimation::iterator itor = m_aMapAnimation.begin();
			itor != m_aMapAnimation.end();
			++itor)
		{
			CGUIAnimationManager::Instance()->DeallocateResource( itor->second );
		}
		m_aMapAnimation.clear();

		//release as
		for( TMapAs::iterator itor = m_aMapAs.begin();
			itor != m_aMapAs.end();
			++itor)
		{
			CGUIAsManager::Instance()->DeallocateResource( itor->second );
		}
		m_aMapAs.clear();
		for( TListAs::iterator itor = m_listAsPlaying.begin();
			itor != m_listAsPlaying.end();
			++itor)
		{
			CGUIAsManager::Instance()->DeallocateResource( *itor );
		}
		m_listAsPlaying.clear();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set property set of this widget, if this widget has gotten the 
	* property, the old property will be destroyed.
	*/
	void CGUIWidget::SetProperty( const CGUIProperty&	rProperty)
	{
		ClearProperty();

		m_aPropertySet = rProperty;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief set property of this widget, if this widget has exist, reset it.
	* if,this widget hasn't this property, add it
	*/
	void CGUIWidget::InsertProperty( const CGUIProperty& rProperty)
	{
		if( m_aPropertySet.HasProperty(rProperty.GetName()))
		{
			*(m_aPropertySet.GetProperty(rProperty.GetName())) = rProperty;
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
		////////////////////////////////////////////////////////////////////////////////////////////////////
		if( rProperty.GetType() == ePropertyType_WidgetPosition && rProperty.GetName() == "position")
		{
			ValueToProperty( m_aWidgetPosition, rProperty );
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_WidgetSize && rProperty.GetName() == "size")
		{
			ValueToProperty( m_aWidgetSize, rProperty );
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_Vector2 && rProperty.GetName() == "anchor" )
		{
			ValueToProperty( m_aWidgetAnchorPoint, rProperty );
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if(  rProperty.GetType()== ePropertyType_Size && rProperty.GetName() == "scale" )
		{
			ValueToProperty( GetScale(), rProperty );
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_Size && rProperty.GetName() == "max_size" )
		{
			ValueToProperty( m_aMaxSize, rProperty );
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_Size && rProperty.GetName() == "min_size" )
		{
			ValueToProperty( m_aMinSize, rProperty );
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
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
		////////////////////////////////////////////////////////////////////////////////////////////////////
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
		////////////////////////////////////////////////////////////////////////////////////////////////////
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
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_Real && rProperty.GetName() == "alpha" )
		{
			ValueToProperty( GetAlpha(), rProperty);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_Vector3 && rProperty.GetName() == "rotation" )
		{
			ValueToProperty( GetRotation(), rProperty);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "activable" )
		{
			ValueToProperty( IsActivable(), rProperty);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "EXCLUSIVE" )
		{
			ValueToProperty( IsExclusive(), rProperty);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "disable" )
		{
			ValueToProperty( IsDisable(), rProperty);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "auto_play_as" )
		{
			ValueToProperty( IsAutoPlayAs(), rProperty);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "open_with_parent" )
		{
			ValueToProperty( IsOpenWithParent(), rProperty);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "response_update_event" )
		{
			ValueToProperty( IsResponseUpdateEvent(), rProperty);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "movable" )
		{
			ValueToProperty( IsMovable(), rProperty);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "visible" )
		{
			ValueToProperty( IsVisible(), rProperty);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "disable" )
		{
			ValueToProperty( IsDisable(), rProperty);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "hitable" )
		{
			ValueToProperty( IsHitable(), rProperty);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_Bool && rProperty.GetName() == "clip_children" )
		{
			ValueToProperty( IsClipChildren(), rProperty);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_StringInfo && rProperty.GetName() == "textinfo" )
		{
			ValueToProperty( GetTextInfo(), rProperty);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_String && rProperty.GetName() == "text" )
		{
			CGUIString aStrText;
			WideByteToMultiChar( GetTextContent(), aStrText);
			rProperty.SetValue(aStrText);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_TextAlignmentHorz && rProperty.GetName() == "text_alignment_horz" )
		{
			ValueToProperty( ETextAlignmentHorz(m_uTextAlignment&GUI_TA_HORIZON_MASK), rProperty);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_TextAlignmentVert && rProperty.GetName() == "text_alignment_vert" )
		{
			ValueToProperty( ETextAlignmentVert(m_uTextAlignment&GUI_TA_VERTICAL_MASK), rProperty);
		}
		else
		{
			//failed
			throw CGUIException("[CGUIWidget::GenerateProperty]: failed to generate property: name=[%s] type=[%s]!", 
				rProperty.GetName().c_str(),
				rProperty.GetTypeAsString().c_str());

			return -1;
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::ProcessProperty( const CGUIProperty& rProperty )
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for parent
		if(rProperty.GetType() == ePropertyType_String && rProperty.GetName() == "parent")
		{
			CGUIWidget* pParent = CGUIWidgetManager::Instance()->GetWidget( rProperty.GetValue(), GetSceneName());
			SetParent( pParent);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_WidgetPosition && rProperty.GetName() == "position")
		{
			PropertyToValue( rProperty, m_aWidgetPosition);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if( rProperty.GetType() == ePropertyType_WidgetSize && rProperty.GetName() == "size")
		{
			PropertyToValue( rProperty, m_aWidgetSize);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// load image
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
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// load as
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
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for text
		else if( rProperty.GetType() == ePropertyType_StringInfo && rProperty.GetName() == "textinfo" )
		{
			CGUIStringInfo aInfo;
			PropertyToValue( rProperty, aInfo);
			SetTextInfo(aInfo);
		}
		else if( rProperty.GetType() == ePropertyType_String && rProperty.GetName() == "text" )
		{
			CGUIStringW aStrText;
			MultiByteToWideChar( rProperty.GetValue(), aStrText );
			SetTextContent( aStrText );
		}
		else if( rProperty.GetType() == ePropertyType_TextAlignmentHorz && rProperty.GetName() == "text_alignment_horz" )
		{
			ETextAlignmentHorz eTextAlignmentH = eTextAlignment_Horz_Center;
			PropertyToValue( rProperty, eTextAlignmentH );
			m_uTextAlignment = (m_uTextAlignment & GUI_TA_VERTICAL_MASK) + eTextAlignmentH;
		}
		else if( rProperty.GetType() == ePropertyType_TextAlignmentVert && rProperty.GetName() == "text_alignment_vert" )
		{
			ETextAlignmentVert eTextAlignmentV = eTextAlignment_Vert_Center;
			PropertyToValue( rProperty, eTextAlignmentV );
			m_uTextAlignment = (m_uTextAlignment & GUI_TA_HORIZON_MASK) + eTextAlignmentV;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for sound
		else if(  rProperty.GetType()== ePropertyType_Sound)
		{
			int32 nSoundIndex = 0;
			PropertyToValue(rProperty, nSoundIndex);
			RegisterSound( rProperty.GetName(), nSoundIndex);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for script event
		else if(   rProperty.GetType()== ePropertyType_Event )
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
		//property for anchor point
		else if(  rProperty.GetType()== ePropertyType_Vector2 && rProperty.GetName() == "anchor" )
		{
			CGUIVector2 aValue;
			PropertyToValue( rProperty, aValue );
			SetAnchorPoint( aValue );
		}
		//property for scale
		else if(  rProperty.GetType()== ePropertyType_Size && rProperty.GetName() == "scale" )
		{
			CGUISize aValue;
			PropertyToValue( rProperty, aValue );
			SetScale( aValue );
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for size
		////////////////////////////////////////////////////////////////////////////////////////////////////
		else if(  rProperty.GetType()== ePropertyType_Size && rProperty.GetName() == "MAX_SIZE" )
		{
			CGUISize aSize;
			PropertyToValue( rProperty, aSize );
			SetMaximumSize(aSize);
		}
		else if( rProperty.GetType()== ePropertyType_Size && rProperty.GetName() == "MIN_SIZE" )
		{
			CGUISize aSize;
			PropertyToValue( rProperty, aSize );
			SetMaximumSize( aSize );
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for alpha
		else if(  rProperty.GetType()== ePropertyType_Real && rProperty.GetName()=="alpha" )
		{
			real fAlpha = 0.f;
			PropertyToValue( rProperty, fAlpha );
			SetAlpha( fAlpha );
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for rotation
		else if(  rProperty.GetType()== ePropertyType_Vector3 && rProperty.GetName()=="rotation" )
		{
			CGUIVector3 vRotation;
			PropertyToValue( rProperty, vRotation );
			SetRotation( vRotation );
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		//property for flag
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
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName()=="hitable" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetHitable( bValue );
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
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName()=="EXCLUSIVE" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetExclusive( bValue );
		}
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName()=="open_with_parent" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetOpenWithParent( bValue );
		}
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName()=="response_update_event" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetResponseUpdateEvent(bValue );
		}
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName()=="response_parentsizechange_event" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetResponseParentSizeChangeEvent(bValue );
		}		
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName()=="EVENT_CLICK" )
		{
			bool bValue = false;
			PropertyToValue(rProperty, bValue );
			SetGenerateClickEvent( bValue );
		}		
		else if( rProperty.GetType()== ePropertyType_Bool && rProperty.GetName()=="EVENT_DBCLICK" )
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
			throw CGUIException("[CGUIWidget::ProcessProperty]: failed to process property: name=[%s] type=[%s] value=[%s]!", 
				rProperty.GetName().c_str(),
				rProperty.GetTypeAsString().c_str(),
				rProperty.GetValue().c_str());
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::LoadProperty()
	{
		//set parent first
		CGUIProperty* pPropertyParent = m_aPropertySet.GetProperty( "parent" );
		if( pPropertyParent )
		{
			ProcessProperty( *pPropertyParent );
		}

		uint32 nSize = m_aPropertySet.GetPropertyNum();
		for( uint32 nIdx = 0; nIdx<nSize;++nIdx)
		{
			const CGUIProperty* pProperty = m_aPropertySet.GetProperty(nIdx);
			ProcessProperty( *pProperty );
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

		// perform render for 'this' Window
		RenderExtraSelfInfo(pRender);

		// render any child windows
		CGUIWidget* pWidget = GetChild();
		while( pWidget )
		{
			pWidget->RenderExtraInfo(pRender);
			pWidget = pWidget->GetNextSibling();
		}
	}
	//------------------------------------------------------------------------------
	/**
	* @brief render this widget, 
	* it will render this widget and all children.
	* @param pRender current render
	*/
	void	CGUIWidget::Render(IGUIInterfaceRender* pRender)
	{
		// don't do anything if window is not visible or closed
		if (IsOpen()==false || !IsDerivedVisible()) 
		{
			return;
		}

		// perform render for 'this' Window
		RenderSelf(pRender);

		PushClipRect( pRender );

		// render any child windows
		CGUIWidget* pWidget = GetChild();
		while( pWidget )
		{
			pWidget->Render(pRender);
			pWidget = pWidget->GetNextSibling();
		}

		PopClipRect( pRender );
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
	void	CGUIWidget::RenderSelf(IGUIInterfaceRender* pRender)
	{
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::RenderExtraSelfInfo(IGUIInterfaceRender* pRender)
	{
		//draw bound
		DrawRect( pRender, GetBoundArea(), 3.0f, CGUIColor( 0.f,1.f,0.f,1.f) );

		//draw client area
		DrawRect( pRender, GetBoundArea(), 1.0f, CGUIColor( 1.f,0.f,0.f,1.f) );
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::PushClipRect( IGUIInterfaceRender* pRender )
	{
		if( IsClipChildren() )
		{
			pRender->PushClipRect( getFullTransform(), GetClipArea() );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::PopClipRect( IGUIInterfaceRender* pRender )
	{
		if( IsClipChildren() )
		{
			pRender->PopClipRect( );
		}
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
		if( IsResponseUpdateEvent() )
		{
			CGUIEventNotification aEvent;
			aEvent.SetEventId(eEVENT_UPDATE);
			aEvent.SetReceiver(this);
			GSystem->SendEvent( &aEvent);
		}

		//for timer event
		if( !m_aMapTimer.empty())
		{
			TMapTimer::iterator itor = m_aMapTimer.begin();
			TMapTimer::iterator itorEnd = m_aMapTimer.end();
			for( ; itor!=itorEnd; ++itor )
			{
				STimer& rCurrentTimer = itor->second;
				rCurrentTimer.m_fTimeLeft -= fDeltaTime;
				if( rCurrentTimer.m_fTimeLeft <= 0.0f )
				{
					rCurrentTimer.m_fTimeLeft = rCurrentTimer.m_fTimeWaiting;

					//call function
					CGUIEventTimer aEvent;
					aEvent.SetEventId(eEVENT_TIMER);
					aEvent.SetReceiver(this);
					aEvent.SetTimerName(itor->first);
					GSystem->SendEvent( &aEvent);
				}
			}
		}

	}
	//------------------------------------------------------------------------------
	void CGUIWidget::DrawCharacter(IGUIInterfaceRender* pRender, 
		wchar_t charCode, 
		const CGUIStringInfo& rInfo,
		const CGUIVector2& rPos)
	{
		pRender->GetFontRender()->DrawCharacter(pRender,getFullTransform(), charCode, rInfo, rPos, GetDerivedAlpha());
	}
	//------------------------------------------------------------------------------
	/**
	* @brief draw a string
	*/
	void	CGUIWidget::DrawString(
		IGUIInterfaceRender* pRender, 
		const CGUIStringEx& strText, 
		const CGUIRect& rDrawRect,
		uint8 uTextAlignment,
		int32 nStartPos,
		int32 nEndPos)
	{
		pRender->GetFontRender()->DrawString(pRender,getFullTransform(), strText, rDrawRect, uTextAlignment, GetDerivedAlpha(), nStartPos, nEndPos);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::DrawString(
		IGUIInterfaceRender* pRender, 
		const CGUIStringEx& strText, 
		const CGUIVector2& rPos,
		int32 nStartPos,
		int32 nEndPos)
	{
		pRender->GetFontRender()->DrawString(pRender,getFullTransform(), strText,rPos, GetDerivedAlpha(), nStartPos,nEndPos);
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::DrawImage(IGUIInterfaceRender* pRender, 
		const CGUIImage* pImage, 
		const CGUIRect& rDestRect)
	{
		if( pImage )
		{
			pImage->Draw( pRender, getFullTransform(), rDestRect,pRender->GetAndIncZ(),m_aColor,GetDerivedAlpha() );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::DrawRect( IGUIInterfaceRender* pRender,
		const CGUIRect& rDestRect, 
		real fLineWidth,
		const CGUIColor& rColor )
	{
		pRender->DrawRect( getFullTransform(), rDestRect, fLineWidth, pRender->GetAndIncZ(),
			rColor.GetARGB(), rColor.GetARGB(), rColor.GetARGB(), rColor.GetARGB() );
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::DrawAnimation(IGUIInterfaceRender* pRender, 
		const CGUIAnimation* pAnimation, 
		const CGUIRect& rDestRect)
	{
		if( pAnimation )
		{
			pAnimation->Draw( pRender,getFullTransform(), rDestRect,pRender->GetAndIncZ(),GetDerivedAlpha() );
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIWidget::DrawImage(IGUIInterfaceRender* pRender, 
		const CGUIString& rName, 
		const CGUIRect& rDestRect)
	{
		TMapImage::iterator itor= m_aMapImage.find(rName);
		if( itor == m_aMapImage.end())
		{
			throw CGUIException("[CGUIWidget::RenderImage]: failed to render image <%s>!", rName.c_str());
			return;
		}
		else
		{
			DrawImage(pRender, itor->second, rDestRect );
		}
	}
	//------------------------------------------------------------------------------
	const CGUIRect&	CGUIWidget::GetClipArea() const
	{
		return m_aClipArea;
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
			GUI_ASSERT( 0, "unknown widget size type");
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
			GUI_ASSERT( 0, "unknown widget position type");
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
	const CGUIVector2&	CGUIWidget::GetPixelPosition() const
	{
		return m_aWidgetPosition.m_aPixelValue;
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
			GUI_ASSERT( 0, "unknown widget size type");
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
			GUI_ASSERT( 0, "unknown widget size type");
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
		m_vRotation.x = x;
		m_vRotation.y = y;
		m_vRotation.z = z;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetRotation(const CGUIVector3& rRotation)
	{
		SetRotation(rRotation.x, rRotation.y, rRotation.z);
	}
	//------------------------------------------------------------------------------
	const CGUIVector3& CGUIWidget::GetRotation( ) const
	{
		return m_vRotation;
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
			GUI_ASSERT( 0, "unknown widget position type");
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
			GUI_ASSERT( 0, "unknown widget size type");
			break;
		}

		//refresh widget rect
		CGUIVector2 aOffsetPos( -m_aWidgetSize.m_aPixelValue.m_fWidth*m_aWidgetAnchorPoint.x,
			-m_aWidgetSize.m_aPixelValue.m_fHeight*m_aWidgetAnchorPoint.y );
		m_aBoundArea.SetRect( aOffsetPos, m_aWidgetSize.m_aPixelValue );
		m_aClientArea = m_aBoundArea;
		m_aClipArea = m_aClientArea;

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
		setPosition( aPos );
		//scale
		setScale( rScale.m_fWidth, rScale.m_fHeight, 1.0f );
		//rotation
		//real w = CGUIMath::Cos( m_vRotation.z/2 );
		//real z = CGUIMath::Sin( m_vRotation.z/2 );
		//setOrientation( w, 0.0f, 0.0f, z );
		CGUIMatrix3 aRotMat;
		aRotMat.FromEulerAnglesXYZ( CGUIDegree(m_vRotation.x), CGUIDegree(m_vRotation.y), CGUIDegree(m_vRotation.z) );
		setOrientation( CGUIQuaternion( aRotMat) );

		updateFromParent( );

		//refresh render rect
		LocalToWorld( m_aBoundArea, m_aBound );
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
	bool	CGUIWidget::IsDerivedFocusable() const
	{
		if( IsFocusAgency())
		{
			//is a focus agency
			if( GetParent() )
			{
				return GetParent()->IsDerivedFocusable( );
			}

			throw CGUIException("[CGUIWidget::IsDerivedFocusable]: lack of parent! TYPE<%s>  NAME<%s>",
				GetType().c_str(),GetName().c_str());
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
	void CGUIWidget::SetExclusive( bool bFlag )
	{
		m_bIsExclusive = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsExclusive( ) const
	{
		return m_bIsExclusive;
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
	void CGUIWidget::SetResponseUpdateEvent( bool bFlag )
	{
		m_bIsResponseUpdateEvent = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsResponseUpdateEvent( ) const
	{
		return m_bIsResponseUpdateEvent;
	}
	//------------------------------------------------------------------------------
	void CGUIWidget::SetResponseParentSizeChangeEvent( bool bFlag )
	{
		m_bIsResponseParentSizeChangeEvent = bFlag;
	}
	//------------------------------------------------------------------------------
	bool CGUIWidget::IsResponseParentSizeChangeEvent( ) const
	{
		return m_bIsResponseParentSizeChangeEvent;
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
