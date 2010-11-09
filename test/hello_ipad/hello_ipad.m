//
//  hello_ipad.m
//  libguiex
//
//  Created by Lou Guo Liang on 11/9/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "hello_ipad.h"
#import <UIKit/UIKit.h>
//#import "GLView.h"

@implementation AppDelegate


#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
    // Override point for customization after application launch.
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
	
    [m_window = [UIWindow alloc] initWithFrame:screenBounds];
	//m_view = [[GLView alloc] initWithFrame: screenBounds];
	
	//[m_window addSubview:m_view];
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
	//[m_view release];
    [m_window release];
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
