/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		utility functions and structures
		(based on code from CGL, University of Waterloo), 
		modify this file as you see fit.

***********************************************************/

#ifndef _UTIL_
#define _UTIL_

#include <iostream>
#include <cmath>
#include "bmp_io.h"
#include <stdio.h>

#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif

class Point3D {
public:
	Point3D(); 
	Point3D(double x, double y, double z);  
	Point3D(const Point3D& other); 

	Point3D& operator =(const Point3D& other); 
	double& operator[](int i); 
	double operator[](int i) const; 
	bool equal(const Point3D& other) const;
	
private:
	double m_data[3];
};

//Texture class that reads in a bmp image

class Texture {

public:

    Texture(){

    }
    
    Texture(char *fileName){
        bmp_read(fileName, (&t_width), (&t_height), &redArray, &greenArray, &blueArray);
    }

    int getHeight(){
       return t_height;
    } 

    unsigned int getWidth()
    {
        return t_width;
    }

    unsigned char* getGreen(int row, int col){
   
        int offset = row*t_width + col;

        return greenArray + offset; 
    }

    unsigned char* getBlue(int row, int col){

        int offset = row*t_width + col;
        return blueArray + offset;
    }

    unsigned char* getRed(int row, int col) {
        
        int offset = row*t_width + col;
        return redArray + offset; 
    }

private:

    unsigned long int t_width;
    long int t_height;
    unsigned char *redArray, *blueArray, *greenArray;

};
    
//stores a mapping for a general polygon to a texture map
class Mapping {
public:
    //Get the corresponding texture position for a intersection point 
    virtual bool getTexel(int *row, int* column, int height, int width, Point3D intersection) = 0;

};

class SphereMapping: public Mapping {
public:
    //Get the corresponding texture position for a intersection point on the 
    // sphere
    virtual bool getTexel(int *row, int *column, int height, int width, Point3D intersection);

};

class Vector3D {
public:
	Vector3D(); 
	Vector3D(double x, double y, double z); 
	Vector3D(const Vector3D& other); 

	Vector3D& operator =(const Vector3D& other); 
	double& operator[](int i);  
	double operator[](int i) const;  

	double length() const; 
	double normalize();
	double dot(const Vector3D& other) const; 
	Vector3D cross(const Vector3D& other) const; 

private:
	double m_data[3];
};

// standard operators on points and vectors
Vector3D operator *(double s, const Vector3D& v); 
Vector3D operator +(const Vector3D& u, const Vector3D& v); 
Point3D operator +(const Point3D& u, const Vector3D& v); 
Vector3D operator -(const Point3D& u, const Point3D& v); 
Vector3D operator -(const Vector3D& u, const Vector3D& v); 
Vector3D operator -(const Vector3D& u); 
Point3D operator -(const Point3D& u, const Vector3D& v); 
Vector3D cross(const Vector3D& u, const Vector3D& v); 
std::ostream& operator <<(std::ostream& o, const Point3D& p); 
std::ostream& operator <<(std::ostream& o, const Vector3D& v); 

class Vector4D {
public:
	Vector4D(); 
	Vector4D(double w, double x, double y, double z); 
	Vector4D(const Vector4D& other); 

	Vector4D& operator =(const Vector4D& other); 
	double& operator[](int i);  
	double operator[](int i) const;  

private:
	double m_data[4];
};

class Matrix4x4 {
public:
  Matrix4x4(); 
  Matrix4x4(const Matrix4x4& other); 
  Matrix4x4& operator=(const Matrix4x4& other); 

  Vector4D getRow(int row) const; 
  double *getRow(int row); 
  Vector4D getColumn(int col) const; 

  Vector4D operator[](int row) const; 
  double *operator[](int row); 

  Matrix4x4 transpose() const; 
		
private:
  double m_data[16];
};

Matrix4x4 operator *(const Matrix4x4& M, const Matrix4x4& N); 
Vector3D operator *(const Matrix4x4& M, const Vector3D& v); 
Point3D operator *(const Matrix4x4& M, const Point3D& p);
// Multiply n by the transpose of M, useful for transforming normals.  
// Recall that normals should be transformed by the inverse transpose 
// of the matrix.  
Vector3D transNorm(const Matrix4x4& M, const Vector3D& n); 
std::ostream& operator <<(std::ostream& os, const Matrix4x4& M); 

class Colour {
public:
	Colour(); 
	Colour(double r, double g, double b); 
	Colour(const Colour& other); 

	Colour& operator =(const Colour& other); 
	Colour operator *(const Colour& other); 
	double& operator[](int i);  
	double operator[](int i) const; 
    
	void clamp(); 	

private:
	double m_data[3];
};

Colour operator *(double s, const Colour& c); 
Colour operator +(const Colour& u, const Colour& v); 
std::ostream& operator <<(std::ostream& o, const Colour& c); 

struct Material {
	Material( Colour ambient, Colour diffuse, Colour specular, double exp , 
		double refrative_index, double reflection_coef) :
		ambient(ambient), diffuse(diffuse), specular(specular), specular_exp(exp), 
		refrative_index(refrative_index), reflection_coef(reflection_coef) {}
	
	// Ambient components for Phong shading.
	Colour ambient; 
	// Diffuse components for Phong shading.
	Colour diffuse;
	// Specular components for Phong shading.
	Colour specular;
	// Specular expoent.
	double specular_exp;
	
	// Refraction index
	double refrative_index;
	
	// Material reflection factor 0 - 1
	double reflection_coef;
};

struct Intersection {
	// Location of intersection.
	Point3D point;
	// Normal at the intersection.
	Vector3D normal;
	// Material at the intersection.
	Material* mat;

    //Mapping for textures
    bool isMap;
    Texture text;
    //for texture mapping
    Point3D pointObjectCoords;

    Point3D CenterPoint;

	double t_value;	
	// Set to true when no intersection has occured.
	bool none;
};

// Ray structure. 
struct Ray3D {
	Ray3D() {
		intersection.none = true; 
       intersection.isMap = false; 
	}
	Ray3D( Point3D p, Vector3D v ) : origin(p), dir(v) {
		intersection.none = true;
        // textmap
        intersection.isMap = false;
	}
	
	// Max depth recursion shading
	int maxDepth;
	int refraction_depth;
	
	// Origin and direction of the ray.
	
	Point3D origin;
	Vector3D dir;
	
	// Intersection status, should be computed by the intersection
	// function.
	Intersection intersection;
	
	// Current colour of the ray, should be computed by the shading
	// function.
	Colour col;
	
	Point3D point_at(double t_value);
};
#endif





