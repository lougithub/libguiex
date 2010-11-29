// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "UI\ReSettingsWidget.h"
#include <QString>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>


namespace RE
{


// -----------------------------------------------------
// General
// -----------------------------------------------------
ReSettingsWidget::ReSettingsWidget( QWidget* _parent )
: TSuper( _parent )
{
	m_qrcLineEdit = new QLineEdit( this );
	QPushButton* btnQrc = new QPushButton( this );
	btnQrc->setText( tr( "Qrc..." ) );
	connect( btnQrc, SIGNAL( clicked() ), this, SLOT( BrowseForQrc() ) );
	QHBoxLayout* layoutQrc = new QHBoxLayout;
	layoutQrc->addWidget( m_qrcLineEdit, 1 );
	layoutQrc->addWidget( btnQrc, 0 );

	QPushButton* btnOk = new QPushButton( this );
	btnOk->setText( tr( "OK" ) );
	connect( btnOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
	QPushButton* btnCancel = new QPushButton( this );
	btnCancel->setText( tr( "Cancel" ) );
	connect( btnCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	QHBoxLayout* layoutDefault = new QHBoxLayout;
	layoutDefault->addWidget( btnOk, 0, Qt::AlignLeft );
	layoutDefault->addWidget( btnCancel, 0, Qt::AlignRight );

	QVBoxLayout* layoutMain = new QVBoxLayout;
	layoutMain->addLayout( layoutQrc );
	layoutMain->setAlignment( layoutQrc, Qt::AlignTop );
	layoutMain->addLayout( layoutDefault );
	layoutMain->setAlignment( layoutDefault, Qt::AlignBottom );
	setLayout( layoutMain );
}


// -----------------------------------------------------
// Override QDialog
// -----------------------------------------------------
void ReSettingsWidget::paintEvent( QPaintEvent* _event )
{
	TSuper::paintEvent( _event );
}


// -----------------------------------------------------
// Settings
// -----------------------------------------------------
void ReSettingsWidget::BrowseForQrc()
{
	QString qrc = QFileDialog::getOpenFileName( this, tr( "Find Qrc File" ), tr( "." ), tr( "Resource File *.qrc" ) );
	if( !qrc.isNull() )
	{
		m_qrcLineEdit->clear();
		m_qrcLineEdit->insert( qrc );
	}
}


// -----------------------------------------------------
// Utility
// -----------------------------------------------------


}	// namespace RE
