// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110109.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_ENTITYPANEL_H_
#define _RE_ANIM_ENTITYPANEL_H_


#include "Ui\ReBaseWidget.h"
#include <QWidget>


class QGraphicsScene;


namespace RE
{


class ReAnimModel;
class ReAnimGraphicsItem;
class ReRulerWidget;
class ReAnimEntityWidget;


class ReAnimEntityPanel : public ReBaseWidget< QWidget >
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

	// ----------------------------------------------------------------------------
	// General.
	// ----------------------------------------------------------------------------
public:
	ReAnimEntityPanel( ReAnimModel* _model, ReRulerWidget* _ruler, QGraphicsScene* _scene, QWidget* _parent = NULL );

	// ----------------------------------------------------------------------------
	// Override QWidget.
	// ----------------------------------------------------------------------------
protected:
	virtual void		paintEvent( QPaintEvent* _event );
	virtual void		resizeEvent( QResizeEvent* _event );
	virtual void		mousePressEvent( QMouseEvent* _event );
	virtual void		mouseReleaseEvent( QMouseEvent* _event );
	virtual void		mouseMoveEvent( QMouseEvent* _event );

	// ----------------------------------------------------------------------------
	// Slots.
	// ----------------------------------------------------------------------------
public slots:
	void				UpdateLayout();
	void				OnItemAdded( ReAnimGraphicsItem* _item );
	void				OnItemDeleted( ReAnimGraphicsItem* _item );
	void				OnSceneSelectionChanged();
	void				OnRulerCursorChanged( int _cursor );

	// ----------------------------------------------------------------------------
	// Utilities.
	// ----------------------------------------------------------------------------

	// ----------------------------------------------------------------------------
	// Variables.
	// ----------------------------------------------------------------------------
protected:
	ReAnimModel*		m_model;
	TAnimEntityList		m_entityList;
	ReRulerWidget*		m_ruler;
	QGraphicsScene*		m_scene;
	ReDragInfo			m_dragInfo;
	QPoint				m_origin;
};


}
#endif	// _RE_ANIM_ENTITYPANEL_H_
