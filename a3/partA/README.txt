
For this assignment just look for TODO in the code.  You shouldn't
have to change anything else.  In particular, DO NOT change anything
in main().

The TODOs include
 - setting up the rays in world space and calling the right function to
   traverse the scene,
 - implementing the intersection routines for the primitives by filling
   in the intersection field in the Ray3D structure, and
 - implementing the shading function, which should fill in the col
   field in the Ray3D structure (depends on the material of the
   intersected object, of course).

The main() function contains the scene description. It automatically
renders two views from the same scene, which are then saved in files.
If you implemented everything correctly, it should produce images
like those shown in the assignment handout.

You need to hand in images from three runs of the program, showing
the following three renderings of the scene (each of which saves 2 images):
1. scene signature
  You could do this by setting ray.col in the shading function simply 
  to ray.intersection.mat->diffuse.  It will generate the scene 
  signiture if the intersection routine is implemented correctly.
2, scene with diffuse plus ambient shading
3. scene with the full Phong model


File Descriptions:

raytracer.{cpp, h} 
The main body of the raytracer, including the scene graph.  Simple 
addition and traversal code to the graph are provided to you.  

util.{cpp, h}
Includes definitions and implementations for points, vectors, matrices, 
and some structures for intersection and shading.  

light_source.{cpp, h}
Defines the basic light class.  You could define different types of 
lights, which shades the ray differently.  Point lights are sufficient 
for most scenes.  

scene_object.{cpp, h}
Defines object primitives in the scene (spheres, cylinders, etc...).  
Implements the intersect function which finds the intersection point 
between the ray and the primitive. 

bmp_io.{cpp, h}
I/O functions for .bmp files.  You shouldn't have to modify them.   
