/*
 *  Tweeners.h
 *  Chess3D
 *
 *  Created by Quoc Le on 4/17/11.
 *  Copyright 2011 VisionaryBits. All rights reserved.
 *
 */


typedef enum
{
	TWEEN_FUNC_LINEAR,
	TWEEN_FUNC_EASEINSINE,
	TWEEN_FUNC_EASEOUTSINE,
	TWEEN_FUNC_EASEINOUTSINE,
	TWEEN_FUNC_EASEOUTBOUNCE,
	TWEEN_FUNC_EASEINBOUNCE,
	TWEEN_FUNC_EASEOUTTHROW,
	TWEEN_FUNC_EASEINTHROW	
} Tween_Func;
class Tweeners
{
public:
	static float tween(Tween_Func type, float step);
};