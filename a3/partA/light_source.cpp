/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
            CSC418, SPRING 2005

        implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

double max(double a, double b) {
    return a < b ? b : a;
}

void PointLight::shade( Ray3D& ray ) {
    // TODO: implement this function to fill in values for ray.col 
    // using phong shading.  Make sure your vectors are normalized, and
    // clamp colour values to 1.0.
    //
    // It is assumed at this point that the intersection information in ray 
    // is available.  So be sure that traverseScene() is called on the ray 
    // before this function.  

    // normal vector n
    Vector3D normal = ray.intersection.normal; 
    normal.normalize();

    
    // light direction l
    Vector3D lightSource = _pos - ray.intersection.point; 
    lightSource.normalize();
    
    //light Reflection r
    Vector3D lightReflect = ( 2 * lightSource.dot(normal) ) * normal - lightSource; 
    lightReflect.normalize();

    // view direction v
    Vector3D view_vector = -ray.dir;
    view_vector.normalize();


    Colour ambLight = (ray.intersection.mat->ambient) * _col_ambient;

    Colour diffLight = max(0, lightSource.dot(normal)) * (ray.intersection.mat->diffuse) * _col_diffuse;

    Colour specLight = max(0, pow((view_vector.dot(lightReflect)), (ray.intersection.mat->specular_exp))) 
                        * (ray.intersection.mat->specular) * _col_specular;
    
    Colour colour = ambLight + diffLight + specLight;
 

    ray.col = ray.col + colour;

    ray.col.clamp();

}
 void PointLight::shadeSig(Ray3D & ray){
    
     Intersection intersectionPoint = ray.intersection;
     
     ray.col = intersectionPoint.mat->diffuse;
     
     ray.col.clamp();
 }

 void PointLight::shadeDiffuse(Ray3D & ray){

    // normal vector n
    Vector3D normal = ray.intersection.normal; 
    normal.normalize();

    
    // light direction l
    Vector3D lightSource = _pos - ray.intersection.point; 
    lightSource.normalize();
    
    //light Reflection r
    Vector3D lightReflect = ( 2 * lightSource.dot(normal) ) * normal - lightSource; 
    lightReflect.normalize();

    // view direction v
    Vector3D view_vector = -ray.dir;
    view_vector.normalize();


    Colour ambLight = (ray.intersection.mat->ambient) * _col_ambient;

    Colour diffLight = max(0, lightSource.dot(normal)) * (ray.intersection.mat->diffuse) * _col_diffuse;
    
    ray.col = ray.col + ambLight + diffLight;

    ray.col.clamp();

 }
