// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101211.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReAnimGraphicsWidget.h"
#include "Ui\ReRulerWidget.h"
#include "Ui\ReAnimGraphicsView.h"
#include "Ui\ReAnimGraphicsScene.h"
#include "Ui\ReAnimGraphicsItem.h"
#include "Ui\ReAnimUiInfo.h"
#include <QPainter>
#include <QMouseEvent>

namespace
{
	// Named like member variables so that it's easy to port in the future.
	const int m_leftPadding = 0;
	const int m_topPadding = 0;
	const int m_rightPadding = 0;
	const int m_bottomPading = 0;
	const int m_horizontalGap = 0;
	const int m_verticalGap = 0;
}


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReAnimGraphicsWidget::ReAnimGraphicsWidget( ReAnimModel* _model, QGraphicsScene* _scene, QWidget* _parent /* = NULL */ )
: TSuper( _parent )
, m_scene( _scene )
, m_width( 0 )
, m_height( 0 )
, m_origin( 0, 0 )
, m_horizontalRuler( NULL )
, m_verticalRuler( NULL )
{	
	m_view = new ReAnimGraphicsView( m_scene, this );

	m_horizontalRuler = new ReRulerWidget( this );
	m_horizontalRuler->SetUnit( ANIM_RULER_UNIT );
	m_horizontalRuler->SetUnitValue( ( qreal )m_horizontalRuler->GetUnit() );
	m_verticalRuler = new ReRulerWidget( this );
	m_verticalRuler->SetHorizontal( false );
	m_verticalRuler->SetUnit( ANIM_RULER_UNIT );
	m_verticalRuler->SetUnitValue( ( qreal )m_horizontalRuler->GetUnit() );

	connect( m_scene, SIGNAL( selectionChanged() ), m_view, SLOT( OnSceneSelectionChanged() ) );
	connect( m_scene, SIGNAL( ItemAdded( ReAnimGraphicsItem* ) ), this, SIGNAL( ItemAdded( ReAnimGraphicsItem* ) ) );
	connect( m_scene, SIGNAL( ItemDeleted( ReAnimGraphicsItem* ) ), this, SIGNAL( ItemDeleted( ReAnimGraphicsItem* ) ) );
	connect( m_view, SIGNAL( SceneOriginXChanged( int ) ), m_horizontalRuler, SLOT( OnViewportChanged( int ) ) );
	connect( m_view, SIGNAL( SceneOriginYChanged( int ) ), m_verticalRuler, SLOT( OnViewportChanged( int ) ) );
	connect( m_horizontalRuler, SIGNAL( ViewportChanged( int ) ), m_view, SLOT( RequestChangeSceneOriginX( int ) ) );
	connect( m_verticalRuler, SIGNAL( ViewportChanged( int ) ), m_view, SLOT( RequestChangeSceneOriginY( int ) ) );
}


// -----------------------------------------------------------------------------
// Override QWidget.
// -----------------------------------------------------------------------------
void ReAnimGraphicsWidget::mousePressEvent( QMouseEvent* _event )
{
	if( Qt::MidButton == _event->button() )
	{
		m_dragInfo.SetCursorPosBackup( _event->pos() );
		m_dragInfo.SetItemPosBackup( m_origin );
		m_dragInfo.StartMove();
	}
}


void ReAnimGraphicsWidget::mouseReleaseEvent( QMouseEvent* _event )
{
	m_dragInfo.Stop();
}


void ReAnimGraphicsWidget::mouseMoveEvent( QMouseEvent* _event )
{
	if( Qt::MidButton & _event->buttons() )
	{
		if( m_dragInfo.IsMoving() )
		{
			QPoint delta = _event->pos() - m_dragInfo.GetCursorPosBackup();
			m_origin = m_dragInfo.GetItemPosBackup() + delta;
		}
	}
}


void ReAnimGraphicsWidget::resizeEvent( QResizeEvent* _event )
{
	TSuper::resizeEvent( _event );

	int xH = m_leftPadding + m_verticalRuler->GetRulerHeight() + m_horizontalGap;
	int yH = m_topPadding;
	int wH = _event->size().width() - xH - m_rightPadding;
	int hH = m_verticalRuler->GetRulerHeight();
	m_horizontalRuler->setGeometry( xH, yH, wH, hH );

	int xV = m_leftPadding;
	int yV = m_topPadding + m_horizontalRuler->GetRulerHeight() + m_verticalGap;
	int wV = m_verticalRuler->GetRulerHeight();
	int hV = _event->size().height() - yV - m_bottomPading;
	m_verticalRuler->setGeometry( xV, yV, wV, hV );

	m_view->setGeometry( xH, yV, wH, hV );
}


// -----------------------------------------------------------------------------
// Override ReBaseWidget.
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------


}
