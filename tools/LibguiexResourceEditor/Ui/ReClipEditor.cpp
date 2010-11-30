// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101111.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "ReClipEditor.h"
#include "UI\ReClipModelWidget.h"
#include <QMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>


namespace RE
{


// ----------------------------------------------------------------------------
// General.
// ----------------------------------------------------------------------------
ReClipEditor::ReClipEditor( ReClipModel* _model, QWidget* _parent /* = NULL */ )
: TSuper( _parent )
, m_editMenu( NULL )
, m_currentZoomFactor( 1.0f )
, m_minZoomFactor( 1.0f )
, m_maxZoomFactor( 5.0f )
, m_zoomStep( 1.0f )
, m_isDebugEnabled( false )
{
	InitMenus();

	m_modelWidget = new ReClipModelWidget( _model, this );
	m_modelWidget->setScaledContents( true );

	connect( this, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnContextMenu( const QPoint& ) ) );
}


ReClipEditor::~ReClipEditor()
{
}


// ----------------------------------------------------------------------------
// Overrides QWidget.
// ----------------------------------------------------------------------------
void ReClipEditor::paintEvent( QPaintEvent* _event )
{
	QPainter painter( this );

	// Background.
	painter.fillRect( geometry(), QColor( 200, 200, 180 ) );

	// Axes.
	QPoint lt = m_modelWidget->pos();
	QPoint hA( 0, lt.y() );
	QPoint hB( width(), lt.y() );
	QPoint vA( lt.x(), 0 );
	QPoint vB( lt.x(), height() );
	painter.setPen( Qt::DashLine );
	painter.drawLine( hA, hB );
	painter.drawLine( vA, vB );

	// Debug.
	if( m_isDebugEnabled )
	{
		painter.drawText( 5, 10, m_debugInfo );
	}
}


void ReClipEditor::mousePressEvent( QMouseEvent* _event )
{
	if( ( Qt::MidButton == _event->button() ) ||
		( Qt::LeftButton == _event->button() && Qt::ControlModifier & _event->modifiers() ) )
	{
		m_modelWidget->GetDragInfoRef().SetCursorPosBackup( _event->pos() );
		m_modelWidget->GetDragInfoRef().SetItemPosBackup( m_modelWidget->pos() );
		m_modelWidget->GetDragInfoRef().StartMove();
	}
}


void ReClipEditor::mouseReleaseEvent( QMouseEvent* _event )
{
	m_modelWidget->GetDragInfoRef().Stop();
}


void ReClipEditor::mouseMoveEvent( QMouseEvent* _event )
{
	m_cursor = _event->pos();

	if( m_modelWidget->GetDragInfo().IsMoving() )
	{
		QPoint delta = _event->pos() - m_modelWidget->GetDragInfo().GetCursorPosBackup();
		m_modelWidget->move( m_modelWidget->GetDragInfo().GetItemPosBackup() + delta );
	}
}


void ReClipEditor::wheelEvent( QWheelEvent* _event )
{
	int degrees = _event->delta() / 8;
	int stepCount = degrees / 15;

	m_currentZoomFactor += m_zoomStep * stepCount;
	if( m_currentZoomFactor > m_maxZoomFactor )
		m_currentZoomFactor = m_maxZoomFactor;
	else if( m_currentZoomFactor < m_minZoomFactor )
		m_currentZoomFactor = m_minZoomFactor;

	m_modelWidget->OnZoom( m_currentZoomFactor );	
}


void ReClipEditor::keyPressEvent( QKeyEvent* _event )
{
	if( Qt::Key_Control == _event->key() )
	{
		OnToggleDebug();
	}
	else if( Qt::Key_Delete == _event->key() )
	{
		m_modelWidget->OnDelete();
	}
	else
	{
		TSuper::keyPressEvent( _event );
	}
}


void ReClipEditor::keyReleaseEvent( QKeyEvent* _event )
{
	TSuper::keyReleaseEvent( _event );
}


void ReClipEditor::resizeEvent( QResizeEvent* _event )
{
	static bool sIsFirstTime = true;

	if( sIsFirstTime )
	{
		sIsFirstTime = false;

		m_modelWidget->move( width() / 2, height() / 2 );
	}

	TSuper::resizeEvent( _event );
}


// ----------------------------------------------------------------------------
// Override ReBaseWidget.
// ----------------------------------------------------------------------------
void ReClipEditor::Tick( qreal _delta )
{
	// Sync model data ( pull mode ), and do this only when the user is
	// not currently interacting with the editor.
	m_modelWidget->Tick( _delta );

	if( m_isDebugEnabled )
	{
		m_debugInfo = 
			QString( tr( "Zoom Factor: %1" ) )
			.arg( m_currentZoomFactor );
	}

	update();
}


QMenu* ReClipEditor::GetEditMenu() const
{
	return m_editMenu;
}


// ----------------------------------------------------------------------------
// Slots.
// ----------------------------------------------------------------------------
void ReClipEditor::OnContextMenu( const QPoint& _point )
{
	m_editMenu->exec( mapToGlobal( _point ) );
}


void ReClipEditor::OnLoadImage()
{
	bool result = false;
	QString path = QFileDialog::getOpenFileName( this, tr( "Open Image File" ), 
		tr( "." ), tr( "Image File ( *.png *.bmp *.tga *.jpg )" ) );

	if( !path.isNull() )
	{
		if( m_modelWidget->LoadImage( path ) )
		{
			m_currentZoomFactor = 1.0f;
			m_modelWidget->OnZoom( m_currentZoomFactor );
			result = true;			
		}
		else
		{
			QMessageBox::critical( this, tr( "Failure" ), tr( "Failed to import image" ), QMessageBox::Ok );
		}
	}
}


void ReClipEditor::OnImport()
{
}


void ReClipEditor::OnExport()
{
	QString path = QFileDialog::getSaveFileName( this, tr( "Export Clip File" ),
		tr( "." ), tr( "Clip File ( *.xml )" ) );

	if( !path.isNull() )
	{
		if( !m_modelWidget->Export( path ) )
		{
			QMessageBox::critical( this, tr( "Failure" ), tr( "Failed to export clips" ), QMessageBox::Ok );
		}
	}
}


void ReClipEditor::OnToggleDebug()
{
	m_isDebugEnabled = !m_isDebugEnabled;
}


// ----------------------------------------------------------------------------
// Utilities.
// ----------------------------------------------------------------------------
void ReClipEditor::InitMenus()
{
	QMenu* menu = NULL;
	QAction* action = NULL;

	m_editMenu = new QMenu( tr( "&Edit" ) );

	action = m_editMenu->addAction( tr( "&Load Image" ) );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnLoadImage() ) );

	m_editMenu->addSeparator();

	action = m_editMenu->addAction( tr( "&Import" ) );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnImport() ) );

	action = m_editMenu->addAction( tr( "&Export" ) );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnExport() ) );
}


}
