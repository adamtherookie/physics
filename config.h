#ifndef __CONFIG_H__
#define __CONFIG_H__

// This file contains all of the configurations you need

//
// BEGIN DON'T TOUCH PART
//
// A 2D vector, has X and Y coords
struct vec {
  float x;
  float y;
};

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

//
// END DON'T TOUCH PART
//

// Touch this part:
const int screenWidth = 1366; // My monitor resolution, change it to fit yours
const int screenHeight = 768;

// Gravity on planet earth (change if you want to simulate physics elsewhere)
vec gravity = { 0, 9.81 };

float k = 1; // Friction on the screen edges

// Define the objects you want to simulate here:
object circle1 = {
  { 500, 300 },  // position
  { 10, -60 },   // velocity
  { 0, 0 },      // force

  20,            // radius
  5,             // mass
  0.6,

  MAROON         // color
};

object circle2 = {
  { 800, 300 },
  { -50, -50 },
  { 0, 0 },

  20,
  10,
  0.65,

  BLUE
};

#endif

