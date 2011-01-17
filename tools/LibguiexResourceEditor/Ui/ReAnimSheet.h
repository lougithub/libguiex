// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110115.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_SHEET_H_
#define _RE_ANIM_SHEET_H_


#include <QWidget>
#include "Ui\ReBaseWidget.h"


class QTreeView;


namespace RE
{


class ReAnimModel;


class ReAnimSheet : public ReBaseWidget< QWidget >
{
	Q_OBJECT
	typedef ReBaseWidget< QWidget > TSuper;

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
public:
	ReAnimSheet( ReAnimModel* _model, QWidget* _parent = NULL );

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
protected:
	QTreeView*			m_view;
};

}
#endif	// _RE_ANIM_SHEET_H_
