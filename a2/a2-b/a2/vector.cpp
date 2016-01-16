/***********************************************************
             CSC418, FALL 2009
 
                 vector.cpp
                 author: Mike Pratscher

	   Vector class source file

***********************************************************/

#include <assert.h>
#include <math.h>
#include "vector.h"

const float kFloatZero = 0.0001f;


Vector::Vector(int dim)
{
	d = dim;

	v = new float[d];
	for( int i = 0; i < d; i++ )
		v[i] = 0.0;
}

Vector::Vector(const Vector& vec)
{
	d = vec.d;

	v = new float[d];
	for( int i = 0; i < d; i++ )
		v[i] = vec.v[i];
}


Vector::~Vector()
{
	if( v ) delete [] v;
}


Vector&
Vector::operator=(const Vector& vec)
{
	if( d != vec.d )
	{
		d = vec.d;

		if( v ) delete [] v;
		v = new float[d];
	}

	for( int i = 0; i < d; i++ )
		v[i] = vec.v[i];

	return *this;
}


float&
Vector::operator[](int index)
{
	assert( index >= 0 && index < d );
	return v[index];
}

const float&
Vector::operator[](int index) const
{
	assert( index >= 0 && index < d );
	return v[index];
}


Vector
Vector::operator+(const Vector& vec) const
{
	Vector res = *this;

	if( vec.d == d )
	{
		for( int i = 0; i < d; i++ )
			res.v[i] += vec.v[i];
	}
	else
	{
		Vector newVec = makeDim(d, vec);
		for( int i = 0; i < d; i++ )
			res.v[i] += newVec.v[i];
	}

	return res;
}

Vector&
Vector::operator+=(const Vector& vec)
{
	if( vec.d == d )
	{
		*this = *this + vec;
	}
	else
	{
		Vector newVec = makeDim(d, vec);
		*this = *this + newVec;
	}

	return *this;
}


Vector
Vector::operator-(const Vector& vec) const
{
	Vector res = *this;

	if( vec.d == d )
	{
		for( int i = 0; i < d; i++ )
			res.v[i] -= vec.v[i];
	}
	else
	{
		Vector newVec = makeDim(d, vec);
		for( int i = 0; i < d; i++ )
			res.v[i] -= newVec.v[i];
	}

	return res;
}

Vector&
Vector::operator-=(const Vector& vec)
{
	if( vec.d == d )
	{
		*this = *this - vec;
	}
	else
	{
		Vector newVec = makeDim(d, vec);
		*this = *this - newVec;
	}

	return *this;
}


Vector
Vector::operator*(float scalar) const
{
	Vector res = *this;

	for( int i = 0; i < d; i++ )
		res.v[i] *= scalar;

	return res;
}

Vector&
Vector::operator*=(float scalar)
{
	*this = *this * scalar;

	return *this;
}


Vector
Vector::operator/(float scalar) const
{
	Vector res = *this;

	if( fabs(scalar) > kFloatZero )
	{
		for( int i = 0; i < d; i++ )
			res.v[i] /= scalar;
	}

	return res;
}

Vector&
Vector::operator/=(float scalar)
{
	*this = *this / scalar;

	return *this;
}


int
Vector::getDim() const
{
	return d;
}

float*
Vector::getData() const
{
	return v;
}


Vector
Vector::makeDim(int dim, const Vector& vec) const
{
	Vector newVec(dim);

	if( vec.d > dim )
	{
		for( int i = 0; i < dim; i++ )
			newVec.v[i] = vec.v[i];
	}
	else // vec.d <= dim
	{
		int i;
		for( i = 0; i < vec.d; i++ )
			newVec.v[i] = vec.v[i];
		for( ; i < dim; i++ )
			newVec.v[i] = 0.0;
	}

	return newVec;
}
