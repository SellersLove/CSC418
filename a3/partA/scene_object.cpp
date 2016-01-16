/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
        CSC418, SPRING 2005

    implements scene_object.h

***********************************************************/

#include <cmath>
#include <iostream>
#include "scene_object.h"

bool UnitSquare::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
    const Matrix4x4& modelToWorld ) {
  // TODO: implement intersection code for UnitSquare, which is
  // defined on the xy-plane, with vertices (0.5, 0.5, 0), 
  // (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
  // (0, 0, 1).
  //
  // Your goal here is to fill ray.intersection with correct values
  // should an intersection occur.  This includes intersection.point, 
  // intersection.normal, intersection.none, intersection.t_value.   
  //
  // HINT: Remember to first transform the ray into object space  
  // to simplify the intersection test.


  Ray3D rayModelSpace = Ray3D(worldToModel * ray.origin, worldToModel * ray.dir);

  double t_value = - (rayModelSpace.origin[2] / rayModelSpace.dir[2]);

  Point3D x_check = rayModelSpace.point_at(t_value);

  if(t_value <= 0){
    return false;
  }

  if(x_check[0] >= -0.5 && x_check[0] <= 0.5 && 
    x_check[1] >= -0.5 && x_check[1] <= 0.5){
    if(ray.intersection.none || t_value < ray.intersection.t_value){
      Intersection intersection;
      intersection.t_value = t_value;
      intersection.point = modelToWorld * x_check;
      intersection.none = false;
      intersection.normal = worldToModel.transpose() * Vector3D(0, 0, 1);
      ray.intersection = intersection;
      ray.intersection.normal.normalize();
      return true;  
    }
  }

  
  return false;
}

bool UnitSphere::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
    const Matrix4x4& modelToWorld ) {
  // TODO: implement intersection code for UnitSphere, which is centred 
  // on the origin.  
  //
  // Your goal here is to fill ray.intersection with correct values
  // should an intersection occur.  This includes intersection.point, 
  // intersection.normal, intersection.none, intersection.t_value.   
  //
  // HINT: Remember to first transform the ray into object space  
  // to simplify the intersection test.

  double t_value;
  Ray3D rayModelSpace = Ray3D(worldToModel * ray.origin, worldToModel * ray.dir);
  Vector3D rayOriginModelVector = rayModelSpace.origin - Point3D(0, 0, 0); 
  // convert point to vector for dot product purposes

  /*
   * A point on a unit sphere is given by the equation x^2 + y^2 +z^2 = r^2, or in vector form: p dot p = r^2
   * A point on our ray is given by pO + td 
   * Substituting, we get a quadratic equation in t
  */
  double a = rayModelSpace.dir.dot(rayModelSpace.dir);
  double b = 2 * rayModelSpace.dir.dot(rayOriginModelVector);
  double c = rayOriginModelVector.dot(rayOriginModelVector) - 1;
  
  double discriminant = b * b - 4 * a * c;
  if (discriminant >= 0 && (ray.intersection.none || t_value < ray.intersection.t_value)){
    
    double root1 = ( -b + sqrt(discriminant) ) / (2 * a);
    double root2 = ( -b - sqrt(discriminant) ) / (2 * a);
    
    if(root1 > 0 || root2 > 0){     
      if(root1 > 0 && root2 > 0){       
        if(root1 <= root2){
          t_value = root1;
        } 
        else {
          t_value = root2;
        }
      }
      else if( root1 > 0){
        t_value = root1;
      }
      else{
        t_value = root2;
      } 
      Intersection intersection;
      Point3D intersectionPointModelSpace = rayModelSpace.point_at(t_value);
      intersection.point = modelToWorld * intersectionPointModelSpace;
      intersection.normal = worldToModel.transpose() * Vector3D(intersectionPointModelSpace[0], intersectionPointModelSpace[1], intersectionPointModelSpace[2]);
      intersection.t_value = t_value;
      intersection.none = false;
      ray.intersection = intersection;
      ray.intersection.normal.normalize();
      return true;
    }
  }
  
  return false;
}

