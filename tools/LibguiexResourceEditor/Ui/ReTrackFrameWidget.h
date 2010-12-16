// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101127.
// -----------------------------------------------------------------------------
#ifndef _RE_TRACK_FRAME_WIDGET_H_
#define _RE_TRACK_FRAME_WIDGET_H_


#include "Ui\ReBaseWidget.h"
#include <QLabel>
#include <QVariant>


namespace RE
{


class ReAnimFrame;


// -----------------------------------------------------------------------------
// Class ReTrackFrameWidget.
// -----------------------------------------------------------------------------
class ReTrackFrameWidget : public ReBaseWidget< QLabel >
{
//	Q_OBJECT
	typedef ReBaseWidget< QLabel >	TSuper;

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReTrackFrameWidget( QWidget* _parent = NULL );
	
	//int						GetId() const						{ return m_id; }
	//int&					GetIdRef()							{ return m_id; }

	//virtual void			SetData( const QVariant& _data )	{}
	//virtual QVariant		GetData() const						{ return QVariant(); }

	ReAnimFrame*			GetModelData() const				{ return m_modelData; }
	ReAnimFrame*&			GetModelDataRef()					{ return m_modelData; }

	// -------------------------------------------------------------------------
	// Override QWidget.
	// -------------------------------------------------------------------------
protected:
	virtual void			paintEvent( QPaintEvent* _event );
	virtual void			mousePressEvent( QMouseEvent* _event );
	virtual void			mouseReleaseEvent( QMouseEvent* _event );

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
public:
	static int				sNextId();

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	//int						m_id;
	ReAnimFrame*			m_modelData;
	static int				ms_nextId;
};


/*
// -----------------------------------------------------------------------------
// Class ReTranslationTrackFrameWidget.
// -----------------------------------------------------------------------------
class ReTranslationTrackFrameWidget : public ReTrackFrameWidget
{
	Q_OBJECT

public:
	ReTranslationTrackFrameWidget( QWidget* _parent = NULL ): ReTrackFrameWidget( _parent ) {}

	//--------------------------------------------------------------------------
	// Override ReTrackFrameWidget.
	//--------------------------------------------------------------------------
public:
	virtual void			SetData( const QVariant& _data )	{ m_position = _data.toPoint(); }
	virtual QVariant		GetData() const						{ return QVariant( m_position ); }

	//--------------------------------------------------------------------------
	// Variables.
	//--------------------------------------------------------------------------
protected:
	QPoint					m_position;
};


// -----------------------------------------------------------------------------
// Class ReRotationTrackFrameWidget.
// -----------------------------------------------------------------------------
class ReRotationTrackFrameWidget : public ReTrackFrameWidget
{
	Q_OBJECT

public:
	ReRotationTrackFrameWidget( QWidget* _parent = NULL ): ReTrackFrameWidget( _parent ) {}

	//--------------------------------------------------------------------------
	// Override ReTrackFrameWidget.
	//--------------------------------------------------------------------------
public:
	virtual void			SetData( const QVariant& _data )	{ m_rotation = _data.toRectF(); }
	virtual QVariant		GetData() const						{ return QVariant( m_rotation ); }

	//--------------------------------------------------------------------------
	// Variables.
	//--------------------------------------------------------------------------
protected:
	QRectF					m_rotation;
};


// -----------------------------------------------------------------------------
// Class ReScaleTrackFrameWidget.
// -----------------------------------------------------------------------------
class ReScaleTrackFrameWidget : public ReTrackFrameWidget
{
	Q_OBJECT

public:
	ReScaleTrackFrameWidget( QWidget* _parent = NULL ): ReTrackFrameWidget( _parent ) {}

	//--------------------------------------------------------------------------
	// Override ReTrackFrameWidget.
	//--------------------------------------------------------------------------
public:
	virtual void			SetData( const QVariant& _data )	{ m_scale = _data.toPointF(); }
	virtual QVariant		GetData() const						{ return QVariant( m_scale ); }

	//--------------------------------------------------------------------------
	// Variables.
	//--------------------------------------------------------------------------
protected:
	QPointF					m_scale;
};


// -----------------------------------------------------------------------------
// Class ReAlphaTrackFrameWidget.
// -----------------------------------------------------------------------------
class ReAlphaTrackFrameWidget : public ReTrackFrameWidget
{
	Q_OBJECT

public:
	ReAlphaTrackFrameWidget( QWidget* _parent = NULL ): ReTrackFrameWidget( _parent ) {}

	//--------------------------------------------------------------------------
	// Override ReTrackFrameWidget.
	//--------------------------------------------------------------------------
public:
	virtual void			SetData( const QVariant& _data )	{ m_alpha = _data.toDouble(); }
	virtual QVariant		GetData() const						{ return QVariant( m_alpha ); }

	//--------------------------------------------------------------------------
	// Variables.
	//--------------------------------------------------------------------------
protected:
	qreal					m_alpha;
};
*/


}
#endif	// _RE_TRACK_FRAME_WIDGET_H_
