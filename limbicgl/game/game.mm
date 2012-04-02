/*******************************************************************************
    Copyright (c) 2011, Limbic Software, Inc.
    All rights reserved.
    This code is subject to the Google C++ Coding conventions:
         http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
 ******************************************************************************/
#include <limbicgl/game/game.h>

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#import "TextureManager.h"

#include <limbicgl/config.h>

/*******************************************************************************
  A simple GPU program
 ******************************************************************************/
#include <performancemonitor/glprogram.h>

#define ARC4RANDOM_MAX (0x100000000ull)
static double randomDouble() {
  return double(arc4random())/double(ARC4RANDOM_MAX);
}

Game::Game()
    : Drawable3D(this) ,
      gfx_initialized_(false),
      time_(0.0),
      paused_(false),
      last_update_(0.0){
}

Game::~Game() {
}

void Game::Update() {
  // To simulate a game update, sleep up to 5 ms
  double delay = 0.005 * randomDouble();
  [NSThread sleepForTimeInterval:delay];
  // Update the time
  double time = CFAbsoluteTimeGetCurrent();
  if (paused_) {
    last_update_ = time;
    return;
  }
  if (last_update_ > 0.0) {
    double delta = time - last_update_;
    time_ += delta;
  }
  last_update_ = time;
}

void Game::Draw() {
  if (gfx_initialized_ == false) {
    InitGFX();
  }

  drawInternal();
}

void Game::InitGFX() {
  if (gfx_initialized_ == true) return;
  gfx_initialized_ = true;
    
    
}

void Game::Pause() {
  paused_ = true;
}

void Game::Resume() {
  paused_ = false;
}
