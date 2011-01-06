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
	QAction* action = m_editMenu->addAction( tr( "&Add Item" ) );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnAddItem() ) );
}


// -----------------------------------------------------------------------------
// Override QGraphicsScene.
// -----------------------------------------------------------------------------
void ReAnimGraphicsScene::contextMenuEvent( QGraphicsSceneContextMenuEvent* _event )
{
	m_scenePosBackup = _event->scenePos();
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


}
