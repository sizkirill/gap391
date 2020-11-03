## Module 7 (Formal Grammars)  

Mostly took logic from previous module and slightly changed it for this assignment, but the core logic remained the same  

### 1. [Weapon Name Generator](WeaponsConsoleApp/)  

Meanings:  
Non terminating symbols are just stages in generating a name, while terminating symbols later translate into appropriate words.  

Rules:  
Starting stage - figuring out if a weapon is cursed or enchanted:  
0 -> N1 (probability 0.65)  0.65 probability that weapon becomes enchanted (from 0 to 3) and go to stage 1  
0 -> C4 (probability 0.35)  0.35 probability that weapon is cursed (from -1 to -3) and go to the last stage immediately  

Stage 1: - immediately go to the final stage, or assign either elemental effect or a special property:  
1 -> 4 (probability 0.6)    0.6 probability that enchanted weapon doesn't get any more properties  
1 -> E2 (probability 0.2)   0.2 probability that enchanted weapon gets Elemental effect  
1 -> P3 (probability 0.2)   0.2 probability that enchanted weapon gets special Property  

Stage 2: - if we get here, we have a weapon with an elemental effect. Either we go to the final stage or also assign a special property:  
2 -> 4 (probability 0.75)   0.75 probability that weapon with elemental effect doesn't get the special property  
2 -> P4 (probability 0.25)  0.25 probability that weapon with elemental effect also gets the special property  

Stage 3: - if we get here, we have a weapon with a special property. Either we go to the final stage or also assign an elemental effect: 
3 -> 4 (probability 0.75)   0.75 probability that weapon with elemental effect doesn't get the elemental effect  
3 -> E4 (probability 0.25)  0.25 probability that weapon with elemental effect also gets the elemental effect  

Stage 4: - finalizing the weapon - assigning the weapon type.
4 -> S (probability 0.2)    - sword  
4 -> A (probability 0.2)    - axe  
4 -> B (probability 0.2)    - bow  
4 -> M (probability 0.2)    - mace  
4 -> D (probability 0.2)    - dagger  

Terminating symbols:  
S,
A,
B,
M,
D,
N,
C,
E,
P

Non terminating symbols:  
0, 1, 2, 3, 4

Start state: 0
