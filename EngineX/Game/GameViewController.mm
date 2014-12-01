//
//  GameViewController.m
//  EngineX
//
//  Created by Maxim Reshetey on 22.03.13.
//  Copyright (c) 2013 Maxim Reshetey. All rights reserved.
//

#import "GameViewController.h"

#include "EngineX.h"
#include "GameScene.h"

@implementation GameViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.

}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.

}

- (IBAction)startGame:(id)sender
{
	// TODO: I think this should accept basic GL parameters, e.g. color depth, framerate, etc.
	EXGLViewController * viewController = [[EXGLViewController alloc] init];

	EXScene * scene = new GameScene();
	[viewController setScene:scene];

	[self.navigationController pushViewController:viewController animated:YES];
}

@end
