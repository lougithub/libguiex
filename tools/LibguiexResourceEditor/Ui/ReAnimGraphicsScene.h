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
class QAction;


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
	virtual void	contextMenuEvent( QGraphicsSceneContextMenuEvent* _event );
	virtual void	dragEnterEvent( QGraphicsSceneDragDropEvent* _event );
	virtual void	dragMoveEvent( QGraphicsSceneDragDropEvent* _event );
	virtual void	dropEvent( QGraphicsSceneDragDropEvent* _event );

	// -------------------------------------------------------------------------
	// Signals.
	// -------------------------------------------------------------------------
signals:
	void			ItemAdded( ReAnimGraphicsItem* _item );
	void			ItemDeleted( ReAnimGraphicsItem* _item );

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------
public slots:
	void			OnAddItem();
	void			OnDeleteItem();

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	ReAnimModel*	m_model;
	QMenu*			m_editMenu;
	QAction*		m_createItemAction;
	QAction*		m_deleteItemAction;
	QPointF			m_scenePosBackup;
};


}
#endif	// _RE_ANIM_GRAPHICS_SCENE_H_
