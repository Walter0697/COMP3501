#include <iostream>
#include <time.h>
#include <sstream>

#include "game.h"
#include "bin/path_config.h"

// HANDLING ROCKETSSSSSS ?????????
// HANDLING EVRYTHING BEING A CHILD OF CAMERA ????????????
// ASK ABOUT CAMERA NODE ?????????

// SHOOT ROCKET WITH C
// W,A,S,D for movement
// Right click to move from first to third person (Not implemented!!!!)
namespace game 
{
	// Some configuration constants
	// They are written here as global variables, but ideally they should be loaded from a configuration file

	// Main window settings
	const std::string window_title_g = "Demo";
	const unsigned int window_width_g = 800;
	const unsigned int window_height_g = 600;
	const bool window_full_screen_g = false;

	// Viewport and camera settings
	float camera_near_clip_distance_g = 0.01;
	float camera_far_clip_distance_g = 1000.0;
	float camera_fov_g = 20.0; // Field-of-view of camera
	const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);
	glm::vec3 camera_position_g(0.0, 0.0, 10.0);
	glm::vec3 camera_look_at_g(0.0, 0.0, 0.0);
	glm::vec3 camera_up_g(0.0, 1.0, 0.0);

	// Materials 
	const std::string material_directory_g = MATERIAL_DIRECTORY;

	Game::Game(void)
	{
		//dummy player to compile maybe need to deallocate it 
		player = new Fly("",0,0,0);
	}

	Game::~Game() { glfwTerminate(); }

	void Game::Init(void) 
	{
		// Run all initialization steps
		InitWindow();
		InitView();
		InitEventHandlers();

		// Set variables
		animating_ = true;
	}


	void Game::InitWindow(void) 
	{
		// Initialize the window management library (GLFW)
		if (!glfwInit()) {
			throw(GameException(std::string("Could not initialize the GLFW library")));
		}

		// Create a window and its OpenGL context
		if (window_full_screen_g) {
			window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), glfwGetPrimaryMonitor(), NULL);
		}
		else {
			window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), NULL, NULL);
		}
		if (!window_) {
			glfwTerminate();
			throw(GameException(std::string("Could not create window")));
		}

		// Make the window's context the current one
		glfwMakeContextCurrent(window_);

		// Initialize the GLEW library to access OpenGL extensions
		// Need to do it after initializing an OpenGL context
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			throw(GameException(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
		}
	}

	void Game::InitView(void)
	{
		// Set up z-buffer
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Set viewport
		int width, height;
		glfwGetFramebufferSize(window_, &width, &height);
		glViewport(0, 0, width, height);

		// Set up camera
		// Set current view
		camera_.SetView(camera_position_g, camera_look_at_g, camera_up_g);
		// Set projection
		camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);

		//setup camera as a sceneNode
		camNode = new CameraNode(&camera_);
		scene_.SetRoot(camNode);
	}

	void Game::InitEventHandlers(void)
	{
		// Set event callbacks
		glfwSetKeyCallback(window_, KeyCallback);
		glfwSetFramebufferSizeCallback(window_, ResizeCallback);

		// Set pointer to game object, so that callbacks can access it
		glfwSetWindowUserPointer(window_, (void *) this);
	}

	void Game::SetupResources(void)
	{
		/* Create Geometries */
		resman_.CreateCylinder("RocketMesh");
		resman_.CreateWall("WallMesh");
		resman_.CreateSphere("SimpleSphereMesh");
		resman_.CreateCylinder("TargetMesh", 0.1, 0.6, 0.35, 4, 4, glm::vec3(1,0,0));

		/* Create Resources */
		//Asteroids
		std::string filename = std::string(MATERIAL_DIRECTORY) + std::string("/material");
		resman_.LoadResource(Material, "SimpleSphereMesh", filename.c_str());

		//TARGET
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/material");
		resman_.LoadResource(Material, "TargetMesh", filename.c_str());

		// FLY
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/fly.obj");
		resman_.LoadResource(Mesh, "FlyMesh", filename.c_str());

		// ROCKET
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/material");
		resman_.LoadResource(Material, "ObjectMaterial", filename.c_str());

		// Load material to be used for normal mapping
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/normal_map");
		resman_.LoadResource(Material, "NormalMapMaterial", filename.c_str());

		// Load texture to be used in normal mapping
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/normal_map2.png");
		resman_.LoadResource(Texture, "NormalMap", filename.c_str());
	}

	void Game::SetupScene(void) 
	{
		// Set background color for the scene
		scene_.SetBackgroundColor(viewport_background_color_g);
		CreateAsteroidField(100);
		player = createFly("WallInstance1", "FlyMesh", "NormalMapMaterial", "");
		target = createTarget("playerTarget", "TargetMesh", "ObjectMaterial" , "");
	}

	void Game::MainLoop(void)
	{
		// Loop while the user did not close the window
		while (!glfwWindowShouldClose(window_)) 
		{
			// Animate the scene
			if (animating_)
			{
				static double last_time = 0;
				double current_time = glfwGetTime();
				if ((current_time - last_time) > 0.01) {
					//scene_.Update();

					last_time = current_time;
				}
			}

			// Update player
			player->Update();

			// Draw the scene
			scene_.Draw(&camera_);

			// Push buffer drawn in the background onto the display
			glfwSwapBuffers(window_);

			// Update other events like input handling
			glfwPollEvents();
		}
	}

	void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		// Get user data with a pointer to the game class
		void* ptr = glfwGetWindowUserPointer(window);
		Game *game = (Game *)ptr;

		// Quit game if 'q' is pressed
		if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}

		// Stop animation if space bar is pressed
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
			game->animating_ = (game->animating_ == true) ? false : true;
		}

		// View control
		float rot_factor(glm::pi<float>() / 180);
		float trans_factor = 0.1;
		if (key == GLFW_KEY_UP) {
			game->camera_.Pitch(rot_factor);
		}
		if (key == GLFW_KEY_DOWN) {
			game->camera_.Pitch(-rot_factor);
		}
		if (key == GLFW_KEY_LEFT) {
			game->camera_.Yaw(rot_factor);
		}
		if (key == GLFW_KEY_RIGHT) {
			game->camera_.Yaw(-rot_factor);
		}
		if (key == GLFW_KEY_Z) {
			game->camera_.Roll(-rot_factor);
		}
		if (key == GLFW_KEY_X) {
			game->camera_.Roll(rot_factor);
		}
		if (key == GLFW_KEY_W) {
			game->camera_.Translate(game->camera_.GetForward()*trans_factor);
		}
		if (key == GLFW_KEY_S) {
			game->camera_.Translate(-game->camera_.GetForward()*trans_factor);
		}
		if (key == GLFW_KEY_A) {
			game->camera_.Translate(-game->camera_.GetSide()*trans_factor);
		}
		if (key == GLFW_KEY_D) {
			game->camera_.Translate(game->camera_.GetSide()*trans_factor);
		}
		//TO BE CHANGED!!!!!!!!!!!!!!
		if (key == GLFW_KEY_I) {
			game->camera_.Translate(game->camera_.GetUp()*trans_factor);
		}
		if (key == GLFW_KEY_K) {
			game->camera_.Translate(-game->camera_.GetUp()*trans_factor);
		}
		if (key == GLFW_KEY_C && action == GLFW_PRESS)
		{
			if (game->player->fireRate <= 0)
			{
				game->player->rockets.push_back(game->createRocket("Rocket", "RocketMesh", "ObjectMaterial", ""));
				game->player->fireRate = game->player->maxFireRate;
			}
		}
		if (key == GLFW_MOUSE_BUTTON_RIGHT) 
		{
			//change to third or first person
			//game->camera_.SetThirdPerson(!game->camera_.GetThirdPerson());
			game->camera_.firstPerson = !game->camera_.firstPerson;
		}
	}

	void Game::ResizeCallback(GLFWwindow* window, int width, int height)
	{
		// Set up viewport and camera projection based on new window size
		glViewport(0, 0, width, height);
		void* ptr = glfwGetWindowUserPointer(window);
		Game *game = (Game *)ptr;
		game->camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
	}

	Asteroid *Game::CreateAsteroidInstance(std::string entity_name, std::string object_name, std::string material_name)
	{
		// Get resources
		Resource *geom = resman_.GetResource(object_name);
		if (!geom) { throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\""))); }
		Resource *mat = resman_.GetResource(material_name);
		if (!mat) { throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\""))); }

		// Create asteroid instance
		Asteroid *ast = new Asteroid(entity_name, geom, mat);
		camNode->AddChild(ast);
		return ast;
	}

	void Game::CreateAsteroidField(int num_asteroids)
	{
		// Create a number of asteroid instances
		for (int i = 0; i < num_asteroids; i++)
		{
			// Create instance name
			std::stringstream ss;
			ss << i;
			std::string index = ss.str();
			std::string name = "AsteroidInstance" + index;

			// Create asteroid instance
			Asteroid *ast = CreateAsteroidInstance(name, "SimpleSphereMesh", "ObjectMaterial");

			// Set attributes of asteroid: random position, orientation, and
			// angular momentum
			ast->SetPosition(glm::vec3(-300 + 400*((float)rand() / RAND_MAX), -300 + 400*((float)rand() / RAND_MAX), - 400*((float)rand() / RAND_MAX)));
			ast->SetOrientation(glm::normalize(glm::angleAxis(glm::pi<float>()*((float)rand() / RAND_MAX), glm::vec3(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX)))));
			ast->SetAngM(glm::normalize(glm::angleAxis(0.05f*glm::pi<float>()*((float)rand() / RAND_MAX), glm::vec3(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX)))));
		}
	}

	Fly* Game::createFly(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name)
	{
		// Get resources
		Resource *geom = resman_.GetResource(object_name);
		if (!geom) { throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\""))); }
		Resource *mat = resman_.GetResource(material_name);
		if (!mat) { throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\""))); }

		//THIS NEEDS TO BE CHANGED 
		//Resource *tex = resman_.GetResource(texture_name);
		//if (!tex) { throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\""))); }

		//Create an instance of a fly
		Fly* fly = new Fly(entity_name, geom, mat, 0);
		camNode->AddChild(fly);

		//Set initial values and rotate the fly by 180 degrees
		fly->SetScale(glm::vec3(100, 100, 100));
		fly->Rotate(glm::angleAxis(glm::pi<float>() , glm::vec3(0 , 1 , 0)));
		fly->SetPosition(glm::vec3(0.0, -0.3, -1.5));

		return fly;
	}

	Rocket* Game::createRocket(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name)
	{
		// Get resources
		Resource *geom = resman_.GetResource(object_name);
		if (!geom) { throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\""))); }
		Resource *mat = resman_.GetResource(material_name);
		if (!mat) { throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\""))); }
		//Resource *tex = resman_.GetResource(texture_name);
		//if (!tex) { throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\""))); }

		// Create a rocket instance add it as a root
		Rocket* rocket = new Rocket(entity_name, geom, mat, 0, camera_.GetForward());
		camNode->AddChild(rocket);

		//Set initial values
		rocket->SetScale(glm::vec3(0.08, 0.02, 0.08));
		rocket->Rotate(glm::normalize(glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1.0, 0.0, 0.0))));
		//rocket->SetPosition(player->GetPosition());

		return rocket;
	}

	SceneNode * Game::createTarget(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name)
	{
		// Get resources
		Resource *geom = resman_.GetResource(object_name);
		if (!geom) { throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\""))); }
		Resource *mat = resman_.GetResource(material_name);
		if (!mat) { throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\""))); }
		//Resource *tex = resman_.GetResource(texture_name);
		//if (!tex) { throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\""))); }

		// Create a Target instance and make it a child of camera 
		SceneNode* Target = new SceneNode(entity_name, geom, mat, 0);
		camNode->AddChild(Target);

		//Set initial values
		Target->SetScale(glm::vec3(0.001, 0.001, 0.001));
		Target->SetPosition(glm::vec3(0, 0, -0.2));

		return Target;
	}

	// DO NOT USE THIS FUNCTION IT DOES NOT WORK!!!!!!!!!!
	SceneNode *Game::CreateInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name) 
	{
		Resource *geom = resman_.GetResource(object_name);
		if (!geom) { throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\""))); }
		Resource *mat = resman_.GetResource(material_name);
		if (!mat) { throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\""))); }

		//SceneNode *scn = scene_.CreateNode(entity_name, geom, mat);
		//return scn;
		return NULL;
	}
} // namespace game
