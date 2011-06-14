//
//  EAGLView.m
//  OpenGLES_iPhone
//
//  Created by mmalc Crawford on 11/18/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import <GameKit/GameKit.h>

#import "EAGLView.h"
#import "Renderer.h"

@implementation EAGLView

@synthesize renderer;

// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

//The EAGL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:.
- (id)initWithCoder:(NSCoder*)coder
{
    self = [super initWithCoder:coder];
    if (self) {
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                        nil];
    }
    
    return self;
}

- (void)dealloc {
    [renderer release];    
    [super dealloc];
}

- (void) setRenderer:(Renderer *)rend {
    renderer = rend;
    [renderer setLayer:(CAEAGLLayer*)self.layer];
}

- (void)layoutSubviews {
    [renderer setLayer:(CAEAGLLayer*)self.layer];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  UITouch *touch = [touches anyObject];
  if ([touch tapCount] == 2) {
    [[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:^(NSError *error){
      NSLog(@"Authenticate GKLocalPlayer with error: %@", error);
    }];
  }
}

@end
