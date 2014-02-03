#include "../common/version.glsl"

noperspective out vec2 vTexcoord;
layout(location=0) in vec3 position;

void main() {
	vTexcoord = position.xy * 0.5 + 0.5;
	gl_Position = vec4(position.xy, 0.0, 1.0);
}