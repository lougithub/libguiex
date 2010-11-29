// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101115.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "ReClipWidget.h"
#include "Core\ReClipData.h"
#include "Core\ReDragInfo.h"
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
}


void ReClipWidget::PushData()
{
	if( NULL != m_modelData )
	{
		QWidget* parent = parentWidget();
		if( NULL != parent )
		{
			QRect rect = parent->geometry();
			QSize size = TSuper::size();
			QPoint pos = TSuper::pos();

			qreal x = ( qreal )pos.x() / ( qreal )rect.width();
			qreal y = ( qreal )pos.y() / ( qreal )rect.height();
			qreal w = ( qreal )size.width() / ( qreal )rect.width();
			qreal h = ( qreal )size.height() / ( qreal )rect.height();

			QPointF& offsetRef = m_modelData->GetOffsetRef();
			offsetRef.setX( x );
			offsetRef.setY( y );

			QSizeF& sizeRef = m_modelData->GetSizeRef();
			sizeRef.setWidth( w );
			sizeRef.setHeight( h );

			m_modelData->Flush();
		}
	}
}


void ReClipWidget::PullData()
{
	if( NULL != m_modelData )
	{
		QWidget* parent = parentWidget();
		if( NULL != parent )
		{
			QPointF offset = m_modelData->GetOffset();
			QSizeF size = m_modelData->GetSize();
			QRect rect = parent->geometry();

			int x = ( int )( 0.5f + ( qreal )rect.width() * offset.x() );
			int y = ( int )( 0.5f + ( qreal )rect.height() * offset.y() );
			move( x, y );

			int w = ( int )( 0.5f + ( qreal )rect.width() * size.width() );
			int h = ( int )( 0.5f + ( qreal )rect.height() * size.height() );
			resize( w, h );

			update();
		}
	}
}


ReDragInfo::eResize ReClipWidget::CalcResizeType( const QPoint& _point ) const
{
	ReDragInfo::eResize result = ReDragInfo::EResize_None;	
	QPoint localPos = _point;
	const int padding = 3;
	const int width = this->width();
	const int height = this->height();

	bool isLeft = ( localPos.x() >= 0 ) && ( localPos.x() < padding );
	bool isRight = ( width > localPos.x() ) && ( ( width - localPos.x() ) <= padding );
	bool isTop = ( localPos.y() >= 0 ) && ( localPos.y() < padding );
	bool isBottom = ( height > localPos.y() ) && ( ( height - localPos.y() ) <= padding );
	bool isV = ( localPos.y() >= 0 && localPos.y() < height );
	bool isH = ( localPos.x() >= 0 && localPos.x() < width );

	if( isLeft && isTop )
	{
		result = ReDragInfo::EResize_LeftTop;
	}
	else if( isLeft && isBottom )
	{
		result = ReDragInfo::EResize_LeftBottom;
	}
	else if( isRight && isTop )
	{
		result = ReDragInfo::EResize_RightTop;
	}
	else if( isRight && isBottom )
	{
		result = ReDragInfo::EResize_RightBottom;
	}
	else
	{
		if( isH )
		{
			if( isLeft )
				result = ReDragInfo::EResize_Left;
			else if( isRight )
				result = ReDragInfo::EResize_Right;
		}

		if( isV )
		{
			if( isTop )
				result = ReDragInfo::EResize_Top;
			else if( isBottom )
				result = ReDragInfo::EResize_Bottom;
		}
	}

	return result;
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

		//painter.drawText( 0, height(), tr( "(%1,%2)(%3,%4)" )
		//	.arg( width() ).arg( height() )
		//	.arg( pos().x() ).arg( pos().y() ) );
	}
}


void ReClipWidget::mousePressEvent( QMouseEvent* _event )
{
	_event->ignore();
}


void ReClipWidget::mouseReleaseEvent( QMouseEvent* _event )
{
	_event->ignore();
}


void ReClipWidget::mouseMoveEvent( QMouseEvent* _event )
{
	_event->ignore();
}


// -------------------------------------------------------------------------
// Utilities.
// -------------------------------------------------------------------------



}
