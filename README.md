# noursmath

Collection of various Math and Linear Algebra utilities, mainly for use on matrices e.g. 1-dim arrays mapped to 2D by indexing. 

Originally created for use in a game I am developping using C99: [Codename Firesaga](https://gitlab.com/Gabinou/firesagamaker). Title pending. 

The library consists of two files, which include [DARR](https://gitlab.com/Gabinou/darr), and contain 3 main modules: q_math (quick math), linalg (linear algebra) and pathfinding.

All "matrices" in noursmath as well as DARRs are simple arrays (not array of arrays, not multidimensional arrays).
Higher-dimensions are mapped unto simple arrays by indexing, for example in 2D to access element at ```(row, col)``` you write ```array[row * col_len + col]```.

# Installation
Add ```nmath.c``` and ```nmath.h``` to your source code.

# Features
- Very simple, very fast.
- Naive operations, no overflow protection, wonky unsigned types.
- Includes [DARR](https://gitlab.com/Gabinou/darr).
- Performance is questionable (no vectorization)

# To Do
- Vectorization (```gcc``` only?)
- Benchmarks
- Determinant (requires LU decomposition) ?
- Diagonalization ?
- Eigenvalue/Eigenvector ?
- Matrix Multiplication
- Make operations less naive

## q_math
    
For now, only q_sqrt (otherwise known as the carmack sqrt) is truly a "quick" function.
Otherwise this module contains macros and functions that produce useful sequences of integers.
For example, the macro ```q_cycle4_mppm(i)``` creates the periodic cycle -1, +1, +1, -1... for i++, starting from i = 0.

Similarly, the macro ```q_cycle3_zpm(i)``` outputs 0, +1, -1...

Also, ```q_sequence_pingpong(i, 2, 0)``` outputs 0,1,2,1,0,1,2...

Further, ```q_sequence_geometric(32,1,2)``` -> 16, ```q_sequence_geometric(16,1,2)``` -> 8, until ```q_sequence_geometric(2,1,2)``` -> 1.

## linalg

This module contains various linear algebra functions, mainly element-wise addition, multiplication and boolean operation for arrays (including n-dim matrices).
Common linear algebra operations like dot product and cross product are implemented for 2D matrices.

## pathfinding

These functions are designed for a turn-based strategy game that takes place on a map (square or hexagonal 2D grid) made to compute movement and sight. 
No diagonal movement is possible, but there are no such restriction on sight.
Each map tile has a movement cost; each unit has a maximal movement and sight value, and minimal to maximal attack range.
Enemies and terrain can block movement, terrain can block sight, etc.

For pathfinding, <= 0 is NULL/impassable/blocked, and any positive value evaluates to true/passable/unblocked. 

For convenience sake, this module also defines some useful structures: n-dim nmath_point_*type*, nmath_sq_neighbors_*type* (each square on a square 2D grid has 4 neighbors), nmath_hex_neighbors_*type* (each hexagon on a hexagonal 2D grid has 6 neighbors).

Definitions:
- A *costmap* is a 2D matrix with tile traversal costs for a certain character/class.
- A *movemap* is a 2D matrix with tiles a certain unit can move to.
- A *blockmap* is a 2D matrix with tiles blocked for vision.
- A *sightmap* is a 2D matrix with tiles visible by a certain unit.
- Units can attack enemies that are on *attackable* tiles,
- Units can attack enemies from *assailable* tiles,
- Units can be push/pulled on *push/pullable* tiles,
- Units can be only be push/pulled in *push/pullable* directions.
<!-- a traversable tile is traversable WHETHER IT IS IN OUR MOVEMENT RANGE OR NOT. a MOVABLE tile is DIFFERENT than a TRAVERSIBLE tile. -->
<!-- although... movable implies that THE TILE MOVES. not the player! -> reachable tiles -->

Some other useful function include the computation of a "unit gradient" that computes the distance to the closest unit on every tile, computing a path on a matrix using a list of points, etc.
Some functions that are applicable to points on a hexagonal grid are also implemented.

Also, some functions can output arrays a list of points, or as a 2D matrix of same size to the input matrix.
The list of points have unknown length below the matrix total length, so are created using DARR to have an accessible len value.

# Motivation
Make a very simple math/linear algebra library, with fast operations. 
Use simple arrays everywhere, map higher dimensions by indexing.  
Compileable with ```tcc```.

# Credits
Copyright (c) 2021 Average Bear Games, Made by Gabriel Taillon
