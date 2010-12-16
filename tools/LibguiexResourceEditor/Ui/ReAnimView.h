// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101211.
// -----------------------------------------------------------------------------
#ifndef RE_ANIM_VIEW_H_
#define RE_ANIM_VIEW_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include <QWidget>
#include <QMatrix>
#include "Ui\ReBaseWidget.h"
#include "Core\ReDragInfo.h"


namespace RE
{


class ReRulerWidget;


class ReAnimView : public ReBaseWidget< QWidget >
{
	// -------------------------------------------------------------------------
	// General
	// -------------------------------------------------------------------------
	Q_OBJECT
	typedef ReBaseWidget< QWidget >	TSuper;

public:
	ReAnimView( QWidget* _parent = NULL );

	const QMatrix&		GetMatrix() const	{ return m_matrix; }
	QMatrix&			GetMatrixRef()		{ return m_matrix; }

	// -------------------------------------------------------------------------
	// Override QWidget.
	// -------------------------------------------------------------------------
public:
	virtual void		paintEvent( QPaintEvent* _event );
	virtual void		mousePressEvent( QMouseEvent* _event );
	virtual void		mouseReleaseEvent( QMouseEvent* _event );
	virtual void		mouseMoveEvent( QMouseEvent* _event );
	virtual void		resizeEvent( QResizeEvent* _event );

	// -------------------------------------------------------------------------
	// Override ReBaseWidget.
	// -------------------------------------------------------------------------
public:
	virtual void		Tick( qreal _delta );

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	void				InitRulers();

	// -------------------------------------------------------------------------
	// Variable
	// -------------------------------------------------------------------------
protected:
	int					m_width;
	int					m_height;
	QMatrix				m_matrix;
	ReDragInfo			m_dragInfo;
	QPoint				m_origin;

	ReRulerWidget*		m_horizontalRuler;
	ReRulerWidget*		m_verticalRuler;
};


}
#endif	// RE_ANIM_VIEW_H_
