// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101218.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_GRAPHICS_ITEM_H_
#define _RE_ANIM_GRAPHICS_ITEM_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include <QGraphicsItem>
#include "Core\ReAnimDef.h"
#include "Core\ReDragInfo.h"


namespace RE
{


class ReAnimGraphicsItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
	typedef QGraphicsItem	TSuper;

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReAnimGraphicsItem( QGraphicsItem* _parent = NULL );

	void				SetEditMode( eEditMode _mode );
	eEditMode			GetEditMode() const	{ return m_editMode; }
	void				SetAlpha( qreal _alpha );
	qreal				GetAlpha() const { return m_alpha; }

	// -------------------------------------------------------------------------
	// Override QGraphicsItem.
	// -------------------------------------------------------------------------
public:
	virtual bool		sceneEvent( QEvent* _event );
	virtual QRectF		boundingRect() const;
	virtual void		paint( QPainter* _painter, const QStyleOptionGraphicsItem* _option, QWidget* _widget = NULL );
	virtual int			type() const { return EGraphicsItemType_Base; }

	virtual void		mousePressEvent( QGraphicsSceneMouseEvent* _event );
	virtual void		mouseReleaseEvent( QGraphicsSceneMouseEvent* _event );
	virtual void		mouseMoveEvent( QGraphicsSceneMouseEvent* _event );
	virtual void		keyPressEvent( QKeyEvent* _event );

	// -------------------------------------------------------------------------
	// Signals.
	// -------------------------------------------------------------------------
signals:
	void				PositionChanged( const QPointF& _pos );
	void				RotationChanged( qreal _delta );
	void				ScaleChanged( const QPointF& _scale );
	void				AlphaChanged( qreal _delta );

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	eEditMode			m_editMode;
	ReDragInfoF			m_dragInfo;
	QTransform			m_transformBackup;	
	int					m_rotationAccumulator;	
	QPointF				m_scaleAccumulator;
	qreal				m_scaleMultiplier;
	qreal				m_alpha;
	qreal				m_alphaMultiplier;
};


}
#endif	// _RE_ANIM_GRAPHICS_ITEM_H_
