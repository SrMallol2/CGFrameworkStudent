// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
precision mediump float; // Add this line
uniform sampler2D u_texture;
varying vec3 Ip;

void main()
{

	gl_FragColor = vec4( Ip,1.0);
}