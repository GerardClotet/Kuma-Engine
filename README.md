# KUMA ENGINE

Kuma Engine is a Game Engine made by Pol Recasens Sarrà and Gerard Clotet Rodríguez,
two students from CITM UPC university for the 3D engines subject during 3rd year of Game Design and Game Development degree.

Link to Github Repository: [Kuma Engine GitHub](https://github.com/GerardClotet/Kuma-Engine)

# Authors Githubs
[Pol Recasens Sarrà](https://github.com/PolRecasensSarra) GitHub page

[Gerard Clotet Rodríguez](https://github.com/GerardClotet) GitHub page

# Game Engine Structure

## Main Menu Bar
- File
	- Quit
	- Load Model
	- Save Scene
	- Load Scene
-View
	- Console(panel)
	- Configuration(panel)
	- Hierarchy(panel)
	- Time Clocks(panel)
- GameObject
	- Inspector(panel)
	- Create Empty
		- Creates an empty game object
	- 3D objects
		- Cube
		- Sphere
		- Cone
		- Plane
		- Cylinder
		- Dodecahedron
		- Torus
	
- Components
	- Materials
	- Mesh
	- Transform
	- #### Things to keep in mind: 
		- If you want to put a texture to a game object that doesn't have a component material, this component will be created automatically, to ease the process of component creation. The same happens when you load a fbx or create a shape, automatically the mesh component is created. Mesh and Material components only will be created if the game object can handle them(an empty game object can't have this two components due to it's nature). The transform component is setted by default i ALL game objects, including empty ones.
	
- Help
	- Gui Demo
	- Documentation
	- Download latest
	- Report a bug
	- About
  
 ## Panels
  - Panel Console
    - Output of errors and things you have loaded
  
  - Panel Configuration
    - Options
      - Load and save a json file
      
    - Application 
      - Info about the Engine
      - Framerate
      - Memory consumption
      - Memory info
      
    - Window
      - Window size and window options
      
    - File System
       - Info about engine route path
       
    - Input
      - Info about mouse and keyboard input
      
    - Hardware
      - Info about the computer's hardware
      
    - Render
      - Customize render options
      - Normals, vertex, colors, light, wireframe...
      - Debug mouse picking ray and change color
      - Debug quadtree AABB
    
    - Scene Camera
      - Customize the camera view options
  
  - Panel Hierarchy
    - It shows the hierarchy of all game objects that are in the scene
    
  - Time Clocks
    - It shows the scene and the game time
  
  - Panel Inspector
    - Game object name editor
    - Hide game object
    - Delete game object
    - Transform info
    - Mesh info
    - Texture info

## Controls
- Camera Movement
	- [LALT] + Middle mouse button or using "WASD"
- Orbit camera
	- [LALT]	+ LEFT CLICK
- Zoom in/out
	- Mouse wheel or [LALT] + (drag)RIGHT CLICK
- Focus
	- [F]
- Select/Pick
	- RIGHT CLICK

### Transform Gizmos
- [W] = Position
	
- [E] = Rotate
	
- [R] = Scale
	

## Member Task List
- Gerard Clotet Rodríguez
	- Hierarchy
	- Serialization
	- Own file format
	- Resource Managment

- Pol Recasens Sarrà
	- Component transform
	- Panel Inspector
	- Mouse Picking
	- Frustum Culling
	- Component Camera
	- Quadtree
	- Own file format
	- Time Managment
	- Game objects AABB

