#include "StdAfxEditor.h"
#include "Ui\ReAnimGraphicsScene.h"
#include "Ui\ReAnimGraphicsItem.h"
#include "Core\ReAnimDef.h"
#include <QGraphicsLineItem>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QPainter>
#include <QMimeData>
#include <QByteArray>
#include <QDataStream>
#include <QMap>


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


void ReAnimGraphicsScene::dragEnterEvent( QGraphicsSceneDragDropEvent* _event )
{
	int i = 0;
	_event->accept();
}


void ReAnimGraphicsScene::dragMoveEvent( QGraphicsSceneDragDropEvent* _event )
{
	int i = 0;
	_event->accept();
}


void ReAnimGraphicsScene::dropEvent( QGraphicsSceneDragDropEvent* _event )
{
	const QMimeData* mimeData = _event->mimeData();
	QString type = tr( "application/x-qabstractitemmodeldatalist" );
	if( mimeData->hasFormat( type ) )
	{
		QByteArray rawData = mimeData->data( type );
		QDataStream stream( &rawData, QIODevice::ReadOnly );
		int row = 0;
		int column = 0;
		//QVariant data = QVariant();
		QMap< int , QVariant > data;
		stream >> row >> column >> data;

		int size = data.size();
		QMap< int, QVariant >::iterator itor = data.find( Qt::DecorationRole );
		if( itor != data.end() )
		{
			//QMap< int, QVariant > mp = qvariant_cast< QMap< int, QVariant > >( data );
			//QIcon icon = qvariant_cast< QIcon >( data[ Qt::DecorationRole ] );
			QIcon icon = qvariant_cast< QIcon >( *itor );
			int j = 0;
		}
	}

	int i = 0;
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
