

# Conway's game of life

This program lets you play around a bit with Conway's game of life in the command line.

## Setup
In order to define the starting setup, you can compile and execute the file 'init.c':
```
    gcc init.c -o init
    ./init
```
This creates an empty csv file filled with zeros that matches the grid size in the game of life.
Everytime you execute 'init', everything gets set up with zeros again.
You can use this to create a blank canvas.
In order to define the starting setup, replace all relevant pixels with ones.
The repository also contains a csv that is compatible with the default setup.

## Execution
In order to see the animation, compile and execute the file 'main.c':
```
    gcc main.c -o main
    ./main
```

## Parameters
The header file 'gol.h' contains a few parameters that can be tweaked to modify the behaviour, such as grid size and animation speed.
Keep in mind that you have to recompile everything for the changes to take effect.


