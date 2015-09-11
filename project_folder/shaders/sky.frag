#version 120
uniform samplerCube sky_texture;
void main() {
	vec4 cur = textureCube(sky_texture, vec3(gl_TexCoord[0]));
	gl_FragColor = textureCube(sky_texture, vec3(gl_TexCoord[0]));
}
