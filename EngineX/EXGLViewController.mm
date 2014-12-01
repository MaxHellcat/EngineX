//
//  EXViewController.m
//  EngineX
//
//  Created by Maxim Reshetey on 18.03.13.
//  Copyright (c) 2013 Maxim Reshetey. All rights reserved.
//

#import "EXGLViewController.h"

#include "RenderController.h"
#include "GameScene.h"

@interface EXGLViewController ()
{
	EXScene * _activeScene;
}

@property (strong, nonatomic) EAGLContext * context;

@end

@implementation EXGLViewController

- (id)init
{
	self = [super init];

	if (self)
	{
		EXGLView * view = (EXGLView *)self.view; // Force load view upon initialization
	}

	return self;
}

- (void)loadView
{
	[super loadView];

	EXGLView * view = [[EXGLView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];
	self.view = view;

	// Here we're sure the view is loaded, continue setup GL context
	self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	if (!self.context)
	{
		NSLog(@"Failed to create ES context");
	}

	view.context = self.context;

	view.drawableColorFormat = GLKViewDrawableColorFormatRGB565;
	view.drawableDepthFormat = GLKViewDrawableDepthFormatNone;
	view.drawableStencilFormat = GLKViewDrawableStencilFormatNone;
	view.drawableMultisample = GLKViewDrawableMultisample4X;
	self.preferredFramesPerSecond = 60;

//	self.pauseOnWillResignActive = NO;

	[EAGLContext setCurrentContext:self.context];

	// Start the render controller
	RenderController::ref().initWithBundlePath([[[NSBundle mainBundle] bundlePath] cStringUsingEncoding:NSUTF8StringEncoding]);
}

- (void)viewWillAppear:(BOOL)animated
{
	[super viewWillAppear:animated];

//	self.paused = YES; // Use to pause drawing upon start
}

- (void)dealloc
{
	[EAGLContext setCurrentContext:self.context];

	// Complete EngineX termination - release active scene, models, shaders, de-init matrix stack.
	// Note that all singletons (Render Controller, Matrix Stack) will still be in memory since
	// they are allocated statically on stack. As a result of this, they are just de-initialized
	RenderController::deinit();

	if ([EAGLContext currentContext] == self.context)
	{
		[EAGLContext setCurrentContext:nil];
	}
	self.context = nil;

	NSLog(@"EXGLViewController deallocated");
}

// TODO: Maintain properly
- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning];

    if ([self isViewLoaded] && (self.view.window == nil))
	{
		self.view = nil;

		[EAGLContext setCurrentContext:self.context];

		if ([EAGLContext currentContext] == self.context)
		{
			[EAGLContext setCurrentContext:nil];
		}
		self.context = nil;
	}

	// Dispose of any resources that can be recreated.
}

- (void)setScene:(EXScene *)scene
{
	RenderController::ref().setScene(scene);
}

// This method is used to update GL scene, called N times per second
- (void)update
{
	RenderController::ref().updateScene();
}

@end

#pragma mark - EXGLView implemetation
#pragma mark
@implementation EXGLView

// This method is used to draw GL scene, called along with - (void)update
- (void)drawRect:(CGRect)rect
{
	RenderController::ref().drawScene();
}

@end
