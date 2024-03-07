//Global variables from the CPU
//Material Properties
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float shininess;
//3D properties

//Camera properties
uniform mat4 u_modelMatrix;
uniform mat4 u_viewProjection;
uniform vec3 cameraPosition;

//Light Properties
uniform vec3 Ia;
uniform vec3 lightPosition;
uniform vec3 Id;
uniform vec3 Is;

//Passed variables to fragment shader

varying vec3 position;
varying vec3 N;
varying vec3 V;
varying vec3 L;
varying float dist_;
varying vec3 inv_L;

//Uniform variables
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;


void main() {
     v_uv = gl_MultiTexCoord0.xy;
    
    // Convert local position to world space
    vec3 world_position = (u_modelMatrix * vec4(gl_Vertex.xyz, 1.0)).xyz;

    // Convert local normal to world space
    vec3 world_normal = (u_modelMatrix * vec4(gl_Normal.xyz, 0.0)).xyz;

    // Pass them to the fragment shader interpolated
	v_world_position = world_position;
	v_world_normal = world_normal;

    
    gl_Position = u_viewProjection * vec4(world_position, 1.0); //output of the vertex shader

    position = gl_Position.xyz;
    //position = world_position;
    dist_ = distance(position,lightPosition);
    N = world_normal;
    normalize(N);
    L = lightPosition-position;
    normalize(L);
    V = cameraPosition-position;
    normalize(V);
    inv_L=  (-1.0)*L;
    

    // Pass texture coordinates to fragment shader
   
    
}
