 #include <iostream>
#include <time.h>
#include <sstream>

#include "game.h"
#include "bin/path_config.h"

// TO DO:
// REDO HOW ENEMIES LOCK ON TO PLAYER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// HAVE EACH ROOM CONNECTED TO ONE ANOTHER AND ETC...
// RESETTING TO PREV POSITION IS NOT WORKING COLLISION ON ENVIRONMENT???????
// SPLINE TRAJECTORIES WITH SPIDER WEB ATTACK
// PARTICLE SYSTEMS ???????

// PARTICLE SYSTEMS FOR THE DEATH OR DESTRUCTION OF OBJS
// SHADOW MAPPING (OPTIONAL)

// Spacebar to shoot rocket
// W,A,S,D for movement
// Right click to move from first to third person
// Up,down,left,right arrow keys for camera movement 
// Z and C to roll the camera
// Ascent and Descent are Q and E respectively
// Esc to quit 
namespace game 
{
	// Some configuration constants
	// They are written here as global variables, but ideally they should be loaded from a configuration file

	// Main window settings
	const std::string window_title_g = "FLYING UNDERSIZED CONTROLLED KILLER";
	const unsigned int window_width_g = 1024;
	const unsigned int window_height_g = 768;
	const bool window_full_screen_g = false;

	// Viewport and camera settings
	float camera_near_clip_distance_g = 0.01;
	float camera_far_clip_distance_g = 1000.0;
	//float camera_fov_g = 20.0; // Field-of-view of camera
	float camera_fov_g = 70.0; // Field-of-view of camera
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
		resman_.CreateCube("CubeMesh");

		/* Create Resources */

		/* MATERIAL GLSL FILES */
		// OBJECT MATREIAL FOR GENERAL OBJECTS
		std::string filename = std::string(MATERIAL_DIRECTORY) + std::string("/material");
		resman_.LoadResource(Material, "objectMaterial", filename.c_str());

		// TEXTURE MATERIAL FOR OBJECTS THAT HAVE TEXTURES
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/texture");
		resman_.LoadResource(Material, "textureMaterial", filename.c_str());

		/* TEXTURES */

		// WEB TEXTURE
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/webTexture.png");
		resman_.LoadResource(Texture, "webTex", filename.c_str());

		// ROCKET TEXTURE
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/rocketTexture.png");
		resman_.LoadResource(Texture, "rocketTex", filename.c_str());

		// HUMAN TEXTURE
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/skin.png");
		resman_.LoadResource(Texture, "humanTex", filename.c_str());

		// FLY TEXTURES
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/flyBodyTexture.png");
		resman_.LoadResource(Texture, "flyBodyTex", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/flyWingsTexture.png");
		resman_.LoadResource(Texture, "flyWingsTex", filename.c_str());

		// SPIDER TEXTURES
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/spiderBodyTexture.png");
		resman_.LoadResource(Texture, "spiderBodyTex", filename.c_str());

		// DRAGONFLY TEXTURES
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/dragonFlyBodyTexture.png");
		resman_.LoadResource(Texture, "dragonFlyBodyTex", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/dragonFlyWingsTexture.png");
		resman_.LoadResource(Texture, "dragonFlyWingsTex", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/dragonFlyLegsTexture.png");
		resman_.LoadResource(Texture, "dragonFlyLegsTex", filename.c_str());

		// ENVIRONMENT TEXTURES
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/floorTexture.png");
		resman_.LoadResource(Texture, "floorTex", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/wallTexture.png");
		resman_.LoadResource(Texture, "wallTex", filename.c_str());

		// BLOCK TEXTURE
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/metalTexture.png");
		resman_.LoadResource(Texture, "blockTex", filename.c_str());

		/* GEOMETRIES */

		// HUMAN BODY LEFT AND RIGHT HANDS AND LEGS
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/humanBody.obj");
		resman_.LoadResource(Mesh, "humanBodyMesh", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/humanLeftHand.obj");
		resman_.LoadResource(Mesh, "humanLeftHandMesh", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/humanRightHand.obj");
		resman_.LoadResource(Mesh, "humanRightHandMesh", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/humanLeftLeg.obj");
		resman_.LoadResource(Mesh, "humanLeftLegMesh", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/humanRightLeg.obj");
		resman_.LoadResource(Mesh, "humanRightLegMesh", filename.c_str());
		
		// SPIDER BODY and RIGHT AND LEFT LEGS
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/spiderBody.obj");
		resman_.LoadResource(Mesh, "spiderBodyMesh", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/spiderLeftLeg.obj");
		resman_.LoadResource(Mesh, "spiderLeftLegMesh", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/spiderRightLeg.obj");
		resman_.LoadResource(Mesh, "spiderRightLegMesh", filename.c_str());

		// DRAGONFLY BODY, RIGHT and LEFT WINGS and LEGS
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/dragonFlyBody.obj");
		resman_.LoadResource(Mesh, "dragonFlyBodyMesh", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/dragonFlyLeftWing.obj");
		resman_.LoadResource(Mesh, "dragonFlyLeftWingMesh", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/dragonFlyRightWing.obj");
		resman_.LoadResource(Mesh, "dragonFlyRightWingMesh", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/dragonFlyLegs.obj");
		resman_.LoadResource(Mesh, "dragonFlyLegsMesh", filename.c_str());

		// FLY BODY WINGS AND LEGS
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/flyBody.obj");
		resman_.LoadResource(Mesh, "flyBodyMesh", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/flyWings.obj");
		resman_.LoadResource(Mesh, "flyWingsMesh", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/flyLegs.obj");
		resman_.LoadResource(Mesh, "flyLegsMesh", filename.c_str());

		// TABLE 
		//filename = std::string(MATERIAL_DIRECTORY) + std::string("/assets/table.obj");
		//resman_.LoadResource(Mesh, "tableMesh", filename.c_str());
	}

	/* Setup game elements */
	void Game::SetupScene(void) 
	{
		scene_.SetBackgroundColor(viewport_background_color_g);								// Set background color for the scene

		player = createFly("player");														// Create player
		target = createTarget("playerTarget");												// Create target for shooting
		human = createHuman("human1");														// Create human enemy
		spider = createSpider("spider1");													// Create Spider enemy
		dragonFly = createDragonFly("dragonfly1");											// Create dragonfly enemy
<<<<<<< HEAD
		block = createBlock("block1");
		environment = createEnvironment();
=======
		room = createRoom();
>>>>>>> ec9318c8617119d3cd384f4b92923ae5c8ec771b
	}

	void Game::MainLoop(void)
	{
		// Loop while the user did not close the window
		while (!glfwWindowShouldClose(window_)) 
		{
			/* INPUT */
			checkInput(); 

<<<<<<< HEAD

			/* Check collision with bullets and all characters (friendly fire is fine) */
			/* Rockets Collision Detection */
			bool collided = false;
			for (int i = 0; i < rockets.size(); i++)
=======
			//check if player health > 0 
			if (player->health > 0)
>>>>>>> ec9318c8617119d3cd384f4b92923ae5c8ec771b
			{
				/* DRAW */
				scene_.Draw(&camera_);		// Draw the scene

				/* COLLISION DETECTION */
				gameCollisionDetection();

				/* Check collision with bullets and all characters (friendly fire is fine) */
				// Webs and player
				/* Check collision with characters and other characters */

				// Human attack with player
				// WE MIGHT NEED A SEPERATE COLLISION DETECTION FOR THE HUMANS!!!!


				/* UPDATE */
				// Check distances
				// redo locking on to player
				if (player) { player->update(); }

				for (int i = 0; i < dragonFlies.size(); i++)
				{
					dragonFlies[i]->updateTarget(player->body->getAbsolutePosition());
					dragonFlies[i]->updateTargetOrientation(player->body->getAbsoluteOrientation());

					dragonFlies[i]->update();
					if (dragonFlies[i]->getFiring()) { dragonFlies[i]->fire(createRocket("Rocket4", dragonFlies[i]->getDirection(), dragonFlies[i]->body->getAbsolutePosition())); }
				}

				for (int j = 0; j < spiders.size(); j++)
				{
					spiders[j]->updateTarget(player->body->getAbsolutePosition());
					spiders[j]->updateTargetOrientation(player->body->getAbsoluteOrientation());

					spiders[j]->update();
					if (spiders[j]->getFiring()) { spiders[j]->fire(createWeb("Rocket3", spiders[j]->getDirection(), spiders[j]->body->getAbsolutePosition())); }
				}

				for (int k = 0; k < humans.size(); k++)
				{
					humans[k]->updateTarget(player->body->getAbsolutePosition());
					humans[k]->updateTargetOrientation(player->body->getAbsoluteOrientation());

					humans[k]->update();
					if (humans[k]->getFiring()) { human->fire(createRocket("Rocket2", humans[k]->getDirection(), humans[k]->body->getAbsolutePosition())); }
				}
			}

			//UPDATE BLOCK
			if (block->getDrag()) {
				block->setDragger(camera_.GetPosition());
			}
			block->update();


			glfwSwapBuffers(window_);	// Push buffer drawn in the background onto the display
			glfwPollEvents();			// Update other events like input handling
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

		// Move camera up, down, and to the sides
		if (glfwGetKey(window_, GLFW_KEY_UP)) { camera_.Pitch(rot_factor); }
		if (glfwGetKey(window_, GLFW_KEY_DOWN)) { camera_.Pitch(-rot_factor); }
		if (glfwGetKey(window_, GLFW_KEY_LEFT)) { camera_.Yaw(rot_factor); }
		if (glfwGetKey(window_, GLFW_KEY_RIGHT)) { camera_.Yaw(-rot_factor); }

		// Forward backward and side movements
		if (glfwGetKey(window_, GLFW_KEY_W)) { camera_.Translate(camera_.GetForward() * player->speed); }
		if (glfwGetKey(window_, GLFW_KEY_S)) { camera_.Translate(-camera_.GetForward() * player->speed); }
		if (glfwGetKey(window_, GLFW_KEY_A)) { camera_.Translate(-camera_.GetSide() * player->speed); }
		if (glfwGetKey(window_, GLFW_KEY_D)) { camera_.Translate(camera_.GetSide() * player->speed); }

		// Roll camera
		if (glfwGetKey(window_, GLFW_KEY_Z)) { camera_.Roll(-rot_factor); }
		if (glfwGetKey(window_, GLFW_KEY_C)) { camera_.Roll(rot_factor); }

		// TO BE CHANGED!!!!!!!!!!!!!! (movement up and down)
		if (glfwGetKey(window_, GLFW_KEY_Q)) { camera_.Translate(camera_.GetUp() * player->speed); }
		if (glfwGetKey(window_, GLFW_KEY_E)) { camera_.Translate(-camera_.GetUp() * player->speed); }

		// Drag
		if (glfwGetKey(window_, GLFW_KEY_G)) { 
			if (player->dragtimer == -1 || player->dragtimer + player->dragwait <= glfwGetTime()) {
				if (player->dragging == NULL) {
					if (player->collision(block->object, 10.0f)) {
						player->dragging = block;
						player->dragging->setDrag(true);
						player->dragging->setDragger(camera_.GetPosition());
						block->object->SetPosition(camera_.GetPosition());
						//player->legs->AddChild(block->object);
						player->dragtimer = glfwGetTime();
					}
				}
				else {
					player->dragging->setDrag(false);
					player->dragging = NULL;
					//player->legs->RemoveChild(block->object);
					player->dragtimer = glfwGetTime();
					block->object->SetPosition(block->object->GetPosition() + glm::vec3(1,1,1));
					block->speed = 1;
				}
			}
		}

		// Shoot a rocket
		if (glfwGetKey(window_ , GLFW_KEY_SPACE))
		{
			if (player->fireRate <= 0)
			{
				player->rockets.push_back(createRocket("Rocket1", target->getAbsolutePosition() - player->body->getAbsolutePosition(), player->body->getAbsolutePosition()));
				player->fireRate = player->maxFireRate;
			}
		}
	}

	// (MAYBE MAKE IT INDEPENDENT OF CAMERA LATER ON) initial values are set in the constructor
	Fly* Game::createFly(std::string entity_name)
	{
		// Setup fly parts
		SceneNode* flyBody = createSceneNode(entity_name + "Body", "flyBodyMesh", "textureMaterial", "flyBodyTex");
		SceneNode* flyWings = createSceneNode(entity_name + "Wings", "flyWingsMesh", "textureMaterial", "flyWingsTex");
		SceneNode* flyLegs = createSceneNode(entity_name + "Legs", "flyLegsMesh", "textureMaterial", "flyWingsTex");

		// Setup heirarchy
		camNode->AddChild(flyBody);
		//world->AddChild(flyBody);			// testing
		flyBody->AddChild(flyWings);
		flyBody->AddChild(flyLegs);

		// Setup parts Scales
		flyBody->SetScale(glm::vec3(10, 10, 10));
		flyWings->SetScale(glm::vec3(10, 10, 10));
		flyLegs->SetScale(glm::vec3(10, 10, 10));

		// Setup fly Rotations
		flyBody->Rotate(glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0)));

		// Setup parts Positions
		//flyWings->SetPosition(glm::vec3(0, 0.07, 0));
		flyBody->SetPosition(glm::vec3(0, -0.3, -0.6));

		return new Fly(flyBody, flyWings, flyLegs);
	}


	Block* Game::createBlock(std::string entity_name)
	{
		// Setup fly parts
		SceneNode* object = createSceneNode(entity_name, "simpleSphereMesh", "textureMaterial", "blockTex");

		world->AddChild(object);

		// Setup parts Scales
		object->SetScale(glm::vec3(10, 10, 10));

		// Setup fly Rotations
		//flyBody->Rotate(glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0)));

		// Setup parts Positions
		object->SetPosition(glm::vec3(0, -0.3, -0.6));

		return new Block(object);
	}


	// Create a Spider instance
	Spider* Game::createSpider(std::string entity_name)
	{
		// Setup spider parts 
		SceneNode* spiderBody = createSceneNode(entity_name + "Body", "spiderBodyMesh", "textureMaterial", "spiderBodyTex");
		SceneNode* spiderLeftLeg = createSceneNode(entity_name + "LeftLeg", "spiderLeftLegMesh", "textureMaterial", "spiderBodyTex");
		SceneNode* spiderRightLeg = createSceneNode(entity_name + "RightLeg", "spiderRightLegMesh", "textureMaterial", "spiderBodyTex");

		// Setup heirarchy
		world->AddChild(spiderBody);
		spiderBody->AddChild(spiderLeftLeg);
		spiderBody->AddChild(spiderRightLeg);

		// Setup parts Scaling
		spiderBody->SetScale(glm::vec3(glm::vec3(0.02, 0.02, 0.02)));
		spiderLeftLeg->SetScale(glm::vec3(0.02, 0.02, 0.02));
		spiderRightLeg->SetScale(glm::vec3(glm::vec3(0.02, 0.02, 0.02)));

		// Setup parts positions
		spiderBody->SetPosition(glm::vec3(10, 0, -20));
		spiderBody->Rotate(glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0)));

		Spider* spi = new Spider(spiderBody, spiderLeftLeg, spiderRightLeg);
		spiders.push_back(spi);

		return spi;
	}

	// Create a Dragonfly instance
	DragonFly* Game::createDragonFly(std::string entity_name)
	{
		// Setup dragonFly parts 
		SceneNode* dragonFlyBody = createSceneNode(entity_name + "Body", "dragonFlyBodyMesh", "textureMaterial", "dragonFlyBodyTex");
		SceneNode* dragonFlyLeftWing = createSceneNode(entity_name + "LeftWing", "dragonFlyLeftWingMesh", "textureMaterial", "dragonFlyWingsTex");
		SceneNode* dragonFlyRightWing = createSceneNode(entity_name + "RightWing", "dragonFlyRightWingMesh", "textureMaterial", "dragonFlyWingsTex");
		SceneNode* dragonFlyLegs = createSceneNode(entity_name + "Legs", "dragonFlyLegsMesh", "textureMaterial", "dragonFlyLegsTex");

		// Setup heirarchy
		world->AddChild(dragonFlyBody);
		dragonFlyBody->AddChild(dragonFlyLeftWing);
		dragonFlyBody->AddChild(dragonFlyRightWing);
		dragonFlyBody->AddChild(dragonFlyLegs);

		// Setup parts scale
		dragonFlyBody->SetScale(glm::vec3(40, 40, 40));
		dragonFlyLeftWing->SetScale(glm::vec3(40, 40, 40));
		dragonFlyRightWing->SetScale(glm::vec3(40, 40, 40));
		dragonFlyLegs->SetScale(glm::vec3(40, 40, 40));

		// Setup parts positions
		dragonFlyBody->SetPosition(glm::vec3(-10, 0, -20));

		// Create dragon fly instance and add it to the character collidables
		DragonFly* dragon = new DragonFly(dragonFlyBody, dragonFlyLeftWing, dragonFlyRightWing, dragonFlyLegs);
		dragonFlies.push_back(dragon);

		return dragon;
	}

	// CREATE HUMAN
	Human* Game::createHuman(std::string entity_name)
	{
		SceneNode* humanBody = createSceneNode(entity_name + "Body", "humanBodyMesh", "textureMaterial", "humanTex");
		SceneNode* humanLeftHand = createSceneNode(entity_name + "LeftHand", "humanLeftHandMesh", "textureMaterial", "humanTex");
		SceneNode* humanRightHand = createSceneNode(entity_name + "RightHand", "humanRightHandMesh", "textureMaterial", "humanTex");
		SceneNode* humanLeftLeg = createSceneNode(entity_name + "LeftLeg", "humanLeftLegMesh", "textureMaterial", "humanTex");
		SceneNode* humanRightLeg = createSceneNode(entity_name + "RightLeg", "humanRightLegMesh", "textureMaterial", "humanTex");

		// Setup Heirarchy
		world->AddChild(humanBody);
		humanBody->AddChild(humanLeftHand);
		humanBody->AddChild(humanRightHand);
		humanBody->AddChild(humanLeftLeg);
		humanBody->AddChild(humanRightLeg);

		// Setup part scale
		humanBody->SetScale (glm::vec3(1, 1, 1));
		humanLeftHand->SetScale(glm::vec3(1, 1, 1));
		humanRightHand->SetScale(glm::vec3(1, 1, 1));
		humanLeftLeg->SetScale(glm::vec3(1, 1, 1));
		humanRightLeg->SetScale(glm::vec3(1, 1, 1));

		// Setup part Positions
		humanBody->SetPosition(glm::vec3(0, 0, -200));

		Human* hum = new Human(humanBody, humanLeftHand, humanRightHand, humanLeftLeg, humanRightLeg);
		humans.push_back(hum);

		return hum;
	}

	// GENERALIZE TO RECEIVE DIRECTION INSTEAD OF CODING IT FOR ONLY THE FLY AND TEXTURE NAME FOR DIFFERENT TYPE OF ROCKETS
	Rocket* Game::createRocket(std::string entity_name, glm::vec3 direction, glm::vec3 pos)
	{
		SceneNode* rock = createSceneNode(entity_name, "simpleSphereMesh", "textureMaterial", "rocketTex");
		world->AddChild(rock);

		// Set initial values
		rock->SetScale(glm::vec3(0.1, 0.1, 0.1));
		rock->SetOrientation(player->body->getAbsoluteOrientation());
		rock->Rotate(glm::normalize(glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1.0, 0.0, 0.0))));
		rock->SetPosition(pos + 2.f * glm::normalize(direction));
		
		//add rockets for collision detection
		Rocket* rocket = new Rocket(rock, direction);
		rockets.push_back(rocket);

		// Set the rocket node and the direction of the rocket
		return rocket;
	}

	// GENERALIZE TO RECEIVE DIRECTION INSTEAD OF CODING IT FOR ONLY THE FLY AND TEXTURE NAME FOR DIFFERENT TYPE OF ROCKETS
	Web* Game::createWeb(std::string entity_name, glm::vec3 direction, glm::vec3 pos)
	{
		SceneNode* webNode = createSceneNode(entity_name, "simpleSphereMesh", "textureMaterial", "webTex");
		world->AddChild(webNode);

		// Set initial values
		webNode->SetScale(glm::vec3(0.1, 0.1, 0.1));
		webNode->SetOrientation(player->body->getAbsoluteOrientation());
		webNode->Rotate(glm::normalize(glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1.0, 0.0, 0.0))));
		webNode->SetPosition(pos + 2.f * glm::normalize(direction));

		//add rockets for collision detection
		Web* web = new Web(webNode, direction);
		webs.push_back(web);

		// Set the rocket node and the direction of the rocket
		return web;
	}
	
	// TARGET IS A CHILD OF CAMERA SINCE OUR TARGET IS BASED ON THE CAMERA POSITION AND MOVE WITH IT AND NOT THE PLAYER
	SceneNode* Game::createTarget(std::string entity_name)
	{
		// Create a Target instance and make it a child of camera 
		SceneNode* Target = createSceneNode(entity_name, "targetMesh", "objectMaterial", "");
		camNode->AddChild(Target);

		//Set initial values
		Target->SetScale(glm::vec3(1, 1, 1));
		Target->SetPosition(glm::vec3(0, 0, -30));

		return Target;
	}

	//GENERALIZE THIS FUNCTION
	Room* Game::createRoom()
	{
		Room* myRoom = new Room();
		float dimensionsWalls = 300; // wall dimensions

		SceneNode* floor = createSceneNode("floor", "wallMesh", "textureMaterial", "floorTex");
		world->AddChild(floor);
		floor->SetScale(glm::vec3(300, 300, 1));
		floor->Translate(glm::vec3(0.0, -30.0, 0.0));
		floor->Rotate(glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1.0, 0.0, 0.0)));

		glm::vec3 pos = floor->GetPosition();

		// y-axis is the normal of the floor and size is 600x600
		Wall* floorWall = new Wall(floor, glm::vec3(0, 1, 0), 600, 600);
		myRoom->addWall(floorWall);

		SceneNode* wall = createSceneNode("wall1", "wallMesh", "textureMaterial", "wallTex");
		world->AddChild(wall);
		wall->SetScale(glm::vec3(200, 300, 1));
		//wall->Translate(glm::vec3(0.0, 270.0, -300.0));
		wall->Translate(glm::vec3(-100.0, 270.0, -300.0));

		glm::vec3 pos1 = wall->GetPosition();

		//z-axis is the normal of this plane and size is 600x400
		Wall* myWall1 = new Wall(wall , glm::vec3(0 , 0 , 1), 600, 400);
		myRoom->addWall(myWall1);
		
		
		SceneNode* wall2 = createSceneNode("wall2", "wallMesh", "textureMaterial", "wallTex");
		world->AddChild(wall2);
		wall2->SetScale(glm::vec3(300, 300, 1));
		wall2->Translate(glm::vec3(300.0, 270.0, 0.0));
		wall2->Rotate(glm::angleAxis(glm::pi<float>() / 2, glm::vec3(0.0, 1.0, 0.0)));

		glm::vec3 pos2 = wall2->GetPosition();

		//-x-axis is the normal of this plane size is 600x600
		Wall* myWall2 = new Wall(wall2, glm::vec3(-1, 0, 0), 600, 600);
		myRoom->addWall(myWall2);
		
		SceneNode* wall3 = createSceneNode("wall3", "wallMesh", "textureMaterial", "wallTex");
		world->AddChild(wall3);
		wall3->SetScale(glm::vec3(300, 300, 1));
		wall3->Translate(glm::vec3(-300.0, 270.0, 0.0));
		wall3->Rotate(glm::angleAxis(glm::pi<float>() / 2, glm::vec3(0.0, 1.0, 0.0)));

		glm::vec3 pos3 = wall3->GetPosition();

		//x-axis is the normal of this plane and size 600x600
		Wall* myWall3 = new Wall(wall3, glm::vec3(1, 0, 0), 600, 600);
		myRoom->addWall(myWall3);
		
		SceneNode* wall4 = createSceneNode("wall4", "wallMesh", "textureMaterial", "wallTex");
		world->AddChild(wall4);
		wall4->SetScale(glm::vec3(300, 300, 1));
		wall4->Translate(glm::vec3(0.0, 270.0, 300.0));

		glm::vec3 pos4 = wall4->GetPosition();

		//-z-axis is the normal of this plane and size is 600x600
		Wall* myWall4 = new Wall(wall4, glm::vec3(0, 0, -1), 600, 600);
		myRoom->addWall(myWall4);

		//SceneNode* table = createSceneNode("table", "tableMesh", "textureMaterial", "floorTex");
		//wall->AddChild(table);

		return myRoom;
	}

	// Function to create a new SceneNode
	SceneNode* Game::createSceneNode(std::string entity_name, std::string geometryName, std::string materialName, std::string textureName )
	{
		// Get resources
		Resource *geom = resman_.GetResource(geometryName);
		if (!geom) { throw(GameException(std::string("Could not find resource \"") + geometryName + std::string("\""))); }
		Resource *mat = resman_.GetResource(materialName);
		if (!mat) { throw(GameException(std::string("Could not find resource \"") + materialName + std::string("\""))); }
		Resource *tex = resman_.GetResource(textureName);
		if (!tex) { std::cout << std::string("Could not find resource \"") + textureName + std::string("\"") << std::endl; }

		//return a new sceneNode object
		return new SceneNode(entity_name, geom, mat, tex);
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

	void Game::gameCollisionDetection()
	{
		projectileCollision();
		environmentCollision();
		EnemiesCollision();
	}

	void Game::projectileCollision()
	{
		/* Rocket Collision detection */

		/* PLAYER ROCKET COLLISION DETECTION */
		bool collided = false;
		for (int i = 0; i < rockets.size(); i++)
		{

			if (rockets[i]->collision(player->body, player->boundingRadius))
			{
				rockets[i]->node->del = true;
				rockets.erase(rockets.begin() + i);
				i--;

				//player->health -= 10;

				if (player->health <= 0)
				{
					player->body->del = true;					// Delete node from sceneGraph
					player->wings->del = true;					// Delete node from sceneGraph
					player->legs->del = true;					// Delete node from sceneGraph
					std::cout << "You are Dead" << std::endl;
					//delete player;
				}
				collided = true;
				break;
			}

			if (collided) { continue; }

			/* DRAGONFLY COLLISION DETECTION WITH ROCKETS */
			for (int j = 0; j < dragonFlies.size(); j++)
			{
				if (rockets[i]->collision(dragonFlies[j]->body, dragonFlies[j]->boundingRadius))
				{
					rockets[i]->node->del = true;
					rockets.erase(rockets.begin() + i);
					i--;

					dragonFlies[j]->health -= 10;

					if (dragonFlies[j]->health <= 0)
					{
						dragonFlies[j]->body->del = true;			// Delete node from sceneGraph
						dragonFlies[j]->leftWing->del = true;		// Delete node from sceneGraph
						dragonFlies[j]->rightWing->del = true;		// Delete node from sceneGraph
						dragonFlies[j]->legs->del = true;			// Delete node from sceneGraph
						delete dragonFlies[j];
						dragonFlies.erase(dragonFlies.begin() + j); // Delete from dragonfly vector
					}
					collided = true;
					break;
				}
			}

			if (collided) { continue; }

			/* HUMAN COLLISION DETECTION WITH ROCKETS */
			/*
			// Should not be here
			for (int k = 0 ; k < humans.size(); k++)
			{
			if (!collided && rockets[i]->collision(humans[k]->body, humans[k]->boundingRadius))
			{
			rockets[i]->rocketNode->del = true;
			rockets.erase(rockets.begin() + i);
			i--;

			humans[k]->health -= 10;

			if (humans[k]->health <= 0)
			{
			humans[k]->body->del = true;				// Delete node from sceneGraph
			humans[k]->leftLeg->del = true;				// Delete node from sceneGraph
			humans[k]->leftHand->del = true;			// Delete node from sceneGraph
			humans[k]->rightHand->del = true;			// Delete node from sceneGraph
			humans[k]->rightLeg->del = true;			// Delete node from sceneGraph
			delete humans[k];
			humans.erase(humans.begin() + k);			// Delete from human vector
			}
			collided = true;
			break;

			}
			}
			*/

			/* SPIDERS COLLISION DETECTION WITH ROCKETS */
			for (int l = 0; l < spiders.size(); l++)
			{
				if (rockets[i]->collision(spiders[l]->body, spiders[l]->boundingRadius))
				{
					rockets[i]->node->del = true;
					rockets.erase(rockets.begin() + i);
					i--;

					spiders[l]->health -= 10;

					if (spiders[l]->health <= 0)
					{
						spiders[l]->body->del = true;			// Delete node from sceneGraph
						spiders[l]->leftLeg->del = true;		// Delete node from sceneGraph
						spiders[l]->rightLeg->del = true;		// Delete node from sceneGraph
						delete spiders[l];
						spiders.erase(spiders.begin() + l);		// Delete from spiders vector
					}

					break;
				}
			}
		}


		collided = false;

		/* PLAYER WEBS COLLISION */
		for (int i = 0; i < webs.size(); i++)
		{
			/* PLAYER COLLISION WITH WEBS */
			if (webs[i]->collision(player->body, player->boundingRadius))
			{
				webs[i]->node->del = true;
				webs.erase(webs.begin() + i);
				i--;

				//player->health -= 10;

				if (player->health <= 0)
				{
					player->body->del = true;					// Delete node from sceneGraph
					player->wings->del = true;					// Delete node from sceneGraph
					player->legs->del = true;					// Delete node from sceneGraph
					std::cout << "You are Dead" << std::endl;
					//delete player;
				}
				collided = true;
				break;
			}

			/* DRAGONFLY COLLISION DETECTION WITH WEBS */
			for (int j = 0; j < dragonFlies.size(); j++)
			{
				if (webs[i]->collision(dragonFlies[j]->body, dragonFlies[j]->boundingRadius))
				{
					webs[i]->node->del = true;
					webs.erase(webs.begin() + i);
					i--;

					dragonFlies[j]->health -= 10;

					if (dragonFlies[j]->health <= 0)
					{
						dragonFlies[j]->body->del = true;			// Delete node from sceneGraph
						dragonFlies[j]->leftWing->del = true;		// Delete node from sceneGraph
						dragonFlies[j]->rightWing->del = true;		// Delete node from sceneGraph
						dragonFlies[j]->legs->del = true;			// Delete node from sceneGraph
						delete dragonFlies[j];
						dragonFlies.erase(dragonFlies.begin() + j); // Delete from dragonfly vector
					}
					collided = true;
					break;
				}
			}


			/* HUMAN COLLISION DETECTION WITH WEBS */
			/*
			// Should not be here
			for (int k = 0 ; k < humans.size(); k++)
			{
			if (!collided && webs[i]->collision(humans[k]->body, humans[k]->boundingRadius))
			{
			webs[i]->rocketNode->del = true;
			webs.erase(webs.begin() + i);
			i--;

			humans[k]->health -= 10;

			if (humans[k]->health <= 0)
			{
			humans[k]->body->del = true;				// Delete node from sceneGraph
			humans[k]->leftLeg->del = true;				// Delete node from sceneGraph
			humans[k]->leftHand->del = true;			// Delete node from sceneGraph
			humans[k]->rightHand->del = true;			// Delete node from sceneGraph
			humans[k]->rightLeg->del = true;			// Delete node from sceneGraph
			delete humans[k];
			humans.erase(humans.begin() + k);			// Delete from human vector
			}
			collided = true;
			break;

			}
			}
			*/

			/* SPIDERS COLLISION DETECTION WITH WEBS */
			for (int l = 0; l < spiders.size(); l++)
			{
				if (!collided && webs[i]->collision(spiders[l]->body, spiders[l]->boundingRadius))
				{
					webs[i]->node->del = true;
					webs.erase(webs.begin() + i);
					i--;

					spiders[l]->health -= 10;

					if (spiders[l]->health <= 0)
					{
						spiders[l]->body->del = true;			// Delete node from sceneGraph
						spiders[l]->leftLeg->del = true;		// Delete node from sceneGraph
						spiders[l]->rightLeg->del = true;		// Delete node from sceneGraph
						delete spiders[l];
						spiders.erase(spiders.begin() + l);		// Delete from spiders vector
					}
					break;
				}
			}
		}
	}

	void Game::environmentCollision()
	{
		/* PLAYER ROOM COLLISION */
		glm::vec3 norm;
		if (room->collision(player->body, player->boundingRadius, &norm))
		{
			// move it to previous position not working
			// camera_.SetPosition(camNode->getPrevAbsolutePosition());
			camera_.Translate(norm * 2.f * player->speed);
		}
		
		/* PROJECTILES ROOM COLLISION DETECTION */
		for (int k = 0; k < rockets.size(); k++)
		{
			if (room->collision(rockets[k]->node, rockets[k]->boundingRadius, &norm))
			{
				rockets.erase(rockets.begin() + k);
				k--;
			}
		}

		/* WEBS ROOM COLLISION */
		for (int w = 0; w < webs.size(); w++)
		{
			if (room->collision(webs[w]->node, webs[w]->boundingRadius, &norm))
			{
				webs.erase(webs.begin() + w);
				w--;
			}
		}

		/* DRAGONFLIES AND WALLS COLLISION */
		for (int i = 0; i < dragonFlies.size(); i++)
		{
			if (room->collision(dragonFlies[i]->body, dragonFlies[i]->boundingRadius, &norm))
			{
				dragonFlies[i]->body->Translate(norm * 2.f * dragonFlies[i]->speed);
			}
		}

		/* SPIDERS WALL COLLISION */
		for (int j = 0; j < spiders.size(); j++)
		{
			if (room->collision(spiders[j]->body, spiders[j]->boundingRadius, &norm))
			{
				if (norm == glm::vec3(0, 1, 0)) { spiders[j]->onFloor = true; }
				else { spiders[j]->onFloor = false; }
				spiders[j]->body->Translate(norm * 2.f * spiders[j]->speed);
			}
		}
		
		/* HUMANS WALL COLLISION */
		for (int h = 0; h < humans.size(); h++)
		{
			if (room->collision(humans[h]->body, humans[h]->boundingRadius, &norm))
			{
				if (norm == glm::vec3(0, 1, 0)) { humans[h]->onFloor = true; }
				else { humans[h]->onFloor = false; }
				humans[h]->body->Translate(norm * 2.f * humans[h]->speed);
			}
		}
	}

	/* ENEMY COLLISION DETECTION */
	void Game::EnemiesCollision()
	{
		/* DRAGONFLIES ENEMIES AND PLAYER COLLISION  */
		for (int i = 0; i < dragonFlies.size(); i++)
		{
			//player collision
			if (player->collision(dragonFlies[i]->body, dragonFlies[i]->boundingRadius))
			{
				camera_.Translate((float)player->speed * 2.f * -camera_.GetForward());
			}

			//Other dragonflies collision
			for (int m = 0; m < dragonFlies.size(); m++)
			{
				if (i != m && dragonFlies[i]->collision(dragonFlies[m]->body, dragonFlies[m]->boundingRadius))
				{
					dragonFlies[i]->body->Translate(-dragonFlies[i]->getDirection() * 3.f * dragonFlies[i]->speed);
				}
			}

			//spiders collision
			for (int w = 0; w < spiders.size(); w++)
			{
				if (dragonFlies[i]->collision(spiders[w]->body, spiders[w]->boundingRadius))
				{
					dragonFlies[i]->body->Translate(-dragonFlies[i]->getDirection() * 3.f * dragonFlies[i]->speed);
				}
			}

			/*
			//humans collision
			for (int q = 0; q < humans.size(); q++)
			{
				if (dragonFlies[i]->collision(humans[q]->body, humans[q]->boundingRadius))
				{
					dragonFlies[i]->body->Translate(glm::normalize(dragonFlies[i]->body->getAbsolutePosition() - player->body->getAbsolutePosition()) * 2.f * dragonFlies[i]->speed);
				}
			}
			*/
		}

		/* SPIDERS PLAYER AND ENEMIES COLLISION */
		for (int j = 0; j < spiders.size(); j++)
		{
			//player collision
			if (player->collision(spiders[j]->body, spiders[j]->boundingRadius))
			{
				camera_.Translate((float)player->speed * 2.f * -camera_.GetForward());
			}

			//other spiders collision
			for (int m = 0; m < spiders.size(); m++)
			{
				if (j != m &&  spiders[j]->collision(spiders[m]->body, spiders[m]->boundingRadius))
				{
					spiders[j]->body->Translate(-spiders[j]->getDirection() * 2.f * spiders[j]->speed);
				}
			}

			//dragonfly collision
			for (int n = 0; n < dragonFlies.size(); n++)
			{
				if (spiders[j]->collision(dragonFlies[n]->body, dragonFlies[n]->boundingRadius))
				{
					spiders[j]->body->Translate(-spiders[j]->getDirection() * 2.f * spiders[j]->speed);
				}
			}

			/*
			//human collision
			for (int m = 0; m < humans.size(); m++)
			{
				if (spiders[j]->collision(spiders[m]->body, spiders[m]->boundingRadius))
				{
					spiders[j]->body->Translate(glm::normalize(spiders[j]->body->getAbsolutePosition() - player->body->getAbsolutePosition()) * 3.f * spiders[j]->speed);
				}
			}
			*/
		}

		/* HUMANS PLAYER AND ENEMIES COLLISION */
		for (int k = 0; k < humans.size(); k++)
		{
			if (player->collision(humans[k]->body, humans[k]->boundingRadius))
			{
				camera_.Translate((float)player->speed * 2.f * -camera_.GetForward());
			}

			/*
			for (int m = 0; m < dragonFlies.size(); m++)
			{
			if (humans[k]->collision(humans[m]->body, humans[m]->boundingRadius))
			{
			std::cout << "hello!!!" << std::endl;
			}
			}


			for (int m = 0; m < dragonFlies.size(); m++)
			{
			if (spiders[j]->collision(spiders[m]->body, spiders[m]->boundingRadius))
			{
			std::cout << "hello!!!" << std::endl;
			}
			}


			for (int m = 0; m < dragonFlies.size(); m++)
			{
			if (spiders[j]->collision(spiders[m]->body, spiders[m]->boundingRadius))
			{
			std::cout << "hello!!!" << std::endl;
			}
			}
			*/
		}
	}
} 
// namespace game
