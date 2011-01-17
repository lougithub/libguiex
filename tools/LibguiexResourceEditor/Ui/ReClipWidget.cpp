// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101115.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "ReClipWidget.h"
#include "Core\ReDragInfo.h"
#include "Core\ReClipModel.h"
#include <QPainter>
#include <QMouseEvent>


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
QSize ReClipWidget::ms_minSize = QSize( 8, 8 );


ReClipWidget::ReClipWidget( QWidget* _parent )
: TSuper( _parent )
, m_zoomFactor( 1 )
, m_isShowOutline( false )
{
	setMouseTracking( true );
	setFocusPolicy( Qt::NoFocus );
}


ReClipWidget::~ReClipWidget()
{
	ReClipModel* clipModel = m_modelData->GetModel();
	clipModel->DestroyClip( m_modelData );
}


// -----------------------------------------------------------------------------
// Override.
// -----------------------------------------------------------------------------
void ReClipWidget::paintEvent( QPaintEvent* _event )
{
	if( NULL != pixmap() )
	{
		TSuper::paintEvent( _event );
	}
	else
	{
		QPainter painter( this );
		painter.fillRect( 0, 0, width(), height(), QColor( 255, 255, 0, 50 ) );

		if( m_isShowOutline )
		{
			painter.setPen( Qt::DashDotLine );
			painter.setPen( QColor( 0, 255, 0 ) );
			painter.drawRect( 0, 0, width() - 1, height() - 1 );
		}

		painter.drawText( 0, height(), tr( "(%1,%2)(%3,%4)" )			
			.arg( pos().x() ).arg( pos().y() )
			.arg( width() ).arg( height() ) );
	}
}


void ReClipWidget::mouseMoveEvent( QMouseEvent* _event )
{
	ReDragInfo::eResize resizeDir = CalcResizeType( _event->pos() );

	if( ReDragInfo::EResize_LeftTop == resizeDir || ReDragInfo::EResize_RightBottom == resizeDir )
		setCursor( QCursor( Qt::SizeFDiagCursor ) );
	else if( ReDragInfo::EResize_RightTop == resizeDir || ReDragInfo::EResize_LeftBottom == resizeDir )
		setCursor( QCursor( Qt::SizeBDiagCursor ) );
	else if( ReDragInfo::EResize_Left == resizeDir || ReDragInfo::EResize_Right == resizeDir )
		setCursor( QCursor( Qt::SizeHorCursor ) );
	else if( ReDragInfo::EResize_Top == resizeDir || ReDragInfo::EResize_Bottom == resizeDir )
		setCursor( QCursor( Qt::SizeVerCursor ) );
	else
		setCursor( QCursor( Qt::ArrowCursor ) );

	_event->ignore();
}


void ReClipWidget::resizeEvent( QResizeEvent* _event )
{
	if( NULL != m_modelData )
		m_modelData->SetSize( QSize( _event->size() ) );
}


void ReClipWidget::moveEvent( QMoveEvent* _event )
{
	if( NULL != m_modelData )
		m_modelData->SetOffset( _event->pos() );
}


// -------------------------------------------------------------------------
// Utilities.
// -------------------------------------------------------------------------



}
