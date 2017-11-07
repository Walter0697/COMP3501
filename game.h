#ifndef GAME_H_
#define GAME_H_

#include <exception>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene_graph.h"
#include "resource_manager.h"
#include "camera.h"
#include "asteroid.h"
#include "Rocket.h"
#include "CameraNode.h"
#include "fly.h"
#include "Human.h"

namespace game 
{
    // Exception type for the game
    class GameException: public std::exception
    {
        private:
            std::string message_;
        public:
            GameException(std::string message) : message_(message) {};
            virtual const char* what() const throw() { return message_.c_str(); };
            virtual ~GameException() throw() {};
    };

    // Game application
    class Game 
	{
        public:
            // Constructor and destructor
            Game(void);
            ~Game();

            void Init(void);				// Call Init() before calling any other method
            void SetupResources(void);		// Set up resources for the game
            void SetupScene(void);			// Set up resources for the game
            void MainLoop(void);			// Run the game: keep the application active

        private:
            GLFWwindow* window_;			// GLFW window
            SceneGraph scene_;				// Scene graph containing all nodes to render
			ResourceManager resman_;		// Resources available to the game
            Camera camera_;					// Camera abstraction
			CameraNode* camNode;
            bool animating_;				// Flag to turn animation on/off
			Fly* player;					// Player fly
			SceneNode* target;				// Target for shooting
			SceneNode* world;				// Dummy for root of the heirarchy
			Human* human;					// human enemy
			// STORE COLLIDABLES OR SMTHG

            // Methods to initialize the game
            void InitWindow(void);
            void InitView(void);
            void InitEventHandlers(void);
 
            // Methods to handle events
            static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
			static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
            static void ResizeCallback(GLFWwindow* window, int width, int height);
			
			void checkInput();
			std::vector<Resource*> loadAssetResources(std::string, std::string, std::string);
            Asteroid* CreateAsteroidInstance(std::string entity_name, std::string object_name, std::string material_name);							// Create instance of one asteroid
			Rocket* createRocket(std::string , std::string , std::string,std::string);																// Asteroid field
            void CreateAsteroidField(int num_asteroids = 1500);																						// Create entire random asteroid field
			Fly* createFly(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name);					// Create a fly instance
			Human* createHuman(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name);
			SceneNode* createTarget(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name = std::string(""));
	}; // class Game
} // namespace game
#endif // GAME_H_
