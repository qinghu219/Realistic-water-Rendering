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
float box_size = 64*3.0;
float zero = 0.00000001;
float zero1 = 0.0001;
vec3 ray_trace_x(vec3 direction, vec3 position, float cur_x) {
	if (direction.x > -zero && direction.x < zero) {
		return vec3(position.x, box_size+box_size, box_size+box_size);
	}		
	float k = (cur_x - position.x)/direction.x;

	return vec3(cur_x, position.y+direction.y*k, position.z+direction.z*k);
}
vec3 ray_trace_y(vec3 direction, vec3 position, float cur_y) {
	if (direction.y > -zero && direction.y < zero) {
		return vec3(box_size+box_size, position.y, box_size+box_size);
	}
	
	float k = (cur_y - position.y)/direction.y;
	return vec3(position.x+direction.x*k, cur_y, position.z+direction.z*k);
}
vec3 ray_trace_z(vec3 direction, vec3 position, float cur_z) {
	if (direction.x > -zero && direction.x < zero) {
		return vec3(box_size+box_size, box_size+box_size, position.z);
	}	

	float k = (cur_z - position.z)/direction.z;
	return vec3(position.x+direction.x*k, position.y+direction.y*k, cur_z);
}
void main() {
	vec4 fc = vec4(0.0,0.0,0.0,0.0);
	vec3 normal1 = normalize(normal_vector);
	vec3 halfway_vector1 = normalize(halfway_vector);

	//now calculate reflection
	vec3 reflect_vector = reflect(eye_direction,normal);
	//calculate the intersection between ray and the box
  	vec3 xneg = ray_trace_x(reflect_vector, world_position, -1.0*box_size);
	vec3 xpos = ray_trace_x(reflect_vector, world_position, 1.0*box_size);
	vec3 yneg = ray_trace_y(reflect_vector, world_position, -1.0*box_size);
	vec3 ypos = ray_trace_y(reflect_vector, world_position, 1.0*box_size);
	vec3 zneg = ray_trace_z(reflect_vector, world_position, -1.0*box_size);
	vec3 zpos = ray_trace_z(reflect_vector, world_position, 1.0*box_size);
	
	vec3 true_reflect_vector = vec3(0.0, 0.0, 0.0);
	float is_boarder = 0.0;
	if (abs(xneg.z) <= box_size+zero && xneg.y >= -zero && xneg.y <= box_size+zero)
		true_reflect_vector = xneg;
	else if (abs(xpos.z) <= box_size+zero && xpos.y >= -zero && xpos.y <= box_size+zero)
		true_reflect_vector = xpos;
	else if (abs(zneg.x) <= box_size+zero && zneg.y >= -zero && zneg.y <= box_size+zero)
		true_reflect_vector = zneg;
	else if (abs(zpos.x) <= box_size+zero && zpos.y >= -zero && zpos.y <= box_size+zero)
		true_reflect_vector = zpos;
	else if (abs(ypos.z) <= box_size+zero && abs(ypos.x) <= box_size+zero)
		true_reflect_vector = ypos;
	else
		is_boarder = 1.0;
	vec4 reflect_color = textureCube(sky_texture, true_reflect_vector);
    if (is_boarder == 1.0) {
        if (abs(abs(world_position.x)-box_size) < 10 || abs(abs(world_position.z)-box_size)<10)
            reflect_color = textureCube(sky_texture,world_position);
        else
            reflect_color = textureCube(sky_texture, world_position);
    }
//	reflect_color = textureCube(sky_texture, reflect_vector);

    	//now calcluate refraction
	vec3 refract_vector = refract(eye_direction, normal, 1.0/1.33333);
    	xneg = ray_trace_x(refract_vector, world_position, -1.0*box_size);
	xpos = ray_trace_x(refract_vector, world_position, 1.0*box_size);
	yneg = ray_trace_y(refract_vector, world_position, -1.0*box_size);
	zneg = ray_trace_z(refract_vector, world_position, -1.0*box_size);
	zpos = ray_trace_z(refract_vector, world_position, 1.0*box_size);
	vec3 true_refract_vector = vec3(0.0,0.0,0.0);
	is_boarder = 0.0;
	if (-box_size-zero <= xneg.z && xneg.z <= box_size+zero && xneg.y <= zero && xneg.y >= -box_size-zero)
		true_refract_vector = xneg;
	else if (-box_size-zero <= xpos.z && xpos.z <= box_size+zero && xpos.y <= zero && xpos.y >= -box_size-zero)
		true_refract_vector = xpos;
	else if (-box_size-zero <= zneg.x && zneg.x <= box_size+zero && zneg.y <= zero && zneg.y >= -box_size-zero)
		true_refract_vector = zneg;
	else if (-box_size-zero <= zpos.x && zpos.x <= box_size+zero && zpos.y <= zero && zpos.y >= -box_size-zero)
		true_refract_vector = zpos;
	else if (-box_size-zero <= yneg.z && yneg.z <= box_size+zero && -box_size-zero <= yneg.x && yneg.x <= box_size+zero)
		true_refract_vector = yneg;
	else {
		is_boarder = 1.0;
		true_refract_vector = yneg;
	}
	vec4 refract_color = textureCube(sky_texture, true_refract_vector);
	if (is_boarder == 1.0) {
		refract_color = textureCube(sky_texture,world_position);
	}

	float dist = distance(true_refract_vector, world_position);
	float dist1 = distance(eye_position, world_position);
	float abs = exp(-pow(8,-3)*(2*dist));
//	refract_color = refract_color*abs;
	
	float fresnel = max(0, min(1, 0.2+pow(1+dot(normal, eye_direction),5.0)));
	
//this is for skybox
//	vec4 emissive_color = vec4(1.0, 1.0, 1.0, 1.0);
//	vec4 ambient_color  = vec4(0.0, 0.53, 1.0, 1.0);
//	vec4 diffuse_color  = vec4(0.4, 0.8, 0.95, 1.0);
//	vec4 specular_color = vec4(1.0, 1.0, 1.0, 1.0);

//this is for blood_sport
//	vec4 emissive_color = vec4(1.0, 1.0, 1.0,  1.0);
//	vec4 ambient_color  = vec4(1.0, 0.0, 0.3, 1.0);
//	vec4 diffuse_color  = vec4(1.0, 0.0, 0.3, 1.0);
//	vec4 specular_color = vec4(0.0, 0.0, 0.0,  1.0);
//this is for city
	vec4 emissive_color = vec4(1.0, 1.0, 1.0,  1.0);
	vec4 ambient_color  = vec4(1.0, 0.25, 0.0, 1.0);
	vec4 diffuse_color  = vec4(1.0, 0.25, 0.0, 1.0);
	vec4 specular_color = vec4(1.0, 0.25, 0.0,  1.0);
//this is for jajsundown1
//	vec4 emissive_color = vec4(1.0, 1.0, 1.0,  1.0);
//	vec4 ambient_color  = vec4(0.2 , 0.15 ,0.05, 1.0);
//	vec4 diffuse_color  = vec4(0.8 , 0.55 , 0.15, 1.0);
//	vec4 specular_color = vec4(1.0, 0.25, 0.0,  1.0);
//this is for nuke
//    	vec4 emissive_color = vec4(1.0, 1.0, 1.0,  1.0);
//  	vec4 ambient_color  = vec4(255.0/255.0, 203.0/255.0, 158.0/255.0, 1.0);
//  	vec4 diffuse_color  = vec4(255.0/255.0, 203.0/255.0, 158.0/255.0, 1.0);
//  	vec4 specular_color = vec4(1.0, 0.25, 0.0,  1.0);


	float emissive_contribution = 0.00;
	float ambient_contribution  = 0.30;
	float diffuse_contribution  = 0.30;
	float specular_contribution = 0.00;

	

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
	gl_FragColor = 0.5*fc+1.0*(fresnel*reflect_color+(1-fresnel)*refract_color*1.0);
//	gl_FragColor = fc;
//	gl_FragColor = fresnel*reflect_color+(1-fresnel)*refract_color*1.0;
//	reflect_color.a = 1.0;
//	gl_FragColor = refract_color;

}
