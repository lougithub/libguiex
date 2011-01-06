// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_CONSOLE_WIDGET_H_
#define _RE_ANIM_CONSOLE_WIDGET_H_
// -----------------------------------------------------------------------------
// The interpretation of QModelIndex for this model:
// - row:		track type ( translation, rotation, alpha, etc. );
// - column:	cursor in editor.
// -----------------------------------------------------------------------------
#include <list>
#include <QWidget>
#include <QMatrix>
#include "Core\ReAnimDef.h"
#include "Ui\ReBaseWidget.h"


class QMenu;
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


class ReAnimConsoleWidget : public ReBaseWidget< QWidget >
{
	Q_OBJECT
	typedef ReBaseWidget< QWidget >	TSuper;

	// -------------------------------------------------------------------------
	// Internal classes.
	// -------------------------------------------------------------------------
public:
	class ReAnimEntityInfo
	{
	public:
		ReAnimGraphicsItem*		m_graphicsItem;
		ReAnimEntityWidget*		m_entityWidget;		

		ReAnimEntityInfo(): m_graphicsItem( NULL ), m_entityWidget( NULL ) {}
	};

	typedef QList< ReAnimEntityInfo >		TAnimEntityList;
	typedef TAnimEntityList::Iterator		TAnimEntityListItor;
	typedef TAnimEntityList::const_iterator	TAnimEntityListCItor;

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
	virtual void		resizeEvent( QResizeEvent* _event );

	// -------------------------------------------------------------------------
	// Overrides ReBaseWidget.
	// -------------------------------------------------------------------------
public:
	virtual QMenu*		GetEditMenu() const;

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
	void				UpdateLayout();

	void				OnSceneSelectionChanged();
	void				OnRulerCursorChanged( int _cursor );

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	void				InitMenus();
	void				InitRuler();

protected:
	QGraphicsScene*		m_scene;
	ReAnimModel*		m_animMode;
	ReRulerWidget*		m_rulerWidget;

	TAnimEntityList		m_animEntityList;

	// Menu.
	QMenu*				m_editMenu;

	// UI stats.
	int					m_leftMargin;
	int					m_rightMargin;
	int					m_topMargin;
	int					m_bttomMargin;
	int					m_verticalGap;
	int					m_horizontalGap;
	int					m_rulerHeight;
	int					m_trackHeight;
	int					m_headWidth;

	// Debug.
	QString				m_debugInfo;
	bool				m_isDebugEnabled;
};


}
#endif	// _RE_ANIM_CONSOLE_WIDGET_H_
