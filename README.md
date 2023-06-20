![NDot Engine](/src/assets/NDot_Engine_logo.svg)

NDot Engine
===========

The NDot Engine is a new/modern game engine designed for use with the N64, and hopefully be portable to PC/Linux, and other low-spec systems.

While it currently uses libdragon, but may switch over to the SDL2_N64 project to aid in portability.

It aims to be easy to use and make games with, taking heavy inspiration from the Godot Engine's node system where rather than an Entity Component System, the nodes themselves have their behaviors built in.

Currently, it is in its very early stages, but we intend to take full advantage of the N64's 3D capabilities, as well as support 2D scenes.

When it is complete, it should have the following features:

- Visual Server
	- All visual entities, upon entereing the scenetree will register themselves with their respective visual server.
	- All visual entities are able to toggle their visibility.
	- 3D scene server(High Priority)
		- 3D math
			- basis orientations
			- quaternions
			- euler rotations
		- Builtin 3D primitives
		- 3D Particle system
		- 3D audio
		- Meshes with vertex colors, textures, and animations by:
			- sausage-link
			- rigid-skinning
			- vertex deformation
		- Billboards
		- A built-in level format with collisions and occlusion culling
			- TBD how this will be done
		
	- 2D scene server(Low Priority)
		- 2D math(duh)
		- Builtin 2D polygon primitives
		- Custom shapes
		- Textured Quads
		- Animated sprites
	- UI Server
		- A basic widget that's more or less a panel with support for:
			- Text content
			- Image content
			- Input events
			- Selectability
			- Visual styling(border, color, margins, padding, position, etc.)
- Simple collision server
	- Like the Visual Server, all collision-enabled entities will register themselves with the collision server upon scene entry
	- A simple 3D collision system supporting Static and Kinematic 3D Bodies with the following shapes:
		- Raycasts
		- Triangles
		- Axis-Aligned Bounding Boxes
		- Spheres
		- Elipsoids
	- A simple 2D collision system supporting static and Kinematic 2D Bodies with the following shapes:
		- Raycasts
		- Lines
		- Boxes
		- Spheres
- Audio Server
	- Like the Visual and Collision servers, all audio-emitting entities will register themselves with the Audio Server upon scenetree entry.
	- sample playback
	- Music streaming using the .XM format
- Input Abstraction Layer
	- Definitions of actions and their respective input
- Animation system
	- uses keyframes to change values of various properties of various nodes over time
- Timers
- Groups
- Events
- File loading system to handle loading of assets(such as models, textures, audio), and scenes.

Most of these will be exposed through nodes, placing a priority on implementing 3D-related nodes first. To create your game, you will need to choose a node or set of nodes in C++ and extend their behavior through the following virtual functions:

	_ready()
	_update(float delta_time)
	_enter_tree()
	_exit_tree()

