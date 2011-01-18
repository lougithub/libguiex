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
ReClipWidget::ReClipWidget( QWidget* _parent )
: TSuper( _parent )
, m_isShowOutline( false )
{
	setMouseTracking( true );
	setFocusPolicy( Qt::NoFocus );
}


ReClipWidget::~ReClipWidget()
{
}


void ReClipWidget::InitFromModelData( ReClipNode* _modelData )
{
	m_modelData = _modelData;
	move( _modelData->GetOffset() );
	resize( _modelData->GetSize() );
}


// -----------------------------------------------------------------------------
// Override.
// -----------------------------------------------------------------------------
void ReClipWidget::paintEvent( QPaintEvent* _event )
{
	QPainter painter( this );
	painter.fillRect( 0, 0, width(), height(), QColor( 255, 255, 0, 50 ) );

	if( m_isShowOutline )
	{
		painter.setPen( Qt::DashDotLine );
		painter.setPen( QColor( 0, 255, 0 ) );
		painter.drawRect( _event->rect().adjusted( 0, 0, -1, -1 ) );
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
