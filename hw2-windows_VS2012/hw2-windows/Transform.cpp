// Transform.cpp: implementation of the Transform class.


#include "Transform.h"


// Helper rotation function.  Please implement this.
mat3 Transform::rotate(const float degrees, const vec3& axis) {
	// YOUR CODE FOR HW2 HERE
	// Please implement this.  Likely the same as in HW 1.
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

void Transform::left(float degrees, vec3& eye, vec3& up) {
	// YOUR CODE FOR HW2 HERE
	// Likely the same as in HW 1.
	mat3 rotationMatrix = Transform::rotate(degrees, up);
	eye = eye * rotationMatrix;
}

void Transform::up(float degrees, vec3& eye, vec3& up) {
	// YOUR CODE FOR HW2 HERE
	// Likely the same as in HW 1.
	mat3 rotationMatrix = Transform::rotate(degrees, glm::cross(eye, up));
	eye = eye * rotationMatrix;
	up = up * rotationMatrix;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) {
	// YOUR CODE FOR HW2 HERE
	// Likely the same as in HW 1.
	vec3 w = glm::normalize(eye - center);
	vec3 u = glm::normalize(glm::cross(up, w));
	vec3 v = glm::cross(w, u);
	mat4 transformM = mat4(u.x, u.y, u.z, -glm::dot(u, eye),
		v.x, v.y, v.z, -glm::dot(v, eye),
		w.x, w.y, w.z, -glm::dot(w, eye),
		0, 0, 0, 1.0f);
	return transformM;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
	// YOUR CODE FOR HW2 HERE
	// New, to implement the perspective transform as well.
	float theta = fovy / 2;
	float d = 1 / tan(glm::radians(theta));
	float a = -(zFar + zNear) / (zFar - zNear);
	float b = -(2 * zFar * zNear) / (zFar - zNear);
	mat4 perspectiveM = mat4((d / aspect), 0, 0, 0,
		0, d, 0, 0,
		0, 0, a, b,
		0, 0, -1, 0);
	return perspectiveM;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) {
	// YOUR CODE FOR HW2 HERE
	// Implement scaling
	mat4 scaleM = mat4(sx, 0, 0, 0,
		0, sy, 0, 0,
		0, 0, sz, 0,
		0, 0, 0, 1);
	return scaleM;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) {
	// YOUR CODE FOR HW2 HERE
	// Implement translation
	mat4 translateM = mat4(1, 0, 0, tx,
		0, 1, 0, ty,
		0, 0, 1, tz,
		0, 0, 0, 1);
	return translateM;
}

// To normalize the up direction and construct a coordinate frame.
// As discussed in the lecture.  May be relevant to create a properly
// orthogonal and normalized up.
// This function is provided as a helper, in case you want to use it.
// Using this function (in readfile.cpp or display.cpp) is optional.

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) {
	vec3 x = glm::cross(up, zvec);
	vec3 y = glm::cross(zvec, x);
	vec3 out = glm::normalize(y);
	return out;
}


Transform::Transform()
{

}

Transform::~Transform()
{

}