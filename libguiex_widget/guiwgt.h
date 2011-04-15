/** 
 * @file guiwgt.h
 * @brief include file for all widget in this module
 * @author ken
 * @date 2006-09-20
 */

#ifndef __GUI_WIDGET_INCLUDE_20060920_H__
#define __GUI_WIDGET_INCLUDE_20060920_H__

//============================================================================//
// include
//============================================================================// 
#if defined(GUIEX_PLATFORM_WIN32)
#	include "guiwgttag.h"
#	include "guiwgtframe.h"
#	include "guiwgtbutton.h"
#	include "guiwgtradiobutton.h"
#	include "guiwgtcheckbutton.h"
#	include "guiwgtframe.h"
#	include "guiwgtstatictext.h"
#	include "guiwgtstaticimage.h"
#	include "guiwgtpanel.h"
#	include "guiwgteditbox.h"
#	include "guiwgtmultieditbox.h"
#	include "guiwgtscrollbar.h"
#	include "guiwgtscrollpanel.h"
#	include "guiwgtdialog.h"
#	include "guiwgtemptynode.h"
#	include "guiwgtanimation.h"
#	include "guilistboxtextitem.h"
#	include "guimenutextitem.h"
#	include "guiwgtlistbox.h"
#	include "guiwgtcombobox.h"
#	include "guiwgtprogress.h"
#	include "guiwgtscrollbarcontainer.h"
#	include "guiwgttabcontrol.h"
#	include "guiwgttabbutton.h"
#	include "guiwgtpopupmenu.h"
#	include "guiwgtcolumnlist.h"
#	include "guicollistheader.h"
#	include "guicollistheadersegment.h"
#	include "guicollisttextitem.h"
#elif defined(GUIEX_PLATFORM_MAC)
#	include "guiwgttag.h"
#	include "guiwgtscrollbar.h"
#	include "guiwgtscrollbarcontainer.h"
#	include "guiwgteditbox.h"
#	include "guiwgtmultieditbox.h"
#	include "guiwgtstaticimage.h"
#	include "guiwgtstatictext.h"
#	include "guiwgtpanel.h"
#	include "guiwgtbutton.h"
#	include "guiwgtcheckbutton.h"
#	include "guiwgtradiobutton.h"
#	include "guiwgtemptynode.h"
#else
#	error "unknown platform"	
#endif


#include "guiwgtgeneratewidgets.h"


#endif //__GUI_WIDGET_INCLUDE_20060920_H__
