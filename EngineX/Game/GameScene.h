//
//  GameScene.h
//  EngineX
//
//  Created by Maxim Reshetey on 21.03.13.
//  Copyright (c) 2013 Maxim Reshetey. All rights reserved.
//

#ifndef __EngineX__GameScene__
#define __EngineX__GameScene__

#include "EngineX.h"

class GameScene : public EXScene
{
public:
	GameScene();
	virtual ~GameScene() {}

private:
	virtual void update() override;

	Shape * _shape1, * _shape2, * _shape3;
};

#endif /* defined(__EngineX__GameScene__) */
