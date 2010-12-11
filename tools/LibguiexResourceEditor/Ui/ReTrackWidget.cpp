// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReTrackWidget.h"
#include "Core\ReAnimFrame.h"
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <QKeyEvent>


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

	ShowCursorValue( false );
}


ReTrackFrameWidget* ReTrackWidget::CreateFrameAtCurrentCursor()
{
	ReTrackFrameWidget* result = DoCreateFrame();
	result->move( m_cursor - m_frameSize.width() / 2, ( height() - m_frameSize.height() ) / 2  );
	result->resize( m_frameSize );

	return result;
}


void ReTrackWidget::DeleteCurrentFrame()
{
	if( NULL != m_currentFrame )
	{
		RecycleData( m_currentFrame );
		m_currentFrame = NULL;
	}
}


QVariant ReTrackWidget::Interpolate( int _cursor ) const
{
	ReTrackFrameWidget* left = NULL;
	ReTrackFrameWidget* right = NULL;
	( const_cast< ReTrackWidget* >( this ) )->GetNearestFrames( _cursor, left, right );

	if( NULL != left && NULL != right )
	{
		if( left == right )
		{
			//return left->GetData();
			left->GetModelData()->GetTime();
		}
		else
		{
			int leftCursor = CalcCursorAtFrame( left );
			int rightCursor = CalcCursorAtFrame( right );
			qreal factor = ( qreal )( _cursor - leftCursor ) / ( qreal )( rightCursor - leftCursor );
			return Interpolate( left->GetModelData()->GetTime(), right->GetModelData()->GetTime(), factor );
		}
	}
	else if( NULL != left )
	{
		//return left->GetData();
		return left->GetModelData()->GetTime();
	}
	else
	{
		int rightCursor = CalcCursorAtFrame( right );
		qreal factor = ( qreal )_cursor / ( qreal )rightCursor;
		return Interpolate( QVariant(), right->GetModelData()->GetTime(), factor );
	}
}


ReTrackFrameWidget* ReTrackWidget::GetFrameByIndex( int _index )
{
	// TODO
	return NULL;
}


// -----------------------------------------------------------------------------
// Override QWidget.
// -----------------------------------------------------------------------------
void ReTrackWidget::paintEvent( QPaintEvent* _event )
{
	TSuper::paintEvent( _event );

	QPainter painter( this );

	TFramePoolItor itor = m_frameList.Begin();
	TFramePoolItor itorEnd = m_frameList.End();
	for( ; itor != itorEnd; ++itor )
	{
		ReTrackFrameWidget* frame = *itor;
		int pos = CalcCursorAtFrame( frame );
		painter.drawText( frame->pos().x() + 10, height() / 2, QString( tr( "%1" ) ).arg( pos ) );
	}

	painter.drawText( GetCursor(), height() - 10, QString( tr( "%1" ) ).arg( GetCursor() ) );
}


void ReTrackWidget::mousePressEvent( QMouseEvent* _event )
{
	ReTrackFrameWidget* frame = dynamic_cast< ReTrackFrameWidget* >( childAt( _event->pos() ) );
	if( NULL != frame )
	{
		m_currentFrame = frame;
		frame->GetDragInfoRef().SetCursorPosBackup( _event->pos() );
		frame->GetDragInfoRef().SetItemPosBackup( frame->pos() );
		frame->GetDragInfoRef().StartMove();
	}
	else
	{
		m_isShowCursorValue = true;
		m_currentFrame = NULL;
		TSuper::mousePressEvent( _event );
	}
}


void ReTrackWidget::mouseReleaseEvent( QMouseEvent* _event )
{
	if( NULL != m_currentFrame && m_currentFrame->GetDragInfo().IsMoving() )
	{
		m_currentFrame->GetDragInfoRef().Stop();
	}

	m_isShowCursorValue = false;
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
		qreal time = GetValueAt( newPos.x() + m_currentFrame->width() / 2 );

		emit FrameMoved( this, m_currentFrame, time );
	}
	else
	{
		TSuper::mouseMoveEvent( _event );
	}
}


void ReTrackWidget::keyPressEvent( QKeyEvent* _event )
{
	if( Qt::Key_A == _event->key() )
	{
		OnCreateFrame();
	}
	else if( Qt::Key_D == _event->key() )
	{
		OnDeleteFrame();
	}
	else
	{
		TSuper::keyPressEvent( _event );
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

	if( NULL != m_currentFrame )
	{
		int half = m_currentFrame->size().width() / 2;
		int cursor = m_currentFrame->pos().x() + half;
		QString valueText = QString().setNum( GetValueAt( cursor ) );
		if( cursor <= ( width() / 2 ) )
		{
			_painter.drawText( cursor + half + 2, height() - 2, valueText );
		}
		else
		{
			QFontMetrics fm( _painter.font() );
			_painter.drawText( cursor - half - 2 - fm.width( valueText ), height() - 2, valueText );
		}
	}
}


// -----------------------------------------------------------------------------
// Override ReModelBase.
// -----------------------------------------------------------------------------
void ReTrackWidget::RecycleData( ReTrackFrameWidget* _frame )
{
	if( NULL != _frame )
	{
		_frame->setParent( NULL );
		m_frameList.Erase( _frame );
		TSuperB::RecycleData( _frame );
	}
}


// -----------------------------------------------------------------------------
// Override ReRulerWidget.
// -----------------------------------------------------------------------------
void ReTrackWidget::OnViewportChanged( int _pos )
{
	TFramePoolItor itor = m_frameList.Begin();
	TFramePoolItor itorEnd = m_frameList.End();
	for( int delta = m_viewport - _pos; itor != itorEnd; ++itor )
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


void ReTrackWidget::OnCreateFrame()
{
	emit CreateFrameRequested( this );
}


void ReTrackWidget::OnDeleteFrame()
{
	emit DeleteFrameRequested( this );
}


// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------
void ReTrackWidget::InitMenus()
{
	QAction* action = NULL;

	m_editMenu = new QMenu( "&Edit" );	
	action = m_editMenu->addAction( tr( "New &Frame" ) );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnCreateFrame() ) );
}


ReTrackFrameWidget* ReTrackWidget::DoCreateFrame()
{
	// Update UI.
	ReTrackFrameWidget* result = CreateData();
	result->setParent( this );
	//result->GetIdRef() = ReTrackFrameWidget::sNextId();
	result->setScaledContents( true );
	result->setPixmap( QPixmap( ":/image/editor_track_frame.png" ) );
	result->setVisible( true );

	m_frameList.Push( result );

	return result;
}


void ReTrackWidget::GetNearestFrames( int _cursor, ReTrackFrameWidget*& _left, ReTrackFrameWidget*& _right )
{
	if( NULL != ( &_left ) || NULL != ( &_right ) )
	{
		ReTrackFrameWidget* left = NULL;
		ReTrackFrameWidget* right = NULL;
		int minLeft = 999999;
		int minRight = 999999;

		TFramePoolItor itor = m_frameList.Begin();
		TFramePoolItor itorEnd = m_frameList.End();
		for( ; itor != itorEnd; ++itor )
		{		
			ReTrackFrameWidget* frame = *itor;
			int cursor = CalcCursorAtFrame( frame );
			if( cursor == _cursor )
			{
				if( NULL != ( &_left ) )
					_left = frame;
				if( NULL != ( &_right ) )
					_right = frame;
			}
			else if( cursor < _cursor )
			{
				if( NULL != ( &_left ) )
				{
					int delta = _cursor - cursor;
					if( delta < minLeft )
					{
						minLeft = delta;
						_left = frame;
					}
				}
			}
			else
			{
				if( NULL != ( &_right ) )
				{
					int delta = cursor - _cursor;
					if( delta < minRight )
					{
						minRight = delta;
						_right = frame;
					}
				}
			}
		}
	}
}


int ReTrackWidget::CalcCursorAtFrame( const ReTrackFrameWidget* _frame ) const
{
	return ( NULL != _frame ) ? ( _frame->pos().x() + _frame->width() / 2 ) : ms_invalidCursor;
}



// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


}
