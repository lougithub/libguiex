#include "StdAfxEditor.h"
#include "Ui\ReAnimGraphicsScene.h"
#include "Ui\ReAnimGraphicsItem.h"
#include "Core\ReAnimDef.h"
#include <QGraphicsLineItem>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QPainter>


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReAnimGraphicsScene::ReAnimGraphicsScene( ReAnimModel* _model, QObject* _parent /* = NULL */ )
: TSuper( _parent )
, m_editMenu( NULL )
{
	const int width = 2000;
	const int height = 2000;
	QColor penColor( 190, 190, 190 );
	setSceneRect( -width / 2, -height / 2, width, height );
	setBackgroundBrush( QBrush( Qt::gray ) );
	QGraphicsLineItem* lineA = addLine( -width / 2, 0, width / 2, 0, QPen( penColor ) );
	lineA->setZValue( -999.0f );
	QGraphicsLineItem* lineB = addLine( 0, -height / 2, 0, height / 2, QPen( penColor ) );
	lineA->setZValue( -999.0f );

	m_editMenu = new QMenu();
	m_createItemAction = m_editMenu->addAction( tr( "&Add Item" ) );
	connect( m_createItemAction, SIGNAL( triggered() ), this, SLOT( OnAddItem() ) );
	m_deleteItemAction = m_editMenu->addAction( tr( "&Delete Item" ) );
	connect( m_deleteItemAction, SIGNAL( triggered() ), this, SLOT( OnDeleteItem() ) );
}


// -----------------------------------------------------------------------------
// Override QGraphicsScene.
// -----------------------------------------------------------------------------
void ReAnimGraphicsScene::contextMenuEvent( QGraphicsSceneContextMenuEvent* _event )
{
	m_scenePosBackup = _event->scenePos();
	QGraphicsItem* item = itemAt( m_scenePosBackup );
	m_createItemAction->setDisabled( NULL != item );
	m_deleteItemAction->setDisabled( NULL == item );
	m_editMenu->exec( _event->screenPos() );
}


// -----------------------------------------------------------------------------
// Slots.
// -----------------------------------------------------------------------------
void ReAnimGraphicsScene::OnAddItem()
{
	ReAnimGraphicsItem* item = new ReAnimGraphicsItem();
	addItem( item );
	item->setPos( m_scenePosBackup );

	emit ItemAdded( item );
}


void ReAnimGraphicsScene::OnDeleteItem()
{
	ReAnimGraphicsItem* item = ( ReAnimGraphicsItem* )itemAt( m_scenePosBackup );
	if( NULL != item )
	{
		emit ItemDeleted( item );
		removeItem( item );
	}
}


}
