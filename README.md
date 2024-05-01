### KikooRenderer

In order to gain some expertise on real time rendering techniques and algorithms, as a hobby project I developped this thing. Not sure how to call it as it's has no true purpose.

I guess playground is the word.

The interface is built with Qt, and the rendering part is done with openGL.

I stopped the development in 2018 because of time constraints.

## Build

# Requirements
  Qt5 installed on the system
  MSVC Compiler
  CMake
  
# Commands


Here are some of the features illustrated with screen grabs.

## Overall Interface

<img src="Images/Kikoo/Interface.PNG" width="720" />

Shot of the interface of the software. The structure of the software is very much influenced by Unity's. 

It is all based on an "Entity Component System" pattern.


Left panel is a scene graph visualisation

Right panel shows the selected object parameters and components.

Bottom panel is a debugging console

## Bloom post process

<img src="Images/Kikoo/Bloom.png" width="720" />

Quite a simple implementation, lots of room for improvement.

## Depth of field post process

<img src="Images/Kikoo/Dof3.png" width="720" />

Example of a depth of field with a rather large focus distance.

## Fast approximate Anti aliasing Post process

<p float="left">
  <img src="Images/Kikoo/NoFxaa.png" />
  <img src="Images/Kikoo/Fxaa.png" /> 
</p>

[Clever algorithm](https://en.wikipedia.org/wiki/Fast_approximate_anti-aliasing) that applies some image processing techniques to reduce jaggered edges due to aliasing.

## Cubemap reflections

<img src="Images/Kikoo/Cubemap.png" width="720" />

## Directional Flow

<img src="Images/Gifs/Waterflow.gif" width="720" />
<img src="Images/Kikoo/DirFlow_0.png" width="720" />

Implementation of the great tutorial by [Catlike Coding](https://catlikecoding.com/unity/tutorials/flow/directional-flow/).

## Water Plane

<p float="left">
  <img src="Images/Kikoo/DUDV_0.png" width="500" />
  <img src="Images/Kikoo/DUDV.png" width="500" /> 
</p>

The normals and texture coordinates of a plane are displaced using a "dudv" map.

The objects of the scene are reflected onto that plane using the displaced normals.

It also uses the fresnel equations to make the water more reflective based on the camera direction.

## Shadow mapping

The 3 most common types of light are supported :
    * Directional
    * Point
    * Spot
    
<img src="Images/Kikoo/LowResShadowmap.PNG" width="512" />

*Example of a low resolution shadow map*

<img src="Images/Kikoo/Shadows.PNG" width="512" />

*Higher resolution shadow map*

Soft shadows are achieved using either percentage close filtering or poisson disk sampling.


## Materials

Several type of materials are built in :

    * Unlit

    * Blinn-phong shading

    * Physically based shading

Lots of room for improvement in that area.


<img src="Images/Kikoo/Materials.PNG" width="720" />

*A few different material parameters*

## Normal maps

Basic implementation of normal mapping

<img src="Images/Kikoo/NormalMapping.png" width="720" />

*Simple plane with displaced normals*

## Bump mapping

Basic implementation of bump mapping

<p float="left">
  <img src="Images/Kikoo/WithHeightMap.png" />
  <img src="Images/Kikoo/WithoutHeightMap.png" /> 
</p>

*First image is with bump mapping, second is without bump mapping*

## Particle system

Simple particle system that uses texture atlases and billboards.


<p float="left">
  <img src="Images/Gifs/ParticleSystem.gif" width="512" />
  <img src="Images/Kikoo/Particles.png" width="512" />
  <img src="Images/Kikoo/Particles_1.png" width="512" /> 
</p>

*Same Particle system with two differents particle sizes*

## Volumetric ray marching

Tried to implement some ray marching for volumetric to render clouds.

Not 100% satisfied with the result.

It uses a 3d perlin worley noise.

<img src="Images/Gifs/Clouds.gif"/>

<p float="left">
  <img src="Images/Clouds/1.png" width="400" />
  <img src="Images/Clouds/2.png" width="400" /> 
  <img src="Images/Clouds/3.png" width="400" />
</p>
<p float="left">
  <img src="Images/Clouds/4.png" width="400" />
  <img src="Images/Clouds/5.png" width="400" /> 
  <img src="Images/Clouds/6.png" width="400" />
</p>
<p float="left">
  <img src="Images/Clouds/7.png" width="400" />
  <img src="Images/Clouds/8.png" width="400" /> 
  <img src="Images/Clouds/9.png" width="400" />
</p>
