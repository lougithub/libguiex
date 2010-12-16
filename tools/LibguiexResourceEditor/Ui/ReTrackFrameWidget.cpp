// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101127.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReTrackFrameWidget.h"
#include <QPainter>
#include <QMouseEvent>


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
int ReTrackFrameWidget::ms_nextId = 0;


int ReTrackFrameWidget::sNextId()
{
	return ms_nextId++;
}


ReTrackFrameWidget::ReTrackFrameWidget( QWidget* _parent /* = NULL */ )
: TSuper( _parent )
//, m_id( 0 )
, m_modelData( NULL )
{
	setCursor( Qt::OpenHandCursor );
}


// -----------------------------------------------------------------------------
// Override QWidget.
// -----------------------------------------------------------------------------
void ReTrackFrameWidget::paintEvent( QPaintEvent* _event )
{
	//QPainter painter( this );
	//painter.fillRect( 0, 0, width(), height(), QColor( 255, 255, 255 ) );
	TSuper::paintEvent( _event );
}


void ReTrackFrameWidget::mousePressEvent( QMouseEvent* _event )
{
	if( Qt::LeftButton == _event->button() )
	{
		setCursor( Qt::ClosedHandCursor );
	}

	_event->ignore();
}


void ReTrackFrameWidget::mouseReleaseEvent( QMouseEvent* _event )
{
	if( Qt::LeftButton == _event->button() )
	{
		setCursor( Qt::OpenHandCursor );
	}

	_event->ignore();
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------



}