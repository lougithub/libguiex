// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101205.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_PANEL_WIDGET_H_
#define _RE_ANIM_PANEL_WIDGET_H_


#include <QWidget>
#include "Ui\ReBaseWidget.h"


class QTreeView;


namespace RE
{


class ReAnimModel;


class ReAnimPanelWidget : public ReBaseWidget< QWidget >
{
	Q_OBJECT
	typedef ReBaseWidget< QWidget > TSuper;

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
public:
	ReAnimPanelWidget( ReAnimModel* _model, QWidget* _parent = NULL );

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
protected:
	QTreeView*			m_view;
};

}
#endif	// _RE_ANIM_PANEL_WIDGET_H_
