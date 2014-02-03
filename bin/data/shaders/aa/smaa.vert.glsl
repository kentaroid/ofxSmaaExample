#include "../common/version.glsl"

/* uStep 
 * 0:Edge Detection
 * 1:Weight Calculation 
 * 2:BlendingWeight
*/
uniform int uStep=0; 

layout(binding=0) uniform sampler2D uInpTex; 
vec2 texSize = vec2(textureSize(uInpTex, 0));
#define SMAA_RT_METRICS vec4(1.0/texSize,texSize)
#define SMAA_INCLUDE_PS 0
#define SMAA_GLSL_4 1 
#define SMAA_PRESET_ULTRA 1
#include "SMAA.hlsl"

out vec2 texcoord; 
out vec2 pixcoord; 
out vec4 offset[3];

layout(location=0) in vec3 position;

void main(){
	texcoord = position.xy * 0.5 + 0.5;
	if(uStep==2){
		SMAANeighborhoodBlendingVS(texcoord, offset[0]); 
	}else if(uStep==1){
		SMAABlendingWeightCalculationVS(texcoord, pixcoord, offset);
	}else{	  
		SMAAEdgeDetectionVS(texcoord, offset);	  
	}
	gl_Position = vec4(position.xy, 0.0, 1.0);
}
