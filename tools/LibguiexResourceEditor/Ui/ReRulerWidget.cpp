// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101126.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReRulerWidget.h"
#include "Ui\ReAnimUiInfo.h"
#include <QPainter>
#include <QMouseEvent>
#include <QFontMetrics>


namespace
{
	static int gsShortMarkLength = 3;
	static int gsLongMarkLength = 8;
}


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
int ReRulerWidget::ms_invalidCursor = -1;


ReRulerWidget::ReRulerWidget( QWidget* _parent /* = NULL */ )
: TSuper( _parent )
, m_unit( 1 )
, m_viewport( 0 )
, m_cursor( 0 )
, m_snap( 1 )
, m_rulerHeight( 30 )
, m_longMarkDivision( 5 )
, m_unitValue( 1.0f )
, m_isHorizontal( true )
, m_isMarkOnSideA( true )
, m_isShowCursorValue( true )
, m_isDragViewportEnabled( true )
, m_isDragCursorEnabled( true )
{
	SetRulerHeight( m_rulerHeight );
}


void ReRulerWidget::SetHorizontal( bool _h )
{
	m_isHorizontal = _h;

	if( IsHorizontal() )
	{
		setMaximumHeight( m_rulerHeight );
		setMaximumWidth( 999999 );
	}
	else
	{
		setMaximumWidth( m_rulerHeight );
		setMaximumHeight( 999999 );
	}
}


void ReRulerWidget::SetRulerHeight( int _height )
{
	if( _height > 0 )
	{
		m_rulerHeight = _height;

		IsHorizontal() ? setMaximumHeight( m_rulerHeight ) : setMaximumWidth( m_rulerHeight );
	}	
}


// -----------------------------------------------------------------------------
// Override QWidget.
// -----------------------------------------------------------------------------
void ReRulerWidget::paintEvent( QPaintEvent* _event )
{
	QPainter painter( this );

	DrawBackground( painter );
	DrawMarks( painter );
	DrawForeground( painter );
}


void ReRulerWidget::mousePressEvent( QMouseEvent* _event )
{
	QPoint pos = QPoint( _event->pos().x() / m_snap * m_snap, _event->pos().y() / m_snap * m_snap );

	if( ( Qt::MidButton == _event->button() ) ||
		( Qt::LeftButton == _event->button() && Qt::ControlModifier & _event->modifiers() ) )
	{
		if( IsDragViewportEnabled() )
		{
			// Use middle mouse button or left mouse button plus the control key to
			// move the viewport.		
			m_viewportDragInfo.SetCursorPosBackup( pos );
			m_viewportDragInfo.SetItemPosBackup( IsHorizontal() ? QPoint( m_viewport, 0 ) : QPoint( 0, m_viewport ) );
			m_viewportDragInfo.StartMove();
		}
	}
	else if( Qt::LeftButton == _event->button() )
	{
		if( IsDragCursorEnabled() )
		{
			// Use left button to move the cursor.
			m_cursorDragInfo.SetCursorPosBackup( pos );
			m_cursorDragInfo.StartMove();
		}
	}
}


void ReRulerWidget::mouseReleaseEvent( QMouseEvent* _event )
{
	m_cursorDragInfo.Stop();
	m_viewportDragInfo.Stop();
}


void ReRulerWidget::mouseMoveEvent( QMouseEvent* _event )
{
	if( m_cursorDragInfo.IsMoving() )
	{
		int cursorBackup = m_cursor;
		int cursor = 0;
		int delta = 0;
		int newCursor = 0;

		if( IsHorizontal() )
		{
			cursor = _event->pos().x();
			if( cursor < 0 )
				cursor = 0;
			else if( cursor >= width() )
				cursor = width() - 1;

			delta = cursor - m_cursorDragInfo.GetCursorPosBackup().x();
			newCursor = m_cursorDragInfo.GetCursorPosBackup().x() + delta / m_snap * m_snap;
		}
		else
		{
			cursor = _event->pos().y();
			if( cursor < 0 )
				cursor = 0;
			else if( cursor >= height() )
				cursor = height() - 1;

			delta = cursor - m_cursorDragInfo.GetCursorPosBackup().y();
			newCursor = m_cursorDragInfo.GetCursorPosBackup().y() + delta / m_snap * m_snap;
		}
		
		if( newCursor != m_cursor )
		{
			OnCursorChanged( newCursor );
			emit CursorChanged( newCursor );
		}
	}
	else if( m_viewportDragInfo.IsMoving() )
	{
		int viewPortBackup = m_viewport;

		QPoint delta = _event->pos() - m_viewportDragInfo.GetCursorPosBackup();
		int newViewportPos = IsHorizontal()
			? m_viewportDragInfo.GetItemPosBackup().x() - delta.x()
			: m_viewportDragInfo.GetItemPosBackup().y() - delta.y();

		newViewportPos = newViewportPos / m_snap * m_snap;
		if( newViewportPos != viewPortBackup )
		{
			OnViewportChanged( newViewportPos );
			emit ViewportChanged( newViewportPos );
		}
	}
}


// -----------------------------------------------------------------------------
// Slots.
// -----------------------------------------------------------------------------
void ReRulerWidget::OnCursorChanged( int _pos )
{	
	m_cursor = _pos;
}


void ReRulerWidget::OnViewportChanged( int _pos )
{
	m_cursor += ( m_viewport - _pos );
	m_viewport = _pos;
}


// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------
qreal ReRulerWidget::GetValueAt( int _cursor ) const
{
	int pixelCount = m_viewport + _cursor;
	return m_unitValue * ( ( qreal )pixelCount / ( qreal )m_unit );
}


void ReRulerWidget::SetValue( qreal _value, bool _scrollViewport )
{
	int newCursor = qRound( _value / m_unitValue * m_unit ) - m_viewport;

	OnCursorChanged( newCursor );
	emit CursorChanged( newCursor );

	if( m_cursor < 0 && _scrollViewport )
	{
		OnViewportChanged( newCursor );
		emit ViewportChanged( newCursor );
	}
}


void ReRulerWidget::DrawBackground( QPainter& _painter )
{
	_painter.fillRect( 0, 0, width(), height(), QColor( 0, 0, 0 ) );
}


void ReRulerWidget::DrawMarks( QPainter& _painter )
{
	// For horizontal ruler:
	// Draw the mark just left to or on the viewport position, even if
	// it's hidden. This hidden mark would be clipped by Qt, but would 
	// be visually correct if the mark is thicker than one pixel.
	// For vertical ruler:
	// The above applies.
	QVector< QPoint > markerList;
	int markIndex = m_viewport / m_unit;
	int pos = -m_viewport % m_unit;

	int limit = IsHorizontal() ? width() : height();
	while( pos < limit )
	{
		bool isShortMark = ( 0 == ( markIndex % m_longMarkDivision ) ? false : true );

		if( IsMarkOnSizeA() )
		{
			markerList.push_back( IsHorizontal() 
				? QPoint( pos, m_rulerHeight )
				: QPoint( m_rulerHeight, pos ) );
			markerList.push_back( IsHorizontal()
				? QPoint( pos, m_rulerHeight - ( isShortMark ? gsShortMarkLength : gsLongMarkLength ) )
				: QPoint( m_rulerHeight - ( isShortMark ? gsShortMarkLength : gsLongMarkLength ), pos ) );
		}
		else
		{
			markerList.push_back( IsHorizontal() ? QPoint( pos, 0 ) : QPoint( 0, pos ) );
			markerList.push_back( IsHorizontal()
				? QPoint( pos, isShortMark ? gsShortMarkLength : gsLongMarkLength )
				: QPoint( isShortMark ? gsShortMarkLength : gsLongMarkLength, pos ) );
		}

		// Advance to the next mark.
		pos += m_unit;
		++markIndex;
	}

	_painter.setPen( QColor( 250, 250, 250 ) );
	_painter.drawLines( markerList );
}


void ReRulerWidget::DrawForeground( QPainter& _painter )
{
	_painter.setPen( QColor( 0, 255, 255 ) );
	IsHorizontal()
		? _painter.drawLine( QPoint( m_cursor, 0 ), QPoint( m_cursor, height() ) )
		: _painter.drawLine( QPoint( 0, m_cursor ), QPoint( width(), m_cursor ) );
	QString valueText = QString().setNum( GetValueAt( m_cursor ) );

	if( IsShowCursorValue() )
	{
		QFontMetrics fm( _painter.font() );
		_painter.save();
		const int cursorMargin = 2;
		const int edgeMargin = 2;

		if( IsHorizontal() )
		{
			_painter.translate( m_cursor, IsMarkOnSizeA() ? fm.height() : height() - edgeMargin );
			if( m_cursor <= ( width() / 2 ) )
				_painter.drawText( cursorMargin, 0, valueText );
			else
				_painter.drawText( -fm.width( valueText ) - cursorMargin, 0, valueText );
		}
		else
		{
			_painter.translate( IsMarkOnSizeA() ? fm.height() : width() - edgeMargin, m_cursor );
			_painter.rotate( -90 );
			if( m_cursor <= ( height() / 2 ) )
				_painter.drawText( -fm.width( valueText ) - cursorMargin, 0, valueText );
			else
				_painter.drawText( cursorMargin, 0, valueText );
		}

		_painter.restore();
	}
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


}
