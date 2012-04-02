/*******************************************************************************
    Copyright (c) 2011, Limbic Software, Inc.
    All rights reserved.
    This code is subject to the Google C++ Coding conventions:
         http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
 ******************************************************************************/
#ifndef LIMBICGL_GAME_GAME_H_
#define LIMBICGL_GAME_GAME_H_

#include <performancemonitor/codingguides.h>
#import "Drawable3D.h"

class Game : Drawable3D {
 public:
  Game();
  ~Game();
  void Update();
  void Draw();
  void InitGFX();
  void Pause();
  void Resume();
    
 private:
  double time_;
  double last_update_;
  bool paused_;
  bool gfx_initialized_;

    DISALLOW_COPY_AND_ASSIGN(Game);
};

#endif  // LIMBICGL_GAME_GAME_H_
