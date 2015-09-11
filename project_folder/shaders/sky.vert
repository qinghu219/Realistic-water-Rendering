attribute vec3 vp;
void main(void) {
	gl_Position = gl_ModelViewProjectionMatrix * vec4(vp,1.0);
	gl_TexCoord[0] = vec4(vp,1.0);
}
