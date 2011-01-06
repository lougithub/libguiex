// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_EDITOR_H_
#define _RE_ANIM_EDITOR_H_
// -----------------------------------------------------------------------------
// Action Sequence Editor.
// It consists of a track panel widget and a viewer widget for editing and viewing
// the animations.
// -----------------------------------------------------------------------------


#include <QWidget>
#include "Ui\ReBaseWidget.h"


class QLabel;
class QMenu;


namespace RE
{


class ReAnimConsoleWidget;
class ReAnimGraphicsWidget;
class ReAnimModel;
class ReAnimGraphicsScene;


class ReAnimEditor : public ReBaseWidget< QWidget >
{
	Q_OBJECT
	typedef ReBaseWidget< QWidget >	TSuper;

	// ----------------------------------------------------------------------------
	// General.
	// ----------------------------------------------------------------------------
public:
	ReAnimEditor( ReAnimModel* _model, QWidget* _parent = NULL );
	~ReAnimEditor();

	// ----------------------------------------------------------------------------
	// Overrides QWidget.
	// ----------------------------------------------------------------------------
protected:
	virtual void		paintEvent( QPaintEvent* _event );	
	virtual void		mousePressEvent( QMouseEvent* _event );
	virtual void		mouseReleaseEvent( QMouseEvent* _event );
	virtual void		mouseMoveEvent( QMouseEvent* _event );
	virtual void		wheelEvent( QWheelEvent* _event );
	virtual void		keyPressEvent( QKeyEvent* _event );
	virtual void		keyReleaseEvent( QKeyEvent* _event );

	// ----------------------------------------------------------------------------
	// Override ReBaseWidget.
	// ----------------------------------------------------------------------------
public:
	virtual void		Tick( qreal _delta );
	virtual QMenu*		GetEditMenu() const;

	// ----------------------------------------------------------------------------
	// Signals.
	// ----------------------------------------------------------------------------

	// ----------------------------------------------------------------------------
	// Slots.
	// ----------------------------------------------------------------------------
public slots:
	void				OnToggleDebug();

	// ----------------------------------------------------------------------------
	// Utilities.
	// ----------------------------------------------------------------------------

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
protected:
	// Widgets.	
	ReAnimConsoleWidget*	m_trackConsole;
	ReAnimGraphicsWidget*	m_animGraphics;
	QWidget*				m_lastFocusedWidget;
	ReAnimGraphicsScene*	m_scene;
	
	// Menu.
	QMenu*				m_animViewMenu;

	// Debug.
	QPoint				m_cursor;
	QString				m_debugInfo;
	bool				m_isDebugEnabled;
};


}
#endif	// _RE_ANIM_EDITOR_H_
