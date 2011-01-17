// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_CONSOLE_WIDGET_H_
#define _RE_ANIM_CONSOLE_WIDGET_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include <list>
#include <QWidget>
#include <QMatrix>
#include "Core\ReAnimDef.h"
#include "Ui\ReBaseWidget.h"


class QMenu;
class QAction;
class QLabel;
class QGraphicsScene;


namespace RE
{


class ReAnimModel;
class ReAnimTrackWidget;
class ReRulerWidget;
class ReAnimEntityWidget;
class ReAnimFrameWidget;
class ReAnimGraphicsItem;
class ReAnimPlayerPanel;
class ReAnimEntityPanel;


class ReAnimConsoleWidget : public ReBaseWidget< QWidget >
{
	Q_OBJECT
	typedef ReBaseWidget< QWidget >	TSuper;

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReAnimConsoleWidget( ReAnimModel* _model, QGraphicsScene* _scene, QWidget* _parent = NULL );
	~ReAnimConsoleWidget();

	// -------------------------------------------------------------------------
	// Overrides QWidget.
	// -------------------------------------------------------------------------
protected:
	virtual void		paintEvent( QPaintEvent* _event );	
	virtual void		keyReleaseEvent( QKeyEvent* _event );
	virtual void		resizeEvent( QResizeEvent* _event );

	// -------------------------------------------------------------------------
	// Overrides ReBaseWidget.
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// Signals.
	// -------------------------------------------------------------------------
signals:
	void				Focus( ReAnimGraphicsItem* _item );

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------
public slots:
	void				OnItemAdded( ReAnimGraphicsItem* _item );
	void				OnItemDeleted( ReAnimGraphicsItem* _item );
	void				OnContextMenu( const QPoint& _point );
	void				OnExport();

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:	
	ReAnimPlayerPanel*	m_playerPanel;
	ReAnimEntityPanel*	m_entityPanel;
	QMenu*				m_editMenu;
	QAction*			m_exportAction;

	// Debug.
	QString				m_debugInfo;
	bool				m_isDebugEnabled;
};


}
#endif	// _RE_ANIM_CONSOLE_WIDGET_H_
