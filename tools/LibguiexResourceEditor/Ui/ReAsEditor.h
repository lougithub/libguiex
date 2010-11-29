// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#ifndef _RE_AS_EDITOR_H_
#define _RE_AS_EDITOR_H_
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


class ReTrackPanelWidget;


class ReAsEditor : public ReBaseWidget< QWidget >
{
	Q_OBJECT
	typedef ReBaseWidget< QWidget >	TSuper;

	// ----------------------------------------------------------------------------
	// General.
	// ----------------------------------------------------------------------------
public:
	ReAsEditor( QWidget* _parent = NULL );
	~ReAsEditor();

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
	QWidget*			m_lastFocusedWidget;
	ReTrackPanelWidget*	m_trackPanelWidget;
	QLabel*				m_asViewWidget;
	
	// Menu.
	QMenu*				m_asViewMenu;

	// Debug.
	QPoint				m_cursor;
	QString				m_debugInfo;
	bool				m_isDebugEnabled;
};


}
#endif	// _RE_CLIP_EDITOR_H_
