# specular lighting in opengl
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


