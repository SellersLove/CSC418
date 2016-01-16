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

  if(t_value <= 0.001){
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
  // convert point to vector for dot product 

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
      intersection.normal = worldToModel.transpose() * Vector3D(intersectionPointModelSpace[0], 
      intersectionPointModelSpace[1], intersectionPointModelSpace[2]);
      intersection.t_value = t_value;
      intersection.none = false;
      ray.intersection = intersection;
      // set text to true
      ray.intersection.isMap = false;
      ray.intersection.pointObjectCoords = intersectionPointModelSpace;
      //get the texture object
      ray.intersection.normal.normalize();
      return true;
    }
  }
  
  return false;
} 

bool UnitTextureSphere::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
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
  // convert point to vector for dot product 

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
      intersection.normal = worldToModel.transpose() * Vector3D(intersectionPointModelSpace[0], 
      intersectionPointModelSpace[1], intersectionPointModelSpace[2]);
      intersection.t_value = t_value;
      intersection.none = false;
      ray.intersection = intersection;
      // set text to true
      ray.intersection.isMap = true;
      ray.intersection.pointObjectCoords = intersectionPointModelSpace;
      //get the texture object
      ray.intersection.text = getTexture();
      ray.intersection.normal.normalize();
      return true;
    }
  }
  
  return false;
} 

bool UnitCylinder::intersect( Ray3D& ray, const Matrix4x4& worldToModel, const Matrix4x4& modelToWorld ) 
{
  // convert ray to model space
  Ray3D rayModelSpace = Ray3D(worldToModel * ray.origin, worldToModel * ray.dir);
  
  // center of unit cyclinder
  Point3D center(0,0,0); 

  
  double t_value = 0.0;
  double temp1, temp2;

  //the radius of the disk
  double radius = 1.0; 

  Vector3D normal; 

  Point3D intersectionPoint; 


  if (rayModelSpace.dir[2] != 0){
    //  intersection with z = 0.5
    temp1 = (-0.5-rayModelSpace.origin[2])/rayModelSpace.dir[2];
    //  intersection with z = 0.5
    temp2 = (0.5-rayModelSpace.origin[2])/rayModelSpace.dir[2];
    

    // intersect z = 0.5 first
    t_value = temp2;
    Point3D normal_temp(0,0,1);
    
    if (temp1 < temp2){
      t_value = temp1;
      // Construct the normal for the closer disk, which points on the negative z axis
      Point3D normal_temp(0,0,-1);

    }
    normal = normal_temp - center;
    normal.normalize();

    if(t_value <= 0.0001){

      return false;
    
    }
  
    intersectionPoint = rayModelSpace.origin + t_value * rayModelSpace.dir;
    
    // intersects with top/ bot disk
    if (intersectionPoint[0]*intersectionPoint[0] + intersectionPoint[1] 
    * intersectionPoint[1] <= (radius*radius)){

      if (ray.intersection.none || t_value < ray.intersection.t_value) {
        ray.intersection.point = intersectionPoint;

        ray.intersection.normal = normal;
        ray.intersection.t_value = t_value;
        ray.intersection.none = false;
        // Every model thinks it is in the center
        ray.intersection.CenterPoint = modelToWorld * Point3D(0,0,0); 
        return true;
      }

     return false;
    }

    // intersection with body
    double a = rayModelSpace.dir[0]*rayModelSpace.dir[0] + 
    rayModelSpace.dir[1]*rayModelSpace.dir[1];
    double b = (rayModelSpace.origin[0]*rayModelSpace.dir[0] + 
      rayModelSpace.origin[1]* rayModelSpace.dir[1]);        
    double c = rayModelSpace.origin[0]*rayModelSpace.origin[0] + 
    rayModelSpace.origin[1]*rayModelSpace.origin[1] - radius*radius;
                          
    double discriminant = b*b-a*c;

    if (discriminant<0){
        // no real root
        return false;
    }


    double root1 = (-b + sqrt(discriminant)) / a;
    double root2 = (-b - sqrt(discriminant)) / a;
    
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
      intersectionPoint = rayModelSpace.origin+ t_value * rayModelSpace.dir;
      normal[0] = intersectionPoint[0];
      normal[1] = intersectionPoint[1];
      normal[2] = 0;
      normal.normalize();

      if (intersectionPoint[2] < 0.5 && intersectionPoint[2] > -0.5){
        
        if (ray.intersection.none || t_value < ray.intersection.t_value){
          ray.intersection.point = modelToWorld * intersectionPoint;
          Point3D normalTemp;
          normalTemp[0] = intersectionPoint[0];
          normalTemp[1] = intersectionPoint[1];
          normalTemp[2] = 0;
          ray.intersection.normal = modelToWorld * (normalTemp - center);
          ray.intersection.t_value = t_value;
          ray.intersection.none = false;
          ray.intersection.CenterPoint = modelToWorld * Point3D(0,0,0); 
          return true;
        }

        return false;
      }
      
      return false;
    }

    return false;
  }


  // rayModelSpace.origin[2] == 0
  
  double a = pow(rayModelSpace.dir[0],2) + pow(rayModelSpace.dir[1], 2);
  double b = 2*(rayModelSpace.dir[0]* rayModelSpace.origin[0] + 
    rayModelSpace.dir[1]* rayModelSpace.origin[1]);
  double c = pow(rayModelSpace.origin[0], 2) + pow(rayModelSpace.origin[1], 2) - 1;
  double discriminant = b * b - 4 * a * c;
  
  if(discriminant < 0){
    
    return false;
  
  }

  double root1 = (-b + sqrt(discriminant)) /(2*a);
  double root2 = (-b - sqrt(discriminant))/ (2*a);

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
    
    intersectionPoint = rayModelSpace.origin+ t_value * rayModelSpace.dir;
    normal[0] = intersectionPoint[0];
    normal[1] = intersectionPoint[1];
    normal[2] = 0;
    normal.normalize();

    if (intersectionPoint[2] < 0.5 && intersectionPoint[2] > -0.5){
        
      if (ray.intersection.none || t_value < ray.intersection.t_value){
        ray.intersection.point = modelToWorld * intersectionPoint;
        Point3D normalTemp;
        normalTemp[0] = intersectionPoint[0];
        normalTemp[1] = intersectionPoint[1];
        normalTemp[2] = 0;
        ray.intersection.normal = modelToWorld * (normalTemp - center);
        ray.intersection.t_value = t_value;
        ray.intersection.none = false;
        ray.intersection.CenterPoint = modelToWorld * Point3D(0,0,0); 
        return true;
      }

      return false;
    }
      
      return false;
    }

return false;

}


