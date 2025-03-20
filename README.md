# Asteroid Hills
## An OpenGL graphics application implementing the rendition of a 3D scene and reduced game logic.

## Introduction

This repository represents the submit for my university master's course **Programming of Pipeline Grahpics Processing** final assignment. At the time of writing, the assignment wepbage is still running and publicly available for <a href = "https://ocw.cs.pub.ro/courses/ppbg/tema/2024" target = "_blank"> viewing (assignment)</a>.

The rendering engine used for this application is an open-source, OpenGL C++ graphics engine called **gfx-framework** and was developed by my university for academic teaching. Its repository can be viewed <a href = "https://github.com/UPB-Graphics/gfx-framework" target = "_blank"> here</a>.

The following sections will provide a summarization of the tackled assignment challenges, which can be viewed at the link above.

Code revision is pending.
Working executable upload is pending.

## Application description

The application showcases a player-controlled helicopter flying over deformed terrain, with trees generated in a recursive fashion. The player can left click anywhere on the asteroid's surface to move the helicopter accordingly. Left-clicking spawns a marker, symbolizing the target destination. Right-clicking on trees deactivates them from rendering into scene.

### The Terrain
- plane mesh is generated at runtime (thus not loading a pre-existing 3D object mesh into memory);
- plane surface & texturing is done using height and texture maps in shaders (links below);
- suffers a curvature effect according to the player-controlled helicopter's position to emulate a planet surface;
- as a result, player movement takes place exclusively on the x and z coordinates.
<p align="center">
  <img src="https://github.com/user-attachments/assets/b4846196-6954-4186-8d04-aa4c2ca9d744" width="250"/>
  <img src="https://github.com/user-attachments/assets/e60e1daf-353f-40ea-9014-383b1947d52f" width="250"/><br>
  <i>Textured generated terrain (left) and the curvature effect deformation in practice (right)</i>
</p>

### The Trees
- are constructed using a singular cuboid mesh, which is uniformly rescaled and repositioned for each tree segment;
- depending on tree segment level, tree texture changes between *bark* and *leaves* according to different interpolation steps: **0%**, **50%**, **100%**;
- mesh world placement information is calculated recursively during initalization and then stored into memory for computational efficiency.
<p align="center">
  <img src="https://github.com/user-attachments/assets/939d6601-62a4-4e12-840a-f5acdd2d7345" width="500" height = "400"/><br>
  <i>Visualisation of the recursively indexed segments of a tree</i>
</p>

### The Helicopter & Destination Marker
- the helicopter is constructed visually using a non-uniformly scaled cuboid mesh;
- it can be player-controlled via left-clicking, seeking out a target destination upon action and spawning an animated destination marker;
- both the helicopter blade movement and marker pulsation are animated using the engine's built-in elapsed time fetch function;
- all the 3D objects in the scene (save the helicopter) suffer a dynamic curvature effect dependent on the helicopter's global position.
<p align="center">
  <br>
  <img src="https://github.com/user-attachments/assets/88c0e231-926e-4447-8390-207bf3507370" width = "400" height = "300"/>
  <img src="https://github.com/user-attachments/assets/6ddfad45-e01b-4aa2-b898-35031d34326c" width ="400" height = "300"/><br>
  <i>Up-close shot of the marker/Helicopter traversing the terrain</i>
</p>

### The Framebuffers
- framebuffers are complex data structures storing the visual information displayed on-screen;
- the application uses two framebuffers (each with different sets of shaders):
    1. **A general purpose, default framebuffer** used for displaying the 3D world;
    2. **A secondary, custom programmed framebuffer** used to store pixel coordinates and object indexing;
- The 4 RGBA channels representing the color data are repurposed in the secondary framebuffer: namely, the RGB color scheme turns into a XYZ coordinate system and the alpha channel A is used for storing object IDs;
- as a result, the player can fetch pixel coordinate data by left-clicking, alongside identifying the object the player has clicked on (this prevents out of bounds scenarios).
- visual data displayed in the secondary framebuffer can be toggled using the F key.
<p align="center">
   <img src="https://github.com/user-attachments/assets/fb8c507a-a483-4d57-9867-1b1682f3a9d7" height = "400"/><br>
  <i>The visual data the default framebuffer stores vs. The visual data the secondary framebuffer stores </i>
</p><br>
Due to the fact that the helicopter does not actually ascend/descend, y-axis data is not important for transportation. Therefore, the terrain shader provides values exclusively for the other two dimensions, hence the four red-blue values colour scheme. (The terrain is zero-centered relative to the world coordinate system and any negative values are clamped towards zero when formatting colour).

