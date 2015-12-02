// Transform.cpp: implementation of the Transform class.


#include "Transform.h"
#include <stdio.h>

//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
  // YOUR CODE FOR HW1 HERE

	vec3 a = glm::normalize(axis);
	float sine = sin(glm::radians(degrees)), cosine = cos(glm::radians(degrees));
	float x = a.x, y = a.y, z = a.z;

	mat3 aat = mat3(x*x, y*x, z*x,
		x*y, y*y, z*y,
		x*z, y*z, z*z);

	mat3 aStar = mat3(0, -z, y,
		z, 0, -x,
		-y, x, 0);

	mat3 rotateM = glm::mat3(cosine) + (1 - cosine) * aat + sine * aStar;

	return rotateM;
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW1 HERE
	mat3 rotationMatrix = Transform::rotate(degrees, up);
	eye = eye * rotationMatrix;

	printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", eye.x, eye.y, eye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW1 HERE 
	mat3 rotationMatrix = Transform::rotate(degrees, glm::cross(eye, up));
	eye = eye * rotationMatrix;
	up = up * rotationMatrix;

	printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", eye.x, eye.y, eye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
  // YOUR CODE FOR HW1 HERE

	vec3  u = glm::normalize(vec3(0,0,0) - eye);
	vec3  v = glm::normalize(up);
	vec3  w = glm::normalize(glm::cross(u, v));
	v = glm::cross(w, u);

	mat4 Result(1);
	Result[0][0] = w.x;
	Result[1][0] = w.y;
	Result[2][0] = w.z;
	Result[0][1] = v.x;
	Result[1][1] = v.y;
	Result[2][1] = v.z;
	Result[0][2] = -u.x;
	Result[1][2] = -u.y;
	Result[2][2] = -u.z;
	Result[3][0] = -glm::dot(w, eye);
	Result[3][1] = -glm::dot(v, eye);
	Result[3][2] = glm::dot(u, eye);
	return Result;
}

Transform::Transform()
{

}

Transform::~Transform()
{

}
