// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110116.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Core\ReWidgetSelection.h"


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
void ReWidgetSelection::GotoWaitState()
{
	TItemListItor itor = Begin();
	TItemListItor itorEnd = End();
	for( ; itor != itorEnd; ++itor )
	{
		ReSelItem& item = *itor;
		QWidget* widget = item.GetWidget();
		item.GetDragInfoRef().SetItemPosBackup( widget->pos() );
	}

	m_state = EState_Wait;
}


void ReWidgetSelection::MoveBy( const QPoint& _delta )
{
	TItemListItor itor = Begin();
	TItemListItor itorEnd = End();
	for( ; itor != itorEnd; ++itor )
	{
		ReSelItem& item = *itor;
		QWidget* widget = item.GetWidget();
		widget->move( widget->pos() + _delta );
	}
}


void ReWidgetSelection::DragMoveBy( const QPoint& _delta )
{
	TItemListItor itor = Begin();
	TItemListItor itorEnd = End();
	for( ; itor != itorEnd; ++itor )
	{
		ReSelItem& item = *itor;
		QWidget* widget = item.GetWidget();
		widget->move( item.GetDragInfoRef().GetItemPosBackup() + _delta );
	}
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


}
