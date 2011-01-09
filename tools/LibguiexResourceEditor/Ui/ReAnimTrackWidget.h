// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_TRACK_WIDGET_H_
#define _RE_ANIM_TRACK_WIDGET_H_
// -----------------------------------------------------------------------------
// Tracks do not know about the animation data model, so it delegate all frame
// related operations to a higher level, i.e, the track panel widget, by means 
// of signal:
// - signal CreateFrameRequested: 
// - signal DeleteFrameRequested:
// - signal FrameMoved:
// -----------------------------------------------------------------------------
#include "Core\ReAnimDef.h"
#include "Core\ReModelBase.h"
#include "Core\RePool.h"
#include "Ui\ReRulerWidget.h"
#include "Ui\ReAnimFrameWidget.h"
#include <QVariant>


class QMenu;
class QAction;


namespace RE
{


class ReAnimFrameWidget;
class ReAnimTrack;


// -----------------------------------------------------------------------------
// Class ReAnimTrackWidget.
// -----------------------------------------------------------------------------
class ReAnimTrackWidget : public ReRulerWidget, public ReModelBase< ReAnimFrameWidget >
{
	Q_OBJECT
	typedef ReRulerWidget						TSuper;
	typedef ReModelBase< ReAnimFrameWidget >	TSuperB;

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReAnimTrackWidget( ReAnimTrack* _model, eTrackType _type, QWidget* _parent = NULL );

	ReAnimTrack*		GetModelData() const { return m_modelData; }
	void				SetModelData( ReAnimTrack* _track )	{ m_modelData = _track; }

	ReAnimFrameWidget*	GetFrameAtCursor( int _cursor );
	ReAnimFrameWidget*	CreateFrameAtCursor( int _cursor );

	// -------------------------------------------------------------------------
	// Override QWidget.
	// -------------------------------------------------------------------------
public:
	virtual void		paintEvent( QPaintEvent* _event );
	virtual void		mousePressEvent( QMouseEvent* _event );
	virtual void		mouseReleaseEvent( QMouseEvent* _event );
	virtual void		mouseMoveEvent( QMouseEvent* _event );
	virtual void		keyPressEvent( QKeyEvent* _event );
	
	// -------------------------------------------------------------------------
	// Override ReBaseWidget.
	// -------------------------------------------------------------------------
public:
	virtual QMenu*		GetEditMenu() const;

	// -------------------------------------------------------------------------
	// Override ReRulerWidget
	// -------------------------------------------------------------------------
protected:
	virtual void		DrawBackground( QPainter& _painter );
	virtual void		DrawMarks( QPainter& _painter );
	virtual void		DrawForeground( QPainter& _painter );

	// -------------------------------------------------------------------------
	// Override ReModelBase.
	// -------------------------------------------------------------------------
public:
	virtual void		RecycleData( ReAnimFrameWidget* _frame );

	// -------------------------------------------------------------------------
	// Signals.
	// -------------------------------------------------------------------------
signals:
	void				DataChangedAt( int _cursor );

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------
public slots:
	void				OnContextMenu( const QPoint& _point );
	void				OnCreateFrame();
	void				OnDeleteFrame();
	virtual void		OnViewportChanged( int _pos );

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	virtual void		InitMenus();

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	typedef RePool< ReAnimFrameWidget >		TFramePool;
	typedef TFramePool::TItemListItor		TFramePoolItor;
	typedef TFramePool::TItemListCItor		TFramePoolCItor;

	ReAnimTrack*		m_modelData;

	QMenu*				m_editMenu;
	QAction*			m_createFrameAction;
	QAction*			m_deleteFrameAction;
	TFramePool			m_frameList;
	ReAnimFrameWidget*	m_currentFrame;
};


}
#endif	// _RE_ANIM_TRACK_WIDGET_H_
