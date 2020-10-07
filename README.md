# GAP391 / Procedural Content Generation class / Module 4 work

Biome colors are directly taken from https://en.wikipedia.org/wiki/Biome#/media/File:Climate_influence_on_terrestrial_biome.svg plus Ocean, Beach and Snow

Move around the map with W/A/S/D, Zoom in/out with mouse wheel (although it only zooms in/out relative to the map center, so beware of it). Rendering is also a little buggy because of zooming in/out. Also, with big map sizes the rendering is slow, because my engine doesn't allow me to draw only what fits to the screen, I have to draw everything in the world.

Properties are made so the map initially fits to the screen, but you can zoom in/out (just be aware that it scales relative to center)

#### You can change number of octaves using LShift and LCtrl (+1 / -1)
#### Persistance also can be changed with 0 or 9 (+0.1 / -0.1)

### Code for that assignments in files:

* Yangine/Source/Utils/PerlinNoise.h
* Yangine/Source/Utils/PerlinNoise.cpp
* DungeonGenerator/Source/Logic/Map/*

### Map settings is in:

* DungeonGenerator/Assets/Maps/World.xml
