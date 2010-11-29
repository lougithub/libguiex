#include "WzAfxEditor.h"
#include "UI\WzSpriteWidget.h"
#include <QMouseEvent>
#include <QPainter>


namespace RE
{


// -----------------------------------------------------
// General
// -----------------------------------------------------
WzSpriteWidget::WzSpriteWidget( QWidget* _parent /* = NULL */ )
: TSuper( _parent )
, m_image( NULL )
, m_logicParent( NULL )
, m_localPos( 0, 0 )
, m_resizePadding( 10 )
, m_minSize( 20, 20 )
, m_isDragMoveEnabled( false )
, m_isDragResizeEnabled( false )
{
}


// -----------------------------------------------------
// Override 
// -----------------------------------------------------
void WzSpriteWidget::paintEvent( QPaintEvent* _event )
{
	QPainter painter( this );

	if( NULL != m_image )
	{
		//QRegion regionBackup = painter.clipRegion();
		//QRect rect = *regionBackup.rects().begin();
		//rect.setLeft( rect.left() - 10 );
		//rect.setTop( rect.top() - 10 );
		//rect.setRight( rect.right() + 10 );
		//rect.setBottom( rect.bottom() + 10 );
		//painter.setClipRect( rect );
		painter.drawImage( QRect( 0, 0, geometry().width(), geometry().height() ), *m_image );
		//painter.setClipRegion( regionBackup );
	}
	else
		painter.fillRect( 0, 0, geometry().width(), geometry().height(), QColor( 0, 255, 0, 100 ) );

}


void WzSpriteWidget::mousePressEvent( QMouseEvent* _event )
{
	QPoint point = _event->pos();

	if( Qt::LeftButton == _event->button() )
	{
		WzDragInfo::eResize resizeType = CalcResizeType( point );
		if( WzDragInfo::EResize_None == resizeType )
		{
			if( m_isDragMoveEnabled )
			{
				m_dragInfo.SetCursorPosBackup( point );
				m_dragInfo.SetWidgetPosBackup( pos() );
				m_dragInfo.StartMove();
			}
		}
		else
		{
			if( m_isDragResizeEnabled )
			{
				m_dragInfo.SetCursorPosBackup( point );
				m_dragInfo.SetWidgetPosBackup( pos() );
				m_dragInfo.StartResize( resizeType );
			}
		}
	}
}


void WzSpriteWidget::mouseReleaseEvent( QMouseEvent* _event )
{
	if( Qt::LeftButton == _event->button() )
	{
		m_dragInfo.Stop();
		setCursor( QCursor( Qt::ArrowCursor ) );
	}
}


void WzSpriteWidget::mouseMoveEvent( QMouseEvent* _event )
{
	bool isDirty = false;
	QPoint point = _event->pos();
	QRect geo = geometry();

	if( m_dragInfo.IsMoving() )
	{
		// Use QWidget::move here won't do.
		QPoint delta = point - m_dragInfo.GetCursorPosBackup();		
		geo.translate( delta );
		setGeometry( geo );		

		isDirty = true;
	}
	else if( m_dragInfo.IsResizing() )
	{
		QPoint delta = point - m_dragInfo.GetCursorPosBackup();
		QPoint pointBackup = point;
		if( pointBackup.x() < 0 )
			pointBackup.setX( 0 );
		if( pointBackup.y() < 0 )
			pointBackup.setY( 0 );		

		if( m_dragInfo.IsResizeL() || m_dragInfo.IsResizeLT() || m_dragInfo.IsResizeLB() )
		{
			QRect geoProbe = geo;
			geoProbe.setLeft( geoProbe.left() + delta.x() );

			if( point.x() < geo.width() && geoProbe.width() >= m_minSize.width() )
				geo.setLeft( geo.left() + delta.x() );
		}
		else if( m_dragInfo.IsResizeR() || m_dragInfo.IsResizeRB() || m_dragInfo.IsResizeRT() )
		{
			QRect geoProbe = geo;
			geoProbe.setRight( geoProbe.right() + delta.x() );

			if( point.x() >= 0 && geoProbe.width() >= m_minSize.width() )
			{
				m_dragInfo.SetCursorPosBackup( QPoint( point.x(), m_dragInfo.GetCursorPosBackup().y() ) );
				geo.setRight( geo.right() + delta.x() );
			}
		}

		if( m_dragInfo.IsResizeT() || m_dragInfo.IsResizeLT() || m_dragInfo.IsResizeRT() )
		{
			QRect geoProbe = geo;
			geoProbe.setTop( geoProbe.top() + delta.y() );

			if( point.y() < geo.height() && geoProbe.height() >= m_minSize.height() )
				geo.setTop( geo.top() + delta.y() );
		}
		else if( m_dragInfo.IsResizeB() || m_dragInfo.IsResizeRB() || m_dragInfo.IsResizeLB() )
		{
			QRect geoProbe = geo;
			geoProbe.setBottom( geoProbe.bottom() + delta.y() );

			if( point.y() >= 0 && geoProbe.height() >= m_minSize.height() )
			{
				m_dragInfo.SetCursorPosBackup( QPoint( m_dragInfo.GetCursorPosBackup().x(), point.y() ) );
				geo.setBottom( geo.bottom() + delta.y() );
			}
		}

		setGeometry( geo );
		isDirty = true;
	}
	else
	{
		// Check on cursor only when we are not drag-moving or drag-resizing.
		if( m_isDragResizeEnabled )
		{
			WzDragInfo::eResize resizeDir = CalcResizeType( point );

			if( WzDragInfo::EResize_LeftTop == resizeDir || WzDragInfo::EResize_RightBottom == resizeDir )
				setCursor( QCursor( Qt::SizeFDiagCursor ) );
			else if( WzDragInfo::EResize_RightTop == resizeDir || WzDragInfo::EResize_LeftBottom == resizeDir )
				setCursor( QCursor( Qt::SizeBDiagCursor ) );
			else if( WzDragInfo::EResize_Left == resizeDir || WzDragInfo::EResize_Right == resizeDir )
				setCursor( QCursor( Qt::SizeHorCursor ) );
			else if( WzDragInfo::EResize_Top == resizeDir || WzDragInfo::EResize_Bottom == resizeDir )
				setCursor( QCursor( Qt::SizeVerCursor ) );
			else
				setCursor( QCursor( Qt::ArrowCursor ) );
		}
	}

	if( isDirty )
		update();
}


void WzSpriteWidget::moveEvent( QMoveEvent* _event )
{
	TSuper::moveEvent( _event );
}


// -----------------------------------------------------
// Utilities
// -----------------------------------------------------
WzSpriteWidget::WzDragInfo::eResize WzSpriteWidget::CalcResizeType( const QPoint& _localPos ) const
{
	WzDragInfo::eResize result = WzDragInfo::EResize_None;
	const QRect& geo = geometry();

	bool isLeft = ( _localPos.x() >= 0 ) && ( _localPos.x() < m_resizePadding );
	bool isRight = ( geo.width() > _localPos.x() ) && ( ( geo.width() - _localPos.x() ) <= m_resizePadding );
	bool isTop = ( _localPos.y() >= 0 ) && ( _localPos.y() < m_resizePadding );
	bool isBotom = ( geo.height() > _localPos.y() ) && ( ( geo.height() - _localPos.y() ) <= m_resizePadding );
	bool isV = ( _localPos.y() >= 0 && _localPos.y() < geo.height() );
	bool isH = ( _localPos.x() >= 0 && _localPos.x() < geo.width() );

	if( isLeft && isTop )
	{
		result = WzDragInfo::EResize_LeftTop;
	}
	else if( isLeft && isBotom )
	{
		result = WzDragInfo::EResize_LeftBottom;
	}
	else if( isRight && isTop )
	{
		result = WzDragInfo::EResize_RightTop;
	}
	else if( isRight && isBotom )
	{
		result = WzDragInfo::EResize_RightBottom;
	}
	else
	{
		if( isH )
		{
			if( isLeft )
				result = WzDragInfo::EResize_Left;
			else if( isRight )
				result = WzDragInfo::EResize_Right;
		}

		if( isV )
		{
			if( isTop )
				result = WzDragInfo::EResize_Top;
			else if( isBotom )
				result = WzDragInfo::EResize_Bottom;
		}
	}

	return result;
}


}	// namespace RE
