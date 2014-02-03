#include "ofSSobj.h"
#include "ofMesh.h"

GLuint vao_;
GLuint vbo_;

ofSSobj::ofSSobj(void){
	GLfloat saqv[8] = {-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,1.0f,1.0f};
	glEnableClientState(GL_VERTEX_ARRAY);
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, 8*sizeof(GLfloat), saqv, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT,GL_FALSE,0,(const GLvoid*)0); //layout 0
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


ofSSobj::~ofSSobj(void){
	//deletion vao and vbo
}


void ofSSobj::draw(){
	glBindVertexArray(vao_);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

}