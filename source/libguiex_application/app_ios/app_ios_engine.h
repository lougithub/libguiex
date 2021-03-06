/*
 *  ApplicationEngine.h
 *  libguiex
 *
 *  Created by Lou Guo Liang on 11/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef _APPLICATIONENGINE_20101110_H__
#define _APPLICATIONENGINE_20101110_H__

enum EDeviceOrientation {
    eDeviceOrientation_Unknown,
    eDeviceOrientation_Portrait,
    eDeviceOrientation_PortraitUpsideDown,
    eDeviceOrientation_LandscapeLeft,
    eDeviceOrientation_LandscapeRight,
    eDeviceOrientation_FaceUp,
    eDeviceOrientation_FaceDown,
};


struct IApplicationEngine
{
	virtual void Initialize( int width, int height, const char* szDataPath ) = 0;
	virtual void Update( float deltaTime ) = 0;
	virtual void OnRotate( EDeviceOrientation newOrientation ) = 0;
	virtual void OnFingerUp(float x, float y) = 0;
    virtual void OnFingerDown(float x,float y) = 0;
    virtual void OnFingerMove(float oldx, float oldy, float x, float y) = 0;	
};

IApplicationEngine* CreateApplicationEngine();
void DestroyApplicationEngine( IApplicationEngine* pEngine );

#endif //_APPLICATIONENGINE_20101110_H__
