//
//  EXScene.h
//  EngineX
//
//  Created by Maxim Reshetey on 21.03.13.
//  Copyright (c) 2013 Maxim Reshetey. All rights reserved.
//

#ifndef __EngineX__EXScene__
#define __EngineX__EXScene__

#include "Model.h"
#include <vector>

class EXScene
{
	friend class RenderController;

public: // Ctor&dtor
	EXScene() {};
	virtual ~EXScene()
	{
		for (Model * model : _models)
		{
			if (model)
				delete model;
			model = 0;
		}

		_models.clear();
	};

public: // Methods
	virtual void update() = 0; // Overriden by descendants

	void addModel(Model * model) { _models.push_back(model); }


private: // Methods
	inline void draw()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		MStack::ref().loadIdentity();
		MStack::ref().setOrtho(-2.0f, 2.0f, -3.0f, 3.0f, -5.0f, 5.0f); // Proper RH system (yes, near is negative)

		for (Model * model : _models)
		{
			model->draw();
		}
	}

private: // Vars
	// TODO: Keep objects, not refs?
	std::vector<Model *> _models; // Continuous memory and instant lookup, swap'n'pop for remove

};

#endif /* defined(__EngineX__EXScene__) */
