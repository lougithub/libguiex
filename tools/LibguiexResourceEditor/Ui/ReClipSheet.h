// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110115.
// -----------------------------------------------------------------------------
#ifndef _RE_CLIP_SHEET_H_
#define _RE_CLIP_SHEET_H_


#include <QWidget>
#include "Ui\ReBaseWidget.h"


class QTreeView;


namespace RE
{


class ReClipEditor;
class ReClipModel;


class ReClipSheet : public ReBaseWidget< QWidget >
{
	Q_OBJECT
	typedef ReBaseWidget< QWidget > TSuper;

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
public:
	ReClipSheet( ReClipModel* _model, QWidget* _parent = NULL );

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
protected:
	QTreeView*		m_clipView;
};

}
#endif	// _RE_CLIP_SHEET_H_
