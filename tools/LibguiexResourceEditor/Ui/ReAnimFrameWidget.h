// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101127.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_FRAME_WIDGET_H_
#define _RE_ANIM_FRAME_WIDGET_H_


#include "Ui\ReBaseWidget.h"
#include <QWidget>
#include <QVariant>


namespace RE
{


class ReAnimFrame;
class ReAnimModel;


// -----------------------------------------------------------------------------
// Class ReAnimFrameWidget.
// -----------------------------------------------------------------------------
class ReAnimFrameWidget : public ReBaseWidget< QWidget >
{
	Q_OBJECT
	typedef ReBaseWidget< QWidget >	TSuper;

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReAnimFrameWidget( QWidget* _parent = NULL );

	ReAnimFrame*			GetModelData() const { return m_modelData; }
	void					SetModelData( ReAnimFrame* _frame ) { m_modelData = _frame; }

	// -------------------------------------------------------------------------
	// Override QWidget.
	// -------------------------------------------------------------------------
protected:
	virtual bool			event( QEvent* _event );
	virtual void			paintEvent( QPaintEvent* _event );
	virtual void			mousePressEvent( QMouseEvent* _event );
	virtual void			mouseReleaseEvent( QMouseEvent* _event );

	// -------------------------------------------------------------------------
	// Signals.
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	ReAnimFrame*			m_modelData;
};


}
#endif	// _RE_ANIM_FRAME_WIDGET_H_
