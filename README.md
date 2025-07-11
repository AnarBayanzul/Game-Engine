# Indie-style Horror Demo Game
A minimalist point and click game where you navigate through a house and unlock rooms to go to sleep.

**For more information on the game engine itself and features, scroll to [bottom.](https://github.com/AnarBayanzul/Game-Engine#game-engine)**

A release for the demo game is available to play.

![alt](https://github.com/AnarBayanzul/Game-Engine/blob/master/Preview%20Media/combined_horizontal.jpg?raw=true)

_An example from the game showing the layered shader effects to create a unique "indie-style" look._

## Shader Effects
- Blinn-Phong and point lighting
- Bloom
- Posterization
- Edge-highlighting kernel effect

## Other Features
- Background and event-based audio
- 3D modeled-house, furniture, and small items
- Scene transitions
- Persistent inventory state

# Game Engine

3D Game engine made in the University of Notre Dame's Advanced Game Development class in the Spring 2025 semester taught by Daniel Rehberg.

![alt text](https://github.com/AnarBayanzul/Game-Engine/blob/master/Preview%20Media/demo_example.jpg?raw=true)

## Engine Features

Engine abstracts away implementation details for simple API calls for potential game developer.

> SDL environment for creating a general space for rendering graphics. This abstracts away OS specific window management, provides low-level access to I/O devices, and creates a rendering context for OpenGL

> Uses self-made quaternion class for object rotations, preventing Gimbal lock.

> **Scene Graph that allows for parent-child transformation inheritance.**

> Uses DES (discrete-event simulation) model with initialization step and indefinite logic loop

**Custom filetype for meshes:**
- Interprets custom mesh filetype (or text file) in runtime
- Python script using BPY to export a mesh from Blender to the custom filetype or text file
- File includes:
	- Vertex positions
	- Vertex normals
 	- Texture UV coordinates

**Uses OOP, inheritance, and polymorphism:**
- Game Object and Scene classes with polymorphic potential
	- Custom Game Object classes
	- Storing an array of Game Objects (including custom Game Objects) to be managed by engine.
- Abstract game object class can arbitrarily and interchangibly change meshes and textures.

**Reads and compiles custom shader programs in runtime.**
- MVP transformation in shaders

**Deferred Rendering:**
- FBO class for creating custom framebuffers
- Allows for multiple render passes and framebuffers
- Can attach an arbitrary number of textures

**Various shader and post processing effects:**
- Blinn-Phong lighting
- Point lighting with arbitrary number of lights
- Bloom
- Posterization
- Edge-detecting and highlighting image processing kernel

**Audio Rendering:**
- Automatically manages audio device
- Event-driven, object-based playback
- Multiple sounds, with volume control and looping
- Allows for both pre-loaded and dynamically loaded audio

**3D rigid body simulation**
- time integration of position, velocity, and acceleration, both linear and angular.

**Collisions and Event-Driven Interactions**
- Uniform grid spatial hierarchy that reduces collision testing to less than $O(n^2)$
- Broad-phase collision detection through axis-aligned bounding boxes (AABB)
- Narrow-phase collision detection on convex shapes through Gilbert-Johnson Keerthi (GJK) algorithm
	- Returns true on collision, minimum distance between objects on no collision
- User provided function runs on collision between object types for event-driven interation.

## Tools
Uses SDL, OpenGL, and other support libraries.

- SDL2-2.30.11
- glew-2.1.0
- GLM 1.0.1
- freeglut 3.0.0-1.mp for MSVC
