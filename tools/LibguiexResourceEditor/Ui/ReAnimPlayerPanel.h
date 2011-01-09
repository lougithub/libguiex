// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110109.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_PLAYERPANEL_H_
#define _RE_ANIM_PLAYERPANEL_H_


#include "Ui\ReBaseWidget.h"
#include "Ui\ReAnimPlayer.h"
#include "Core\ReDragInfo.h"
#include <QWidget>


class QPushButton;
class QComboBox;


namespace RE
{


class ReRulerWidget;
class ReAnimModel;
class ReAnimGraphicsItem;


class ReAnimPlayerPanel : public ReBaseWidget< QWidget >
{
	Q_OBJECT
	typedef ReBaseWidget< QWidget > TSuper;

	enum eButton
	{
		EButton_GotoBegin,
		EButton_Play,
		EButton_Stop,		
		EButton_GotoEnd,
		EButton_More,
		EButton_Count
	};


	// ----------------------------------------------------------------------------
	// General.
	// ----------------------------------------------------------------------------
public:
	ReAnimPlayerPanel( ReAnimModel* _model, QWidget* _parent = NULL );

	ReRulerWidget*		GetRuler() const { return m_rulerWidget; }

	// ----------------------------------------------------------------------------
	// Override QWidget.
	// ----------------------------------------------------------------------------
protected:
	virtual void		paintEvent( QPaintEvent* _event );
	virtual void		resizeEvent( QResizeEvent* _event );

	// ----------------------------------------------------------------------------
	// Signals.
	// ----------------------------------------------------------------------------

	// ----------------------------------------------------------------------------
	// Slots.
	// ----------------------------------------------------------------------------
public slots:
	void				OnTogglePlay();
	void				OnSceneSelectionChanged();
	void				OnRulerCursorChanged( int _cursor );

	// ----------------------------------------------------------------------------
	// Utilities.
	// ----------------------------------------------------------------------------
protected:
	void				UpdateLayout();

	// ----------------------------------------------------------------------------
	// Variables.
	// ----------------------------------------------------------------------------
protected:
	ReAnimModel*		m_animModel;
	QPushButton*		m_buttons[ EButton_Count ];
	QComboBox*			m_modeComboBox;
	ReRulerWidget*		m_rulerWidget;
	ReAnimPlayer		m_player;	
};


}
#endif	// _RE_ANIM_PLAYERPANEL_H_
