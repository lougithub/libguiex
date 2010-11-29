// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
#ifndef _RE_SETTINGS_WIDGET_H_
#define _RE_SETTINGS_WIDGET_H_

#include <QDialog>
#include <QLineEdit>

namespace RE
{


class ReSettingsWidget : public QDialog
{
	Q_OBJECT

	typedef QDialog		TSuper;


	// -----------------------------------------------------
	// General
	// -----------------------------------------------------
public:
	ReSettingsWidget( QWidget* _parent = NULL );

	QString				GetQrcFilename() const	{ return m_qrcLineEdit->text(); }

	// -----------------------------------------------------
	// Override QDialog
	// -----------------------------------------------------
protected:
	void				paintEvent( QPaintEvent* _event );

	// -----------------------------------------------------
	// Settings
	// -----------------------------------------------------
public slots:
	void				BrowseForQrc();

	// -----------------------------------------------------
	// Utility
	// -----------------------------------------------------
protected:

	// -----------------------------------------------------
	// Variable
	// -----------------------------------------------------
protected:
	QLineEdit*			m_qrcLineEdit;
};


}		// namespace RE
#endif	// _RE_SETTINGS_WIDGET_H_
