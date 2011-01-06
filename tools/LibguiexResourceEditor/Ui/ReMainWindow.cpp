// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QIcon>
#include <QAction>
#include <QPainter>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QStackedWidget>
#include <QMessageBox>
#include <QPixmap>
#include "Core\ReEditor.h"
#include "Core\ReClipModel.h"
#include "Core\ReAnimModel.h"
#include "UI\ReMainWindow.h"
#include "UI\ReEditorPanelWidget.h"
#include "Ui\ReClipEditor.h"
#include "Ui\ReAnimEditor.h"
#include "Ui\ReAnimConsoleWidget.h"


namespace RE
{


// -------------------------------------------------------------------------
// General
// -------------------------------------------------------------------------
ReMainWindow::ReMainWindow( QWidget* _parent /* = NULL */ )
: TSuper( _parent )
// Editors.
, m_stackedEditorWidget( NULL )	
, m_clipEditor( NULL )
, m_animEditor( NULL )
, m_viewWidget( NULL )
// Panels.
, m_panelDockWidget( NULL )
, m_controlPanelWidget( NULL )
// Models.
, m_clipModel( NULL )
, m_animModel( NULL )
// Menus.
, m_menuBarEx( NULL )
, m_editorMenu( NULL )
, m_aboutMenu( NULL )
//, m_contextMenuTrackPanel( NULL )
// Misc.
, m_updateDelta( 0.0f )
{
	setObjectName( "ReMainWindow" );
	setWindowTitle( tr( "Resource Editor" ) );

	InitData();
	InitMainViews();
	InitDockWindows();
	InitMenuBar();
	InitContextMenu();
}


// -------------------------------------------------------------------------
// Override QMainWindow
// -------------------------------------------------------------------------
void ReMainWindow::paintEvent( QPaintEvent* _event )
{
	TSuper::paintEvent( _event );
}


// -------------------------------------------------------------------------
// Slot
// -------------------------------------------------------------------------
void ReMainWindow::Tick( qreal _delta )
{
	// Menu.
	UpdateEditMenu();

	// Editor.
	int index = m_stackedEditorWidget->currentIndex();
	m_editorWidgets[ index ]->Tick( _delta );
	m_editorWidgets[ index ]->update();
	
	// Control panel. 
	m_controlPanelWidget->Tick( _delta );

	// Hack multi-thread.
	if( NULL != m_editor )
		m_editor->Sleep( 3 );
}

// -------------------------------------------------------------------------
// Routines
// -------------------------------------------------------------------------
void ReMainWindow::InitData()
{
	m_clipModel = new ReClipModel();
	m_clipModel->setColumnCount( ReClipModel::EColumn_Count );
	m_clipModel->setHeaderData( ReClipModel::EColumn_Icon, Qt::Horizontal, tr( "Clip" ) );
	m_clipModel->setHeaderData( ReClipModel::EColumn_X, Qt::Horizontal, tr( "X" ) );
	m_clipModel->setHeaderData( ReClipModel::EColumn_Y, Qt::Horizontal, tr( "Y" ) );
	m_clipModel->setHeaderData( ReClipModel::EColumn_W, Qt::Horizontal, tr( "W" ) );
	m_clipModel->setHeaderData( ReClipModel::EColumn_H, Qt::Horizontal, tr( "H" ) );

	m_animModel = new ReAnimModel();
//	m_animModel->setHeaderData( ReAnimModel::EColumn_TrackOrFrame, Qt::Horizontal, tr( "Type" ) );
//	m_animModel->setHeaderData( ReAnimModel::EColumn_X, Qt::Horizontal, tr( "X" ) );
//	m_animModel->setHeaderData( ReAnimModel::EColumn_Y, Qt::Horizontal, tr( "Y" ) );
//	m_animModel->setHeaderData( ReAnimModel::EColumn_Z, Qt::Horizontal, tr( "Z" ) );
//	m_animModel->setHeaderData( ReAnimModel::EColumn_W, Qt::Horizontal, tr( "W" ) );
	m_animModel->setHeaderData( ReAnimModel::EColumn_Key, Qt::Horizontal, tr( "Key" ) );
	m_animModel->setHeaderData( ReAnimModel::EColumn_Value, Qt::Horizontal, tr( "Value" ) );
}


void ReMainWindow::InitMainViews()
{
	// Clip editor.	
	m_clipEditor = new ReClipEditor( m_clipModel, this );
	m_clipEditor->setContextMenuPolicy( Qt::CustomContextMenu );
	m_clipEditor->setFocusPolicy( Qt::ClickFocus );

	// Animation editor.
	m_animEditor = new ReAnimEditor( m_animModel, this );
	m_animEditor->setContextMenuPolicy( Qt::CustomContextMenu );
	m_animEditor->setFocusPolicy( Qt::ClickFocus );

	// Image editor.
	m_viewWidget = new ReViewWidget( this );

	// Cache for uniformed access.
	m_editorWidgets[ EEditor_Clip ] = m_clipEditor;
	m_editorWidgets[ EEditor_Anim ] = m_animEditor;
	m_editorWidgets[ EEditor_Image ] = m_viewWidget;	

	// Put them all together.
	m_stackedEditorWidget = new QStackedWidget( this );
	m_stackedEditorWidget->setMinimumSize( 400, 200 );
	for( int i = 0; i < EEditor_Count; ++i )
		m_stackedEditorWidget->addWidget( m_editorWidgets[ i ] );
	m_stackedEditorWidget->setCurrentIndex( 0 );

	setCentralWidget( m_stackedEditorWidget );
}


void ReMainWindow::InitDockWindows()
{
	ReEditorPanelWidget::ReParam param;
	param.m_clipModel = m_clipModel;
	param.m_animModel = m_animModel;
	m_controlPanelWidget = new ReEditorPanelWidget( param, this );
	m_panelDockWidget = new QDockWidget( tr( "Control Panel" ), this );
	m_panelDockWidget->setWidget( m_controlPanelWidget );
	m_panelDockWidget->setAllowedAreas( Qt::AllDockWidgetAreas );
	addDockWidget( Qt::RightDockWidgetArea, m_panelDockWidget );

	{
		//QLabel* label = NULL;
		//QDockWidget* dock = NULL;

		//label = new QLabel( this );
		//label->setText( tr( "PlaceholderA" ) );	
		//dock = new QDockWidget( tr( "PlaceholderA" ), this );
		//dock->setWidget( label );
		//dock->setAllowedAreas( Qt::AllDockWidgetAreas );
		//addDockWidget( Qt::TopDockWidgetArea, dock );

		//label = new QLabel( this );
		//label->setText( tr( "PlaceholderB" ) );	
		//dock = new QDockWidget( tr( "PlaceholderB" ), this );
		//dock->setWidget( label );
		//dock->setAllowedAreas( Qt::AllDockWidgetAreas );
		//addDockWidget( Qt::LeftDockWidgetArea, dock );

		//label = new QLabel( this );
		//label->setText( tr( "PlaceholderC" ) );	
		//dock = new QDockWidget( tr( "PlaceholderC" ), this );
		//dock->setWidget( label );
		//dock->setAllowedAreas( Qt::AllDockWidgetAreas );
		//addDockWidget( Qt::BottomDockWidgetArea, dock );
	}
}


void ReMainWindow::InitMenuBar()
{
	QAction* action = NULL;
	QActionGroup* group = new QActionGroup( this );;

	m_menuBarEx = new QMenuBar();

	// Editors.
	m_editorMenu = new QMenu( tr( "Editors" ) );

	action = m_editorMenu->addAction( tr( "&Clip Editor" ) );
	action->setCheckable( true );
	action->setChecked( true );
	action->setActionGroup( group );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnGotoClipEditor() ) );
	
	action = m_editorMenu->addAction( tr( "&Animation Editor" ) );
	action->setCheckable( true );
	action->setChecked( false );
	action->setActionGroup( group );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnGotoAsEditor() ) );

	action = m_editorMenu->addAction( tr( "&Image Editor" ) );
	action->setCheckable( true );
	action->setChecked( false );
	action->setActionGroup( group );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnGotoImageEditor() ) );

	// About
	m_aboutMenu = new QMenu( tr( "About" ) );

	action = m_aboutMenu->addAction( tr( "&Software" ) );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnAboutSoftware() ) );
	action = m_aboutMenu->addAction( tr( "&Author" ) );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnAboutAuthor() ) );

	m_menuBarEx->addMenu( m_editorMenu );
	m_menuBarEx->addMenu( m_aboutMenu );
	setMenuBar( m_menuBarEx );
}


void ReMainWindow::InitContextMenu()
{
}


// -------------------------------------------------------------------------
// Slots.
// -------------------------------------------------------------------------
void ReMainWindow::OnGotoClipEditor()
{
	m_stackedEditorWidget->setCurrentIndex( EEditor_Clip );
	m_controlPanelWidget->SwitchPanel( ReEditorPanelWidget::EPanel_ClipPanel );
	m_panelDockWidget->setWindowTitle( tr( "Clip Editor" ) );
}


void ReMainWindow::OnGotoAsEditor()
{
	m_stackedEditorWidget->setCurrentIndex( EEditor_Anim );
	m_controlPanelWidget->SwitchPanel( ReEditorPanelWidget::EPanel_AnimPanel );
	m_panelDockWidget->setWindowTitle( tr( "Animation Editor" ) );
}


void ReMainWindow::OnGotoImageEditor()
{
	m_stackedEditorWidget->setCurrentIndex( EEditor_Image );
	m_controlPanelWidget->SwitchPanel( ReEditorPanelWidget::EPanel_ImagePanel );
	m_panelDockWidget->setWindowTitle( tr( "Image Editor" ) );
}


void ReMainWindow::OnAboutSoftware()
{
	QMessageBox msg;
	msg.setIconPixmap( QPixmap( ":/image/hero_earthshaker.png" ) );
	msg.setWindowTitle( tr( "Software" ) );
	msg.setText( tr( "Resource Editor\r\n\r\nV1.0\r\nNov. 21th, 2010" ) );
	msg.exec();
}


void ReMainWindow::OnAboutAuthor()
{
	QMessageBox msg;
	msg.setIconPixmap( QPixmap( ":/image/hero_stormspirit.png" ) );
	msg.setWindowTitle( tr( "Author" ) );
	msg.setText( tr( "GameCrashDebug\r\n\r\n\r\nNov. 21th, 2010" ) );
	msg.exec();
}


// -------------------------------------------------------------------------
// Utilities.
// -------------------------------------------------------------------------
void ReMainWindow::UpdateEditMenu()
{
	m_menuBarEx->clear();
	m_menuBarEx->addMenu( m_editorMenu );

	int index = m_stackedEditorWidget->currentIndex();
	QMenu* editMenu = m_editorWidgets[ index ]->GetEditMenu();
	if( NULL != editMenu )
		m_menuBarEx->addMenu( editMenu );

	m_menuBarEx->addMenu( m_aboutMenu );
}


}	// namespace RE
