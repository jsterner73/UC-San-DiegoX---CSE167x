/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167x by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This file is display.cpp.  It includes the skeleton for the display routine

// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Transform.h"

using namespace std; 
#include "variables.h"
#include "readfile.h"

// New helper transformation function to transform vector by modelview 
// May be better done using newer glm functionality.
// Provided for your convenience.  Use is optional.  
// Some of you may want to use the more modern routines in readfile.cpp 
// that can also be used.  

void transformvec(const GLfloat input[4], GLfloat output[4]) {
	GLfloat modelview[16]; // in column major order
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	for (int i = 0; i < 4; i++) {
		output[i] = 0;
		for (int j = 0; j < 4; j++)
			output[i] += modelview[4 * j + i] * input[j];
	}
}

void display() {
	glClearColor(0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// I'm including the basic matrix setup for model view to 
	// give some sense of how this works.  

	glMatrixMode(GL_MODELVIEW);
	mat4 mv;

	if (useGlu) mv = glm::lookAt(eye, center, up);
	else {
		mv = Transform::lookAt(eye, center, up);
		mv = glm::transpose(mv); // accounting for row major
	}
	glLoadMatrixf(&mv[0][0]);

	// Set Light and Material properties for the teapot
	// Lights are transformed by current modelview matrix. 
	// The shader can't do this globally. 
	// So we need to do so manually.  
	if (numused) {
		glUniform1i(enablelighting, true);

		// YOUR CODE FOR HW 2 HERE.  
		// You need to pass the lights to the shader. 
		// Remember that lights are transformed by modelview first.  
		for (int iter = 0; iter < numLights; iter++) {
			int offset = 4 * iter;
			/*
			GLfloat tmpin[4], tmpout[4];
			for (int i = 0; i < 4; i++) {
			tmpin[i] = lightposn[offset + i];
			}
			transformvec(tmpin, tmpout);
			for (int i = 0; i < 4; i++) {
			lightransf[offset + i] = tmpout[i];
			}*/
			vec4 tmp;
			for (int i = 0; i < 4; i++) {
				tmp[i] = lightposn[offset + i];
			}
			tmp = mv * tmp;
			for (int i = 0; i < 4; i++) {
				lightransf[offset + i] = tmp[i];
			}
		}
		glUniform4fv(lightpos, numLights, lightransf);
		glUniform4fv(lightcol, numLights, lightcolor);
		glUniform1i(numusedcol, numused);
	}
	else glUniform1i(enablelighting, false);




	// Transformations for objects, involving translation and scaling 
	mat4 sc(1.0), tr(1.0), transf(1.0);
	sc = glm::transpose(Transform::scale(sx, sy, 1.0));
	tr = glm::transpose(Transform::translate(tx, ty, 0.0));

	// YOUR CODE FOR HW 2 HERE.  
	// You need to use scale, translate and modelview to 
	// set up the net transformation matrix for the objects.  
	// Account for GLM issues etc.  
	transf = mv * tr * sc;
	glLoadMatrixf(&transf[0][0]);

	for (int i = 0; i < numobjects; i++) {
		object* obj = &(objects[i]);

		{
			mat4 _objtransform = glm::transpose(obj->transform);
			mat4 objtransform = transf * _objtransform;
			glLoadMatrixf(&(objtransform)[0][0]);
			// YOUR CODE FOR HW 2 HERE. 
			// Set up the object transformations 
			// And pass in the appropriate material properties
			GLfloat _ambient[4] = { obj->ambient[0], obj->ambient[1], obj->ambient[2], obj->ambient[3] };
			GLfloat _diffuse[4] = { obj->diffuse[0], obj->diffuse[1], obj->diffuse[2], obj->diffuse[3] };
			GLfloat _specular[4] = { obj->specular[0], obj->specular[1], obj->specular[2], obj->specular[3] };
			GLfloat _emission[4] = { obj->emission[0], obj->emission[1], obj->emission[2], obj->emission[3] };
			glUniform4fv(ambientcol, 1, _ambient);
			glUniform4fv(diffusecol, 1, _diffuse);
			glUniform4fv(specularcol, 1, _specular);
			glUniform4fv(emissioncol, 1, _emission);
			glUniform1f(shininesscol, obj->shininess);
		}

		// Actually draw the object
		// We provide the actual glut drawing functions for you.  
		if (obj->type == cube) {
			glutSolidCube(obj->size);
		}
		else if (obj->type == sphere) {
			const int tessel = 20;
			glutSolidSphere(obj->size, tessel, tessel);
		}
		else if (obj->type == teapot) {
			glutSolidTeapot(obj->size);
		}

	}

	glutSwapBuffers();
}