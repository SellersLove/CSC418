/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		   light source classes

***********************************************************/

#include "util.h"
#include <vector>

// Base class for a light source.  You could define different types
// of lights here, but point light is sufficient for most scenes you
// might want to render.  Different light sources shade the ray 
// differently.
class LightSource {
public:
	void shade( Ray3D&, double);
	virtual std::vector<Ray3D> get_shadow_rays(Ray3D& ray) = 0; 
	virtual Point3D get_position() const = 0;
	virtual Colour get_ambient() const = 0;
	virtual Colour get_diffuse() const = 0;
	virtual Colour get_specular() const = 0;
};


class PointLight : public LightSource {

// A point light is defined by its position in world space and its
// colour.
public:
	PointLight( Point3D pos, Colour col ) : _pos(pos), _col_ambient(col), 
	_col_diffuse(col), _col_specular(col) {}
	PointLight( Point3D pos, Colour ambient, Colour diffuse, Colour specular ) 
	: _pos(pos), _col_ambient(ambient), _col_diffuse(diffuse), 
	_col_specular(specular) {}
	Point3D get_position() const { return _pos; }
	Colour get_ambient() const { return _col_ambient; }
	Colour get_diffuse() const { return _col_diffuse; }
	Colour get_specular() const { return _col_specular; }
	std::vector<Ray3D> get_shadow_rays(Ray3D& ray);
	
private:
	Point3D _pos;
	Colour _col_ambient;
	Colour _col_diffuse; 
	Colour _col_specular; 
};

class ParallelogramLight : public LightSource {

// Extended light source
public:
	ParallelogramLight( Point3D pos, Vector3D p, Vector3D q, Colour col ) : _pos(pos), _p(p), _q(q), _col_ambient(col), 
	_col_diffuse(col), _col_specular(col) {}
	ParallelogramLight( Point3D pos, Vector3D p, Vector3D q, Colour ambient, Colour diffuse, Colour specular ) 
	: _pos(pos), _p(p), _q(q), _col_ambient(ambient), _col_diffuse(diffuse), 
	_col_specular(specular) {}
	Point3D get_position() const { return _pos; }
	Vector3D get_p() const { return _p; }
	Vector3D get_q() const { return _q; }
	Colour get_ambient() const { return _col_ambient; }
	Colour get_diffuse() const { return _col_diffuse; }
	Colour get_specular() const { return _col_specular; }
	std::vector<Ray3D> get_shadow_rays(Ray3D& ray);
	
	private:
		Point3D _pos;
		Colour _col_ambient;
		Colour _col_diffuse; 
		Colour _col_specular; 
		Vector3D _p;
		Vector3D _q;
		double _N;
};

