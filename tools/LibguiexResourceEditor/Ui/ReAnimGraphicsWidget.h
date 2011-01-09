// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101211.
// -----------------------------------------------------------------------------
#ifndef RE_ANIM_GRAPHICS_WIDGET_H_
#define RE_ANIM_GRAPHICS_WIDGET_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include <QWidget>
#include <QMatrix>
#include "Ui\ReBaseWidget.h"
#include "Core\ReDragInfo.h"


class QGraphicsScene;


namespace RE
{


class ReRulerWidget;
class ReAnimGraphicsView;
class ReAnimGraphicsItem;
class ReAnimModel;


class ReAnimGraphicsWidget : public ReBaseWidget< QWidget >
{
	// -------------------------------------------------------------------------
	// General
	// -------------------------------------------------------------------------
	Q_OBJECT
	typedef ReBaseWidget< QWidget >	TSuper;

public:
	ReAnimGraphicsWidget( ReAnimModel* _model, QGraphicsScene* _scene, QWidget* _parent = NULL );

	const QMatrix&		GetMatrix() const	{ return m_matrix; }
	QMatrix&			GetMatrixRef()		{ return m_matrix; }

	// -------------------------------------------------------------------------
	// Override QWidget.
	// -------------------------------------------------------------------------
public:
	virtual void		mousePressEvent( QMouseEvent* _event );
	virtual void		mouseReleaseEvent( QMouseEvent* _event );
	virtual void		mouseMoveEvent( QMouseEvent* _event );
	virtual void		resizeEvent( QResizeEvent* _event );

	// -------------------------------------------------------------------------
	// Signals.
	// -------------------------------------------------------------------------
signals:
	void				ItemAdded( ReAnimGraphicsItem* _item );
	void				ItemDeleted( ReAnimGraphicsItem* _item );

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------
public slots:

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	//void				InitRulers();

	// -------------------------------------------------------------------------
	// Variable
	// -------------------------------------------------------------------------
protected:
	QGraphicsScene*		m_scene;
	ReAnimGraphicsView*	m_view;

	int					m_width;
	int					m_height;
	QMatrix				m_matrix;
	ReDragInfo			m_dragInfo;
	QPoint				m_origin;

	ReRulerWidget*		m_horizontalRuler;
	ReRulerWidget*		m_verticalRuler;
};


}
#endif	// RE_ANIM_GRAPHICS_WIDGET_H_
