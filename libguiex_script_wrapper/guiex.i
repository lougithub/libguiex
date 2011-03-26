%module guiex
%module lib1

%{
#include "libguiex_core/guiex.h"
#include "libguiex_widget/guiwgt.h"
#include "libguiex_widget_box2d/guiwgt_box2d.h"
#include "libguiex_widget_game/guiwgt_game.h"
%}

%include "std_string.i"

%include 	"guitypes.i" 
%include	"guistring.i"
%include	"guiexception.i"
%include	"guicolor.i"
%include	"guisize.i"
%include	"guivector2.i"
%include	"guivector3.i"
%include	"guirect.i"
%include	"guicolorrect.i"
%include	"guitimer.i"
%include	"guilog.i"
%include	"guievent.i"
%include	"guiproperty.i"
%include	"guias.i"
%include	"guiimage.i"
%include	"guianimation.i"
%include	"guiinterfacerender.i"
%include	"guiinterfacesound.i"
%include	"guiinterfacecommand.i"
%include	"guiinterfacekeyboard.i"
%include	"guiinterfacemanager.i"
%include	"guiwidget.i"
%include	"guiwidget_box2d.i"
%include	"guiwidgetutility.i"
%include	"guiwidgetfactory.i"
%include	"guiuicanvaslayer.i"
%include	"guisystem.i"




