//
//  ThreadedDriver.h
//  limbicgl
//
//  Created by Volker Schoenefeld on 6/15/11.
//  Copyright 2011 Limbic Software, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Driver.h"

class Game;
@class EAGLContext;

@interface ThreadedDriver : NSObject<Driver> {
@private
  EAGLContext *context_;
  bool animating_;
  NSThread *renderthread_;
  RenderTarget *rendertarget_;
  NSRunLoop *renderloop_;
  Game *game_;
}

- (id) initWithRenderTarget:(RenderTarget*)renderTarget andGame:(Game*)game;
- (void) startAnimation;
- (void) stopAnimation;
- (void) teardown;
- (void) setLayer:(CAEAGLLayer*)layer;

@end
