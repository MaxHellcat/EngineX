//
//  common.h
//  EngineX
//
//  Created by Maxim Reshetey on 29.11.12.
//  Copyright (c) 2012 Maxim Reshetey. All rights reserved.
//

#ifndef EngineX_common_h
#define EngineX_common_h

#include "stdio.h"

namespace EngineX
{
	typedef
	struct
	{
		float x, y, z;
	} Point;

	typedef
	struct
	{
		float r, g, b, a;
	} ColorRGBA;

	typedef
	enum
	{
		eOrientationPortrait = 0,
		eOrientationLandscape = 1
	} eDeviceOrientation;

	static const float kSceneHeight = 6.0f;
	static const float kSceneWidth = 4.0f;
	static const float kSceneDepth = 1.0f;

	static const float kSceneLeft = -kSceneWidth/2.0f;
	static const float kSceneRight = kSceneWidth/2.0f;

	static const float kSceneBottom = -kSceneHeight/2.0f;
	static const float kSceneTop = kSceneHeight/2.0f;
}

#if defined(DEBUG)

#define say(...) printf(__VA_ARGS__)
#define info(...) printf("INFO: " __VA_ARGS__)
#define warning(...) printf("WARNING: " __VA_ARGS__)
#define err(...) printf("ERROR: " __VA_ARGS__)

#else

#define say(...)
#define info(...)
#define warning(...)
#define err(...)

#endif

#endif /* defined(EngineX_common_h) */
