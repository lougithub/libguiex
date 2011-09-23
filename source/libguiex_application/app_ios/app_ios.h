//
//  hello_ipad.h
//  libguiex
//
//  Created by Lou Guo Liang on 11/9/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

#import "common_ios_engine.h"

@interface GLView : UIView {
@private
    IApplicationEngine* m_pEngine;
    EAGLContext* m_context;
    float m_timestamp;
}

- (void) drawView: (CADisplayLink*) displayLink;
- (void) didRotate: (NSNotification*) notification;
@end


@interface AppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *m_window;
	GLView *m_view;
}
@end
