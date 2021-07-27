# noursmath

Collection of various Math and Linear Algebra utilities, mainly for use on matrices e.g. 1-dim arrays mapped to 2D by indexing. 
Originally created for use in a game I am developping using C99: [Codename Firesaga](https://gitlab.com/Gabinou/firesagamaker). Title pending. 

The library ```linalg.h``` makes use of [DARR](https://gitlab.com/Gabinou/darr), and contains 3 main modules: q_math (quick math), linalg (linear algebra) and pathfinding.

## q_math
    
For now, only q_sqrt (otherwise known as the carmack sqrt) is truly a "quick" function.
Otherwise this module contains macros and functions that produce useful sequences of integers.
For example, the macro ```q_cycle4_mppm(i)``` creates the periodic cycle -1, +1, +1, -1... for i++, starting from i = 0.

Similarly, the macro ```q_cycle3_zpm(i)``` outputs 0, +1, -1...

Also, ```q_sequence_pingpong(i, 2, 0)``` outputs 0,1,2,1,0,1,2...

Further, ```q_sequence_geometric(32,1,2)``` -> 16, ```q_sequence_geometric(16,1,2)``` -> 8, until ```q_sequence_geometric(2,1,2)``` -> 1.

## linalg

This module contains various linear algebra functions, mainly element-wise addition, multiplication and boolean operation for arrays (including n-dim matrices).
Common operations like dot product and cross product are implemented for 2D matrices.

## pathfinding

These functions are designed for a strategy game that takes place on a map (square or hexagonal 2D grid) made to compute movement and sight. 
No diagonal movement is possible, but there are no such restriction on sight.
Each map tile has a movement cost; each unit has a movement value and minimal to maximal attack range.
Enemies and terrain can block movement, terrain can block sight, etc.

For pathfinding, <= 0 is NULL/impassable/blocked, and any positive value evaluates to true. 

For convenience sake, this module also defines some useful structures: n-dim linalg_point_*type*, linalg_sq_neighbors_*type* (each square on a square 2D grid has 4 neighbors), linalg_hex_neighbors_*type* (each hexagon on a hexagonal 2D grid has 6 neighbors).
The functions are name intuitively: units can move to moveable tiles, units can attack enemies that are on assailable tiles, units can be pushed/pulled on pusheable/pulleable tiles, etc.

Some other useful function include the computation of a "unit gradient" that essentially computes the proximity to units of a certain faction, computing a path on a matrix using a list of points, etc.
Some functions that are applicable to points on a hexagonal grid are also implemented.

Also, these functions can output arrays as a list of points, or as a 2D matrix of same size to the input matrix.
The list of points have unknown length below the matrix ```row_len``` \* ```col_len```, so are created using DARR to have an accessible len value.

# Installation
Add ```linalg.c``` and ```linalg.h``` to your source code.

# To Do
- Vectorization (```gcc``` only?)
- Benchmarks
- Determinant (requires LU decomposition) ?
- Diagonalization ?
- Eigenvalue/Eigenvector ?
- Matrix Multiplication

# Features
- Very Simple.
- ```linalg.h``` contains [DARR](https://gitlab.com/Gabinou/darr).
- Operations are naive, no overflow protection
- Performance is questionable (no vectorization)
- Unsigned types are wonky.

# Motivation
Make a very simple linear algebra library, with simple operations. For vectors, 2D and 3D matrices. 

# Credits
Copyright (c) 2021 Average Bear Games, Made by Gabriel Taillon
