// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
#ifndef _RE_EDITOR_VIEWWIDGET_H_
#define _RE_EDITOR_VIEWWIDGET_H_


#include <QWidget>
#include "UI\ReBaseWidget.h"


namespace RE
{
	class ReGlWidget;
}


namespace RE
{


class ReViewWidget : public ReBaseWidget< QWidget >
{
	// -----------------------------------------------------
	// General
	// -----------------------------------------------------
	Q_OBJECT

	typedef ReBaseWidget< QWidget >	TSuper;

public:
	ReViewWidget( QWidget* _parent = NULL );

	// -----------------------------------------------------
	// Override QWidget.
	// -----------------------------------------------------
protected:
	virtual void		paintEvent( QPaintEvent* _event );
	virtual void		resizeEvent( QResizeEvent* _event );	

	// -----------------------------------------------------
	// Override ReBaseWidget.
	// -----------------------------------------------------
public:
	virtual void		Tick( float _delta );

	// -----------------------------------------------------
	// Variable
	// -----------------------------------------------------
protected:
	ReGlWidget*			m_glWidget;
};


}		// namespace RE
#endif	// _RE_EDITOR_VIEWWIDGET_H_
