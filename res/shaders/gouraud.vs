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

//Uniform variables
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;
//Variable to pass to the fragment shader
varying vec3 Ip;

void main(){
    v_uv = gl_MultiTexCoord0.xy;

	// Convert local position to world space
	vec3 world_position = (u_modelMatrix * vec4( gl_Vertex.xyz, 1.0)).xyz;

	// Convert local normal to world space
	vec3 world_normal = (u_modelMatrix * vec4( gl_Normal.xyz, 0.0)).xyz;

	// Pass them to the fragment shader interpolated
	v_world_position = world_position;
	v_world_normal = world_normal;

	// Project the vertex using the model view projection matrix
	gl_Position = u_viewProjection * vec4(world_position, 1.0); //output of the vertex shader

    //Phong-Model

    vec3 position = gl_Position.xyz;
    float dist_ = distance(position,lightPosition);
    vec3 N = world_normal;
    normalize(N);
    vec3 L = lightPosition-position;
    normalize(L);
    vec3 V = cameraPosition-position;
    normalize(V);
    vec3 inv_L=  (-1.0)*L;
    vec3 R = reflect(inv_L, N);
    normalize(R);
    
    float dot_l_n = dot(L,N);
    float dot_r_v = dot(R,V);

    Ip = Ka*Ia +(Kd*(clamp(dot_l_n,0.0,1.0))*Id+
    Ks*pow(clamp(dot_r_v,0.0,1.0),shininess)*Is)/pow(dist_,2.0);

}