// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101211.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReAnimView.h"
#include "Ui\ReRulerWidget.h"
#include <QPainter>
#include <QMouseEvent>


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReAnimView::ReAnimView( QWidget* _parent /* = NULL */ )
: TSuper( _parent )
, m_width( 0 )
, m_height( 0 )
, m_origin( 0, 0 )
, m_horizontalRuler( NULL )
, m_verticalRuler( NULL )
{
}


// -----------------------------------------------------------------------------
// Override QWidget.
// -----------------------------------------------------------------------------
void ReAnimView::paintEvent( QPaintEvent* _event )
{
	QPainter painter( this );

	painter.drawText( 0, height(), QString( tr( "(%1,%2)" ) ).arg( m_origin.x() ).arg( m_origin.y() ) );

	painter.translate( m_origin );
	painter.setMatrix( m_matrix, true );
	painter.drawText( 0, 0, tr( "Animation View" ) );
}


void ReAnimView::mousePressEvent( QMouseEvent* _event )
{
	if( Qt::MidButton == _event->button() )
	{
		m_dragInfo.SetCursorPosBackup( _event->pos() );
		m_dragInfo.SetItemPosBackup( m_origin );
		m_dragInfo.StartMove();
	}
}


void ReAnimView::mouseReleaseEvent( QMouseEvent* _event )
{
	m_dragInfo.Stop();
}


void ReAnimView::mouseMoveEvent( QMouseEvent* _event )
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


void ReAnimView::resizeEvent( QResizeEvent* _event )
{
	TSuper::resizeEvent( _event );

	QSize oldSize = _event->oldSize();
	QSize newSize = _event->size();
	int widthDelta = newSize.width() - oldSize.width();
	int heightDelta = newSize.height() - oldSize.height();

	// Ruler.
	if( NULL != m_horizontalRuler && NULL != m_verticalRuler )
	{
		QRect rulerGeo = m_horizontalRuler->geometry();
		rulerGeo.setWidth( rulerGeo.width() + widthDelta );
		m_horizontalRuler->setGeometry( rulerGeo );

		rulerGeo = m_verticalRuler->geometry();
		rulerGeo.setHeight( rulerGeo.height() + heightDelta );
		m_verticalRuler->setGeometry( rulerGeo );
	}
	else
	{
		InitRulers();
	}
}


// -----------------------------------------------------------------------------
// Override ReBaseWidget.
// -----------------------------------------------------------------------------
void ReAnimView::Tick( qreal _delta )
{

}


// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------
void ReAnimView::InitRulers()
{
	m_horizontalRuler = new ReRulerWidget( this );
	m_verticalRuler = new ReRulerWidget( this );
	m_verticalRuler->SetHorizontal( false );

	int x = m_verticalRuler->GetRulerHeight();
	int y = 0;
	int w = width() - x;
	int h = m_verticalRuler->GetRulerHeight();
	m_horizontalRuler->setGeometry( x, y, w, h );

	x = 0;
	y = m_horizontalRuler->GetRulerHeight();
	w = m_verticalRuler->GetRulerHeight();
	h = height() - y;
	m_verticalRuler->setGeometry( x, y, w, h );
}


}
