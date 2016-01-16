/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		This file contains the interface and 
		datastructures of the raytracer.  
		Simple traversal and addition code to 
		the datastructures are given to you.

***********************************************************/

#include "util.h"
#include "scene_object.h"
#include "light_source.h"

// Linked list containing light sources in the scene.
struct LightListNode {
	LightListNode() : light(NULL), next(NULL) {}
	LightListNode( LightSource* light, LightListNode* next = NULL ) : 
		light(light), next(next) {}
	~LightListNode() { 
		if (!light) delete light; 
	}
	LightSource* light;
	LightListNode* next;
};

// The scene graph, containing objects in the scene.
struct SceneDagNode {
	SceneDagNode() : 
		obj(NULL), mat(NULL), 
		next(NULL), parent(NULL), child(NULL) {
	}	

	SceneDagNode( SceneObject* obj, Material* mat ) : 
		obj(obj), mat(mat), next(NULL), parent(NULL), child(NULL) {
		}
	
	~SceneDagNode() {
		if (!obj) delete obj;
		if (!mat) delete mat;
	}

	// Pointer to geometry primitive, used for intersection.
	SceneObject* obj;
	// Pointer to material of the object, used in shading.
	Material* mat;
	// Each node maintains a transformation matrix, which maps the 
	// geometry from object space to world space and the inverse.
	Matrix4x4 trans;
	Matrix4x4 invtrans;
	
	// Internal structure of the tree, you shouldn't have to worry 
	// about them.
	SceneDagNode* next;
	SceneDagNode* parent;
	SceneDagNode* child;
};

class Raytracer {
public:
	Raytracer();
	~Raytracer();

	// Renders an image fileName with width and height and a camera
	// positioned at eye, with view vector view, up vector up, and 
	// field of view fov.
	void render( int width, int height, Point3D eye, Vector3D view, 
			Vector3D up, double fov, char* fileName );

	// Add an object into the scene, with material mat.  The function
	// returns a handle to the object node you just added, use the 
	// handle to apply transformations to the object.
	SceneDagNode* addObject( SceneObject* obj, Material* mat ) {
		return addObject(_root, obj, mat);
	}
	
	// Add an object into the scene with a specific parent node, 
	// don't worry about this unless you want to do hierarchical 
	// modeling.  You could create nodes with NULL obj and mat, 
	// in which case they just represent transformations.  
	SceneDagNode* addObject( SceneDagNode* parent, SceneObject* obj, 
			Material* mat );

	// Add a light source.
	LightListNode* addLightSource( LightSource* light );

	// Transformation functions are implemented by right-multiplying 
	// the transformation matrix to the node's transformation matrix.
	
	// Apply rotation about axis 'x', 'y', 'z' angle degrees to node.
	void rotate( SceneDagNode* node, char axis, double angle );

	// Apply translation in the direction of trans to node.
	void translate( SceneDagNode* node, Vector3D trans );

	// Apply scaling about a fixed point origin.
	void scale( SceneDagNode* node, Point3D origin, double factor[3] );
	
private:
	// Allocates and initializes the pixel buffer for rendering, you
	// could add an interesting background to your scene by modifying 
	// this function.
	void initPixelBuffer();

	// Saves the pixel buffer to a file and deletes the buffer.
	void flushPixelBuffer(char *file_name);

	// Return the colour of the ray after intersection and shading, call 
	// this function recursively for reflection and refraction.  
	Colour shadeRay( Ray3D& ray ); 

	// Constructs a view to world transformation matrix based on the
	// camera parameters.
	Matrix4x4 initInvViewMatrix( Point3D eye, Vector3D view, Vector3D up );

	// Traversal code for the scene graph, the ray is transformed into 
	// the object space of each node where intersection is performed.
	void traverseScene( SceneDagNode* node, Ray3D& ray );

	// After intersection, calculate the colour of the ray by shading it
	// with all light sources in the scene.
	void computeShading( Ray3D& ray );
	
	// Width and height of the viewport.
	int _scrWidth;
	int _scrHeight;

	// Light list and scene graph.
	LightListNode *_lightSource;
	SceneDagNode *_root;

	// Pixel buffer.
	unsigned char* _rbuffer;
	unsigned char* _gbuffer;
	unsigned char* _bbuffer;

	// Maintain global transformation matrices similar to OpenGL's matrix
	// stack.  These are used during scene traversal. 
	Matrix4x4 _modelToWorld;
	Matrix4x4 _worldToModel;
};
