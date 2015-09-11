#version 120
attribute vec3 vp; //vertex position
attribute vec3 np; //normal vector
attribute vec3 tp; //texture position
uniform vec3 lp; //light position
uniform mat4 VM;
uniform mat4 MM;
uniform mat4 TVM;
uniform vec3 eye_position;
varying vec3 light_vector;
varying vec3 normal_vector;
varying vec3 halfway_vector;
varying vec3 world_position;
varying vec4 gp;
varying vec3 normal;
varying vec3 eye_direction; 
void main(void) {
//	gl_Position = gl_ModelViewMatrix * vec4(vp,1.0);
	gl_Position = VM * MM * vec4(vp, 1.0);
	gl_Position = gl_ProjectionMatrix * gl_Position;
	gp = gl_Position;

	vec4 v = VM*MM * vec4(vp, 1.0);
	normal = normalize(np);
	
	light_vector = normalize((VM*vec4(lp,1.0)).xyz - v.xyz);
	normal_vector = (TVM * vec4(normal,0.0)).xyz;
	halfway_vector = light_vector+normalize(-v.xyz);
	
	world_position = (MM * vec4(vp,1.0)).xyz;
	eye_direction = normalize(world_position-eye_position);
    gl_TexCoord[0] = vec4(reflect(eye_direction,normal),0.0);

}
