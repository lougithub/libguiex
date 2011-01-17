// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101121.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Core\ReClipModel.h"
#include "Core\ReZoomInfo.h"
#include "UI\ReClipWorkshop.h"
#include "UI\ReClipWidget.h"
#include "UI\ReClipImage.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QFileDialog>


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReClipWorkshop::ReClipWorkshop( ReClipModel* _model, QWidget* _parent )
: TSuper( _parent )
, m_model( _model )
, m_clipImage( NULL )
, m_filePath( tr( "" ) )
{
	setMouseTracking( true );
	setContextMenuPolicy( Qt::CustomContextMenu );
	setFocusPolicy( Qt::ClickFocus );
	connect( this, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnContextMenu( const QPoint& ) ) );	
	
	InitMenus();

	m_zoomInfo.Init( 1, 1, 6, 1 );
}


// -----------------------------------------------------------------------------
// Override QWidget.
// -----------------------------------------------------------------------------
void ReClipWorkshop::paintEvent( QPaintEvent* _event )
{
	QPainter painter( this );

	// Background.
	painter.fillRect( geometry(), QColor( 200, 200, 180 ) );
}


void ReClipWorkshop::wheelEvent( QWheelEvent* _event )
{
	if( NULL != m_clipImage )
	{
		int degrees = _event->delta() / 8;
		int stepCount = degrees / 15;

		m_zoomInfo.Zoom( stepCount );
	}
}


void ReClipWorkshop::mousePressEvent( QMouseEvent* _event )
{
	if( Qt::MidButton == _event->button() )
	{
		if( NULL != m_clipImage )
		{
			m_dragInfo.SetCursorPosBackup( _event->pos() );
			m_dragInfo.SetItemPosBackup( m_clipImage->pos() );
			m_dragInfo.StartMove();
		}
	}
}


void ReClipWorkshop::mouseReleaseEvent( QMouseEvent* _event )
{
	m_dragInfo.Stop();
}


void ReClipWorkshop::mouseMoveEvent( QMouseEvent* _event )
{
	if( Qt::MidButton & _event->buttons() )
	{
		if( NULL !=m_clipImage )
		{
			QPoint delta = _event->pos() - m_dragInfo.GetCursorPosBackup();
			m_clipImage->move( m_dragInfo.GetItemPosBackup() + delta );
		}
	}
}


// -----------------------------------------------------------------------------
// Override ReModelBase.
// -----------------------------------------------------------------------------
void ReClipWorkshop::Tick( qreal _delta )
{
}


// -----------------------------------------------------------------------------
// Slots.
// -----------------------------------------------------------------------------
void ReClipWorkshop::OnContextMenu( const QPoint& _point )
{
	m_editMenu->exec( mapToGlobal( _point ) );
}


void ReClipWorkshop::OnLoadImage()
{
	ePromptResult pr = CheckAndPromptToSave();
	if( EPromptResult_Cancel != pr )
	{
		if( EPromptResult_Yes == pr )
			OnSave();

		QString path = QFileDialog::getOpenFileName( this, tr( "Open Image File" ), 
			tr( "." ), tr( "Image File ( *.png *.bmp *.tga *.jpg )" ) );

		if( !path.isEmpty() )
			DoLoadImage( path );
	}
}


void ReClipWorkshop::OnImport()
{
	ePromptResult pr = CheckAndPromptToSave();
	if( EPromptResult_Cancel != pr )
	{
		if( EPromptResult_Yes == pr )
			OnSave();

		QString path = QFileDialog::getOpenFileName( this, tr( "Open Clip File" ), 
			tr( "." ), tr( "Xml File ( *.xml )" ) );

		if( !path.isEmpty() )
		{
			DoImport( path );
		}
	}
}


void ReClipWorkshop::OnSave()
{
	if( m_filePath.isEmpty() )
	{
		QFileDialog dlg;
		QString path = dlg.getSaveFileName( this, tr( "Save as" ), tr( "." ), tr( "Xml File( *.xml )" ) );
		if( !path.isEmpty() )
		{
			m_filePath = path;			
		}
	}

	if( !m_filePath.isEmpty() )
	{
		if( !m_model->Export( m_filePath ) )
			QMessageBox::critical( this, tr( "Failure" ), QString( tr( "Failed to save to file: %1\r\n" ) ).arg( m_filePath ), QMessageBox::Ok );
	}	
}


void ReClipWorkshop::OnSaveAs()
{
	QFileDialog dlg;
	QString path = dlg.getSaveFileName( this, tr( "Save as" ), tr( "." ), tr( "Xml File( *.xml )" ) );
	if( !path.isEmpty() )
	{
		if( m_model->Export( path ) )
			m_filePath = path;
		else
			QMessageBox::critical( this, tr( "Failure" ), QString( tr( "Failed to save to file: %1\r\n" ) ).arg( path ), QMessageBox::Ok );
	}
}


// -----------------------------------------------------------------------------
// Utilities.
// -----------------------------------------------------------------------------
void ReClipWorkshop::InitMenus()
{
	m_editMenu = new QMenu( tr( "&Edit" ) );

	m_loadImageAction = m_editMenu->addAction( tr( "&Load Image" ) );
	connect( m_loadImageAction, SIGNAL( triggered() ), this, SLOT( OnLoadImage() ) );

	m_editMenu->addSeparator();

	m_importAction = m_editMenu->addAction( tr( "&Import" ) );
	connect( m_importAction, SIGNAL( triggered() ), this, SLOT( OnImport() ) );

	m_saveAction = m_editMenu->addAction( tr( "&Save" ) );
	connect( m_saveAction, SIGNAL( triggered() ), this, SLOT( OnSave() ) );

	m_saveAsAction = m_editMenu->addAction( tr( "Save &As" ) );
	connect( m_saveAsAction, SIGNAL( triggered() ), this, SLOT( OnSaveAs() ) );
}


void ReClipWorkshop::DoLoadImage( const QString& _path )
{
	QImage image( _path );
	if( !image.isNull() )
	{
		DoReset();

		// Model.
		ReClipGroupNode* group = m_model->CreateGroup( _path );

		// UI.
		m_clipImage = new ReClipImage( m_model, &m_zoomInfo, this );
		m_clipImage->setVisible( true );
		m_clipImage->SetModelData( group );
		m_clipImage->SetImagePath( _path );
		m_clipImage->move( 0, 0 );
		
		update();
	}
	else
	{
		QMessageBox::critical( this, tr( "Failure" ), QString( tr( "Failed to load image: %1\r\n" ) ).arg( _path ), QMessageBox::Ok );
	}
}


void ReClipWorkshop::DoImport( const QString& _path )
{
	QFile file( _path );
	if( file.open( QFile::ReadOnly | QFile::Text ) )
	{
		DoReset();

		if( m_model->Import( _path ) )
			m_filePath = _path;
		else
			QMessageBox::critical( this, tr( "Failure" ), QString( tr( "Failed to import file: %1\r\n" ) ).arg( _path ), QMessageBox::Ok );
	}
}


void ReClipWorkshop::DoReset()
{
	if( NULL != m_clipImage )
	{
		delete m_clipImage;
		m_clipImage = NULL;
	}

	m_filePath.clear();
}


ePromptResult ReClipWorkshop::CheckAndPromptToSave()
{
	// -1:	cancel;
	// 0:	no;
	// 1:	yes.
	ePromptResult result = EPromptResult_Cancel;

	if( IsDirty() )
	{
		QMessageBox mb( QMessageBox::Question, tr( "Save" ), tr( "Save the current document?" ),
			QMessageBox::Yes | QMessageBox::No, this );
		int btn = mb.exec();

		if( QMessageBox::Yes == btn )
			result = EPromptResult_Yes;
		else if( QMessageBox::No == btn )
			result = EPromptResult_No;
	}

	return result;
}


bool ReClipWorkshop::IsDirty() const
{
	// TODO: implementation.
	return true;
}


bool ReClipWorkshop::IsReadyForEdit() const
{
	return NULL != m_clipImage && NULL != m_clipImage->pixmap();
}


bool ReClipWorkshop::IsClipValid( const ReClipWidget* _clip ) const
{
	return _clip->width() >= 4 && _clip->height() >= 4;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

}
