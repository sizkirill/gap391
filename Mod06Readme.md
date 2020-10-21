## Module 6 (L-systems)

Didn't do moving around, so in some cases the whole picture doesn't fit onto the screen.

### Structure
C++ code is in [LSystems/Logic/LSystems/](LSystems/Logic/LSystems/)

`class RuleContainer` is an adapter for `std::multimap` that contains L-system rules and checks for validity of probabilities (in Debug mode)  
`class LineBuilder` is used for building the line list (and exposes it's functionality to Lua environment, so rules can be written in Lua)  
`class LSystem` is the L-system itself, initializes from XML file.  
XML file example: [FractalTree.xml](LSystems/Assets/LSystems/FractalTree.xml)  
Lua script example: [FractalTree.lua](LSystems/Assets/Scripts/FractalTree.lua)  

### L-systems

#### 1. Fractal Tree  
Simple fractal tree with some angle tweaks. Nothing really to talk about. The angles can be adjusted in the corresponding Lua script. The step amount is adjusted so with any iterations the tree is always the same height.

#### 2. Dragon Curve  
Variables: X, Y  
Constants: F, +, -  
Initial state: FX  
Rules: X -> X+YF+  
       Y -> -FX-Y  
Meanings: F - draw forward
          + - rotate clockwise 90 degrees
          - - rotate counterclockwise 90 degrees

#### 3. Twin Dragon Curve  
Rules: X -> X+YF  
       Y -> FX-Y  
Initial state: FX+FX+  
Everything else is the same as in Dragon Curve  

4. Some random circular thing (just liked how it looked haha)
5. Seaweed
6. Some highly random curve

### Controls

Numbers from `1` to `6` switch to corresponding L-system  
`Up arrow` increases number of iterations (using the same rng device)
`Down arrow` decreases number of iterations (using the same rng device)
`Space bar` resets the RNG device with a new seed.
