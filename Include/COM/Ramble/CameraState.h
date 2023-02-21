#ifndef __CAMERA_STATE_H__
#define __CAMERA_STATE_H__

enum MotionType
{
	NONE                = 0,
	FORWARD             = 1<<0, 
	BACKWARD            = 1<<1,
	UP                  = 1<<2,
	DOWN                = 1<<3,
	LEFT_SHIFT          = 1<<4,
	RIGHT_SHIFT         = 1<<5,
	LEFT_TURN           = 1<<6,
	RIGHT_TURN          = 1<<7,
	HOME                = 1<<8,
	END                 = 1<<9
};

#endif
