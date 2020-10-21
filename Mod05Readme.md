## Module 5 - Cellular Automata  

### [GameOfLife](GameOfLife/)
Just basic game of life simulation from a random state  

### [CellularAutomata](CellularAutomata/)  
3 cell states: Alive, Dead, Grass  
Rules: 
- Alive cell with less than 2 grass neighbors dies  
- Alive cell with more than 3 alive neighbors dies  
- Otherwise it stays alive   

- Dead cell with at least 2 alive neighbors and at least one grass neighbor becomes alive  
- Otherwise, if there are at least two grass neighbors, it becomes a grass  
- Otherwise it remains dead  

- Grass cell with exactly 1 or 2 alive neighbors becomes dead  
- Grass cell with exactly 3 alive neighbors becomes alive  
- Otherwise it remains grass  
I wanted to make something like animals eating grass or dying of starvation and grass is respawning near other grass, but as a result I just like how it looked :)

### [CellularAutomataWorld](CellularAutomataWorld/)  
Attempt to generate a simple world with cellular automata. 6 different tile states: grass, lake, forest, village, ice, rock  
Starts with a world filled with grass  
Generation works in this order:  
- Generate lakes  
- Generate starts of forests  
- Propagate forests from the previous step  
- Generate villages  
- Generate rocks & ice (elevation simulation)  

Starting lake positions are generated based on moisture noise map from Module 4 with some randomness. Then small islands are filled with water (if a tile has 6 or more water neighbors it becomes water)  
Starting forest positions are based on amount of water tiles around and moisture noise values  
New forest tiles spawn near other forest tiles (chance is greater if there are also water tiles nearby)  
Villages almost always spawn near water or near other villages, but it has a chance to spawn in the middle of nowhere, but it also has a chance to die out.  
Rocks and ice are generated based on elevation and moisture noise map (ice is spawned where moisture is big and rocks where moisture is low)  
Numbers are hardcoded and adjusted to look decent.  


