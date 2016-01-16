/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements light_source.h

***********************************************************/

#include <cmath>
#include <math.h>
#include <iostream>
#include "light_source.h"
#include <stdlib.h>

double max(double a, double b) {
    return a < b ? b : a;
}

std::vector<Ray3D> PointLight::get_shadow_rays(Ray3D& ray){
	
	std::vector<Ray3D> shadow_rays;
	
	Ray3D shadowRay;
	
	shadowRay.origin = ray.intersection.point - 0.0001 * ray.dir;
	shadowRay.dir = get_position() - shadowRay.origin;
	shadowRay.dir .normalize();
	
	shadow_rays.push_back(shadowRay);
	
	return shadow_rays;
}

/*
from text book page 312 trd edition for parallelogram light
*/
std::vector<Ray3D> ParallelogramLight::get_shadow_rays(Ray3D& ray){

	std::vector<Ray3D> shadow_rays;
	std::vector<int> r;
	std::vector<int> s;	
	Vector3D x = _p;
	Vector3D y = _q;
	Point3D o = _pos;
	
	int N = 40;
	
	r.clear();
	s.clear();
	for(int i = 0; i < N; i++){
		r.push_back(((double) rand() / (RAND_MAX)+1));
		s.push_back(((double) rand() / (RAND_MAX)+1));
	}
	
	// Shuffle
	double shuffle;

	for(int i = N-1; i >=0; i--){
		int j = rand() % (i+1);
		shuffle = s.at(j);
		s.at(j) = s.at(i);	
		s.at(i) = shuffle;
	}
	
	Point3D origin = ray.intersection.point - 0.0001 * ray.dir;
	for(int i = 0; i < N; i++){
		
		Ray3D shadowRay;
		shadowRay.origin = origin;
		shadowRay.dir = o + r.at(i) * x + s.at(i) * y - shadowRay.origin;
		shadowRay.dir.normalize();

		shadow_rays.push_back(shadowRay);
	}
	return shadow_rays;
}


void LightSource::shade( Ray3D& ray, double contribution) {
    // TODO: implement this function to fill in values for ray.col 
    // using phong shading.  Make sure your vectors are normalized, and
    // clamp colour values to 1.0.
    //
    // It is assumed at this point that the intersection information in ray 
    // is available.  So be sure that traverseScene() is called on the ray 
    // before this function.  

    // normal vector n
    Colour diffuse =  ray.intersection.mat->diffuse; 
    if (ray.intersection.isMap)
    {
        
        int row, col;
   
        SphereMapping sphereMap;
        sphereMap.getTexel(&row, &col, ray.intersection.text.getHeight(), ray.intersection.text.getWidth(), 
        	ray.intersection.pointObjectCoords);
        
        //change diffuse coefficient to that of texture
        
        unsigned char * red = ray.intersection.text.getRed(row,col);
        unsigned char * green = ray.intersection.text.getGreen(row,col);
        unsigned char * blue = ray.intersection.text.getBlue(row,col);


        double RedValue = (*red)/255.0;
        double BlueValue = (*blue)/255.0;
        double GreenValue = (*green)/255.0;
        
        diffuse  = diffuse *  Colour(RedValue, GreenValue, BlueValue); 
    }
    Vector3D normal = ray.intersection.normal; 
    normal.normalize();

    // light direction l
    Vector3D lightSource = get_position() - ray.intersection.point; 
    lightSource.normalize();
    
    //light Reflection r
    Vector3D lightReflect = ( 2 * lightSource.dot(normal) ) * normal - lightSource; 
    lightReflect.normalize();

    // view direction v
    Vector3D view_vector = -ray.dir;
    view_vector.normalize();


    Colour ambLight = (ray.intersection.mat->ambient) * get_ambient();

    Colour diffLight = max(0, lightSource.dot(normal)) * diffuse * get_diffuse();

    Colour specLight = max(0, pow((view_vector.dot(lightReflect)), (ray.intersection.mat->specular_exp))) 
                        * (ray.intersection.mat->specular) * get_specular();
    

 

    ray.col = ray.col + contribution * (ambLight + diffLight + specLight); 


    ray.col.clamp();

}
