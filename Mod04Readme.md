## Module 4 - Perlin Noise

Biome colors are directly taken from https://en.wikipedia.org/wiki/Biome#/media/File:Climate_influence_on_terrestrial_biome.svg plus Ocean, Beach and Snow

Move around the map with W/A/S/D, Zoom in/out with mouse wheel (although it only zooms in/out relative to the map center, so beware of it). Rendering is also a little buggy because of zooming in/out. Also, with big map sizes the rendering is slow, because my engine doesn't allow me to draw only what fits to the screen, I have to draw everything in the world.

Initially map is zoomed in at center, but you can zoom in/out (just be aware that it scales relative to center)

#### You can change number of octaves using LShift and LCtrl (+1 / -1)
#### Persistance also can be changed with 0 or 9 (+0.1 / -0.1)

### Code for that assignments in files:

* [Yangine/Source/Utils/PerlinNoise.h](Yangine/Source/Utils/PerlinNoise.h)
* [Yangine/Source/Utils/PerlinNoise.cpp](Yangine/Source/Utils/PerlinNoise.cpp)
* [PerlinNoiseWorld/Source/Logic/Map/](PerlinNoiseWorld/Source/Logic/Map/)

### Map settings is in:

* [PerlinNoiseWorld/Assets/Maps/World.xml](PerlinNoiseWorld/Assets/Maps/World.xml)

### Numbers to tweak:

The lower persistance is in the map settings, the more local maximas you get, therefore more trees are spawned in the biomes.
Initially I tried the noise range (4,4) and it was pretty cool. Noise ranges up to (9,9) (and maybe more) also produce pretty examples.
Increasing octaves also increases number of generated trees, but if persistance is <= 0.2 and octaves are 10 and greater, it bugs.
