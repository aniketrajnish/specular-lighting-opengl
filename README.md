# Specular lighting in OpenGL
 Specular Lighting in OpenGL (Followed tutorial by [Michael Grieco](https://michaelg29.github.io/)).  

https://user-images.githubusercontent.com/58925008/153727298-6eea2a8c-8e6f-4ddd-b9a8-e7ac1701fc9a.mp4

# Setup
* Clone
 ```
 $ git clone https://github.com/aniketrajnish/specular-lighting-opengl.git
 ```
* Open the solution file using VS 2014 or newer.

# Changing the texture
* Import the albedo and specular maps in the \assets folder.
* Change the albedo and specular names in cube.hpp
```
Texture tex("assets/your_name.png", "material.diffuse");
Texture tex_specular("assets/your_name.png", "material.specular");
```
# Changing the background
* Change the RGBA values of colour buffer in the rendering loop of main.cpp
```
glClearColor(// your RGBA here);
```
# Changing the properties of Cubes
Properties of cubes can be changed by making the following changes in main.cpp
* Declaration
```
Cube cubes[//desired number];
```
* Adding / Removing position vectors of the cubes accordingly
```
glm::vec3 cubePositions[] = {
    // add / remove here	
};
 ```
* Initialization
```
for (unsigned int i = 0; i < //desired number; i++) {
    cubes[i] = Cube(cubePositions[i], glm::vec3(//desired size));
    cubes[i].init();    
}
 ```
 # Changing the properties of Lamps
Properties of lamps can be changed by making the following changes in main.cpp
* Declaration
```
Lamp lamps[//desired number];
```
* Adding / Removing position vectors of the lamps accordingly
```
glm::vec3 pointLightPositions[] = {
    // add / remove here	
};
 ```
* Initialization
```
for (unsigned int i = 0; i < //desired number; i++) {
    lamps[i] = Lamp(// rgb of desired color,
    	//desired ambience, //desired diffusion, //desired specular value,
    	1.0f, 0.07f, 0.032f, // attenuation constants
    	pointLightPositions[i], //desired size;
    lamps[i].init();
}
 ```
# Changing the properties of spotlight
Properties of spotlight can be changed by making the following changes in main.cpp
* Initialization
```
SpotLight s = {
		Camera::cam.pos, Camera::cam.cameraFront,
		//desired inner cut off, //desired outer cut off,
		1.0f, 0.07f, 0.032f, // attenuation constants
		//desired ambience, //desired diffusion, //desired specular value
	};
 ```

