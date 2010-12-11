// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101211.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReAnimView.h"
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
{

}


// -----------------------------------------------------------------------------
// Override QWidget.
// -----------------------------------------------------------------------------
void ReAnimView::paintEvent( QPaintEvent* _event )
{
	QPainter painter( this );

	painter.drawText( 0, height(), QString( tr( "(%1,%2)" ) ).arg( m_origin.x() ).arg( m_origin.y() ) );

	//painter.translate( m_origin );
	painter.setMatrix( m_matrix );
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


// -----------------------------------------------------------------------------
// Override ReBaseWidget.
// -----------------------------------------------------------------------------
void ReAnimView::Tick( qreal _delta )
{

}



// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------



}
