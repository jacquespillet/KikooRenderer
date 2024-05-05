# KikooRenderer

In order to gain some expertise on real time rendering techniques and algorithms, as a hobby project I developped this thing. Not sure how to call it as it's has no true purpose.

I guess playground is the word.

The interface is built with Qt, and the rendering is done with openGL.

I stopped the development in 2018 because of time constraints.

## Build

### Requirements
  * Qt5 installed on the system
  * MSVC Compiler
  * CMake
  
### Commands

```
### Clone the repo and checkout to the latest branch
git clone --recursive https://github.com/jacquespillet/KikooRenderer.git
cd vulkan

### Generate the solution
mkdir build
cd build
cmake ../

### Build
cd ..
Build.bat
```
First build may take a while because it's going to build all the dependencies with the project.


## Features
Here are some of the features illustrated with screen grabs.

### Overall Interface

<img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Kikoo/Interface.PNG?raw=true" width="720" />

Shot of the interface of the software. The structure of the software is very much influenced by Unity's. 

It is all based on an "Entity Component System" pattern.


Left panel is a scene graph visualisation

Right panel shows the selected object parameters and components.

Bottom panel is a debugging console

### Bloom post process


<img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Kikoo/Bloom.png?raw=true" width="720" />

Quite a simple implementation, lots of room for improvement.

### Depth of field post process

<img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Kikoo/Dof3.png?raw=true" width="720" />

Example of a depth of field with a rather large focus distance.

### Fast approximate Anti aliasing Post process

<p float="left">
  <img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Kikoo/NoFxaa.png?raw=true" />
  <img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Kikoo/Fxaa.png?raw=true" /> 
</p>

[Clever algorithm](https://en.wikipedia.org/wiki/Fast_approximate_anti-aliasing) that applies some image processing techniques to reduce jaggered edges due to aliasing.

### Cubemap reflections

<img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Kikoo/Cubemap.png?raw=true" width="720" />

### Directional Flow

<img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Gifs/Waterflow.gif" width="720" />
<img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Kikoo/DirFlow_0.png?raw=true" width="720" />

Implementation of the great tutorial by [Catlike Coding](https://catlikecoding.com/unity/tutorials/flow/directional-flow/).

### Water Plane

<p float="left">
  <img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Kikoo/DUDV_0.png?raw=true" width="500" />
  <img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Kikoo/DUDV.png?raw=true" width="500" /> 
</p>

The normals and texture coordinates of a plane are displaced using a "dudv" map.

The objects of the scene are reflected onto that plane using the displaced normals.

It also uses the fresnel equations to make the water more reflective based on the camera direction.

### Shadow mapping

The 3 most common types of light are supported :
    * Directional
    * Point
    * Spot
    
<img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Kikoo/LowResShadowmap.PNG?raw=true" width="512" />

*Example of a low resolution shadow map*

<img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Kikoo/Shadows.PNG?raw=true" width="512" />

*Higher resolution shadow map*

Soft shadows are achieved using either percentage close filtering or poisson disk sampling.


### Materials

Several type of materials are built in :

    * Unlit

    * Blinn-phong shading

    * Physically based shading

Lots of room for improvement in that area.


<img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Kikoo/Materials.PNG?raw=true" width="720" />

*A few different material parameters*

### Normal maps

Basic implementation of normal mapping

<img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Kikoo/NormalMapping.png?raw=true" width="720" />

*Simple plane with displaced normals*

### Bump mapping

Basic implementation of bump mapping

<p float="left">
  <img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Kikoo/WithHeightMap.png?raw=true" />
  <img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Kikoo/WithoutHeightMap.png?raw=true" /> 
</p>

*First image is with bump mapping, second is without bump mapping*

### Particle system

Simple particle system that uses texture atlases and billboards.


<img src="https://github.com/jacquespillet/KikooRenderer/blob/master/resources/Gallery/particles.gif?raw=true" width="512" />

*Same Particle system with two differents particle sizes*

### Volumetric ray marching

Tried to implement some ray marching for volumetric to render clouds.

Not 100% satisfied with the result.

It uses a 3d perlin worley noise.

<img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Gifs/Clouds.gif"/>

<p float="left">
  <img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Clouds/1.png?raw=true" width="400" />
  <img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Clouds/2.png?raw=true" width="400" /> 
  <img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Clouds/3.png?raw=true" width="400" />
</p>
<p float="left">
  <img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Clouds/4.png?raw=true" width="400" />
  <img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Clouds/5.png?raw=true" width="400" /> 
  <img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Clouds/6.png?raw=true" width="400" />
</p>
<p float="left">
  <img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Clouds/7.png?raw=true" width="400" />
  <img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Clouds/8.png?raw=true" width="400" /> 
  <img src="https://github.com/jacquespillet/jacquespillet.github.io/blob/main/Images/Clouds/9.png?raw=true" width="400" />
</p>

### Integration of Bullet Physics

I integrated bullet physics in the software, here are a few videos :

<img src="https://github.com/jacquespillet/KikooRenderer/blob/master/resources/Gallery/Physics_1.gif?raw=true" width="512" />
<img src="https://github.com/jacquespillet/KikooRenderer/blob/master/resources/Gallery/Physics_2.gif?raw=true" width="512" />
<img src="https://github.com/jacquespillet/KikooRenderer/blob/master/resources/Gallery/Physics_3.gif?raw=true" width="512" />
<img src="https://github.com/jacquespillet/KikooRenderer/blob/master/resources/Gallery/Physics_4.gif?raw=true" width="512" />
<img src="https://github.com/jacquespillet/KikooRenderer/blob/master/resources/Gallery/Physics_5.gif?raw=true" width="512" />
