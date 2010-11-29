// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101126.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReRulerWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QFontMetrics>


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReRulerWidget::ReRulerWidget( QWidget* _parent /* = NULL */ )
: TSuper( _parent )
, m_shortMarkLength( 3 )
, m_longMarkLength( 8 )
, m_interval( 8 )
, m_minInterval( 4 )
, m_maxInterval( 12 )
, m_viewportPos( 0 )
, m_cursor( 0 )
, m_intervalValue( 1.0f )
, m_divisionBetweenLongMarks( 5 )
{
	setMaximumHeight( 50 );
}


// -----------------------------------------------------------------------------
// Override QWidget.
// -----------------------------------------------------------------------------
void ReRulerWidget::paintEvent( QPaintEvent* _event )
{
	QPainter painter( this );

	DrawBackground( painter );
	DrawContent( painter );
	DrawForeground( painter );
}


void ReRulerWidget::mousePressEvent( QMouseEvent* _event )
{
	if( ( Qt::MidButton == _event->button() ) ||
		( Qt::LeftButton == _event->button() && Qt::ControlModifier & _event->modifiers() ) )
	{
		m_viewportDragInfo.SetCursorPosBackup( _event->pos() );
		m_viewportDragInfo.SetItemPosBackup( QPoint( m_viewportPos, 0 ) );
		m_viewportDragInfo.StartMove();
	}
	else if( Qt::LeftButton == _event->button() )
	{
		OnCursorChanged( _event->pos().x() );
		emit CursorChanged( _event->pos().x() );

		m_cursorDragInfo.SetCursorPosBackup( _event->pos() );
		m_cursorDragInfo.StartMove();
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
		int cursor = _event->pos().x();
		if( cursor < 0 )
			cursor = 0;
		else if( cursor >= width() )
			cursor = width() - 1;

		OnCursorChanged( cursor );
		emit CursorChanged( cursor );
	}
	else if( m_viewportDragInfo.IsMoving() )
	{
		QPoint delta = _event->pos() - m_viewportDragInfo.GetCursorPosBackup();
		int newViewportPos = m_viewportDragInfo.GetItemPosBackup().x() - delta.x();

		OnViewportChanged( newViewportPos );
		emit ViewportChanged( newViewportPos );
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
	m_cursor += ( m_viewportPos - _pos );
	m_viewportPos = _pos;
}


// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------
qreal ReRulerWidget::GetValueAt( int _cursor ) const
{
	int pos = m_viewportPos + _cursor;
	return m_intervalValue * ( ( qreal )pos / ( qreal )m_interval );
}


void ReRulerWidget::DrawBackground( QPainter& _painter )
{
	_painter.fillRect( 0, 0, width(), height(), QColor( 0, 0, 0 ) );
}


void ReRulerWidget::DrawContent( QPainter& _painter )
{
	// Draw the mark just left to or on the viewport position, even if
	// it's hidden. This hidden mark would be clipped by Qt, but would 
	// be visually correct if the mark is thicker than one pixel.
	QVector< QPoint > markerList;
	int markIndex = m_viewportPos / m_interval;
	int pos = -m_viewportPos % m_interval;

	while( pos < width() )
	{
		bool isShortMark = ( 0 == ( markIndex % m_divisionBetweenLongMarks ) ? false : true );
		markerList.push_back( QPoint( pos, 0 ) );
		markerList.push_back( QPoint( pos, isShortMark ? m_shortMarkLength : m_longMarkLength ) );

		// Advance to the next mark.
		pos += m_interval;
		++markIndex;
	}

	_painter.setPen( QColor( 250, 250, 250 ) );
	_painter.drawLines( markerList );
}


void ReRulerWidget::DrawForeground( QPainter& _painter )
{
	_painter.setPen( QColor( 0, 255, 255 ) );
	_painter.drawLine( QPoint( m_cursor, 0 ), QPoint( m_cursor, height() ) );
	QString valueText = QString().setNum( GetValueAt( m_cursor ) );
	if( m_cursor <= ( width() / 2 ) )
	{
		_painter.drawText( m_cursor + 2, height() - 2, valueText );
	}
	else
	{
		QFontMetrics fm( _painter.font() );
		_painter.drawText( m_cursor - 2 - fm.width( valueText ), height() - 2, valueText );
	}
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


}
