// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101120.
// -----------------------------------------------------------------------------
#ifndef _RE_CLIP_PANEL_WIDGET_H_
#define _RE_CLIP_PANEL_WIDGET_H_


#include <QWidget>
#include "Ui\ReBaseWidget.h"


class QTableView;


namespace RE
{


class ReClipEditor;
class ReClipModel;


class ReClipPanelWidget : public ReBaseWidget< QWidget >
{
	Q_OBJECT
	typedef ReBaseWidget< QWidget > TSuper;

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
public:
	ReClipPanelWidget( QWidget* _parent = NULL );

	void			SetClipMode( ReClipModel* _model );

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
protected:
	QTableView*		m_clipTableView;
};

}
#endif	// _RE_CLIP_PANEL_WIDGET_H_
