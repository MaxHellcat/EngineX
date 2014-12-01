//
//  GameScene.cpp
//  EngineX
//
//  Created by Maxim Reshetey on 21.03.13.
//  Copyright (c) 2013 Maxim Reshetey. All rights reserved.
//

#include "GameScene.h"

GameScene::GameScene()
{
	_shape1 = &Shape::createRect(2.0f).setColor(0.95f, 0.95f, 0.0f);
	addModel(_shape1);

	_shape2 = &Shape::createTriangle(2.0f).setColor(0.2f, 0.95f, 0.0f);
	addModel(_shape2);

	_shape3 = &Shape::createCircle(0.5f).setColor(0.0f, 0.0f, 0.65f);
	addModel(_shape3);

}

void GameScene::update()
{
	static float rad = 0.0f; rad += 0.01f;

	_shape1->setAngle(rad);
	_shape2->setAngle(-rad*3.0f);
//	_shape3->setAngle(-rad*0.5f);


	static float move = 0.0f; move += 0.03f;
	_shape1->setPosY(sinf(move));
	_shape2->setPosY(-sinf(move));
	_shape3->setPosX(sinf(move));

//	_shape1->setColor(move, sinf(move), move);
}
