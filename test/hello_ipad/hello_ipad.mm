//
//  hello_ipad.m
//  libguiex
//
//  Created by Lou Guo Liang on 11/9/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "hello_ipad.h"
#import <UIKit/UIKit.h>
#import <OpenGLES/ES1/glext.h>

@implementation AppDelegate


#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
    // Override point for customization after application launch.
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
	
    [m_window = [UIWindow alloc] initWithFrame:screenBounds];
	m_view = [[GLView alloc] initWithFrame: screenBounds];
	
	[m_window addSubview:m_view];
	[m_window makeKeyAndVisible];
	
	[application setStatusBarHidden: YES withAnimation: UIStatusBarAnimationNone];	
    
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive.
     */
}


- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
	[m_view release];
    [m_window release];
    [super dealloc];
}

@end






@implementation GLView

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (id) initWithFrame: (CGRect) frame
{
    if (self = [super initWithFrame:frame])
    {
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*) self.layer;
        eaglLayer.opaque = YES;
		
        m_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        
        if (!m_context || ![EAGLContext setCurrentContext:m_context]) 
		{
            [self release];
            return nil;
        }
		
		m_pEngine = CreateApplicationEngine();
		
        int width = CGRectGetWidth(frame);
        int height = CGRectGetHeight(frame);
		NSString* bundlePath =[[NSBundle mainBundle] resourcePath];
        m_pEngine->Initialize(width, height, [bundlePath UTF8String]);
		[m_context 
		 renderbufferStorage:GL_RENDERBUFFER_OES
		 fromDrawable:eaglLayer];
		
        [self drawView: nil];
        m_timestamp = CACurrentMediaTime();
        
        CADisplayLink* displayLink;
        displayLink = [CADisplayLink displayLinkWithTarget:self
									selector:@selector(drawView:)];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop]
						  forMode:NSDefaultRunLoopMode];
		
		[[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(didRotate:)
		 name:UIDeviceOrientationDidChangeNotification
		 object:nil];	
    }
    return self;
}

- (void) drawView: (CADisplayLink*) displayLink
{
	float elapsedSeconds = 0.0f;
    if (displayLink != nil) 
	{
		elapsedSeconds = displayLink.timestamp - m_timestamp;
		m_timestamp = displayLink.timestamp;
    }
    
	m_pEngine->Update( elapsedSeconds );
	
	[m_context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

- (void) touchesBegan: (NSSet*) touches withEvent: (UIEvent*) event
{
    UITouch* touch = [touches anyObject];
    CGPoint location  = [touch locationInView: self];
    m_pEngine->OnFingerDown(location.x, location.y);
}

- (void) touchesEnded: (NSSet*) touches withEvent: (UIEvent*) event
{
    UITouch* touch = [touches anyObject];
    CGPoint location  = [touch locationInView: self];
    m_pEngine->OnFingerUp(location.x, location.y);
}

- (void) touchesMoved: (NSSet*) touches withEvent: (UIEvent*) event
{
    UITouch* touch = [touches anyObject];
    CGPoint previous  = [touch previousLocationInView: self];
    CGPoint current = [touch locationInView: self];
    m_pEngine->OnFingerMove(previous.x, previous.y,
							current.x, current.y);
}

- (void) didRotate:(NSNotification *)notification
{
	UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
	
	m_pEngine->OnRotate( DeviceOrientation(orientation) );
	
	[self drawView:nil];
}
- (void)dealloc {
	if( [EAGLContext currentContext] == m_context )
	{
		[EAGLContext setCurrentContext:nil];
	}
	
	[m_context release];
	
    [super dealloc];
}

@end





//
// main entry point. Like any c or c++ program, the "main" is the entry point
//
int main(int argc, char *argv[]) {
	// it is safe to leave these lines as they are.
	NSAutoreleasePool *pool = [NSAutoreleasePool new];
	UIApplicationMain(argc, argv, nil, @"AppDelegate");
	[pool release];
	return 0;
}
