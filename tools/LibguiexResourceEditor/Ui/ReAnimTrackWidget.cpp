// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Core\ReAnimFrame.h"
#include "Core\ReAnimTrack.h"
#include "Core\ReAnimModel.h"
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
{
	InitMenus();

	connect( this, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnContextMenu( const QPoint& ) ) );

	ShowCursorValue( false );
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
	if( Qt::LeftButton == _event->button() )
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
	else
	{
		_event->ignore();
	}
}


void ReAnimTrackWidget::mouseReleaseEvent( QMouseEvent* _event )
{
	if( Qt::MidButton == _event->button() )
	{
		_event->ignore();
	}
	else
	{
		if( NULL != m_currentFrame )
		{
			m_currentFrame->GetDragInfoRef().Stop();
		}

		TSuper::mouseReleaseEvent( _event );
	}
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
		if( Qt::MidButton & _event->buttons() )
			_event->ignore();
		else
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
	m_createFrameAction->setDisabled( NULL != frameWidget );
	m_deleteFrameAction->setDisabled( NULL == frameWidget );
	m_editMenu->exec( mapToGlobal( _point ) );
}


void ReAnimTrackWidget::OnCreateFrame()
{
	if( NULL == GetFrameAtCursor( GetCursor() ) )
	{
		CreateFrameAtCursor( m_cursor );

		emit DataChangedAt( m_cursor );
	}
}


void ReAnimTrackWidget::OnDeleteFrame()
{
	ReAnimFrameWidget* frame = GetFrameAtCursor( GetCursor() );
	if( NULL != frame )
	{
		ReAnimModel* animModel = m_modelData->GetModel();
		animModel->DestroyFrame( frame->GetModelData() );
		RecycleData( frame );
		frame = NULL;

		emit DataChangedAt( m_cursor );
	}
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
// Utilities.
// -----------------------------------------------------------------------------
void ReAnimTrackWidget::InitMenus()
{
	QAction* action = NULL;

	m_editMenu = new QMenu( "&Edit" );	
	m_createFrameAction = m_editMenu->addAction( tr( "New &Frame" ) );
	connect( m_createFrameAction, SIGNAL( triggered() ), this, SLOT( OnCreateFrame() ) );
	m_deleteFrameAction = m_editMenu->addAction( tr( "Delete &Frame" ) );
	connect( m_deleteFrameAction, SIGNAL( triggered() ), this, SLOT( OnDeleteFrame() ) );
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

	ReAnimModel* animModel = m_modelData->GetModel();
	ReAnimFrame* frameData = animModel->CreateFrame( m_modelData );
	frameData->SetTime( time );
	if( isValueValid )
		frameData->SetFrameValue( value );
	frameWidget->SetModelData( frameData );

	//emit FrameCreatedAt( _cursor );

	return frameWidget;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


}
