// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReTrackWidget.h"
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReTrackWidget::ReTrackWidget( QWidget* _parent /* = NULL */ )
: TSuper( _parent )
, m_editMenu( NULL )
, m_currentFrame( NULL )
, m_frameSize( 5, 15 )
{
	InitMenus();

	connect( this, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnContextMenu( const QPoint& ) ) );
}


// -----------------------------------------------------------------------------
// Override QWidget.
// -----------------------------------------------------------------------------
void ReTrackWidget::paintEvent( QPaintEvent* _event )
{
	TSuper::paintEvent( _event );
}


void ReTrackWidget::mousePressEvent( QMouseEvent* _event )
{
	ReTrackFrameWidget* frame = qobject_cast< ReTrackFrameWidget* >( childAt( _event->pos() ) );
	if( NULL != frame )
	{
		m_currentFrame = frame;
		frame->GetDragInfoRef().SetCursorPosBackup( _event->pos() );
		frame->GetDragInfoRef().SetItemPosBackup( frame->pos() );
		frame->GetDragInfoRef().StartMove();
	}
	else
	{
		TSuper::mousePressEvent( _event );
	}
}


void ReTrackWidget::mouseReleaseEvent( QMouseEvent* _event )
{
	if( NULL != m_currentFrame && m_currentFrame->GetDragInfo().IsMoving() )
	{
		m_currentFrame->GetDragInfoRef().Stop();
	}

	TSuper::mouseReleaseEvent( _event );
}


void ReTrackWidget::mouseMoveEvent( QMouseEvent* _event )
{
	if( NULL != m_currentFrame && m_currentFrame->GetDragInfo().IsMoving() )
	{
		QPoint delta = _event->pos() - m_currentFrame->GetDragInfo().GetCursorPosBackup();
		QPoint newPos = m_currentFrame->GetDragInfo().GetItemPosBackup() + delta;
		newPos.setY( 0 );
		int centerX = newPos.x() + m_currentFrame->width() / 2;
		if( centerX < 0 )
			newPos.setX( -m_currentFrame->width() / 2 );
		else if( centerX >= width() )
			newPos.setX( width() - m_currentFrame->width() / 2 );

		m_currentFrame->move( newPos.x(), newPos.y() );
	}
	else
	{
		TSuper::mouseMoveEvent( _event );
	}
}


// -----------------------------------------------------------------------------
// Override ReBaseWidget.
// -----------------------------------------------------------------------------
QMenu* ReTrackWidget::GetEditMenu() const
{
	return m_editMenu;
}


void ReTrackWidget::DrawBackground( QPainter& _painter )
{
	_painter.fillRect( 0, 0, width(), height(), QColor( 50, 50, 50 ) );
}


void ReTrackWidget::DrawContent( QPainter& _painter )
{
}


void ReTrackWidget::DrawForeground( QPainter& _painter )
{
	TSuper::DrawForeground( _painter );
}


// -----------------------------------------------------------------------------
// Override ReModelBase.
// -----------------------------------------------------------------------------
void ReTrackWidget::RecycleData( ReTrackFrameWidget* _frame )
{
	TSuperB::RecycleData( _frame );
}


// -----------------------------------------------------------------------------
// Override ReRulerWidget.
// -----------------------------------------------------------------------------
void ReTrackWidget::OnViewportChanged( int _pos )
{
	TFramePoolItor itor = m_frameList.Begin();
	TFramePoolItor itorEnd = m_frameList.End();
	for( int delta = m_viewportPos - _pos; itor != itorEnd; ++itor )
	{
		ReTrackFrameWidget* frame = *itor;
		frame->move( frame->pos() + QPoint( delta, 0 ) );
	}

	TSuper::OnViewportChanged( _pos );
}


// -----------------------------------------------------------------------------
// Slots.
// -----------------------------------------------------------------------------
void ReTrackWidget::OnContextMenu( const QPoint& _point )
{
	m_editMenu->exec( mapToGlobal( _point ) );
}


void ReTrackWidget::OnNewFrame()
{
	ReTrackFrameWidget* newFrame = CreateFrame();
	newFrame->move( m_cursor - m_frameSize.width() / 2, ( height() - m_frameSize.height() ) / 2  );
	newFrame->resize( m_frameSize );
}


// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------
void ReTrackWidget::InitMenus()
{
	QAction* action = NULL;

	m_editMenu = new QMenu( "&Edit" );	
	action = m_editMenu->addAction( tr( "New &Frame" ) );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnNewFrame() ) );
}


ReTrackFrameWidget* ReTrackWidget::CreateFrame()
{
	ReTrackFrameWidget* result = CreateData();
	result->setParent( this );
	result->setScaledContents( true );
	result->setPixmap( QPixmap( ":/image/editor_track_frame.png" ) );
	result->setVisible( true );

	m_frameList.Push( result );

	return result;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


}
