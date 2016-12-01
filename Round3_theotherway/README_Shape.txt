CSCI 1230 Introduction to Computer Graphics
Brown Univeristy
2016 Fall

Name: Fumeng Yang
CS Login: fyang7
Date: Sep 25, 2016

Assignment 2: Shape

Design

1. [Object-Oriented design]
I have surfaces and objects. Objects contain one or more than one surfaces.
I have a base class for objects called Shape and a base class for surfaces called Surface.
The surface, in mathematics, has a more general meaning.

2. [Corner cases]
The smallest number of parameters are following the design in the TA demo, except for my own shape (see following).
The normals of the tip of the cone are done by using the normal vectors on the same normal plane.

3. [Extra Credits]
a. I used LAYOUT_TRIANGLE_STRIPS. It is in Surface class and this is the drawing code for everything.

b. Tobrus -- donut!

c. Mobius -- I made two.
             For Mobius, Klein Bottle, and Seashell which you could see the backside, the shape is done by drawing the same shape
             twice. One back to the other so it looks like each vertex as two normals.
             I think the TA demo is doing the same thing?

d. Klein Bottle -- They are Mobius with 4 dimensions. The "inside" is connected to the "outside"
                   I have "The figure 8 immersion" and "Klein Bottle".
                   For the bottle, parameter 2 has to be an even number.

r. Seashell -- just to have fun

f. Sierpinski Cube -- I did with a cube. Notice that it can be anything -- sphere, mobius etc.
                      Parameter 1 and 2 are for each cube / sphere.
                      Parameter 3 is controlling the level of the fractal.
                      Since I use recursion, I limit the level of the fractal to at most 8 given parameter 3 in [1, 100].
                      Notice that please keep parameter 1 small when using a larger Parameter 3.
                      Also, please notice that the program might crush if you use a large parameter 1 with a large parameter 3....
                      No solution, it is because the system is running out of resources...

g. Axes -- for debugging. You can turn on or turn off.

h. No fill -- Using the line strip mode. Also for debugging and it's really cool if you try Klein Bottle one!
              I disable this mode for Sierpinski Cube for some reasons...

4. [Collaborators]
a. I use Mathematica to compute partial derivative that is used in normals
b. About making Mobius, Klein Bottle and Seashell's backside visible, I was doing this by disabling GL_GULL_FACE.
Then TA Vivian pointed that this might have some side effects and it is better to draw the triangles twice.
I followed her suggestion.
c. Acknowledge
I saw Sacha Servan-Schreiber (aservans)'s demo.
I really like his white background and the resizable window so I implemented them myself.
These two features were borrowed from his demo.
