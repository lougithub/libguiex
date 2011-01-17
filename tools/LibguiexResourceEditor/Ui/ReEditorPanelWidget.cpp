// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "CORE\ReQrcHelper.h"
#include "UI\ReEditorPanelWidget.h"
#include "UI\ReImagePanel.h"
#include "UI\ReSettingsWidget.h"
#include "UI\ReClipSheet.h"
#include "UI\ReAnimSheet.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QImage>
#include <QStackedWidget>
#include <QGroupBox>
#include <QPainter>
#include <QTabWidget>


namespace RE
{


// -----------------------------------------------------
// General
// -----------------------------------------------------
ReEditorPanelWidget::ReEditorPanelWidget( const ReParam& _param, QWidget* _parent /* = NULL */ )
: TSuper( _parent )
//, m_settingBtn( NULL )
, m_imagePanelWidget( NULL )
, m_lastPanel( EPanel_ImagePanel )
{
	// Settings button.
	//m_settingBtn = new QPushButton( this );
	//m_settingBtn->setText( tr( "Settings..." ) );
	//m_settingBtn->setCheckable( true );
	//connect( m_settingBtn, SIGNAL( clicked() ), this, SLOT( OnToggleSetting() ) );

	// Panel where all images are displayed.
	m_imagePanelWidget = new ReImagePanel( this );

	// Clip panel.
	m_clipSheet = new ReClipSheet( _param.m_clipModel, this );

	// Animation panel.
	m_animSheet = new ReAnimSheet( _param.m_animModel, this );

	// Settings.
	m_settingsWidget = new ReSettingsWidget( this );
	connect( m_settingsWidget, SIGNAL( accepted() ), this, SLOT( OnSettingsChanged() ) );
	connect( m_settingsWidget, SIGNAL( rejected() ), this, SLOT( OnSettingsChanged() ) );

	// Tab.
	m_tab = new QTabWidget( this );
	m_tab->addTab( m_clipSheet, tr( "Clip" ) );
	m_tab->addTab( m_animSheet, tr( "Anim" ) );
	m_tab->addTab( m_imagePanelWidget, tr( "Image" ) );
	m_tab->setCurrentIndex( EPanel_ClipPanel );

	// Layout.	
	QVBoxLayout* layoutGroupBox = new QVBoxLayout( this );
	layoutGroupBox->addWidget( m_tab );
	QGroupBox* groupBox = new QGroupBox( this );
	groupBox->setLayout( layoutGroupBox );	

	QVBoxLayout* layoutMain = new QVBoxLayout( this );
	//layoutMain->addWidget( m_settingBtn, 0, Qt::AlignTop );
	layoutMain->addWidget( groupBox, 1 );
	setLayout( layoutMain );
}


ReEditorPanelWidget::~ReEditorPanelWidget()
{
}


// -----------------------------------------------------
// ImageBrowser
// -----------------------------------------------------
void ReEditorPanelWidget::Tick( qreal _delta )
{
	if( NULL != m_imagePanelWidget )
		m_imagePanelWidget->Tick( _delta );
}


void ReEditorPanelWidget::SwitchPanel( ePanel _panel )
{
	ePanel curPanel = ( ePanel )m_tab->currentIndex();
	if( curPanel != _panel )
		m_tab->setCurrentIndex( _panel );
}


void ReEditorPanelWidget::OnToggleSetting()
{
}


void ReEditorPanelWidget::OnSettingsChanged()
{
	//if( QDialog::Accepted == m_settingsWidget->result() )
	//{
	//	QString qrcFilename = m_settingsWidget->GetQrcFilename();
	//	ReQrcHelper qrc( qrcFilename );

	//	if( qrc.IsValid() )
	//	{
	//		QList< QString > idArray;

	//		const ReQrcHelper::TResList& resList = qrc.GetResourceList();
	//		ReQrcHelper::TResListCItor itorRes = resList.begin();
	//		ReQrcHelper::TResListCItor itorResEnd = resList.end();

	//		for( ; itorRes != itorResEnd; ++itorRes )
	//		{
	//			const ReQrcHelper::ReQResource& resource = *itorRes;
	//			const ReQrcHelper::ReQResource::TFileList& fileList = resource.GetFileList();
	//			ReQrcHelper::ReQResource::TFileListCItor itorFile = fileList.begin();
	//			ReQrcHelper::ReQResource::TFileListCItor itorFileEnd = fileList.end();

	//			QString prefix = resource.GetPrefix();
	//			for( ; itorFile != itorFileEnd; ++itorFile )
	//			{
	//				const ReQrcHelper::ReQFile& file = *itorFile;
	//				QString id;
	//				id = ":" + prefix + "/" + file.m_filename;
	//				idArray.push_back( id );
	//			}
	//		}

	//		if( idArray.size() > 0 )
	//		{
	//			m_imagePanelWidget->Clear();
	//			m_imagePanelWidget->Upload( idArray );
	//		}
	//	}
	//}

	//m_stack->setCurrentIndex( EPanel_ImagePanel );
	//m_settingBtn->setChecked( !m_settingBtn->isChecked() );
	//update();
}


// -----------------------------------------------------
// Override QWidget
// -----------------------------------------------------
void ReEditorPanelWidget::paintEvent( QPaintEvent* _event )
{
	QPainter painter( this );

	//painter.fillRect( 0, 0, width(), height(), QColor( 200, 200, 200, 255 ) );
	painter.fillRect( 0, 0, width(), height(), QColor( 50, 50, 50, 255 ) );

	//if( m_settingBtn->isChecked() )
	//	painter.fillRect( 0, 0, width(), height(), QColor( 200, 200, 200, 255 ) );
	//else
	//	painter.fillRect( 0, 0, width(), height(), QColor( 50, 50, 50, 255 ) );

	TSuper::paintEvent( _event );
}


// -----------------------------------------------------
// Utility
// -----------------------------------------------------
void ReEditorPanelWidget::RefreshItemList()
{
}


void ReEditorPanelWidget::RefreshImages()
{
}


}	// namespace RE
