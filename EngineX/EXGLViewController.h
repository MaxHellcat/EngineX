//
//  EXViewController.h
//  EngineX
//
//  Created by Maxim Reshetey on 18.03.13.
//  Copyright (c) 2013 Maxim Reshetey. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

class EXScene;

// Main class to initialize GL and provide interface for drawing and updating
// Sub-class GLKViewController to directly update in -(void)update, instead of delegating
@interface EXGLViewController : GLKViewController

- (void)setScene:(EXScene *)scene;

@end

// Sub-class EXGLView to directly draw in -(void)drawRect:(CGRect)rect, instead of delegating
@interface EXGLView : GLKView

@end
