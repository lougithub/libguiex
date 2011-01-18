// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101121.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Core\ReClipModel.h"
#include "Core\ReZoomInfo.h"
#include "UI\ReClipWorkshop.h"
#include "UI\ReClipCell.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QFileDialog>
#include <QTabWidget>


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReClipWorkshop::ReClipWorkshop( ReClipModel* _model, QWidget* _parent )
: TSuper( _parent )
, m_clipModel( _model )
, m_filePath( tr( "" ) )
{
	setMouseTracking( true );
	setContextMenuPolicy( Qt::CustomContextMenu );
	setFocusPolicy( Qt::ClickFocus );
	connect( this, SIGNAL( customContextMenuRequested( const QPoint& ) ), this, SLOT( OnContextMenu( const QPoint& ) ) );	
	
	InitMenus();

	m_tab = new QTabWidget( this );
	m_tab->hide();
}


// -----------------------------------------------------------------------------
// Override QWidget.
// -----------------------------------------------------------------------------
void ReClipWorkshop::paintEvent( QPaintEvent* _event )
{
	QPainter painter( this );
	painter.fillRect( geometry(), QColor( 200, 200, 180 ) );
}


void ReClipWorkshop::resizeEvent( QResizeEvent* _event )
{
	m_tab->move( 0, 0 );
	m_tab->resize( _event->size() );
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
	QString path = QFileDialog::getOpenFileName( this, tr( "Open Image File" ), 
		tr( "." ), tr( "Image File ( *.png *.bmp *.tga *.jpg )" ) );

	if( !path.isEmpty() )
	{
		ReClipNodeGroup* group = m_clipModel->GetGroupById( path );
		if( NULL == group )
		{
			// Model.
			group = m_clipModel->CreateGroup( path );

			// UI.
			ReClipCell* cell = new ReClipCell( this );
			cell->InitFromModelData( group );
			m_cellList.Add( cell );
			int index = m_tab->addTab( cell, QIcon( *group->GetImage() ), tr( "‡å" ) );
			m_tab->setCurrentIndex( index );
			m_tab->show();
		}
		else
		{
			QMessageBox::warning( this, tr( "Warning" ), tr( "Image already loaded." ) );
		}
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
			tr( "." ), tr( "Clip File ( *.xml )" ) );

		if( !path.isEmpty() )
		{
			Reset();

			if( m_clipModel->Import( path ) )
			{
				m_filePath = path;

				int groupCount = m_clipModel->GetGroupCount();
				if( groupCount > 0 )
				{
					for( int i = 0; i < groupCount; ++i )
					{
						// Model.
						ReClipNodeGroup* group = m_clipModel->GetGroupByIndex( i );

						// UI.
						ReClipCell* cell = new ReClipCell( this );
						cell->InitFromModelData( group );
						m_cellList.Add( cell );
						int index = m_tab->addTab( cell, QIcon( *group->GetImage() ), tr( "‡å" ) );					
					}

					m_tab->setCurrentIndex( 0 );
				}
			}
			else
			{
				QMessageBox::critical( this, tr( "Failure" ), QString( tr( "Failed to import file: %1\r\n" ) ).arg( path ), QMessageBox::Ok );
			}
		}
	}
}


void ReClipWorkshop::OnSave()
{
	bool isVirgin = m_filePath.isEmpty();
	if( isVirgin )
	{
		QFileDialog dlg;
		QString path = dlg.getSaveFileName( this, tr( "Save as" ), tr( "." ), tr( "Clip File( *.xml )" ) );
		if( !path.isEmpty() )
		{
			m_filePath = path;			
		}
	}

	if( !m_filePath.isEmpty() )
	{
		if( !m_clipModel->Export( m_filePath ) )
		{
			QMessageBox::critical( this, tr( "Failure" ), QString( tr( "Failed to save to file: %1\r\n" ) ).arg( m_filePath ), QMessageBox::Ok );
			if( isVirgin )
				m_filePath.clear();
		}
	}	
}


void ReClipWorkshop::OnSaveAs()
{
	QFileDialog dlg;
	QString path = dlg.getSaveFileName( this, tr( "Save as" ), tr( "." ), tr( "Clip File( *.xml )" ) );
	if( !path.isEmpty() )
	{
		if( m_clipModel->Export( path ) )
		{
			m_filePath = path;
		}
		else
		{
			QMessageBox::critical( this, tr( "Failure" ), QString( tr( "Failed to save to file: %1\r\n" ) ).arg( path ), QMessageBox::Ok );
		}
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

	m_importAction = m_editMenu->addAction( tr( "&Import" ) );
	connect( m_importAction, SIGNAL( triggered() ), this, SLOT( OnImport() ) );

	m_editMenu->addSeparator();

	m_saveAction = m_editMenu->addAction( tr( "&Save" ) );
	connect( m_saveAction, SIGNAL( triggered() ), this, SLOT( OnSave() ) );

	m_saveAsAction = m_editMenu->addAction( tr( "Save &As" ) );
	connect( m_saveAsAction, SIGNAL( triggered() ), this, SLOT( OnSaveAs() ) );
}


void ReClipWorkshop::Reset()
{
	m_tab->clear();
	m_cellList.Destroy();
	m_dragInfo.Stop();
	m_filePath.clear();
}


ePromptResult ReClipWorkshop::CheckAndPromptToSave()
{
	// -1:	cancel;
	// 0:	no;
	// 1:	yes.
	ePromptResult result = EPromptResult_Cancel;

	if( m_clipModel->IsDirty() )
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


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

}
