// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110115.
// -----------------------------------------------------------------------------
#ifndef _RE_EDITOR_CLIPWORKSHOP
#define _RE_EDITOR_CLIPWORKSHOP
// -----------------------------------------------------------------------------
// This widget is the working area of the clip editor.
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
#include <QWidget>
#include "Core\ReTypes.h"
#include "Core\ReModelBase.h"
#include "Core\ReDragInfo.h"
#include "Core\ReZoomInfo.h"
#include "Core\ReItemGroup.h"
#include "Core\ReWidgetSelection.h"
#include "UI\ReClipWidget.h"
#include "Ui\ReBaseWidget.h"


class QMenu;
class QAction;


namespace RE
{


class ReClipImage;
class ReClipWidget;
class ReClipModel;


class ReClipWorkshop : public ReBaseWidget< QWidget >
{
	Q_OBJECT
	typedef ReBaseWidget< QWidget >			TSuper;

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReClipWorkshop( ReClipModel* _model, QWidget* _parent = NULL );

	ReDragInfo&			GetDragInfoRef()		{ return m_dragInfo; }
	const ReDragInfo&	GetDragInfo() const		{ return m_dragInfo; }

	// -------------------------------------------------------------------------
	// Override QWidget.
	// -------------------------------------------------------------------------
public:
	virtual void		paintEvent( QPaintEvent* _event );
	virtual void		wheelEvent( QWheelEvent* _event );
	virtual void		mousePressEvent( QMouseEvent* _event );
	virtual void		mouseReleaseEvent( QMouseEvent* _event );
	virtual void		mouseMoveEvent( QMouseEvent* _event );

	// -------------------------------------------------------------------------
	// Override ReBaseWidget.
	// -------------------------------------------------------------------------
public:
	virtual void		Tick( qreal _delta );

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------
public slots:
	void				OnContextMenu( const QPoint& _point );
	void				OnLoadImage();
	void				OnImport();
	void				OnSave();
	void				OnSaveAs();

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	void				InitMenus();
	void				DoLoadImage( const QString& _path );
	void				DoImport( const QString& _path );
	void				DoReset();
	ePromptResult		CheckAndPromptToSave();
	bool				IsDirty() const;
	bool				IsReadyForEdit() const;
	bool				IsClipValid( const ReClipWidget* _clip ) const;

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	//// Data.
	ReClipModel*		m_model;
	ReClipImage*		m_clipImage;
	ReDragInfo			m_dragInfo;
	ReZoomInfo			m_zoomInfo;

	QPoint				m_cursor;
	QString				m_filePath;

	QMenu*				m_editMenu;
	QAction*			m_loadImageAction;
	QAction*			m_importAction;
	QAction*			m_saveAction;
	QAction*			m_saveAsAction;

	QString				m_debugInfo;
};


}
#endif	// _RE_EDITOR_CLIPWORKSHOP
