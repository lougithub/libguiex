// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
#ifndef _RE_EDITOR_MAINWINDOW_H_
#define _RE_EDITOR_MAINWINDOW_H_


#include <QMainWindow>
#include "UI\ReViewWidget.h"
#include "Ui\ReBaseWidget.h"


class QLabel;
class QDockWidget;
class QStackedWidget;
class QAction;


namespace RE
{


class ReEditorPanelWidget;
class ReEditor;
class ReClipEditor;
class ReAnimEditor;
class ReAnimConsoleWidget;
class ReClipModel;
class ReAnimModel;


class ReMainWindow : public QMainWindow
{
	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
	Q_OBJECT

	typedef QMainWindow				TSuper;
	typedef ReBaseWidget< QWidget >	TEditorBase;

public:
	ReMainWindow( QWidget* _parent = NULL );

	void					SetUpdateDelta( qreal _delta )		{ m_updateDelta = _delta; }
	qreal					GetUpdateDelta() const				{ return m_updateDelta; }

	// -------------------------------------------------------------------------
	// Override QMainWindow
	// -------------------------------------------------------------------------
protected:
	void					paintEvent( QPaintEvent* _event );

	// -------------------------------------------------------------------------
	// Slot
	// -------------------------------------------------------------------------
public slots:
	void					Tick( qreal _delta = 1.0f / 60.0f );

	// -------------------------------------------------------------------------
	// Routines
	// -------------------------------------------------------------------------
protected:
	void					InitData();
	void					InitMainViews();
	void					InitDockWindows();
	void					InitMenuBar();
	void					InitContextMenu();

	// -------------------------------------------------------------------------
	// Menu
	// -------------------------------------------------------------------------
protected:
	enum eEditor			{ EEditor_Clip, EEditor_Anim, EEditor_Image, EEditor_Count };

	// -------------------------------------------------------------------------
	// Slots.
	// -------------------------------------------------------------------------
protected slots:
	void					OnGotoClipEditor();
	void					OnGotoAnimEditor();
	void					OnGotoImageEditor();
	void					OnAboutSoftware();
	void					OnAboutAuthor();

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	void					UpdateEditMenu();

	// -------------------------------------------------------------------------
	// Variable.
	// -------------------------------------------------------------------------
private:
	// Editors.
	QStackedWidget*			m_stackedEditorWidget;
	TEditorBase*			m_editorWidgets[ EEditor_Count ];	
	ReClipEditor*			m_clipEditor;
	ReAnimEditor*			m_animEditor;
	ReViewWidget*			m_viewWidget;	// Will be replaced by image editor.

	// Panels: dockable and stacked widget usually left or right aligned on the screen.
	QDockWidget*			m_panelDockWidget;
	ReEditorPanelWidget*	m_controlPanelWidget;

	// Models.
	ReClipModel*			m_clipModel;
	ReAnimModel*			m_animModel;

	// Menu.
	QMenuBar*				m_menuBarEx;
	QMenu*					m_editorMenu;	
	QMenu*					m_aboutMenu;
	QMenu*					m_contextMenuClipEditor;

	// Misc.
	qreal					m_updateDelta;

public:
	ReEditor*				m_editor;
};


}		// namespace RE
#endif	// _RE_EDITOR_MAINWINDOW_H_
