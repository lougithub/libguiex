// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#ifndef _RE_TRACK_PANEL_WIDGET_H_
#define _RE_TRACK_PANEL_WIDGET_H_
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


namespace RE
{


class ReAnimModel;
class ReTrackWidget;
class ReRulerWidget;
class ReTrackHeadWidget;
class ReTrackFrameWidget;


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
		ReTrackSuite( ReTrackHeadWidget* _head, ReTrackWidget* _track, int _type )
			: m_headWidget( _head )
			, m_trackWidget( _track )
			, m_type( _type ) 
		{}

		ReTrackHeadWidget*	m_headWidget;
		ReTrackWidget*		m_trackWidget;
		int					m_type;
	};

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReTrackPanelWidget( ReAnimModel* _model, QWidget* _parent = NULL );
	~ReTrackPanelWidget();

	bool				PrepareTransform( eTrackType _type, QVariant& _left, QVariant& _right, qreal& _factor ) const;
	QMatrix				GetTranslationMatrix() const;
	QMatrix				GetRotationMatrix() const;
	QMatrix				GetScaleMatrix() const;	

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
	// Overrides ReBaseWidget.
	// -------------------------------------------------------------------------
public:
	virtual void		Tick( qreal _delta );
	virtual QMenu*		GetEditMenu() const;

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------
public slots:	
	ReTrackSuite*		OnNewTranslationTrack();
	ReTrackSuite*		OnNewRotationTrack();
	ReTrackSuite*		OnNewScaleTrack();
	ReTrackSuite*		OnNewAlphaTrack();
	ReTrackSuite*		OnNewTrack( eTrackType _type );
	void				OnContextMenu( const QPoint& _point );

	void				OnCreateFrameRequested( ReTrackWidget* _track );
	void				OnDeleteFrameRequested( ReTrackWidget* _track );
	void				OnFrameMoved( ReTrackWidget* _track, ReTrackFrameWidget* _frame, qreal _time );

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	void				InitMenus();
	void				InitRuler();	
	void				LayoutTracks();

	eTrackType			GetTrackType( ReTrackWidget* _track ) const;

protected:
	ReAnimModel*		m_animMode;

	// Widgets.
	typedef std::list< ReTrackSuite >		TTrackList;
	typedef TTrackList::iterator			TTrackListItor;
	typedef TTrackList::const_iterator		TTrackListCItor;

	TTrackList			m_trackList;
	ReTrackSuite*		m_trackSuites[ ETrackType_Count ];
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
	QString				m_debugInfo;
	bool				m_isDebugEnabled;
};


}
#endif	// _RE_TRACK_PANEL_WIDGET_H_
