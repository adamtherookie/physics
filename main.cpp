#include "raylib.h"
#include <vector>
#include <stdlib.h>

const int screenWidth = 1366; // My monitor resolution, change it to fit yours
const int screenHeight = 768; //

// A 2D vector, has X and Y coordinates 
typedef struct vec {
  float x;
  float y;
}vec;

// An object, could be anything but we made it a circle because who 
// doesn't like circles, plus it's easier to simulate circles because 
// they only have 2 defining characteristics: their center and their radius
// which leaves us tons of time to focus on the hard stuff instead of how
// to render and calculate collisions and stuff between other shapes
struct object {
  vec position;
  vec velocity;
  vec force;

  float radius;  
  float mass;
  float bounciness;

  Color color;
};

// The universe (it's just a big collection of all the objects
// we will be simulating in a 2D vector (in C++'s definintion not
// the physics definition))
std::vector<object *> universe;

// Gravity on planet earth
vec gravity = { 0, 9.81 };

// The actual update function
void step(float dt) {
  // Update each object in the universe
  for (auto obj : universe) {
    // Check if the object went above the screen, and make it go back
    if(obj->position.y + obj->radius > screenHeight) {
      obj->position.y = screenHeight - obj->radius; // reset the object to the top of the screen
                                                    // in case it went above and beyond
      obj->velocity.y *= -obj->bounciness; // bounce it off the wall according to its elasticity
      obj->force.y += -1 * obj->velocity.y; // friction, we have f = k*v, and we chose k for the
                                            // screen's edge to be -1, though it still seems a bit
                                            // slippery, like its moving on ice. we should change that
      obj->force.x += -1 * obj->velocity.x;
    }

    // Check if the object went below the screen, and make it go back
    if(obj->position.y - obj->radius < 0) {
      obj->position.y = obj->radius;  // Same explanation as before
      obj->velocity.y *= -obj->bounciness; 
      obj->force.y += -1 * obj->velocity.y;
      obj->force.x += -1 * obj->velocity.x;
    }

    // Check if the object went off the right of the screen
    if(obj->position.x + obj->radius > screenWidth) {
      obj->position.x = screenWidth - obj->radius;
      obj->velocity.x *= -obj->bounciness;
      obj->force.y += -1 * obj->velocity.y;
      obj->force.x += -1 * obj->velocity.x;
    }

    // Check if the object went off the left of the screen
    if(obj->position.x - obj->radius < 0) {
      obj->position.x = obj->radius;
      obj->velocity.x *= -obj->bounciness;
      obj->force.y += -0.1 * obj->velocity.y;
      obj->force.x += -0.1 * obj->velocity.x;
    }

    // Add the effect of gravity to the Y-force, we have: P = m*g
    obj->force.y += obj->mass * gravity.y;

    // Add the effect of velocity to the force
    obj->force.x += obj->velocity.x * dt;
    obj->force.y += obj->velocity.y * dt;

    // Make the force work, changes into velocity
    // we have F = m*a => a = (F / m)
    obj->velocity.x += (obj->force.x / obj->mass) * dt;
    obj->velocity.y += (obj->force.y / obj->mass) * dt;

    // Update the position according to the current velocity
    obj->position.x += obj->velocity.x * dt;
    obj->position.y += obj->velocity.y * dt;

    // Reset net force to 0
    obj->force.x = 0;
    obj->force.y = 0;
  }
}

int main(void) {
  // Raylib stuff
  InitWindow(screenWidth, screenHeight, "physics");
  SetTargetFPS(60);

  // Define objects
  object circle1 = {
    { 500, 300 },  // position
    { 50, -50 },    // velocity
    { 0, 0 },    // force

    20,          // radius
    5,           // mass
    0.6,

    MAROON       // color
  };

  object circle2 = {
    { 800, 300 },
    { -50, -50 },
    { 0, 0 },

    50,
    10,
    0.2,

    BLUE
  };

  // Add the objects to the universe (basically give birth to them)
  universe.push_back(&circle1);
  universe.push_back(&circle2);

  // The update loop
  while(!WindowShouldClose()) {
    BeginDrawing();
    
    ClearBackground(BLACK);
    
    step(0.1); // We chose dt = 0.1 as a nice step for our update function
               // it gave the most natural feeling physics out of all the
               // powers of 10

    // Loop over all the objects in the universe, and draw a circle at their position,
    // with their radius and their color
    for (auto obj : universe) {
      Vector2 ballPosition = { obj->position.x, obj->position.y };
      DrawCircleV(ballPosition, obj->radius, obj->color);
    }

    EndDrawing();
  }

  return 0;
}

