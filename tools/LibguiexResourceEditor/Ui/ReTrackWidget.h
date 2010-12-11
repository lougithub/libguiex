// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101123.
// -----------------------------------------------------------------------------
#ifndef _RE_TRACK_WIDGET_H_
#define _RE_TRACK_WIDGET_H_
// -----------------------------------------------------------------------------
// Tracks do not know about the animation data model, so it delegate all frame
// related operations to a higher level, i.e, the track panel widget, by means 
// of signal:
// - signal CreateFrameRequested: 
// - signal DeleteFrameRequested:
// - signal FrameMoved:
// -----------------------------------------------------------------------------


#include "Ui\ReRulerWidget.h"
#include "Ui\ReTrackFrameWidget.h"
#include "Core\ReModelBase.h"
#include "Core\RePool.h"
#include <QVariant>


class QMenu;


namespace RE
{


class ReTrackFrameWidget;


// -----------------------------------------------------------------------------
// Class ReTrackWidget.
// -----------------------------------------------------------------------------
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

	ReTrackFrameWidget*	CreateFrameAtCurrentCursor();
	void				DeleteCurrentFrame();

	ReTrackFrameWidget*	GetFrameByIndex( int _index );
	ReTrackFrameWidget*	GetCurrentFrame() const			{ return m_currentFrame; }
	void				SetFrameSize( int _w, int _h )	{ m_frameSize = QSize( _w, _h ); }

	QVariant			Interpolate( int _cursor ) const;	
	void				GetNearestFrames( int _cursor, ReTrackFrameWidget*& _left, ReTrackFrameWidget*& _right );
	int					CalcCursorAtFrame( const ReTrackFrameWidget* _frame ) const;	

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
	virtual void		DrawContent( QPainter& _painter );
	virtual void		DrawForeground( QPainter& _painter );

public slots:
	virtual void		OnViewportChanged( int _pos );

	// -------------------------------------------------------------------------
	// Override ReModelBase.
	// -------------------------------------------------------------------------
public:
	virtual void		RecycleData( ReTrackFrameWidget* _frame );

	// -------------------------------------------------------------------------
	// Signals.
	// -------------------------------------------------------------------------
signals:
	void				CreateFrameRequested( ReTrackWidget* _track );
	void				DeleteFrameRequested( ReTrackWidget* _track );
	void				FrameMoved( ReTrackWidget* _track, ReTrackFrameWidget* _frame, qreal _time );

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------
public slots:
	void				OnContextMenu( const QPoint& _point );
	virtual void		OnCreateFrame();
	virtual void		OnDeleteFrame();

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	virtual void		InitMenus();
	virtual QVariant	Interpolate( const QVariant& _left, const QVariant& _right, qreal _factor ) const { return QVariant(); }

	ReTrackFrameWidget*	DoCreateFrame();

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


/*
// -----------------------------------------------------------------------------
// Class ReTranslatonTrackWidget. 
// -----------------------------------------------------------------------------
class ReTranslationTrackWidget : public ReTrackWidget
{
	typedef ReTrackWidget	TSuper;

public:
	ReTranslationTrackWidget( QWidget* _parent = NULL ): TSuper( _parent ) {}

	// -------------------------------------------------------------------------
	// Override ReModelBase.
	// -------------------------------------------------------------------------
public:
	virtual ReTrackFrameWidget*	CreateData()
	{
		ReTrackFrameWidget* result = m_recyclePool.Pop();
		if( NULL == result )
			result = new ReTranslationTrackFrameWidget();

		if( NULL != result )
			m_dataPool.Push( result );

		return result;	
	}
	
	// -------------------------------------------------------------------------
	// Override ReTrackWidget.
	// -------------------------------------------------------------------------
protected:
	virtual QVariant Interpolate( const QVariant& _left, const QVariant& _right, qreal _factor ) const
	{
		QPoint posLeft = _left.toPoint();
		QPoint posRight = _right.toPoint();

		return QVariant( ( posLeft + posRight ) / 2 );
	}
};


// -----------------------------------------------------------------------------
// Class ReRotationTrackWidget. 
// -----------------------------------------------------------------------------
class ReRotationTrackWidget : public ReTrackWidget
{
	typedef ReTrackWidget	TSuper;

public:
	ReRotationTrackWidget( QWidget* _parent = NULL ): TSuper( _parent ) {}

	// -------------------------------------------------------------------------
	// Override ReModelBase.
	// -------------------------------------------------------------------------
public:
	virtual ReTrackFrameWidget*	CreateData()
	{
		ReTrackFrameWidget* result = m_recyclePool.Pop();
		if( NULL == result )
			result = new ReRotationTrackFrameWidget();

		if( NULL != result )
			m_dataPool.Push( result );

		return result;	
	}

	// -------------------------------------------------------------------------
	// Override ReTrackWidget.
	// -------------------------------------------------------------------------
protected:
	virtual QVariant Interpolate( const QVariant& _left, const QVariant& _right, qreal _factor ) const
	{
		qreal rotLeft = _left.toDouble();
		qreal rotRight = _right.toDouble();

		return QVariant( ( rotLeft + rotRight ) / 2.0f );
	}
};


// -----------------------------------------------------------------------------
// Class ReScaleTrackWidget. 
// -----------------------------------------------------------------------------
class ReScaleTrackWidget : public ReTrackWidget
{
	typedef ReTrackWidget	TSuper;

public:
	ReScaleTrackWidget( QWidget* _parent = NULL ): TSuper( _parent ) {}

	// -------------------------------------------------------------------------
	// Override ReModelBase.
	// -------------------------------------------------------------------------
public:
	virtual ReTrackFrameWidget*	CreateData()
	{
		ReTrackFrameWidget* result = m_recyclePool.Pop();
		if( NULL == result )
			result = new ReScaleTrackFrameWidget();

		if( NULL != result )
			m_dataPool.Push( result );

		return result;	
	}

	// -------------------------------------------------------------------------
	// Override ReTrackWidget.
	// -------------------------------------------------------------------------
protected:
	virtual QVariant Interpolate( const QVariant& _left, const QVariant& _right, qreal _factor ) const
	{
		QPointF scaleLeft = _left.toPointF();
		QPointF scaleRight = _right.toPointF();

		return QVariant( ( scaleLeft + scaleRight ) / 2.0f );
	}
};


// -----------------------------------------------------------------------------
// Class ReAlphaTrackWidget. 
// -----------------------------------------------------------------------------
class ReAlphaTrackWidget : public ReTrackWidget
{
	typedef ReTrackWidget	TSuper;

public:
	ReAlphaTrackWidget( QWidget* _parent = NULL ): TSuper( _parent ) {}

	// -------------------------------------------------------------------------
	// Override ReModelBase.
	// -------------------------------------------------------------------------
public:
	virtual ReTrackFrameWidget*	CreateData()
	{
		ReTrackFrameWidget* result = m_recyclePool.Pop();
		if( NULL == result )
			result = new ReAlphaTrackFrameWidget();

		if( NULL != result )
			m_dataPool.Push( result );

		return result;	
	}

	// -------------------------------------------------------------------------
	// Override ReTrackWidget.
	// -------------------------------------------------------------------------
protected:
	virtual QVariant Interpolate( const QVariant& _left, const QVariant& _right, qreal _factor ) const
	{
		qreal alphaLeft = _left.toDouble();
		qreal alphaRight = _right.toDouble();

		return QVariant( ( alphaLeft + alphaRight ) / 2.0f );
	}
};
*/


}
#endif	// _RE_TRACK_WIDGET_H_
