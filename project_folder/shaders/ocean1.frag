#version 120

varying vec3 normal_vector;
varying vec3 light_vector;
varying vec3 halfway_vector;
varying vec4 gp;
uniform samplerCube sky_texture;
varying vec3 normal;
varying vec3 world_position;
varying vec3 eye_direction; 
uniform vec3 eye_position;

void main() {
	vec4 fc = vec4(0.0,0.0,0.0,0.0);
	vec3 normal1         = normalize(normal_vector);
	vec3 halfway_vector1 = normalize(halfway_vector);

	vec4 reflect_color = textureCube(sky_texture, vec3(gl_TexCoord[0]));

	vec4 refract_color = textureCube(sky_texture, refract(eye_direction, normal, 1.0/1.3333));
	
	float fresnel = max(0, min(1, 0.2+pow(1+dot(normal, eye_direction),5.0)));
	
//this is for skybox
//	vec4 emissive_color = vec4(1.0, 1.0, 1.0, 1.0);
//	vec4 ambient_color  = vec4(0.0, 0.54, 0.65, 1.0);
//	vec4 diffuse_color  = vec4(0.0, 0.54, 0.65, 1.0);
//	vec4 specular_color = vec4(1.0, 1.0, 1.0, 1.0);

//this is for blood 
//	vec4 emissive_color = vec4(1.0, 1.0, 1.0,  1.0);
//	vec4 ambient_color  = vec4(1.0, 0.0, 0.3, 1.0);
//	vec4 diffuse_color  = vec4(1.0, 0.0, 0.3, 1.0);
//	vec4 specular_color = vec4(1.0, 0.25, 0.0,  1.0);
//this is for city
	vec4 emissive_color = vec4(1.0, 1.0, 1.0,  1.0);
	vec4 ambient_color  = vec4(1.0, 0.25, 0.0, 1.0);
	vec4 diffuse_color  = vec4(1.0, 0.25, 0.0, 1.0);
	vec4 specular_color = vec4(1.0, 0.25, 0.0,  1.0);


	float emissive_contribution = 0.00;
	float ambient_contribution  = 0.30;
	float diffuse_contribution  = 0.30;
	float specular_contribution = 1.8;

	

	float d = dot(normal1, light_vector);
	if (d > 0.0) {
		fc += specular_color * specular_contribution * max(pow(dot(normal1, halfway_vector1), 80.0), 0.0);
		fc += diffuse_color  * diffuse_contribution * d;
	}
	fc += ambient_color * ambient_contribution;

	float fog_factor = min(gp.z/300.0,1.0);
//	fc = fc * (1.0-fog_factor) + vec4(1.0, 1.0, 1.0,1.0)*fog_factor;

//	float dist = abs(world_position.y/refract(eye_direction, normal, 1.0/1.3333).y);
//	float dist1 = distance(world_position, eye_position);
//	float absorb = exp((2.*dist1+2.*dist)*-pow(7,-3));
//	refract_color *= absorb;

	fresnel = 1.0;	
//	gl_FragColor = 0.5*fc + 0.5*(fresnel*reflect_color+(1-fresnel)*refract_color*1.0);
//	gl_FragColor = fresnel*reflect_color+(1-fresnel)*refract_color*1.0;
	gl_FragColor = reflect_color;


}
