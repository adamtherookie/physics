#include "raylib.h"
#include <vector>
#include <iostream>

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
  const int screenWidth = 800;
  const int screenHeight = 450;
  
  InitWindow(screenWidth, screenHeight, "fiziks");
  SetTargetFPS(60);

  object circle = {
    {100, 0}, // Start position
    {0, 0}, // Start velocity
    {0, 0}, // Start force
    1 // Mass
  };

  world.push_back(&circle);

  Vector2 ballPosition = { circle.position.x, circle.position.y };

  while (!WindowShouldClose()) {
    BeginDrawing();
    
    ClearBackground(BLACK);
    DrawText("Hey there!", 190, 200, 20, LIGHTGRAY);
   
    step(0.1);
    ballPosition = { circle.position.x, circle.position.y };
    DrawCircleV(ballPosition, 50, MAROON);

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

