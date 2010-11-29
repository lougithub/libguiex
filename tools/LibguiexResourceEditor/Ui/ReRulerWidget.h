// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101126.
// -----------------------------------------------------------------------------
#ifndef _RE_RULER_WIDGET_H_
#define _RE_RULER_WIDGET_H_


#include <QLabel>
#include <QWidget>
#include "Core\ReDragInfo.h"
#include "Ui\ReBaseWidget.h"


class QPainter;


namespace RE
{


class ReRulerWidget : public ReBaseWidget< QLabel >
{
	Q_OBJECT
	typedef ReBaseWidget< QLabel >	TSuper;

	// ----------------------------------------------------------------------------
	// General.
	// ----------------------------------------------------------------------------
public:
	ReRulerWidget( QWidget* _parent = NULL );

	// ----------------------------------------------------------------------------
	// Override QWidget.
	// ----------------------------------------------------------------------------
public:
	virtual void		paintEvent( QPaintEvent* _event );
	virtual void		mousePressEvent( QMouseEvent* _event );
	virtual void		mouseReleaseEvent( QMouseEvent* _event );
	virtual void		mouseMoveEvent( QMouseEvent* _event );

	// ----------------------------------------------------------------------------
	// Signals.
	// ----------------------------------------------------------------------------
signals:
	void				CursorChanged( int _pos );
	void				ViewportChanged( int _pos );

	// ----------------------------------------------------------------------------
	// Slots.
	// ----------------------------------------------------------------------------
public slots:
	virtual void		OnCursorChanged( int _pos );
	virtual void		OnViewportChanged( int _pos );

	// ----------------------------------------------------------------------------
	// Utilities.
	// ----------------------------------------------------------------------------
protected:
	qreal				GetValueAt( int _cursor ) const;
	virtual void		DrawBackground( QPainter& _painter );
	virtual void		DrawContent( QPainter& _painter );
	virtual void		DrawForeground( QPainter& _painter );

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
protected:
	// Values that are measured in pixels.
	int					m_shortMarkLength;
	int					m_longMarkLength;
	int					m_interval;	
	int					m_minInterval;
	int					m_maxInterval;
	int					m_viewportPos;
	int					m_cursor;	

	// Values that are measured in decimal.
	qreal				m_intervalValue;

	// Misc.
	ReDragInfo			m_cursorDragInfo;
	ReDragInfo			m_viewportDragInfo;
	int					m_divisionBetweenLongMarks;
};


}
#endif	// _RE_RULER_WIDGET_H_
