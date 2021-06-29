# linalg (Linear Algebra) 

Linear algbera library for C99. Basically treats arrays as vectors or matrices.
 
Originally created for use in a game I am developping using C99: [Codename Firesaga](https://gitlab.com/Gabinou/firesagamaker). Title pending. 

## Installation
Add ```linalg.c``` and ```linalg.h``` to your source code.

## To Do
- Vectorization (```gcc``` only?)
- Benchmarks
- Determinant (requires LU decomposition) ?
- Diagonalization ?
- Eignevalue/Eigenvector ?
- Matrix Multiplication

## Features
- Very Simple.
- ```linalg.h``` contains [DARR](https://gitlab.com/Gabinou/darr).
- Operations are naive, no overflow protection
- Performance is questionable (no vectorization)
- Unsigned types are wonky.

## Motivation
Make a very simple linear algebra library, with simple operations. For vectors, 2D and 3D matrices. 

## Credits
Copyright (c) 2021 Average Bear Games, Made by Gabriel Taillon
