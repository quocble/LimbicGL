/*
 *  Tweeners.cpp
 *  Chess3D
 *
 *  Created by Quoc Le on 4/17/11.
 *  Copyright 2011 VisionaryBits. All rights reserved.
 *
 */
/*
 if ([_transition isEqualToString:TWEEN_FUNC_LINEAR]) {
 return progression;
 
 } else if ([_transition isEqualToString:TWEEN_FUNC_EASEINSINE]) {
 return 1.0 - cos(progression * M_PI / 2.0);
 
 } else if ([_transition isEqualToString:TWEEN_FUNC_EASEOUTSINE]) {
 return sin(progression * M_PI / 2.0);
 
 } else if ([_transition isEqualToString:TWEEN_FUNC_EASEINOUTSINE]) {
 return 0.5 * (1.0 + sin(1.5 * M_PI + progression * M_PI));
 
 } else if ([_transition isEqualToString:TWEEN_FUNC_EASEOUTBOUNCE]) {
 return [self easeOutBounce:progression];
 
 } else if ([_transition isEqualToString:TWEEN_FUNC_EASEINBOUNCE]) {
 return 1.0 - [self easeOutBounce:1.0 - progression];
 
 } else if ([_transition isEqualToString:TWEEN_FUNC_EASEOUTTHROW]) {
 return [self easeOutThrow:progression];
 
 } else if ([_transition isEqualToString:TWEEN_FUNC_EASEINTHROW]) {
 return 1.0 - [self easeOutThrow:1.0 - progression];
 
 } 
 */
#include "Tweeners.h"
#include <math.h>

float Tweeners::tween(Tween_Func type, float step)
{
	switch(type)
	{
		case TWEEN_FUNC_LINEAR: 
		{
			return step;
		}
			break;
		case TWEEN_FUNC_EASEINSINE: 
		{
			return fmin(1.0, (1.0 - cos(step * M_PI / 2.0)));
		}
			break;
		case TWEEN_FUNC_EASEOUTSINE:
		{
			return fmin(1.0, (sin(step * M_PI / 2.0)));
		}
			break;
		case TWEEN_FUNC_EASEINOUTSINE: 
		{
			return fmin(1.0, (0.5 * (1.0 + sin(1.5 * M_PI + step * M_PI))));
		}
			break;
		case TWEEN_FUNC_EASEOUTBOUNCE: break;
		case TWEEN_FUNC_EASEINBOUNCE: break;
		case TWEEN_FUNC_EASEOUTTHROW: break;
		case TWEEN_FUNC_EASEINTHROW: break;
	}
	
	return step;
}
