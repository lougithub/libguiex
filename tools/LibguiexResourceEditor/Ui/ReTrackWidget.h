// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#ifndef _RE_TRACK_WIDGET_H_
#define _RE_TRACK_WIDGET_H_


#include "Ui\ReRulerWidget.h"
#include "Ui\ReTrackFrameWidget.h"
#include "Core\ReModelBase.h"
#include "Core\RePool.h"


class QMenu;


namespace RE
{


class ReTrackFrameWidget;


class ReTrackWidget : public ReRulerWidget, public ReModelBase< ReTrackFrameWidget >
{
	Q_OBJECT
	typedef ReRulerWidget						TSuper;
	typedef ReModelBase< ReTrackFrameWidget >	TSuperB;

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReTrackWidget( QWidget* _parent = NULL );

	void				SetFrameSize( int _w, int _h )	{ m_frameSize = QSize( _w, _h ); }

	// -------------------------------------------------------------------------
	// Override QWidget.
	// -------------------------------------------------------------------------
public:
	virtual void		paintEvent( QPaintEvent* _event );
	virtual void		mousePressEvent( QMouseEvent* _event );
	virtual void		mouseReleaseEvent( QMouseEvent* _event );
	virtual void		mouseMoveEvent( QMouseEvent* _event );
	
	// -------------------------------------------------------------------------
	// Override ReBaseWidget.
	// -------------------------------------------------------------------------
public:	
	virtual QMenu*		GetEditMenu() const;

protected:
	virtual void		DrawBackground( QPainter& _painter );
	virtual void		DrawContent( QPainter& _painter );
	virtual void		DrawForeground( QPainter& _painter );

	// -------------------------------------------------------------------------
	// Override ReModelBase.
	// -------------------------------------------------------------------------
public:
	virtual void		RecycleData( ReTrackFrameWidget* _frame );

	// -------------------------------------------------------------------------
	// Override ReRulerWidget.
	// -------------------------------------------------------------------------
public slots:
	virtual void		OnViewportChanged( int _pos );

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------
public slots:
	void				OnContextMenu( const QPoint& _point );
	virtual void		OnNewFrame();

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	virtual void		InitMenus();
	ReTrackFrameWidget*	CreateFrame();

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	typedef RePool< ReTrackFrameWidget >	TFramePool;
	typedef TFramePool::TItemListItor		TFramePoolItor;
	typedef TFramePool::TItemListCItor		TFramePoolCItor;

	QMenu*				m_editMenu;
	TFramePool			m_frameList;
	ReTrackFrameWidget*	m_currentFrame;

	QSize				m_frameSize;
};

}
#endif	// _RE_TRACK_WIDGET_H_
