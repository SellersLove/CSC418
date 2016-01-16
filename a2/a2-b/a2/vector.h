/***********************************************************
             CSC418, FALL 2009
 
                 vector.h
                 author: Mike Pratscher

	   Vector class
			This class provides support for an n-dimensional
			vector. Most of the basic arithmetic operations
			are supported:
				  i) addition
				 ii) subtraction
				iii) scalar (post-)multiplication (eg, Vector * scalar)
				 iv) scalar (post-)division (eg, Vector / scalar)
			This class is used to represent the pose vector
			portion of a keyframe. The arithmetic operations
			allow for straight-forward interpolation calculations.
			Eg, midpoint between two Vectors, vec0 and vec1:
				Vector vec0, vec1, midpt;
				midpt = (vec0 + vec1) / 2;

***********************************************************/

#ifndef __VECTOR_H__
#define __VECTOR_H__


class Vector
{
public:

	// constructors
	Vector(int dim = 3);		// specify dimension of vector (default = 3)
	Vector(const Vector& vec);	// copy constructor

	// destructor
	virtual ~Vector();

	// overloaded operators
	Vector& operator=(const Vector& vec);

	float&  operator[](int index);
	const float& operator[](int index) const;

	Vector  operator+ (const Vector& vec) const;
	Vector& operator+=(const Vector& vec);

	Vector  operator- (const Vector& vec) const;
	Vector& operator-=(const Vector& vec);

	Vector  operator* (float scalar) const;
	Vector& operator*=(float scalar);

	Vector  operator/ (float scalar) const;
	Vector& operator/=(float scalar);

	// accessor methods
	int    getDim()  const;
	float* getData() const;

private:

	// make a dim dimension vector from vec
	Vector makeDim(int dim, const Vector& vec) const;

	int d;
	float* v;
};


#endif // __VECTOR_H__
