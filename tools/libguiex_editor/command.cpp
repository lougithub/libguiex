/** 
* @file command.cpp
* @brief command
* @author ken
* @date 2007-11-30
*/

//============================================================================//
// include
//============================================================================//
#include "command.h"
#include "wxmainapp.h"
#include "wxmainframe.h"

//============================================================================//
// function
//============================================================================// 


//***********************************************
// CCommand
//*********************************************** 
//------------------------------------------------------------------------------
CCommand::CCommand()
{

}
//------------------------------------------------------------------------------
CCommand::~CCommand()
{

}
//------------------------------------------------------------------------------


//***********************************************
// CCommandWidgetBase
//*********************************************** 
//------------------------------------------------------------------------------
CCommandWidgetBase::CCommandWidgetBase( CGUIWidget* pWidget )
	:m_strWidgetName( pWidget->GetName() )
{
}
//------------------------------------------------------------------------------
CGUIWidget* CCommandWidgetBase::GetWidget()
{
	CGUIWidget* pWidget = CGUIWidgetManager::Instance()->GetWidget( m_strWidgetName, GetMainFrame()->GetCurrentSceneName() );
	GUI_ASSERT( pWidget, "CCommandWidgetBase::GetWidget: invalid pointer" );
	return pWidget;
}
//------------------------------------------------------------------------------


//***********************************************
// CCommandWidgetProperty
//*********************************************** 
//------------------------------------------------------------------------------
CCommandWidgetProperty::CCommandWidgetProperty( CGUIWidget* pWidget )
	:CCommandWidgetBase( pWidget )
{
}
//------------------------------------------------------------------------------
void CCommandWidgetProperty::Execute()
{
	GetWidget()->InsertProperty( m_ExecuteProp );
}
//------------------------------------------------------------------------------
void CCommandWidgetProperty::Undo()
{
	GetWidget()->InsertProperty( m_ExecuteProp );
}
//------------------------------------------------------------------------------



//***********************************************
// CCommand_SetPosition
//*********************************************** 
//------------------------------------------------------------------------------
CCommand_SetPosition::CCommand_SetPosition( CGUIWidget* pWidget, const CGUIVector2& rPixelPos )
	:CCommandWidgetBase( pWidget )
{
	m_aPosOld = pWidget->GetPixelPosition();
	m_aPosNew = rPixelPos;
}
//------------------------------------------------------------------------------
void CCommand_SetPosition::Execute()
{
	GetWidget()->SetPixelPosition( m_aPosNew );
}
//------------------------------------------------------------------------------
void CCommand_SetPosition::Undo()
{
	GetWidget()->SetPixelPosition( m_aPosOld );
}
//------------------------------------------------------------------------------
