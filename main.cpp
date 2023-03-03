#include "raylib.h"
#include <vector>
#include <stdlib.h>
#include <cmath>

#include "config.h"

std::vector<object *> universe;

float dist(vec a, vec b) {
  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// The actual update function
void step(float dt) {
  // Update each object in the universe
  for (auto obj : universe) {
    // Check if the object went above the screen, and make it go back
    if(obj->position.y + obj->radius > screenHeight) {
      obj->position.y = screenHeight - obj->radius; // reset the object to the top of the screen
                                                    // in case it went above and beyond
      obj->velocity.y *= -obj->bounciness; // bounce it off the wall according to its elasticity
      obj->force.y += -k * obj->velocity.y; // friction, we have f = k*v
      obj->force.x += -k * obj->velocity.x;
    }

    // Check if the object went below the screen, and make it go back
    if(obj->position.y - obj->radius < 0) {
      obj->position.y = obj->radius;  // Same explanation as before
      obj->velocity.y *= -obj->bounciness; 
      obj->force.y += -k * obj->velocity.y;
      obj->force.x += -k * obj->velocity.x;
    }

    // Check if the object went off the right of the screen
    if(obj->position.x + obj->radius > screenWidth) {
      obj->position.x = screenWidth - obj->radius;
      obj->velocity.x *= -obj->bounciness;
      obj->force.y += -k * obj->velocity.y;
      obj->force.x += -k * obj->velocity.x;
    }

    // Check if the object went off the left of the screen
    if(obj->position.x - obj->radius < 0) {
      obj->position.x = obj->radius;
      obj->velocity.x *= -obj->bounciness;
      obj->force.y += -k * obj->velocity.y;
      obj->force.x += -k * obj->velocity.x;
    }

    for (auto other : universe) {
      if(obj == other) continue;
      
      float distance = dist(obj->position, other->position);
      
      if (distance < obj->radius + other->radius) {
        vec n;
        n.x = (other->position.x - obj->position.x) / distance;
        n.y = (other->position.y - obj->position.y) / distance;
        float p = 2 * (obj->velocity.x * n.x + obj->velocity.y * n.y - other->velocity.x * n.x - other->velocity.y * n.y)  / (obj->mass + other->mass);

        obj->velocity.x -= p * obj->mass * n.x;
        obj->velocity.y -= p * obj->mass * n.y;

        other->velocity.x += p * obj->mass * n.x;
        other->velocity.y += p * obj->mass * n.y;
      }
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

