#include "raylib.h"
#include <vector>
#include <iostream>
#include <string>

// 2D vector
typedef struct vec {
  float x;
  float y;
} vec;

// An object, has a position, velocity, force applied to it, and mass
struct object {
  vec position;
  vec velocity;
  vec force;

  float mass;
  float radius; // Only circles sadly, who needs other stuff anyways

  Color color;
};

// Function defs, the rest is under main
void addObject(object *obj); // Add object to the world
void step(float dt); // Simulate the damn thing!

// Contains all objects currently being simulated
std::vector<object *> world;

// Gravity on earth I guess
vec gravity = { 0, 9.81f };

int main(void) {
  // Raylib stuff
  const int screenWidth = 1366;
  const int screenHeight = 768;
  
  InitWindow(screenWidth, screenHeight, "fiziks");
  SetTargetFPS(60);

  object circle1 = {
    {0, 500}, // Start position
    {50, -50}, // Start velocity
    {0, 0}, // Start force
    1, // Mass
    10, // Radius
    MAROON, // Color
  };

  object circle2 = {
    {0, 500},
    {50, -40},
    {0, 0},
    10,
    20,
    BLUE,
  };

  world.push_back(&circle1);
  world.push_back(&circle2);

  while (!WindowShouldClose()) {
    BeginDrawing();
    
    ClearBackground(BLACK);
   
    step(0.1);

    for(auto obj : world) {
      Vector2 ballPosition = { obj->position.x, obj->position.y  };
      DrawCircleV(ballPosition, obj->radius, obj->color);
      DrawText(std::to_string(obj->mass).c_str(), obj->position.x + obj->radius, obj->position.y + obj->radius, 2 * obj->radius, obj->color);
    }

    EndDrawing();
  }
  
  CloseWindow();

  return 0;
}

void addObject(object *obj) {
  world.push_back(obj);
}

void step(float dt) {
  for(object *obj : world) {
    obj->force.x += obj->mass * gravity.x; // Apply force
    obj->force.y += obj->mass * gravity.y;

    obj->velocity.x += (obj->force.x / obj->mass) * dt;
    obj->velocity.y += (obj->force.y / obj->mass) * dt;

    obj->position.x += obj->velocity.x * dt;
    obj->position.y += obj->velocity.y * dt;

    obj->force.x = 0;
    obj->force.y = 0; // Reset net force at the end
  }
}

