/***********************************************************
             CSC418, FALL 2009
 
                 keyframe.h
                 author: Mike Pratscher

		Keyframe class
			This class provides a data structure that
			represents a keyframe: (t_i, q_i)
			where t_i is the time and
			      q_i is the pose vector at that time
			(see vector.h file for info on Vector class).
			The data structure also includes an ID to
			identify the keyframe.

***********************************************************/

#ifndef __KEYFRAME_H__
#define __KEYFRAME_H__

#include "vector.h"


class Keyframe
{
public:

	// Enumeration describing the supported joint DOFs.
	// Use these to access DOF values using the getDOF() function
	// NOTE: If you decide to add more DOFs, be sure to add the
	// corresponding enums _BEFORE_ the NUM_JOINT_ENUM element
	//
	enum { ROOT_TRANSLATE_X, ROOT_TRANSLATE_Y, ROOT_TRANSLATE_Z,
		   ROOT_ROTATE_X,    ROOT_ROTATE_Y,    ROOT_ROTATE_Z,
		   HEAD,
		   R_SHOULDER_PITCH, R_SHOULDER_YAW,   R_SHOULDER_ROLL,
		   L_SHOULDER_PITCH, L_SHOULDER_YAW,   L_SHOULDER_ROLL,
		   R_HIP_PITCH,      R_HIP_YAW,        R_HIP_ROLL,
		   L_HIP_PITCH,      L_HIP_YAW,        L_HIP_ROLL,
		   BEAK,
		   R_ELBOW,
		   L_ELBOW,
		   R_KNEE,
		   L_KNEE,
		   LIGHT_ANGLE,
		   NUM_JOINT_ENUM };

	// constructor
	Keyframe() : id(0), time(0.0), jointDOFS(NUM_JOINT_ENUM) {}

	// destructor
	virtual ~Keyframe() {}

	// accessor methods
	int  getID() const { return id; }
	void setID(int i)  { id = i;    }

	float getTime() const  { return time; }
	void  setTime(float t) { time = t;    }

	// Use enumeration values to specify desired DOF
	float getDOF(int eDOF) const      { return jointDOFS[eDOF]; }
	void  setDOF(int eDOF, float val) { jointDOFS[eDOF] = val;  }

	// These allow the entire pose vector to be obtained / set.
	// Useful when calculating interpolated poses.
	// (see vector.h file for info on Vector class)
	Vector getDOFVector() const            { return jointDOFS;  }
	void   setDOFVector(const Vector& vec) { jointDOFS = vec;   }

	// address accessor methods
	int*   getIDPtr()          { return &id;              }
	float* getTimePtr()        { return &time;            }
	float* getDOFPtr(int eDOF) { return &jointDOFS[eDOF]; }

private:

	int    id;
	float  time;
	Vector jointDOFS;
};

#endif // __KEYFRAME_H__
