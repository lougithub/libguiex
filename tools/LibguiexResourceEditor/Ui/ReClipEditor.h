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


class ReClipWorkshop;


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
	virtual void		resizeEvent( QResizeEvent* _event );

	// ----------------------------------------------------------------------------
	// Slots.
	// ----------------------------------------------------------------------------
public slots:
	void				OnToggleDebug();

	// ----------------------------------------------------------------------------
	// Utilities.
	// ----------------------------------------------------------------------------
protected:

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
protected:
	ReClipWorkshop*		m_workshop;

	// Debug.
	QPoint				m_cursor;
	QString				m_debugInfo;
	bool				m_isDebugEnabled;
};


}
#endif	// _RE_CLIP_EDITOR_H_
