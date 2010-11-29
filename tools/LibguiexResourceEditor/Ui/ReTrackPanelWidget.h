// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#ifndef _RE_TRACK_PANEL_WIDGET_H_
#define _RE_TRACK_PANEL_WIDGET_H_


#include <list>
#include <QWidget>
#include "Ui\ReBaseWidget.h"


class QMenu;


namespace RE
{


class ReTrackWidget;
class ReRulerWidget;
class ReTrackHeadWidget;


class ReTrackPanelWidget : public ReBaseWidget< QWidget >
{
	Q_OBJECT
	typedef ReBaseWidget< QWidget >	TSuper;

	// -------------------------------------------------------------------------
	// Internal classes.
	// -------------------------------------------------------------------------
public:
	class ReTrackSuite
	{
	public:
		ReTrackSuite( ReTrackHeadWidget* _head, ReTrackWidget* _track ): m_headWidget( _head ), m_trackWidget( _track ) {}

		ReTrackHeadWidget*	m_headWidget;
		ReTrackWidget*		m_trackWidget;
	};

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReTrackPanelWidget( QWidget* _parent = NULL );
	~ReTrackPanelWidget();

	// -------------------------------------------------------------------------
	// Overrides QWidget.
	// -------------------------------------------------------------------------
protected:
	virtual void		paintEvent( QPaintEvent* _event );	
	virtual void		mousePressEvent( QMouseEvent* _event );
	virtual void		mouseReleaseEvent( QMouseEvent* _event );
	virtual void		mouseMoveEvent( QMouseEvent* _event );
	virtual void		wheelEvent( QWheelEvent* _event );
	virtual void		keyPressEvent( QKeyEvent* _event );
	virtual void		keyReleaseEvent( QKeyEvent* _event );
	virtual void		resizeEvent( QResizeEvent* _event );

	// -------------------------------------------------------------------------
	// Overrides QWidget.
	// -------------------------------------------------------------------------
public:
	virtual void		Tick( qreal _delta );
	virtual QMenu*		GetEditMenu() const;

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------
public slots:
	ReTrackSuite*		OnNewTrack();
	void				OnContextMenu( const QPoint& _point );

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	void				InitRuler();
	void				InitMenus();

protected:
	// Widgets.
	typedef std::list< ReTrackSuite >		TTrackList;
	typedef TTrackList::iterator			TTrackListItor;
	typedef TTrackList::const_iterator		TTrackListCItor;

	TTrackList			m_trackList;
	ReRulerWidget*		m_rulerWidget;

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
	QPoint				m_cursor;
	QString				m_debugInfo;
	bool				m_isDebugEnabled;
};


}
#endif	// _RE_TRACK_PANEL_WIDGET_H_
