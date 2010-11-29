// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101121.
// -----------------------------------------------------------------------------
#ifndef _RE_CLIP_MODEL_WIDGET_H_
#define _RE_CLIP_MODEL_WIDGET_H_
// -----------------------------------------------------------------------------
// This widget is the working area of the clip editor.
// It derives from QLabel so that a background image could be displayed.
// All its children represent the clips in the clip editor.
// The moving and resizing features could be developed in either of:
// .
// - ReClipWidget or
// - ReClipModeWidget.
// .
// If we choose the first approach, drag-creating clips could be difficult.
// Therefore, I choose to follow the second approach and keep ReClipWidget's
// responsibilities to minimum.
// -----------------------------------------------------------------------------
#include <QLabel>
#include "Core\ReModelBase.h"
#include "Core\ReDragInfo.h"
#include "UI\ReClipWidget.h"
#include "Ui\ReBaseWidget.h"


namespace RE
{


class ReClipWidget;
class ReClipModel;


class ReClipModelWidget : public ReBaseWidget< QLabel >, public ReModelBase< ReClipWidget >
{
	Q_OBJECT
	typedef ReBaseWidget< QLabel >			TSuper;
	typedef ReModelBase< ReClipWidget >		TSuperB;

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReClipModelWidget( ReClipModel* _model, QWidget* _parent = NULL );

	// -------------------------------------------------------------------------
	// Override QLabel.
	// -------------------------------------------------------------------------
public:
	virtual void			paintEvent( QPaintEvent* _event );
	virtual void			mousePressEvent( QMouseEvent* _event );
	virtual void			mouseReleaseEvent( QMouseEvent* _event );
	virtual void			mouseMoveEvent( QMouseEvent* _event );

	// -------------------------------------------------------------------------
	// Override ReModelBase.
	// -------------------------------------------------------------------------
public:
	virtual void			RecycleData( ReClipWidget* _clip );

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------
public slots:
	bool					OnImportImage();
	void					OnZoom( qreal _zoomFactor );
	void					OnDelete();

	// -------------------------------------------------------------------------
	// Specific.
	// -------------------------------------------------------------------------
public:
	void					Tick( qreal _delta );
	ReDragInfo&				GetDragInfoRef()		{ return m_dragInfo; }
	const ReDragInfo&		GetDragInfo() const		{ return m_dragInfo; }

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	bool					IsReadyForEdit() const;
	ReClipWidget*			CreateClip();
	bool					ValidateClip( ReClipWidget* _clip ) const;
	bool					DoImportImage( const QString& _path );
	void					TrimClip( ReClipWidget* _clip );

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	// Data.
	ReClipModel*			m_model;

	// UI.
	typedef RePool< ReClipWidget >		TClipPool;
	typedef TClipPool::TItemListItor	TClipPoolItor;
	typedef TClipPool::TItemListCItor	TClipPoolCItor;

	TClipPool				m_clipList;			// Keep a list of all children.
	ReClipWidget*			m_currentClip;
	ReDragInfo				m_dragInfo;
	QPoint					m_cursor;

	QString					m_debugInfo;
};


}
#endif	// _RE_CLIP_MODEL_WIDGET_H_
