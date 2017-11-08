 #include <iostream>
#include <time.h>
#include <sstream>

#include "game.h"
#include "bin/path_config.h"

// SHOOTING WHILE MOVING IS HORRIBLE !!!!
// CHECKING WHETHER A BUTTON IS HELD DOWN OR NOT IS AN ISSUE???????? WITH THE CHECK INPUT FUNCTION MAYBE USE A MIXTURE OF THE 2 !!!!!
// A LITTLE DISTORTION IN PART OF THE LEFT WING OF THE FLY !!!!
// WHEN MOVING THE BULLETS STOP MOVING !!!!!!!!!!!!
// TARGET AND BULLETS DON'T EXACTLY LINE UP !!!!!
// HOW TO SMOOTH OUT CAMERA AND PLAYER MOVEMENT !!!!!
// WHEN CHANGING BETWEEN FIRST AND THIRD PRERSON VIEW WE ALSO NEED TO MOVE THE TARGET
// STORE COLLIDABLES OR SMTHG

// Spacebar to shoot rocket
// W,A,S,D for movement
// Right click to move from first to third person
// Up,down,left,right arrow keys for camera movement 
// Esc to quit 
// Z,X to roll the camera (maybe we need to change that to be decided)
// MIGHT NEED TO CHANGE THE ACSENT AND DECSENT CONTROLS SINCE THEY ARE HORRIBLE (I , K)
namespace game 
{
	// Some configuration constants
	// They are written here as global variables, but ideally they should be loaded from a configuration file

	// Main window settings
	const std::string window_title_g = "FLYING UNDERSIZED CONTROLLED KILLER";
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

	const std::string material_directory_g = MATERIAL_DIRECTORY;	// Materials 

	Game::Game(void) {}
	Game::~Game() { glfwTerminate(); }

	void Game::Init(void) 
	{
		// Run all initialization steps
		InitWindow();
		InitView();
		InitEventHandlers();

		// Set variables
		animating_ = true;
		world = new SceneNode("world", 0, 0, 0);	// Dummy Node
		scene_.SetRoot(world);						// Set dummy as Root of Heirarchy
		world->AddChild(camNode);					// Set the camera as a child of the world
	}

	void Game::InitWindow(void) 
	{
		// Initialize the window management library (GLFW)
		if (!glfwInit()) { throw(GameException(std::string("Could not initialize the GLFW library"))); }

		// Create a window and its OpenGL context
		if (window_full_screen_g) { window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), glfwGetPrimaryMonitor(), NULL); }
		else { window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), NULL, NULL); }
		if (!window_) 
		{
			glfwTerminate();
			throw(GameException(std::string("Could not create window")));
		}

		// Make the window's context the current one
		glfwMakeContextCurrent(window_);

		// Initialize the GLEW library to access OpenGL extensions
		// Need to do it after initializing an OpenGL context
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK) { throw(GameException(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err)))); }
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

		/* Set up camera */
		camera_.SetView(camera_position_g, camera_look_at_g, camera_up_g);		// Set current view
		camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);	// Set projection
		camNode = new CameraNode(&camera_);		//setup camera as a sceneNode
	}

	void Game::InitEventHandlers(void)
	{
		/* Set event callbacks */
		glfwSetKeyCallback(window_, KeyCallback);
		glfwSetMouseButtonCallback(window_, MouseButtonCallback);
		// Something
		glfwSetInputMode(window_, GLFW_STICKY_KEYS, 1);
		glfwSetFramebufferSizeCallback(window_, ResizeCallback);

		glfwSetWindowUserPointer(window_, (void *) this);	// Set pointer to game object, so that callbacks can access it
	}

	void Game::SetupResources(void)
	{
		/* Create Built-In Geometries */
		resman_.CreateCylinder("rocketMesh");
		resman_.CreateWall("wallMesh");
		resman_.CreateSphere("simpleSphereMesh");
		resman_.CreateCylinder("targetMesh", 0.1, 0.6, 0.35, 4, 4, glm::vec3(1,0,0));

		/* Create Resources */
		// OBJECT MATREIAL FOR GENERAL OBJECTS
		std::string filename = std::string(MATERIAL_DIRECTORY) + std::string("/material");
		resman_.LoadResource(Material, "objectMaterial", filename.c_str());

		// TEXTURE MATERIAL FOR OBJECTS THAT HAVE TEXTURES
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/texture");
		resman_.LoadResource(Material, "textureMaterial", filename.c_str());

		// HUMAN
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/human12.obj");
		resman_.LoadResource(Mesh, "humanMesh", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/skin.png");
		resman_.LoadResource(Texture, "humanTex", filename.c_str());

		// FLY
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/fly2.obj");
		resman_.LoadResource(Mesh, "flyMesh", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/flywing.obj");
		resman_.LoadResource(Mesh, "flyWingMesh", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/stripes.png");
		resman_.LoadResource(Texture, "flyTex", filename.c_str());

		// ROCKET
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/plain.png");
		resman_.LoadResource(Texture, "rocketTex", filename.c_str());
	}

	void Game::SetupScene(void) 
	{
		scene_.SetBackgroundColor(viewport_background_color_g);						// Set background color for the scene
		CreateAsteroidField(100);													// For testing
		player = createFly("player", "flyMesh", "textureMaterial", "flyTex");	    // Create player
		target = createTarget("playerTarget", "targetMesh", "objectMaterial" , ""); // Create target for shooting
		human = createHuman("human1", "humanMesh", "textureMaterial", "humanTex");	// Create human enemy
	}

	void Game::MainLoop(void)
	{
		// Loop while the user did not close the window
		while (!glfwWindowShouldClose(window_)) 
		{
			// Animate the scene
			// ANIMATING THE WINGS SHOULD OCCUR IN THE PLAYER'S UPDATE!!!!!!
			if (animating_)
			{
				static double last_time = 0;
				double current_time = glfwGetTime();
				if ((current_time - last_time) > 0.01) {
					scene_.Update();

					//glm::quat rotation(glm::angleAxis(glm::pi<float>() / 180, glm::vec3(0,1,0)));
					//player->Rotate(rotation);
					last_time = current_time;
				}
			}

			checkInput(); // Check for input

			scene_.Draw(&camera_);	// Draw the scene
			glfwSwapBuffers(window_);	// Push buffer drawn in the background onto the display
			glfwPollEvents();	// Update other events like input handling
		}
	}

	void Game::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		// Get user data with a pointer to the game class
		void* ptr = glfwGetWindowUserPointer(window);
		Game *game = (Game *)ptr;

		// Mouse click checks for 1st or third person
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) { game->camera_.firstPerson = !game->camera_.firstPerson; }
	}

	void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		// Get user data with a pointer to the game class
		void* ptr = glfwGetWindowUserPointer(window);
		Game* game = (Game *)ptr;

		// Quit game if ESC button is pressed
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); }

		// Shoot a rocket
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			if (game->player->fireRate <= 0)
			{
				game->player->rockets.push_back(game->createRocket("Rocket", "simpleSphereMesh", "textureMaterial", "rocketTex"));
				game->player->fireRate = game->player->maxFireRate;
			}
		}
	}

	/* Resize Screen */
	void Game::ResizeCallback(GLFWwindow* window, int width, int height)
	{
		// Set up viewport and camera projection based on new window size
		glViewport(0, 0, width, height);
		void* ptr = glfwGetWindowUserPointer(window);
		Game *game = (Game *)ptr;
		game->camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
	}

	// CHECKING WHETHER A BUTTON IS HELD DOWN OR NOT IS AN ISSUE???????? WITH THE CHECK INPUT FUNCTION
	void Game::checkInput()
	{
		// View control
		float rot_factor(glm::pi<float>() / 360);
		float trans_factor = 1.0;

		// Move camera up, down, and to the sides
		if (glfwGetKey(window_, GLFW_KEY_UP)) { camera_.Pitch(rot_factor); }
		if (glfwGetKey(window_, GLFW_KEY_DOWN)) { camera_.Pitch(-rot_factor); }
		if (glfwGetKey(window_, GLFW_KEY_LEFT)) { camera_.Yaw(rot_factor); }
		if (glfwGetKey(window_, GLFW_KEY_RIGHT)) { camera_.Yaw(-rot_factor); }

		// Forward backward and side movements
		if (glfwGetKey(window_, GLFW_KEY_W)) { camera_.Translate(camera_.GetForward() * trans_factor); }
		if (glfwGetKey(window_, GLFW_KEY_S)) { camera_.Translate(-camera_.GetForward() * trans_factor); }
		if (glfwGetKey(window_, GLFW_KEY_A)) { camera_.Translate(-camera_.GetSide() * trans_factor); }
		if (glfwGetKey(window_, GLFW_KEY_D)) { camera_.Translate(camera_.GetSide() * trans_factor); }

		// Roll camera
		if (glfwGetKey(window_, GLFW_KEY_Z)) { camera_.Roll(-rot_factor); }
		if (glfwGetKey(window_, GLFW_KEY_X)) { camera_.Roll(rot_factor); }

		// TO BE CHANGED!!!!!!!!!!!!!! (movement up and down)
		if (glfwGetKey(window_, GLFW_KEY_I)) { camera_.Translate(camera_.GetUp() * trans_factor); }
		if (glfwGetKey(window_, GLFW_KEY_K)) { camera_.Translate(-camera_.GetUp() * trans_factor); }
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
		world->AddChild(ast);
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
			Asteroid *ast = CreateAsteroidInstance(name, "simpleSphereMesh", "objectMaterial");

			// Set attributes of asteroid: random position, orientation, and
			// angular momentum
			ast->SetPosition(glm::vec3(-300 + 400*((float)rand() / RAND_MAX), -300 + 400*((float)rand() / RAND_MAX), - 400*((float)rand() / RAND_MAX)));
			ast->SetOrientation(glm::normalize(glm::angleAxis(glm::pi<float>()*((float)rand() / RAND_MAX), glm::vec3(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX)))));
			ast->SetAngM(glm::normalize(glm::angleAxis(0.05f*glm::pi<float>()*((float)rand() / RAND_MAX), glm::vec3(((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX), ((float)rand() / RAND_MAX)))));
		}
	}

	// (MAYBE MAKE IT INDEPENDENT OF CAMERA LATER ON)
	Fly* Game::createFly(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name)
	{
		std::vector<Resource*> resources = loadAssetResources(object_name, material_name, texture_name);	// Holder of resources

		//Create an instance of a fly
		Fly* fly = new Fly(entity_name, resources[0], resources[1], resources[2]);
		camNode->AddChild(fly);

		//Set initial values and rotate the fly by 180 degrees
		fly->SetScale(glm::vec3(100, 100, 100));
		fly->Rotate(glm::angleAxis(glm::pi<float>() , glm::vec3(0 , 1 , 0)));
		fly->SetPosition(glm::vec3(0.0, -0.3, -1.5));

		resources = loadAssetResources("flyWingMesh", material_name, texture_name);

		SceneNode *wing = new SceneNode("Fly_wing", resources[0], resources[1], resources[2]);

		fly->AddChild(wing);

		wing->SetScale(glm::vec3(100, 100, 100));
		wing->SetPosition(glm::vec3(0.0, 0.2, 0.0));
		fly->wing = wing;

		return fly;
	}

	// CREATE HUMAN
	Human* Game::createHuman(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name)
	{
		std::vector<Resource*> resources = loadAssetResources(object_name, material_name, texture_name);	// Holder of resources

		Human* hum = new Human(entity_name, resources[0], resources[1], resources[2]);
		world->AddChild(hum);

		hum->SetScale(glm::vec3(1, 1, 1));
		hum->SetPosition(glm::vec3(0, 0, -200));

		return hum;
	}

	// NEED TO FIX ORIGINAL DIRECTION SUCH THAT IT WOULD HAVE THE DIRECTION OF THE PLAYER AS ITS DIRECTION VECTOR (MAYBE USE A QUATERNION INSTEAD OF A DIRECTION VECTOR)
	Rocket* Game::createRocket(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name)
	{
		std::vector<Resource*> resources = loadAssetResources(object_name, material_name, texture_name);	// Holder of resources

		// Create a rocket instance add it as a root
		Rocket* rocket = new Rocket(entity_name, resources[0], resources[1], resources[2], camera_.GetForward());
		world->AddChild(rocket);

		//Set initial value
		rocket->SetScale(glm::vec3(0.1, 0.03, 0.1));
		rocket->SetOrientation(player->getAbsoluteOrientation());
		rocket->Rotate(glm::normalize(glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1.0, 0.0, 0.0))));
		rocket->SetPosition(player->getAbsolutePosition() - glm::normalize(camera_.GetForward()));

		return rocket;
	}

	// NEEDS FIXING (Target is a child of player since it should move according to the player's position)
	SceneNode * Game::createTarget(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name)
	{
		// Get resources
		std::vector<Resource*> resources = loadAssetResources(object_name, material_name, texture_name);	// Holder of resources

		// Create a Target instance and make it a child of camera 
		SceneNode* Target = new SceneNode(entity_name, resources[0], resources[1], resources[2]);
		camNode->AddChild(Target);

		//Set initial values
		Target->SetScale(glm::vec3(0.01, 0.01, 0.01));
		Target->SetPosition(glm::vec3(0, 0, -2.0));

		return Target;
	}

	// LOADS GEOMETRY MATERIAL AND TEXTURE STORES THEM IN A VECTOR AND RETURN THEM
	std::vector<Resource*> Game::loadAssetResources(std::string geometryName, std::string materialName, std::string textureName)
	{
		std::vector<Resource*> resources;	// Holder of resources

		// Load each resource add it to the vector check if its null then return result
		resources.push_back(resman_.GetResource(geometryName));
		if (!resources[0]) { throw(GameException(std::string("Could not find resource \"") + geometryName + std::string("\""))); }
		resources.push_back(resman_.GetResource(materialName));
		if (!resources[1]) { throw(GameException(std::string("Could not find resource \"") + materialName + std::string("\""))); }
		resources.push_back(resman_.GetResource(textureName));
		if (!resources[2]) { std::cout << std::string("Could not find resource \"") + textureName + std::string("\"") << std::endl; }	// FOR OBJECTS THAT DO NOT HAVE TEXTURES

		return resources;
	}
} // namespace game
