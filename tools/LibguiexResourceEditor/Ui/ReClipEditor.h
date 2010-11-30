// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101111.
// -----------------------------------------------------------------------------
#ifndef _RE_CLIP_EDITOR_H_
#define _RE_CLIP_EDITOR_H_
// -----------------------------------------------------------------------------
// A scene widget is itself a model that manages clip widget data.
// It also has reference to another model that manages clip data.
// It does not have the ownership of this clip data model.
// -----------------------------------------------------------------------------


#include <QWidget>
#include "Core\ReClipModel.h"
#include "Ui\ReBaseWidget.h"
#include "Ui\ReClipWidget.h"


class QMenu;


namespace RE
{


class ReClipModelWidget;


class ReClipEditor : public ReBaseWidget< QWidget >
{
	Q_OBJECT
	typedef ReBaseWidget	TSuper;

	// ----------------------------------------------------------------------------
	// General.
	// ----------------------------------------------------------------------------
public:
	ReClipEditor( ReClipModel* _model, QWidget* _parent = NULL );
	~ReClipEditor();

	// ----------------------------------------------------------------------------
	// Overrides QWidget.
	// ----------------------------------------------------------------------------
protected:
	virtual void		paintEvent( QPaintEvent* _event );	
	virtual void		mousePressEvent( QMouseEvent* _event );
	virtual void		mouseReleaseEvent( QMouseEvent* _event );
	virtual void		mouseMoveEvent( QMouseEvent* _event );
	virtual void		wheelEvent( QWheelEvent* _event );
	virtual void		keyPressEvent( QKeyEvent* _event );
	virtual void		keyReleaseEvent( QKeyEvent* _event );
	virtual void		resizeEvent( QResizeEvent* _event );

	// ----------------------------------------------------------------------------
	// Override ReBaseWidget.
	// ----------------------------------------------------------------------------
public:
	virtual void		Tick( qreal _delta );
	virtual QMenu*		GetEditMenu() const;

	// ----------------------------------------------------------------------------
	// Slots.
	// ----------------------------------------------------------------------------
public slots:
	void				OnContextMenu( const QPoint& _point );
	void				OnLoadImage();
	void				OnImport();
	void				OnExport();
	void				OnToggleDebug();

	// ----------------------------------------------------------------------------
	// Utilities.
	// ----------------------------------------------------------------------------
protected:
	void				InitMenus();

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
protected:
	typedef RePool< ReClipWidget >		TClipPool;
	typedef TClipPool::TItemListItor	TClipPoolItor;
	typedef TClipPool::TItemListCItor	TClipPoolCItor;

	// Model.
	ReClipModelWidget*	m_modelWidget;			// The parent of all clip widgets.

	// Menu.
	QMenu*				m_editMenu;

	// Zooming.
	qreal				m_currentZoomFactor;
	qreal				m_minZoomFactor;
	qreal				m_maxZoomFactor;
	qreal				m_zoomStep;

	// Debug.
	QPoint				m_cursor;
	QString				m_debugInfo;
	bool				m_isDebugEnabled;
};


}
#endif	// _RE_CLIP_EDITOR_H_
