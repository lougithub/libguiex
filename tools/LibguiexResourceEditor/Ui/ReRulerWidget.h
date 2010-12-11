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

	int					GetCursor() const				{ return m_cursor; }
	int					GetViewport() const				{ return m_viewport; }
	int					GetUnit() const					{ return m_unit; }
	qreal				GetUnitValue() const			{ return m_unitValue; }	
	void				ShowCursorValue( bool _show )	{ m_isShowCursorValue = _show; }

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
	int					m_unit;				// Number of pixels between two marks.
	int					m_minUnit;
	int					m_maxUnit;
	int					m_viewport;
	int					m_cursor;

	// Values that are measured in decimal.
	qreal				m_unitValue;

	// Misc.
	ReDragInfo			m_cursorDragInfo;
	ReDragInfo			m_viewportDragInfo;
	bool				m_isShowCursorValue;

	static int			ms_invalidCursor;
};


}
#endif	// _RE_RULER_WIDGET_H_
