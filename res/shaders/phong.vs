//Global variables from the CPU
//Material Properties
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float shininess;
//3D properties
uniform mat4 u_modelMatrix;
//Camera properties
uniform mat4 u_viewProjection;
uniform vec3 cameraPosition;
//Light Properties
uniform vec3 Ia;
uniform vec3 lightPosition;
uniform vec3 Id;
uniform vec3 Is;

//Passed variables to fragment shader
varying vec2 v_uv;
varying vec3 position;
varying vec3 N;
varying vec3 V;
varying vec3 L;
varying vec3 R;
varying float dot_l_n;
varying float dot_r_v;
varying float dist_;



void main() {
    // Convert local position to world space
    vec3 world_position = (u_modelMatrix * vec4(gl_Vertex.xyz, 1.0)).xyz;

    // Convert local normal to world space
    vec3 world_normal = (u_modelMatrix * vec4(gl_Normal.xyz, 0.0)).xyz;

    position = gl_Position.xyz;
    dist_ = distance(position,lightPosition);
    N = world_normal;
    normalize(N);
    L = lightPosition-position;
    normalize(L);
    V = cameraPosition-position;
    normalize(V);
    vec3 inv_L=  (-1.0)*L;
    R = reflect(inv_L, N);
    normalize(R);
    
    dot_l_n = dot(L,N);
    dot_r_v = dot(R,V);

    // Pass texture coordinates to fragment shader
    v_uv = gl_MultiTexCoord0.xy;

    gl_Position = u_viewProjection * vec4(world_position, 1.0); //output of the vertex shader
}
