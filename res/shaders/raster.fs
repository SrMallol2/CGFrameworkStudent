// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex

uniform sampler2D u_texture;
varying vec2 v_uv;

void main()
{
    
    vec4 color = texture2D(u_texture,v_uv);

	gl_FragColor = vec4( color );
}