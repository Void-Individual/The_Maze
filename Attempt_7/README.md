## Changes made in this attempt
- Increased the map grid from 8x8 to 10x10
- Smoothened out the movement. Tou can now move in several directions, rotating while moving without any delays or interruptions. (Repurposing the movement functions and tweaking SDL2's default config was a little coplicated but I tweaked it to fit what I wanted)
- Tried to implement time based movement to ensure game speed consistency across devices of varying hardware. I might not have gotten my math right though.
- Handled collision to prevent player from moving forward snd backwards through walls.
- Desingned the 3d field to be drawn pixel by pixel instead of line by line. This have made the program much slower.
