%module guiex

%{
#include "libguiex_core/guiex.h"
#include "libguiex_widget/guiwgt.h"
%}

%include "std_string.i"

%include 	"guitypes.i" 
%include	"guistring.i"
%include	"guicolor.i"
%include	"guisize.i"
%include	"guivector2.i"
%include	"guivector3.i"
%include	"guirect.i"
 /*
%include	"guicolorrect.i"
%include	"guiexception.i"
%include	"guitimer.i"
%include	"guilog.i"
%include	"guievent.i"
%include	"guias.i"
%include	"guiimage.i"
%include	"guiinterfacemanager.i"
%include	"guiinterfacesound.i"
%include	"guiinterfacecommand.i"
%include	"guiinterfacekeyboard.i"
%include	"guiwidget.i"
%include	"guiwidgetsystem.i"
%include	"guiwidgetutility.i"
%include	"guiwidgetfactory.i"
*/



