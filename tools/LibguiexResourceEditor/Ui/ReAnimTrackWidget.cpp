// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Core\ReAnimFrame.h"
#include "Core\ReAnimTrack.h"
#include "Ui\ReAnimTrackWidget.h"
#include "Ui\ReAnimUiInfo.h"
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
ReAnimTrackWidget::ReAnimTrackWidget( ReAnimTrack* _model, eTrackType _type, QWidget* _parent /* = NULL */ )
: TSuper( _parent )
, m_modelData( _model )
, m_editMenu( NULL )
, m_currentFrame( NULL )
, m_type( _type )
{
	InitMenus();

	connect( this, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnContextMenu( const QPoint& ) ) );

	ShowCursorValue( false );
}


ReAnimFrameWidget* ReAnimTrackWidget::GetFrameByIndex( int _index )
{
	// TODO
	return NULL;
}


// -----------------------------------------------------------------------------
// Override QWidget.
// -----------------------------------------------------------------------------
void ReAnimTrackWidget::paintEvent( QPaintEvent* _event )
{
	TSuper::paintEvent( _event );
}


void ReAnimTrackWidget::mousePressEvent( QMouseEvent* _event )
{
	ReAnimFrameWidget* frame = dynamic_cast< ReAnimFrameWidget* >( childAt( _event->pos() ) );
	if( NULL != frame )
	{
		m_currentFrame = frame;
		frame->GetDragInfoRef().SetCursorPosBackup( _event->pos() );
		frame->GetDragInfoRef().SetItemPosBackup( frame->pos() );
		frame->GetDragInfoRef().StartMove();
	}
	else
	{
		m_currentFrame = NULL;
		TSuper::mousePressEvent( _event );
	}
}


void ReAnimTrackWidget::mouseReleaseEvent( QMouseEvent* _event )
{
	if( NULL != m_currentFrame )
	{
		m_currentFrame->GetDragInfoRef().Stop();
	}

	TSuper::mouseReleaseEvent( _event );
}


void ReAnimTrackWidget::mouseMoveEvent( QMouseEvent* _event )
{
	if( NULL != m_currentFrame && m_currentFrame->GetDragInfo().IsMoving() )
	{
		QPoint delta = _event->pos() - m_currentFrame->GetDragInfo().GetCursorPosBackup();
		QPoint newPos = m_currentFrame->GetDragInfo().GetItemPosBackup() + delta / GetSnap() * GetSnap();
		newPos.setY( 0 );
		int centerX = newPos.x() + m_currentFrame->width() / 2;
		m_currentFrame->move( newPos.x(), newPos.y() );

		// Update model data.		
		qreal time = GetValueAt( newPos.x() + m_currentFrame->width() / 2 );
		ReAnimFrame* frameData = m_currentFrame->GetModelData();
		frameData->SetTime( time );
	}
	else
	{
		TSuper::mouseMoveEvent( _event );
	}
}


void ReAnimTrackWidget::keyPressEvent( QKeyEvent* _event )
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
QMenu* ReAnimTrackWidget::GetEditMenu() const
{
	return m_editMenu;
}


// -----------------------------------------------------------------------------
// Override ReRulerWidget.
// -----------------------------------------------------------------------------
void ReAnimTrackWidget::DrawBackground( QPainter& _painter )
{
	_painter.fillRect( 0, 0, width(), height(), QColor( 150, 140, 35 ) );
}


void ReAnimTrackWidget::DrawMarks( QPainter& _painter )
{
}


void ReAnimTrackWidget::DrawForeground( QPainter& _painter )
{
	TSuper::DrawForeground( _painter );
}


void ReAnimTrackWidget::OnViewportChanged( int _pos )
{
	TFramePoolItor itor = m_frameList.Begin();
	TFramePoolItor itorEnd = m_frameList.End();
	for( int delta = m_viewport - _pos; itor != itorEnd; ++itor )
	{
		ReAnimFrameWidget* frame = *itor;
		frame->move( frame->pos() + QPoint( delta, 0 ) );
	}

	TSuper::OnViewportChanged( _pos );
}


// -----------------------------------------------------------------------------
// Override ReModelBase.
// -----------------------------------------------------------------------------
void ReAnimTrackWidget::RecycleData( ReAnimFrameWidget* _frame )
{
	if( NULL != _frame )
	{
		disconnect( _frame, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnContextMenu( const QPoint& ) ) );

		_frame->setParent( NULL );
		m_frameList.Erase( _frame );
		TSuperB::RecycleData( _frame );
	}
}


// -----------------------------------------------------------------------------
// Slots.
// -----------------------------------------------------------------------------
void ReAnimTrackWidget::OnContextMenu( const QPoint& _point )
{
	ReAnimFrameWidget* frameWidget = GetFrameAtCursor( GetCursor() );
	m_editMenu->setDisabled( NULL != frameWidget );
	m_editMenu->exec( mapToGlobal( _point ) );
}


void ReAnimTrackWidget::OnCreateFrame()
{
	if( NULL == GetFrameAtCursor( GetCursor() ) )
	{
		CreateFrameAtCursor( m_cursor );
	}
}


void ReAnimTrackWidget::OnDeleteFrame()
{
	//emit DeleteFrameRequested( this );

	if( NULL != m_currentFrame )
	{		
		m_modelData->DeleteFrame( m_currentFrame->GetModelData() );
		RecycleData( m_currentFrame );
		m_currentFrame = NULL;
	}
}


// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------
void ReAnimTrackWidget::InitMenus()
{
	QAction* action = NULL;

	m_editMenu = new QMenu( "&Edit" );	
	action = m_editMenu->addAction( tr( "New &Frame" ) );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnCreateFrame() ) );
}


int ReAnimTrackWidget::CalcCursorAtFrame( const ReAnimFrameWidget* _frame ) const
{
	return ( NULL != _frame ) ? ( _frame->pos().x() + _frame->width() / 2 ) : ms_invalidCursor;
}


ReAnimFrameWidget* ReAnimTrackWidget::GetFrameAtCursor( int _cursor )
{
	ReAnimFrameWidget* result = NULL;
	qreal value = GetValueAt( _cursor );

	TFramePoolItor itor = m_frameList.Begin();
	TFramePoolItor itorEnd = m_frameList.End();
	for( ; itor != itorEnd; ++itor )
	{
		ReAnimFrameWidget* frameWidget = *itor;
		ReAnimFrame* frameData = frameWidget->GetModelData();
		if( frameData->GetTime() == value )
		{
			result = frameWidget;
			break;
		}
	}

	return result;
}


ReAnimFrameWidget* ReAnimTrackWidget::CreateFrameAtCursor( int _cursor )
{
	// UI.
	ReAnimFrameWidget* frameWidget = CreateData();
	frameWidget->setParent( this );
	frameWidget->setVisible( true );
	frameWidget->move( _cursor - ANIM_FRAME_WIDTH / 2, ( height() - ANIM_FRAME_HEIGHT ) / 2  );
	frameWidget->resize( ANIM_FRAME_SIZE );
	m_frameList.Push( frameWidget );

	// Model.
	QVariant value = QVariant();
	qreal time = GetValueAt( _cursor );
	bool isValueValid = m_modelData->Interpolate( time, value, true );

	ReAnimFrame* frameData = m_modelData->CreateFrame( time );
	if( isValueValid )
		frameData->SetFrameValue( value );
	frameWidget->SetModelData( frameData );

	// Interpolate.

	return frameWidget;
}



// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


}
