// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110118.
// -----------------------------------------------------------------------------
#ifndef _RE_EDITOR_CLIPCELL_H_
#define _RE_EDITOR_CLIPCELL_H_
// -----------------------------------------------------------------------------
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
class ReClipNodeGroup;


class ReClipCell : public ReBaseWidget< QWidget >
{
	Q_OBJECT
	typedef ReBaseWidget< QWidget > TSuper;

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReClipCell( QWidget* _parent = NULL );

	void				InitFromModelData( ReClipNodeGroup* _modelData );

	// -------------------------------------------------------------------------
	// Override QWidget.
	// -------------------------------------------------------------------------
public:
	virtual void		paintEvent( QPaintEvent* _event );
	virtual void		wheelEvent( QWheelEvent* _event );
	virtual void		mousePressEvent( QMouseEvent* _event );
	virtual void		mouseReleaseEvent( QMouseEvent* _event );
	virtual void		mouseMoveEvent( QMouseEvent* _event );
	virtual void		mouseDoubleClickEvent( QMouseEvent* _event );

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------
public slots:

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	//// Data.
	ReClipNodeGroup*	m_modelData;
	ReClipImage*		m_clipImage;
	ReDragInfo			m_dragInfo;
	ReZoomInfo			m_zoomInfo;
};


}
#endif	// _RE_EDITOR_CLIPCELL_H_
