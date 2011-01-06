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
	void				SetUnit( int _pixels )			{ m_unit = _pixels; }
	qreal				GetUnitValue() const			{ return m_unitValue; }
	void				SetUnitValue( qreal _value )	{ m_unitValue = _value; }
	int					GetSnap() const					{ return m_snap; }
	void				SetSnap( int _snap )			{ m_snap = _snap; }	

	bool				IsHorizontal() const			{ return m_isHorizontal; }
	void				SetHorizontal( bool _h );
	int					GetRulerHeight() const			{ return m_rulerHeight; }
	void				SetRulerHeight( int _height );
	int					GetLongMarkDivision() const		{ return m_longMarkDivision; }
	void				SetLongMarkDevision( int _d )	{ m_longMarkDivision = _d; }
	bool				IsMarkOnSizeA() const			{ return m_isMarkOnSideA; }
	void				SetMarkOnSizeA( bool _isOn )	{ m_isMarkOnSideA = _isOn; }

	void				ShowCursorValue( bool _show )	{ m_isShowCursorValue = _show; }
	bool				IsShowCursorValue() const		{ return m_isShowCursorValue; }
	void				EnableDragViewport( bool _b )	{ m_isDragViewportEnabled = _b; }
	bool				IsDragViewportEnabled() const	{ return m_isDragViewportEnabled; }
	void				EnableDragCursor( bool _b )		{ m_isDragCursorEnabled = _b; }
	bool				IsDragCursorEnabled() const		{ return m_isDragCursorEnabled; }

	qreal				GetValueAt( int _cursor ) const;

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
	virtual void		DrawBackground( QPainter& _painter );
	virtual void		DrawMarks( QPainter& _painter );
	virtual void		DrawForeground( QPainter& _painter );

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
protected:
	// Values that are measured in pixels.
	int					m_unit;				// Number of pixels between two marks.
	int					m_viewport;
	int					m_cursor;
	int					m_snap;				// The # of pixels that the cursor should snap to.
	int					m_rulerHeight;
	int					m_longMarkDivision;	// The # of units between two long marks.

	// Values that are measured in decimal.
	qreal				m_unitValue;

	// Misc.
	ReDragInfo			m_cursorDragInfo;
	ReDragInfo			m_viewportDragInfo;	
	bool				m_isHorizontal;
	bool				m_isMarkOnSideA;	// Size A is, for horizontal ruler the bottom side, for vertical ruler the right side.
	bool				m_isShowCursorValue;
	bool				m_isDragViewportEnabled;
	bool				m_isDragCursorEnabled;

	static int			ms_invalidCursor;
};


}
#endif	// _RE_RULER_WIDGET_H_
