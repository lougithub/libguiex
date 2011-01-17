// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110109.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Core\ReAnimDef.h"
#include "Core\ReAnimModel.h"
#include "Core\ReAnimFrame.h"
#include "Ui\ReAnimEntityPanel.h"
#include "Ui\ReAnimTrackWidget.h"
#include "Ui\ReAnimEntityWidget.h"
#include "Ui\ReAnimGraphicsItem.h"
#include "Ui\ReAnimUiInfo.h"
#include <QResizeEvent>
#include <QGraphicsScene>
#include <QPainter>
#include <QPaintEvent>


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReAnimEntityPanel::ReAnimEntityPanel( ReAnimModel* _model, ReRulerWidget* _ruler, QGraphicsScene* _scene, QWidget* _parent )
: TSuper( _parent )
, m_model( _model )
, m_ruler( _ruler )
, m_scene( _scene )
, m_origin( 0, 0 )
{
	GetLeftPaddingRef() = 2;
	GetVerticalGapRef() = 2;
	connect( m_scene, SIGNAL( selectionChanged() ), this, SLOT( OnSceneSelectionChanged() ) );
}


bool ReAnimEntityPanel::Export( const QString& _filePath ) const
{
	bool result = false;

	TAnimEntityListCItor itor = m_entityList.begin();
	TAnimEntityListCItor itorEnd = m_entityList.end();
	for( ; itor != itorEnd; ++itor )
	{
		const ReAnimEntityInfo& info = *itor;
		ReAnimEntityWidget* entityWidget = info.m_entityWidget;
	}

	return result;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Slots.
// -----------------------------------------------------------------------------
// A ReAnimGraphicsItem object has been created in a Qt Graphics View Architecture.
// It's an indication to create a ReAnimEntityWidget object as a UI representation
// in the console, and a ReAnimEntity object in the animation model as its data
// representation.
// The ReAnimGraphicsItem object and ReAnimEntityWidget object are combined in
// a ReAnimEntityInfo object.
void ReAnimEntityPanel::OnItemAdded( ReAnimGraphicsItem* _item )
{
	if( NULL != m_model && NULL != _item )
	{
		ReAnimEntity* entityData = m_model->CreateEntity();

		ReAnimEntityWidget* entityWidget = new ReAnimEntityWidget( entityData, m_ruler, this );
		entityWidget->SetModelData( entityData );
		entityWidget->setContextMenuPolicy( Qt::CustomContextMenu );
		entityWidget->setVisible( true );

		ReAnimEntityInfo info;
		info.m_graphicsItem = _item;
		info.m_entityWidget = entityWidget;
		m_entityList.push_back( info );

		connect( _item, SIGNAL( PositionChanged( const QPointF& ) ), entityWidget, SLOT( OnPositionChanged( const QPointF& ) ) );
		connect( _item, SIGNAL( RotationChanged( qreal ) ), entityWidget, SLOT( OnRotationChanged( qreal ) ) );
		connect( _item, SIGNAL( ScaleChanged( const QPointF& ) ), entityWidget, SLOT( OnScaleChanged( const QPointF& ) ) );
		connect( _item, SIGNAL( AlphaChanged( qreal ) ), entityWidget, SLOT( OnAlphaChanged( qreal ) ) );
		entityWidget->OnPositionChanged( _item->pos() );
		entityWidget->OnRotationChanged( 0.0f );
		entityWidget->OnScaleChanged( QPointF( 1.0f, 1.0f ) );
		entityWidget->OnAlphaChanged( 1.0f );

		connect( m_ruler, SIGNAL( CursorChanged( int ) ), this, SLOT( OnRulerCursorChanged( int ) ) );
		connect( entityWidget, SIGNAL( TotalHeightChanged() ), this, SLOT( UpdateLayout() ) );
		connect( entityWidget, SIGNAL( DataChangedAt( int ) ), this, SLOT( OnRulerCursorChanged( int ) ) );

		UpdateLayout();
	}
}


void ReAnimEntityPanel::OnItemDeleted( ReAnimGraphicsItem* _item )
{
	TAnimEntityListItor itor = m_entityList.begin();
	TAnimEntityListItor itorEnd = m_entityList.end();
	for( ; itor != itorEnd; ++itor )
	{
		ReAnimEntityInfo& info = *itor;
		if( info.m_graphicsItem == _item )
		{
			ReAnimModel* animModel = m_model;
			animModel->DestroyEntity( info.m_entityWidget->GetModelData() );
			delete info.m_entityWidget;
			m_entityList.erase( itor );			
			break;
		}
	}

	UpdateLayout();
}


void ReAnimEntityPanel::OnSceneSelectionChanged()
{	
	QList< QGraphicsItem* > selection = m_scene->selectedItems();

	TAnimEntityListItor itor = m_entityList.begin();
	TAnimEntityListItor itorEnd = m_entityList.end();
	for( ; itor != itorEnd; ++itor )
	{
		ReAnimEntityInfo& info = *itor;
		info.m_entityWidget->Highlight( false );

		QList< QGraphicsItem* >::Iterator selItor = selection.begin();
		QList< QGraphicsItem* >::Iterator selItorEnd = selection.end();
		for( ; selItor != selItorEnd; ++selItor )
		{
			QGraphicsItem* sel = *selItor;
			if( info.m_graphicsItem == sel )
			{
				info.m_entityWidget->Highlight( true );
			}
		}
	}
}


void ReAnimEntityPanel::OnRulerCursorChanged( int _cursor )
{
	TAnimEntityListItor itor = m_entityList.begin();
	TAnimEntityListItor itorEnd = m_entityList.end();
	for( ; itor != itorEnd; ++itor )
	{
		ReAnimEntityInfo& info = *itor;
		ReAnimEntityWidget* entityWidget = info.m_entityWidget;
		ReAnimGraphicsItem* graphicsItem = info.m_graphicsItem;

		QPointF translation;
		qreal rotation = 0.0f;
		QPointF scale;
		qreal alpha = 1.0f;

		bool hasTranslation = entityWidget->GetTranslationAt( _cursor, translation, true );
		bool hasRotation = entityWidget->GetRotationAt( _cursor, rotation, true );
		bool hasScale = entityWidget->GetScaleAt( _cursor, scale, true );
		bool hasAlpha = entityWidget->GetAlphaAt( _cursor, alpha, true );
		if( hasTranslation || hasRotation || hasScale || hasAlpha )
		{
			graphicsItem->setVisible( true );
			graphicsItem->resetTransform();
			if( hasTranslation )
				//graphicsItem->translate( translation.x(), translation.y() );	// Come back later...
				graphicsItem->setPos( translation.x(), translation.y() );
			if( hasRotation )
				graphicsItem->rotate( rotation );
			if( hasScale )
				graphicsItem->scale( scale.x(), scale.y() );
			if( hasAlpha )
				graphicsItem->SetAlpha( alpha );
		}
		else
		{
			graphicsItem->setVisible( false );
		}
	}
}


// -----------------------------------------------------------------------------
// Override QWidget.
// -----------------------------------------------------------------------------
void ReAnimEntityPanel::paintEvent( QPaintEvent* _event )
{
	//QPainter painter( this );
	//painter.fillRect( _event->rect(), QColor( 121, 232, 232 ) );
}


void ReAnimEntityPanel::resizeEvent( QResizeEvent* _event )
{
	TSuper::resizeEvent( _event );

	UpdateLayout();
}


void ReAnimEntityPanel::mousePressEvent( QMouseEvent* _event )
{
	if( Qt::MidButton == _event->button() )
	{
		m_dragInfo.SetCursorPosBackup( _event->pos() );
		m_dragInfo.SetItemPosBackup( m_origin );
		m_dragInfo.StartMove();
	}
	else
	{
		_event->ignore();
	}
}


void ReAnimEntityPanel::mouseReleaseEvent( QMouseEvent* _event )
{
	if( Qt::MidButton == _event->button() )
	{
		m_dragInfo.Stop();
	}
	else
	{
		_event->ignore();
	}
}


void ReAnimEntityPanel::mouseMoveEvent( QMouseEvent* _event )
{
	if( m_dragInfo.IsMoving() )
	{
		QPoint delta = _event->pos() - m_dragInfo.GetCursorPosBackup();
		m_origin = m_dragInfo.GetItemPosBackup() + delta;

		UpdateLayout();
	}
	else
	{
		_event->ignore();
	}
}


// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------
void ReAnimEntityPanel::UpdateLayout()
{
	int x = GetLeftPadding();
	int y = m_origin.y();
	int w = width();

	TAnimEntityListItor itor = m_entityList.begin();
	TAnimEntityListItor itorEnd = m_entityList.end();
	for( ; itor != itorEnd; ++itor )
	{
		ReAnimEntityInfo& item = *itor;
		int h = item.m_entityWidget->GetTotalHeight();
		item.m_entityWidget->setGeometry( x, y, w, h );

		y += h + GetVerticalGap();
	}
}


}
