// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101222.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReAnimTools.h"
#include "Ui\ReAnimGraphicsItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QMouseEvent>


namespace RE
{


// -----------------------------------------------------------------------------
// Animation tool base.
// -----------------------------------------------------------------------------
void ReAnimTool::Release()
{
	if( NULL != m_item )
	{
		m_item = NULL;
	}
}


// -----------------------------------------------------------------------------
// Animation tool for selection.
// -----------------------------------------------------------------------------
void ReAnimSelectTool::Setup( ReAnimGraphicsItem* _item )
{
	if( NULL != _item )
	{
		m_item = _item;
		m_item->SetEditMode( EEditMode_Select );
	}
}


// -----------------------------------------------------------------------------
// Animation tool for translation.
// -----------------------------------------------------------------------------
void ReAnimMoveTool::Setup( ReAnimGraphicsItem* _item )
{
	if( NULL != _item )
	{
		m_item = _item;
		m_item->SetEditMode( EEditMode_Move );
	}
}


// -----------------------------------------------------------------------------
// Animation tool for rotation.
// -----------------------------------------------------------------------------
void ReAnimRotateTool::Setup( ReAnimGraphicsItem* _item )
{
	if( NULL != _item )
	{
		m_item = _item;
		m_item->SetEditMode( EEditMode_Rotate );
	}
}


// -----------------------------------------------------------------------------
// Animation tool for scaling.
// -----------------------------------------------------------------------------
void ReAnimScaleTool::Setup( ReAnimGraphicsItem* _item )
{
	if( NULL != _item )
	{
		m_item = _item;
		m_item->SetEditMode( EEditMode_Scale );
	}
}


// -----------------------------------------------------------------------------
// Animation tool for alpha setting.
// -----------------------------------------------------------------------------
void ReAnimAlphaTool::Setup( ReAnimGraphicsItem* _item )
{
	if( NULL != _item )
	{
		m_item = _item;
		m_item->SetEditMode( EEditMode_Alpha );
	}
}


}
