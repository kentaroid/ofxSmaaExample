#include "../common/version.glsl"

/* uStep 
 * 0:Edge Detection
 * 1:Weight Calculation 
 * 2:BlendingWeight
*/
uniform int uStep=0; 

layout(binding=0) uniform sampler2D uInpTex; 
layout(binding=1) uniform sampler2D uInpTex2;
layout(binding=2) uniform sampler2D uInpTex3;

vec2 texSize = vec2(textureSize(uInpTex, 0));
#define SMAA_RT_METRICS vec4(1.0/texSize,texSize)
#define SMAA_INCLUDE_VS 0
#define SMAA_GLSL_4 1 
#define SMAA_PRESET_ULTRA 1
#include "SMAA.hlsl"

noperspective in vec2 texcoord; 
noperspective in vec2 pixcoord; 
noperspective in vec4 offset[3];
layout(location = 0) out vec4 fOut;

void main(){
	if(uStep==2){
		fOut = SMAANeighborhoodBlendingPS(texcoord, offset[0], uInpTex, uInpTex2);
	}else if(uStep==1){
		fOut = SMAABlendingWeightCalculationPS(texcoord, pixcoord, offset, uInpTex, uInpTex2, uInpTex3, ivec4(0));
	}else{	  
		fOut = vec4(SMAAColorEdgeDetectionPS(texcoord, offset, uInpTex),0.0,0.0); 
	}
}
