/** 
 * @file guiex.h
 * @brief header file of this system.
 * @author ken
 * @date 2006-03-23
 */

#ifndef __GUI_EX_20060323_H__
#define __GUI_EX_20060323_H__

//============================================================================//
// include
//============================================================================// 

#include "guiconfig.h"
#include "guitypes.h"

//common
#include "guicleanup.h"
#include "guiobjectmanagerbase.h"
#include "guiobjectmanager.h"

//math
#include "guimath.h"
#include "guimatrix3.h"
#include "guimatrix4.h"
#include "guiplane.h"
#include "guiquaternion.h"
#include "guicolor.h"
#include "guirect.h"
#include "guicolorrect.h"
#include "guivector2.h"
#include "guivector3.h"
#include "guivector4.h"

//string
#include "guistring.h"
#include "guistringex.h"
#include "guistringconvertor.h"

//log

#include "guilogmsg.h"
#include "guilogmsgmanager.h"
#include "guilogmsgcallback.h"
#include "guilogmsgrecord.h"

//widget
#include "guiwidget.h"
#include "guiwidgetgenerator.h"
#include "guiwidgetfactory.h"
#include "guisystem.h"
#include "guiwidgetutility.h"
#include "guiwidgetmanager.h"

//property
#include "guiproperty.h"
#include "guipropertyconvertor.h"
#include "guipropertymanager.h"

//render
#include "guitexturemanager.h"
#include "guitexture.h"
#include "guitextureimp.h"

//resource
#include "guiresource.h"
#include "guiresourcemanager.h"
#include "guiimage.h"
#include "guiimagemanager.h"
#include "guifontmanager.h"
#include "guianimation.h"
#include "guianimationmanager.h"
#include "guisoundmanager.h"

#include "guiconfigfileloader.h"


//mouse
#include "guimousecursor.h"

//interface
#include "guiinterfacemanager.h"
#include "guiinterfacerender.h"
#include "guiinterfacemouse.h"
#include "guiinterfacekeyboard.h"
#include "guiinterfacesound.h"
#include "guiinterfaceIme.h"
#include "guiinterfacefont.h"
#include "guiinterfacecommand.h"
#include "guiinterfacescript.h"
#include "guiinterfacefilesys.h"
#include "guiinterfaceimageloader.h"

//event
#include "guievent.h"

//as
#include "guiasLite.h"
#include "guias.h"
#include "guiasmanager.h"

//particle 2d
#include "guiparticle2dsystemquad.h"
#include "guiparticle2dmanager.h"

//sound
#include "guisounddata.h"

//exception
#include "guiexception.h"

//scene info
#include "guiscene.h"
#include "guiscenemanager.h"
#include "guisceneUtility.h"

//canvaslayer
#include "guicanvaslayer.h"
#include "guicanvaslayermanager.h"

//camera
#include "guicamera.h"
#include "guicameramanager.h"

#include "guiUtility.h"
#include "guiinterpolation.h"

#include "sigslot.h"

#endif //__GUI_EX_20060323_H__

