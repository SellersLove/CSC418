/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		Implementations of functions in raytracer.h, 
		and the main function which specifies the 
		scene to be rendered.	

***********************************************************/


#include "raytracer.h"
#include "bmp_io.h"
#include <cmath>
#include <iostream>
#include <cstdlib>


Raytracer::Raytracer() : _lightSource(NULL) {
	_root = new SceneDagNode();
}

Raytracer::~Raytracer() {
	delete _root;
}

SceneDagNode* Raytracer::addObject( SceneDagNode* parent, 
		SceneObject* obj, Material* mat ) {
	SceneDagNode* node = new SceneDagNode( obj, mat );
	node->parent = parent;
	node->next = NULL;
	node->child = NULL;
	
	// Add the object to the parent's child list, this means
	// whatever transformation applied to the parent will also
	// be applied to the child.
	if (parent->child == NULL) {
		parent->child = node;
	}
	else {
		parent = parent->child;
		while (parent->next != NULL) {
			parent = parent->next;
		}
		parent->next = node;
	}
	
	return node;
}

LightListNode* Raytracer::addLightSource( LightSource* light ) {
	LightListNode* tmp = _lightSource;
	_lightSource = new LightListNode( light, tmp );
	return _lightSource;
}

void Raytracer::rotate( SceneDagNode* node, char axis, double angle ) {
	Matrix4x4 rotation;
	double toRadian = 2*M_PI/360.0;
	int i;
	
	for (i = 0; i < 2; i++) {
		switch(axis) {
			case 'x':
				rotation[0][0] = 1;
				rotation[1][1] = cos(angle*toRadian);
				rotation[1][2] = -sin(angle*toRadian);
				rotation[2][1] = sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'y':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][2] = sin(angle*toRadian);
				rotation[1][1] = 1;
				rotation[2][0] = -sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'z':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][1] = -sin(angle*toRadian);
				rotation[1][0] = sin(angle*toRadian);
				rotation[1][1] = cos(angle*toRadian);
				rotation[2][2] = 1;
				rotation[3][3] = 1;
			break;
		}
		if (i == 0) {
		    node->trans = node->trans*rotation; 	
			angle = -angle;
		} 
		else {
			node->invtrans = rotation*node->invtrans; 
		}	
	}
}

void Raytracer::translate( SceneDagNode* node, Vector3D trans ) {
	Matrix4x4 translation;
	
	translation[0][3] = trans[0];
	translation[1][3] = trans[1];
	translation[2][3] = trans[2];
	node->trans = node->trans*translation; 	
	translation[0][3] = -trans[0];
	translation[1][3] = -trans[1];
	translation[2][3] = -trans[2];
	node->invtrans = translation*node->invtrans; 
}

void Raytracer::scale( SceneDagNode* node, Point3D origin, double factor[3] ) {
	Matrix4x4 scale;
	
	scale[0][0] = factor[0];
	scale[0][3] = origin[0] - factor[0] * origin[0];
	scale[1][1] = factor[1];
	scale[1][3] = origin[1] - factor[1] * origin[1];
	scale[2][2] = factor[2];
	scale[2][3] = origin[2] - factor[2] * origin[2];
	node->trans = node->trans*scale; 	
	scale[0][0] = 1/factor[0];
	scale[0][3] = origin[0] - 1/factor[0] * origin[0];
	scale[1][1] = 1/factor[1];
	scale[1][3] = origin[1] - 1/factor[1] * origin[1];
	scale[2][2] = 1/factor[2];
	scale[2][3] = origin[2] - 1/factor[2] * origin[2];
	node->invtrans = scale*node->invtrans; 
}

Matrix4x4 Raytracer::initInvViewMatrix( Point3D eye, Vector3D view, 
		Vector3D up ) {
	Matrix4x4 mat; 
	Vector3D w;
	view.normalize();
	up = up - up.dot(view)*view;
	up.normalize();
	w = view.cross(up);

	mat[0][0] = w[0];
	mat[1][0] = w[1];
	mat[2][0] = w[2];
	mat[0][1] = up[0];
	mat[1][1] = up[1];
	mat[2][1] = up[2];
	mat[0][2] = -view[0];
	mat[1][2] = -view[1];
	mat[2][2] = -view[2];
	mat[0][3] = eye[0];
	mat[1][3] = eye[1];
	mat[2][3] = eye[2];

	return mat; 
}

void Raytracer::traverseScene( SceneDagNode* node, Ray3D& ray ) {
	SceneDagNode *childPtr;

	// Applies transformation of the current node to the global
	// transformation matrices.
	_modelToWorld = _modelToWorld*node->trans;
	_worldToModel = node->invtrans*_worldToModel;
	if (node->obj) {
	
		// Perform intersection.
		if (node->obj->intersect(ray, _worldToModel, _modelToWorld)) {
			ray.intersection.mat = node->mat;
		}
	}
	
	// Traverse the children.
	childPtr = node->child;
	while (childPtr != NULL) {
		traverseScene(childPtr, ray);
		childPtr = childPtr->next;
	}
	
	// Removes transformation of the current node from the global
	// transformation matrices.
	_worldToModel = node->trans*_worldToModel;
	_modelToWorld = _modelToWorld*node->invtrans;
	
}

void Raytracer::computeShading( Ray3D& ray ) {
	LightListNode* curLight = _lightSource;
	std::vector<Ray3D> shadow_rays;
	Ray3D shadowRay;
	for (;;) {
		if (curLight == NULL) break;

		 // shadows area lights		
		shadow_rays = curLight->light->get_shadow_rays(ray);
		int numRay = shadow_rays.size();
		
		// we computes color for each ray seperatly
		for(int i = 0; i < numRay; i++){
			
			shadowRay = shadow_rays.at(i);
			
			// Check intesection
			traverseScene(_root, shadowRay);

			if(shadowRay.intersection.none){
				curLight->light->shade(ray, 1.0/numRay);
			}
			else{
				if(_shadows){
					Colour ka = ray.intersection.mat->ambient;
					Colour Ia = curLight->light->get_ambient() * ka;
					ray.col = ray.col + (1.0/numRay) * Ia * ka;
					ray.col.clamp();
				}
				else{
					curLight->light->shade(ray, 1.0/numRay);
				}
			}
		}
		
		curLight = curLight->next;
	}
}

void Raytracer::initPixelBuffer() {
	int numbytes = _scrWidth * _scrHeight * sizeof(unsigned char);
	_rbuffer = new unsigned char[numbytes];
	_gbuffer = new unsigned char[numbytes];
	_bbuffer = new unsigned char[numbytes];
	for (int i = 0; i < _scrHeight; i++) {
		for (int j = 0; j < _scrWidth; j++) {
			_rbuffer[i*_scrWidth+j] = 0;
			_gbuffer[i*_scrWidth+j] = 0;
			_bbuffer[i*_scrWidth+j] = 0;
		}
	}
}

void Raytracer::flushPixelBuffer( char *file_name ) {
	bmp_write( file_name, _scrWidth, _scrHeight, _rbuffer, _gbuffer, _bbuffer );
	delete _rbuffer;
	delete _gbuffer;
	delete _bbuffer;
}

Colour Raytracer::shadeRay( Ray3D& ray ) {
	Colour curRayCol(0.0, 0.0, 0.0); 
	Colour reflRayCol(0.0, 0.0, 0.0);
	Colour refrRayCol(0.0, 0.0, 0.0);
	
	double bulrDegree = _bulrDegree; 
	traverseScene(_root, ray);
	
	// Don't bother shading if the ray didn't hit 
	// anything.
	if (!ray.intersection.none && (ray.maxDepth < _maxdepth)) {
		computeShading(ray);
		curRayCol = ray.col;
		

		//  algorithm from text book for Glossy relfection
		//Stochastic Reflection (Glossy Effect)
		
		// sample Xi in [0,1] X [0, 1]
		double xi1 = ((double) rand() / (RAND_MAX));
		double xi2 = ((double) rand() / (RAND_MAX));
		
		double u = -bulrDegree/2 + xi1 * bulrDegree;
		double v = -bulrDegree/2 + xi1 * bulrDegree;
		
		Vector3D R = ray.dir - 2 * (ray.intersection.normal.dot(ray.dir)) * ray.intersection.normal;
		R.normalize();
		
		Vector3D X(1, 0, 0);
		Vector3D Y(0, 1, 0);
		
		Point3D perturbed_origin = ray.intersection.point - 0.0001 * ray.dir;
		Vector3D perturbed_dir = R + u*X + v*Y;
		perturbed_dir .normalize();
		
		// Mirror-Like Reflection		
		//Vector3D R = V - 2*(N.dot(V))*N;
		//Point3D R1 = ray.intersection.point - 0.0001 * ray.dir;
		
		// reflected ray
		Ray3D reflRay(perturbed_origin, perturbed_dir);
		
		// Check if the ray is below the surface
		if(ray.intersection.normal.dot(perturbed_dir) > 0){
			
			// Ray trace reflected ray 
			reflRay.maxDepth = ray.maxDepth + 1;
			// recusive call reflected
			reflRayCol = pow(ray.intersection.mat->reflection_coef, ray.maxDepth) * shadeRay(reflRay);
		}
		else{
			reflRayCol = Colour(0, 0, 0);
		}
		
		// Refraction ray
		double outside;
		double inside;

		 // check the refaction ray is inside or outside
		 // in order to define refraction index 
		if(ray.refraction_depth%2 == 0){
			outside = 1;
			inside = ray.intersection.mat->refrative_index;

		}else{
			outside = ray.intersection.mat->refrative_index;
			inside = 1;
		}
		
		double n = outside/inside;
		double k;
		Vector3D I = -ray.dir;
		I.normalize();
		double d = 1.0 -n * n * (1.0 - ray.intersection.normal.dot(I) * ray.intersection.normal.dot(I));
		
		Vector3D refra_dir;
		
		// Refraction ray
		Point3D refra_origin;
		if(d >= 0){
			double square_root = sqrt(d);
			refra_dir = (n * ray.intersection.normal.dot(I) - square_root) * ray.intersection.normal - n * I;
			refra_origin = ray.intersection.point + 0.0001 * ray.dir;
			refra_dir.normalize();
			Ray3D refrRay(refra_origin, refra_dir);
			refrRay.maxDepth = ray.maxDepth + 1;
			refrRay.refraction_depth = ray.refraction_depth + 1;
			refrRayCol = pow(0.7, ray.maxDepth) * shadeRay(refrRay);
		}
		
		// Total reflection
		else{
			refrRayCol = Colour(0, 0, 0);
		}
	}
	curRayCol = curRayCol + reflRayCol + refrRayCol;
	curRayCol.clamp();
	return curRayCol; 
}	

void Raytracer::render( int width, int height, Point3D eye, Vector3D view, 
		Vector3D up, double fov, char* fileName ) {
	Matrix4x4 viewToWorld;
	_scrWidth = width;
	_scrHeight = height;
	_shadows = true;
	_maxdepth = 7;
	_antiAlis = 2;
	_bulrDegree = 0.01;
	double factor = (double(height)/2)/tan(fov*M_PI/360.0);
		
	initPixelBuffer();
	viewToWorld = initInvViewMatrix(eye, view, up);
	

	int antiNum = _antiAlis;
  // Construct a ray for each pixel.
   for (int i = 0; i < _scrHeight; i++) {
 		for (int j = 0; j < _scrWidth; j++) {
			
			Colour col;
			Point3D imagePlane;
			Point3D origin(0, 0, 0);
			imagePlane[2] = -1;
			
			// Implementing Anti-aliasing cast (2*antiNum)^2 rays 
			for (int u = 0; u < antiNum; u++){
				for (int v = 0; v < antiNum; v++){
					
					imagePlane[0] = (-double(width)/2 + 0.5 + u * 0.25 + j)/factor;
					imagePlane[1] = (-double(height)/2 + 0.5 + v * 0.25 + i)/factor;
					Ray3D ray;

					// Move view origin to world space
					ray.origin = viewToWorld * origin;

					// Calculate and set direction
					ray.dir = viewToWorld * imagePlane- ray.origin;
					ray.dir.normalize();

					ray.maxDepth = 0;
					ray.refraction_depth = 0;
					
					col = col + shadeRay(ray);

				}
			}

				//store the result of numerical integration into the pixel
			_rbuffer[i*width+j] = int(col[0]*255/ pow(antiNum,2));
			_gbuffer[i*width+j] = int(col[1]*255/ pow(antiNum,2));
			_bbuffer[i*width+j] = int(col[2]*255/ pow(antiNum,2));
		}
		if (i%20 == 0){
			std::cout << "Row "<< i << " shaded.\n";
		}
	}
	flushPixelBuffer(fileName);
}

int main(int argc, char* argv[])
{	
	// Build your scene and setup your camera here, by calling 
	// functions from Raytracer.  The code here sets up an example
	// scene and renders it from two different view points, DO NOT
	// change this if you're just implementing part one of the 
	// assignment.  
	Raytracer raytracer;
	int width = 360*2; 
	int height = 240*2; 

	if (argc == 3) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}

	// Camera parameters.
	Point3D eye(0, 0, 1);
	Vector3D view(0, 0, -1);
	Vector3D up(0, 1, 0);
	double fov = 60;

	// Defines a material for shading.
	// Defines a material for shading.
	Material gold( Colour(0.3, 0.3, 0.3), Colour(0.75164, 0.60648, 0.22648), 
			Colour(0.628281, 0.555802, 0.366065), 
			51.2 , 0.0, 0.0);
	
	Material jade( Colour(0, 0, 0), Colour(0.54, 0.89, 0.63), 
			Colour(0.316228, 0.316228, 0.316228), 
			12.8 , 0.34, 0.5);
	Material metal( Colour(0.1, 0.1, 0.1), Colour(0.6, 0.6, 0.6), 
			Colour(0.7, 0.7, 0.3), 
			51.2 , 1.627, 0.8);
			
	Material glass( Colour(0.0, 0.0, 0.0), Colour(0.588235, 0.670588, 0.729412), 
			Colour(0.9, 0.9, 0.9), 
			96 , 1.3, 0.8);
			
	Material ruby( Colour(0.1745, 0.01175, 0.01175), Colour(0.61424, 0.04136, 0.04136), 
			Colour(0.727811, 0.626959, 0.626959), 
			76.8 , 1.5, 0.3);
	
	Material obsedian( Colour(0.05375, 0.05, 0.06625), Colour(0.18275, 0.17, 0.22525), 
			Colour(0.332741, 0.328634, 0.346435), 
			38.4 , 1.5, 0.4);


/*	// Defines a point light source.
	raytracer.addLightSource( new ParallelogramLight(Point3D(0, 0, 5), Vector3D(2, 0, 0), Vector3D(0, 2, 0),
				Colour(0.9, 0.9, 0.9) ) );
/*	*/
	raytracer.addLightSource( new PointLight(Point3D(0, 0, 5), 
				Colour(0.9, 0.9, 0.9) ) );
	// Add a unit square into the scene with material mat.
	//SceneDagNode* sphere = raytracer.addObject( new UnitSphere(), &gold );
	//SceneDagNode* plane = raytracer.addObject( new UnitSquare(), &jade );

	SceneDagNode* sphere = raytracer.addObject( new UnitSphere(), &glass );
    SceneDagNode* sphere2 = raytracer.addObject( new UnitSphere(), &jade );
    SceneDagNode* sphere3 = raytracer.addObject( new UnitSphere(), &glass );
    SceneDagNode* plane = raytracer.addObject( new UnitSquare(), &gold );
    SceneDagNode* plane2 = raytracer.addObject( new UnitSquare(), &glass );
    SceneDagNode* plane3 = raytracer.addObject( new UnitSquare(), &metal );
	
	// Apply some transformations to the unit square.
    double factor1[3] = { 0.5, 0.5, 0.5 };
    double factor2[3] = { 10.0, 10.0, 10.0 };
    double factor3[3] = { 0.4, 0.4, 0.4 };
    double cylinder_scale[3] = { 1.0, 2.0, 1.0 };

    raytracer.translate(sphere, Vector3D(0, 0, -3));
    raytracer.translate(sphere2, Vector3D(-2, 0.4, -5));
    raytracer.scale(sphere2, Point3D(0, 0, 0), factor1);
    raytracer.translate(sphere3, Vector3D(-1, -1, -3.5));
    raytracer.scale(sphere3, Point3D(0, 0, 0), factor3);

    raytracer.translate(plane, Vector3D(0, 0, -10));
    raytracer.scale(plane, Point3D(0, 0, 0), factor2);

    raytracer.translate(plane2, Vector3D(0, -5, -5));
    raytracer.rotate(plane2, 'x', -90);
    raytracer.scale(plane2, Point3D(0, 0, 0), factor2);
    raytracer.translate(plane3, Vector3D(-5, 0, -5));
    raytracer.rotate(plane3, 'y', 90);
    raytracer.scale(plane3, Point3D(0, 0, 0), factor2);

	// Render the scene, feel free to make the image smaller for
	// testing purposes.	
	//raytracer.render(width, height, eye, view, up, fov, "view1.bmp");
	
	// Render it from a different point of view.
	Point3D eye2(4, 2, 1);
	Vector3D view2(-4, -2, -6);
	raytracer.render(width, height, eye2, view2, up, fov, "view2.bmp");
	
	return 0;
}

