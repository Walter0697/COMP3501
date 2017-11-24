 #include <iostream>
#include <time.h>
#include <sstream>

#include "game.h"
#include "bin/path_config.h"

// EVEN AFTER COLLISION ENEMIES STILL SHOOT?????
// HUD?????
// HEMISPHERICAL LIGHTING ??????????
// IF NOT DO IT WITH SPHERES 
// SHADOW MAPPING ????
// SPLINE TRAJECTORIES 
// PARTICLE SYSTEMS FOR THE DEATH OR DESTRUCTION OF OBJS
// MOVING THE ENEMY WITH THE PLAYER ????
// HOW SOFISTICATED SHOULD OUR AI BE ???

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

		/* MATERIAL GLSL FILES */
		// OBJECT MATREIAL FOR GENERAL OBJECTS
		std::string filename = std::string(MATERIAL_DIRECTORY) + std::string("/material");
		resman_.LoadResource(Material, "objectMaterial", filename.c_str());

		// TEXTURE MATERIAL FOR OBJECTS THAT HAVE TEXTURES
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/texture");
		resman_.LoadResource(Material, "textureMaterial", filename.c_str());

		/* TEXTURES */
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
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/floor-texture.jpg");
		resman_.LoadResource(Texture, "floorTex", filename.c_str());
		filename = std::string(MATERIAL_DIRECTORY) + std::string("/textures/wall-texture.png");
		resman_.LoadResource(Texture, "wallTex", filename.c_str());

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
		room1 = createEnvironment(glm::vec3(0, 0, 0), "1");
  		room2 = createEnvironment(glm::vec3(500, 500, 0), "2");
  		room3 = createEnvironment(glm::vec3(-500, 500, 0), "3");
	}

	void Game::MainLoop(void)
	{
		// Loop while the user did not close the window
		while (!glfwWindowShouldClose(window_)) 
		{
			checkInput(); // Check for input

			/* Check collision with bullets and all characters (friendly fire is fine) */
			// Bullets and all characters
			// Webs and player
			// Human attack with player
			// 

			/* Check collision with bullets and walls and environment */

			bool collided = false;

			for (int i = 0; i < rockets.size(); i++)
			{
				for (int j = 0; j < dragonFlies.size(); j++)
				{
					if (rockets[i]->collision(dragonFlies[j]->body))
					{
						rockets[i]->rocketNode->del = true;
						rockets.erase(rockets.begin() + i);
						i--;

						dragonFlies[j]->body->del = true;			// Delete node from sceneGraph
						dragonFlies[j]->leftWing->del = true;		// Delete node from sceneGraph
						dragonFlies[j]->rightWing->del = true;		// Delete node from sceneGraph
						dragonFlies[j]->legs->del = true;			// Delete node from sceneGraph
						dragonFlies.erase(dragonFlies.begin() + j); // Delete from dragonfly vector
						collided = true;
						break;
					}
				}

				// Should not be here
				for (int k = 0 ; k < humans.size(); k++)
				{
					if (!collided && rockets[i]->collision(humans[k]->body))
					{
						rockets[i]->rocketNode->del = true;
						rockets.erase(rockets.begin() + i);
						i--;

						humans[k]->body->del = true;				// Delete node from sceneGraph
						humans[k]->leftLeg->del = true;				// Delete node from sceneGraph
						humans[k]->leftHand->del = true;			// Delete node from sceneGraph
						humans[k]->rightHand->del = true;			// Delete node from sceneGraph
						humans[k]->rightLeg->del = true;			// Delete node from sceneGraph
						humans.erase(humans.begin() + k);			// Delete from human vector
						collided = true;
						break;

					}
				}

				for (int l = 0; l < spiders.size(); l++)
				{
					if (!collided && rockets[i]->collision(spiders[l]->body))
					{
						rockets[i]->rocketNode->del = true;
						rockets.erase(rockets.begin() + i);
						i--;

						spiders[l]->body->del = true;			// Delete node from sceneGraph
						spiders[l]->leftLeg->del = true;		// Delete node from sceneGraph
						spiders[l]->rightLeg->del = true;		// Delete node from sceneGraph
						spiders.erase(spiders.begin() + l);		// Delete from spiders vector
						break;
					}
				}
			}

			/* Check collision with characters and other characters */


			/* Check collision with characters and the environment */

	
			player->update();

			if (human)
			{
				human->updateTarget(player->body->getAbsolutePosition());
				human->updateTargetOrientation(player->body->getAbsoluteOrientation());
				human->update();
			}

			if (spider)
			{
				spider->updateTarget(player->body->getAbsolutePosition());
				spider->updateTargetOrientation(player->body->getAbsoluteOrientation());
				spider->update();
			}

			if (dragonFly)
			{
				dragonFly->updateTarget(player->body->getAbsolutePosition());
				dragonFly->updateTargetOrientation(player->body->getAbsoluteOrientation());
				dragonFly->update();
			}

			// Human firing
			// Create new function which fires the shots rather than just having them appear
			if (human->getFiring()) { human->fire(createRocket("Rocket2", player->body->getAbsolutePosition() - human->body->getAbsolutePosition(), human->body->getAbsolutePosition())); }

			// Spider firing
			if (spider->getFiring()) { spider->fire(createRocket("Rocket3", player->body->getAbsolutePosition() - spider->body->getAbsolutePosition(), spider->body->getAbsolutePosition())); }

			// Dragonfly firing
			if (dragonFly->getFiring()) { dragonFly->fire(createRocket("Rocket4", player->body->getAbsolutePosition() - dragonFly->body->getAbsolutePosition(), dragonFly->body->getAbsolutePosition())); }

			scene_.Draw(&camera_);		// Draw the scene
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
		if (glfwGetKey(window_, GLFW_KEY_X)) { camera_.Roll(rot_factor); }

		// TO BE CHANGED!!!!!!!!!!!!!! (movement up and down)
		if (glfwGetKey(window_, GLFW_KEY_I)) { camera_.Translate(camera_.GetUp() * player->speed); }
		if (glfwGetKey(window_, GLFW_KEY_K)) { camera_.Translate(-camera_.GetUp() * player->speed); }

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
		flyBody->SetPosition(glm::vec3(0, -0.3, -1.5));

		return new Fly(flyBody, flyWings, flyLegs);
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
		spiderBody->SetScale(glm::vec3(glm::vec3(0.002, 0.002, 0.002)));
		spiderLeftLeg->SetScale(glm::vec3(0.002, 0.002, 0.002));
		spiderRightLeg->SetScale(glm::vec3(glm::vec3(0.002, 0.002, 0.002)));

		// Setup parts positions
		spiderBody->SetPosition(glm::vec3(10, 0, -20));

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
		dragonFlyBody->SetScale(glm::vec3(20, 20, 20));
		dragonFlyLeftWing->SetScale(glm::vec3(20, 20, 20));
		dragonFlyRightWing->SetScale(glm::vec3(20, 20, 20));
		dragonFlyLegs->SetScale(glm::vec3(20, 20, 20));

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
		rock->SetPosition(pos + (float)2.0 * glm::normalize(direction));
		
		//add rockets for collision detection
		Rocket* rocket = new Rocket(rock, direction);
		rockets.push_back(rocket);

		// Set the rocket node and the direction of the rocket
		return rocket;
	}
	
	// TARGET IS A CHILD OF CAMERA SINCE OUR TARGET IS BASED ON THE CAMERA POSITION AND MOVE WITH IT AND NOT THE PLAYER
	SceneNode* Game::createTarget(std::string entity_name)
	{
		// Create a Target instance and make it a child of camera 
		SceneNode* Target = createSceneNode(entity_name, "targetMesh", "objectMaterial", "");
		camNode->AddChild(Target);

		//Set initial values
		Target->SetScale(glm::vec3(1, 1, 1));
		Target->SetPosition(glm::vec3(0, 0, -200));

		return Target;
	}

	Environment *Game::createEnvironment(glm::vec3 position, std::string id)
	{
		SceneNode* floor = createSceneNode("floor", "wallMesh", "textureMaterial", "floorTex");
		world->AddChild(floor);

		floor->SetScale(glm::vec3(300, 300, 1));
		floor->Translate(glm::vec3(0.0, -30.0, 0.0));
		floor->Rotate(glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1.0, 0.0, 0.0)));

		SceneNode* wall = createSceneNode("wall1" + id, "wallMesh", "textureMaterial", "wallTex");
		world->AddChild(wall);
		wall->SetScale(glm::vec3(300, 300, 1));
		wall->Translate(glm::vec3(0.0, 270.0, -300.0));

		SceneNode* wall2 = createSceneNode("wall2" + id, "wallMesh", "textureMaterial", "wallTex");
		world->AddChild(wall2);
		wall2->SetScale(glm::vec3(300, 300, 1));
		wall2->Translate(glm::vec3(300.0, 270.0, 0.0));
		wall2->Rotate(glm::angleAxis(glm::pi<float>() / 2, glm::vec3(0.0, 1.0, 0.0)));

		SceneNode* wall3 = createSceneNode("wall3" + id, "wallMesh", "textureMaterial", "wallTex");
		world->AddChild(wall3);
		wall3->SetScale(glm::vec3(300, 300, 1));
		wall3->Translate(glm::vec3(-300.0, 270.0, 0.0));
		wall3->Rotate(glm::angleAxis(glm::pi<float>() / 2, glm::vec3(0.0, 1.0, 0.0)));

		SceneNode* wall4 = createSceneNode("wall4" + id, "wallMesh", "textureMaterial", "wallTex");
		world->AddChild(wall4);
		wall4->SetScale(glm::vec3(300, 300, 1));
		wall4->Translate(glm::vec3(0.0, 270.0, 300.0));

		//SceneNode* table = createSceneNode("table", "tableMesh", "textureMaterial", "floorTex");
		//wall->AddChild(table);
		
		floor->Translate(position);

		return new Environment(floor);
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
} // namespace game
