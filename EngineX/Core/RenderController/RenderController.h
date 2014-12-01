//
//  RenderController.h
//  EngineX
//
//  Created by Maxim Reshetey on 30.11.12.
//  Copyright (c) 2012 Maxim Reshetey. All rights reserved.
//

#ifndef __EngineX__RenderController__
#define __EngineX__RenderController__

#include "EXScene.h"

// TODO: Should this really be a singleton?
// Singleton
class RenderController // : public b2ContactListener
{
private:
	RenderController() : _activeScene(0), _paused(false)
	{
//		World::init(0.0f, 0.0f);
//		World::ref().SetContactListener(this);
	};
	~RenderController()
	{
		info("Render Controller deleted");
	};
	RenderController(const RenderController &) = delete; // Not implemented
	RenderController & operator=(const RenderController &) = delete; // Not implemented

public: // Methods
	inline static RenderController & ref()
	{
		static RenderController inst; // The inst is really deleted only upon app termination
		return inst;
	}

	void initWithBundlePath(const char * bundlePath)
	{
		assert(bundlePath);

//		_bundlePath = bundlePath;

		MStack::init();

		// Shaders
		Shader::setBundlePath(bundlePath);

		// TODO: Perhaps these should be moved to say Shader::createDefaultSet or like
		Shader::createWithNameAndAttributeCount("Shader", 1).finalizeWithTextureUnitCount();

		// GL initial setup
		glClearColor(0.65f, 0.65f, 0.35f, 1.0f);

//		glLineWidth(8.0f);

		say("=== GL diagnostics ===\n");
#if GLKIT
		say("GLKit: YES\n");
#else
		say("GLKit: NO\n");
#endif
		//	say("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
		//	say("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
		//	say("GL_VERSION: %s\n", glGetString(GL_VERSION));
		//	say("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		//	say("GL_EXTENSIONS:\n %s\n", glGetString(GL_EXTENSIONS));
		say("======================\n");

		info("GL is initialized with error code: %i\n", glGetError());
	}

	void setScene(EXScene * scene) { _activeScene = scene; }
	void setPaused(bool paused) { _paused = paused; }

//	inline void addModel(Model * model) { std::unique_ptr<Model> p(model); _models.push_back(std::move(p)); }
//	inline void removeModel(Model * model) { std::unique_ptr<Model> p(model); _models.remove(std::move(p)); }
//	inline void addModel(Model * model)
//	{
//		if (_activeScene)
//			_activeScene->addModel(model);
//	}
//	inline void removeModel(Model * model) { _models.remove(model); }

	inline void drawScene()
	{
		if (!_paused && _activeScene)
			_activeScene->draw();
	}

	inline void updateScene()
	{
		if (!_paused && _activeScene)
			_activeScene->update();
	}

	void viewDidRotate(EngineX::eDeviceOrientation orientation)
	{
		auto sceneWidth = EngineX::kSceneWidth, sceneHeight = EngineX::kSceneHeight;
		if (orientation == EngineX::eOrientationLandscape)
		{
			sceneWidth = EngineX::kSceneHeight;
			sceneHeight = EngineX::kSceneWidth;
		}

		// Proper RH system (yes, near is negative)
		MStack::ref().setOrtho(-sceneWidth*0.5f, sceneWidth*0.5f,
							   -sceneHeight*0.5f, sceneHeight*0.5f,
							   -EngineX::kSceneDepth*0.5f, EngineX::kSceneDepth*0.5f);
	}

	static void deinit()
	{
		auto & myself = RenderController::ref();

		if (myself._activeScene)
			delete myself._activeScene;
		myself._activeScene = 0;

		Shader::deinit();
		MStack::deinit();
	}

private: // Vars
	// TODO: Temporary solution
	std::string _bundlePath;

	EXScene * _activeScene; // Currently drawn scene
	bool _paused;
};

#endif /* defined(__EngineX__RenderController__) */
