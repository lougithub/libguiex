// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101218.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_GRAPHICS_SCENE_H_
#define _RE_ANIM_GRAPHICS_SCENE_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include <QGraphicsScene>
#include "Core\ReAnimModel.h"


class QMenu;


namespace RE
{


class ReAnimGraphicsItem;


class ReAnimGraphicsScene : public QGraphicsScene
{
	Q_OBJECT
	typedef QGraphicsScene	TSuper;

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReAnimGraphicsScene( ReAnimModel* _model, QObject* _parent = NULL );

	// -------------------------------------------------------------------------
	// Override QGraphicsScene.
	// -------------------------------------------------------------------------
protected:
	void			contextMenuEvent( QGraphicsSceneContextMenuEvent* _event );

	// -------------------------------------------------------------------------
	// Signals.
	// -------------------------------------------------------------------------
signals:
	void			ItemAdded( ReAnimGraphicsItem* _item );

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------
public slots:
	void			OnAddItem();

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	ReAnimModel*	m_model;
	QMenu*			m_editMenu;
	QPointF			m_scenePosBackup;
};


}
#endif	// _RE_ANIM_GRAPHICS_SCENE_H_
