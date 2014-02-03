#pragma once
#include "ofShader.h"
#include "ofGraphics.h"

bool loadShader( ofShader &shader,string v_fname,string f_fname );
bool loadShader( ofShader &shader, GLenum type,string fname );
bool loadShaderFromSourcs( ofShader &shader, GLenum type,string src );
string buildShaderSource(string fname);