// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101127.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Core\ReAnimFrame.h"
#include "Ui\ReAnimFrameWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QEvent>


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReAnimFrameWidget::ReAnimFrameWidget( QWidget* _parent /* = NULL */ )
: TSuper( _parent )
, m_modelData( NULL )
{
	setCursor( Qt::OpenHandCursor );
	setFocusPolicy( Qt::ClickFocus );
}


// -----------------------------------------------------------------------------
// Override QWidget.
// -----------------------------------------------------------------------------
bool ReAnimFrameWidget::event( QEvent* _event )
{
	if( QEvent::ToolTip == _event->type() )
	{
		if( NULL != m_modelData )
		{
			setToolTip( m_modelData->GetDescription() );
		}
	}

	return TSuper::event( _event );
}


void ReAnimFrameWidget::paintEvent( QPaintEvent* _event )
{
	QPainter painter( this );
	painter.fillRect( 0, 0, width(), height(), QColor( 255, 255, 255 ) );
}


void ReAnimFrameWidget::mousePressEvent( QMouseEvent* _event )
{
	if( Qt::LeftButton == _event->button() )
	{
		setCursor( Qt::ClosedHandCursor );
	}

	_event->ignore();
}


void ReAnimFrameWidget::mouseReleaseEvent( QMouseEvent* _event )
{
	if( Qt::LeftButton == _event->button() )
	{
		setCursor( Qt::OpenHandCursor );
	}

	_event->ignore();	
}


void ReAnimFrameWidget::focusInEvent( QFocusEvent* _event )
{
	emit EditStarted( this );
}


void ReAnimFrameWidget::focusOutEvent( QFocusEvent* _event )
{
	//emit EditEnded( this );
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