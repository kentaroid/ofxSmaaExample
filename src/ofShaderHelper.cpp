#include "ofShaderHelper.h"

string includeSource(string fname){

	ofFile f(fname);
	string basePath=f.getEnclosingDirectory();
	ofBuffer buffer = ofBufferFromFile(fname);
	string src=buffer.getText();
	string out="";
	int c=0;
	
	while(src.find("/*")!=string::npos){
		int cp=src.find("/*");
		int ep=src.find("*/",cp);
		string t=src.substr(0,cp);
		t.append(src.substr(ep+2));
		src=t;
	}

	c=0;
	while(c<src.length()){
		string s=src.substr(c,src.find("\n",c)-c);
		if(s.find("#include")!=string::npos){
			 string fname=s.substr(s.find_first_of("\"")+1,s.find_last_of("\"")-s.find_first_of("\"")-1).c_str();
			 string sc= includeSource(basePath+fname);
			 out.append(sc+"\n");
		}else{
			out.append(s+"\n");
		}
		if(src.find("\n",c)==string::npos)break;
		c=src.find("\n",c)+1;
	}

	return out;
}

string buildShaderSource(string fname){
	string src=includeSource(fname);

	//delete comment lines.
	string out="";
	int c=0;
	while(c<src.length()){
		string s=src.substr(c,src.find("\n",c)-c);
		if(s.find("//")!=string::npos){
			out.append(s.substr(0,s.find("//"))+"\n");
		}else{
			out.append(s+"\n");
		}
		if(src.find("\n",c)==string::npos)break;
		c=src.find("\n",c)+1;
	}

	while(out.find("/*")!=string::npos){
		int cp=out.find("/*");
		int ep=out.find("*/",cp);
		string t=out.substr(0,cp);
		t.append(out.substr(ep+2));
		out=t;
	}

	while(out.find("\n\n")!=string::npos){
		src="";
		src.append(out.substr(0,out.find("\n\n")));
		src.append(out.substr(out.find("\n\n")+1));
		out=src;
	}
	
	//patch for src that glsl 4.2 later.
	while(out.find("layout(binding=")!=string::npos){
		int cp=out.find("layout(binding=");
		int ep=out.find(")",cp);
		string t=out.substr(0,cp);
		t.append(out.substr(ep+1));
		out=t;
	}
	return out;
}

bool loadShader( ofShader &shader,string v_fname,string f_fname ){
	if(shader.isLoaded())shader.unload();
	if(!loadShader(shader,GL_VERTEX_SHADER,v_fname))return false;
	if(!loadShader(shader,GL_FRAGMENT_SHADER,f_fname))return false;
	return shader.linkProgram();
}


bool loadShader( ofShader &shader, GLenum type,string fname ){
	if(!loadShaderFromSourcs(shader,type,buildShaderSource(fname))){
		puts(("Error File = "+fname).c_str());
		return false;
	}
	return true;
}

bool loadShaderFromSourcs( ofShader &shader, GLenum type,string src ){
	if(!shader.setupShaderFromSource(type,src)){
		int i=1,c=0;
		while(c<src.length()){
			string s=src.substr(c,src.find("\n",c)-c);
			puts((ofToString(i,3,' ')+": "+s).c_str());
			if(src.find("\n",c)==string::npos)break;
			c=src.find("\n",c)+1;
			i++;
		}
		return false;
	}
	return true;
}


