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
XML file: [FractalTree.xml](LSystems/Assets/LSystems/FractalTree.xml)  
Lua script: [FractalTree.lua](LSystems/Assets/Scripts/FractalTree.lua) 

#### 2. Dragon Curve  
Variables: X, Y  
Constants: F, +, -  
Initial state: FX  
Rules: X -> X+YF+  
       Y -> -FX-Y  
Meanings: F - draw forward
          + - rotate clockwise 90 degrees
          - - rotate counterclockwise 90 degrees  
XML file: [DragonCurve.xml](LSystems/Assets/LSystems/DragonCurve.xml)  
Lua script: [DragonCurve.lua](LSystems/Assets/Scripts/DragonCurve.lua) 

#### 3. Twin Dragon Curve  
Rules: X -> X+YF  
       Y -> FX-Y  
Initial state: FX+FX+  
Everything else is the same as in Dragon Curve  
XML file: [TwinDragonCurve.xml](LSystems/Assets/LSystems/TwinDragonCurve.xml)  
Lua script: [DragonCurve.lua](LSystems/Assets/Scripts/DragonCurve.lua) 

#### 4. Some random circular thing (just liked how it looked haha)  
Variables: B, F, C  
Constants: [, ]  
Initial state: CF  
Rules: B -> BF (probability 0.8)    
       B -> C[B]C (probability 0.2)  
       F -> F (probability 0.7)  
       F -> CBC (probability 0.3)  
       C -> CC (probability 0.6)  
       C -> CB (probability 0.4) 
Meanings: F - draw forward  
          C - rotate 60 degrees clockwise and draw forward  
          [ - push transform  
          ] - pop transform  
Produces circular pattern, looks pretty, but don't know how it can be used.  
XML file: [SomeRandomStuff.xml](LSystems/Assets/LSystems/SomeRandomStuff.xml)  
Lua script: [SomeRandomStuff.lua](LSystems/Assets/Scripts/SomeRandomStuff.lua) 

#### 5. Seaweed
Variables: X  
Constants: F, +, -, [, ]
Initial state: X  
Rules: X -> FXX-[-X+X+X]+[+X-X-X]  
Meanings: F - draw forward at random amount (0.5 * stepAmount - 1.5 * stepAmount)
          + - rotate clockwise 20-30 degrees (at random)
          - - rotate counterclockwise 20-30 degrees (at random)
          [ - push transform  
          ] - pop transform  
XML file: [Seaweed.xml](LSystems/Assets/LSystems/Seaweed.xml)  
Lua script: [Seaweed.lua](LSystems/Assets/Scripts/Seaweed.lua) 

#### 6. Some highly random curve
Variables: F, B  
Constants: +, -, [, ]  
Initial state: F  
Rules:  F -> B (probability 0.3)  
        F -> BFB (probability 0.5)  
        F -> F (probability 0.2)  
        B -> -FB[+BF] (probability 0.3)  
        B -> +FB[-BF] (probability 0.3)  
        B -> -F[B]BF (probability 0.2)  
        B -> +F[B]FB (probability 0.2)  
Meanings: F - draw forward  
          + - rotate clockwise 15-22 degrees (at random)  
          - - rotate counterclockwise 15-22 degrees (at random)  
          [ - push transform  
          ] - pop transform  
Produces highly random curve, which may look like some kind of a plant, but more looks like a river or paths in a cave. With some rule and number tweaks can actually look good, especially on low numbers of iterations (with high numbers it explodes)  
XML file: [RandomizedWeed.xml](LSystems/Assets/LSystems/RandomizedWeed.xml)  
Lua script: [RandomizedWeed.lua](LSystems/Assets/Scripts/RandomizedWeed.lua) 

### Controls

Numbers from `1` to `6` switch to corresponding L-system  
`Up arrow` increases number of iterations (using the same rng device)
`Down arrow` decreases number of iterations (using the same rng device)
`Space bar` resets the RNG device with a new seed.
