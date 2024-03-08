
uniform sampler2D u_color_texture;
uniform sampler2D u_normal_texture;

varying vec2 v_uv;
varying vec3 N;
varying vec3 V;
varying vec3 L;


varying float dist_;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float shininess;
uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;
uniform mat4 u_modelMatrix;
uniform vec3 flag;


void main() {

    vec4 color_texture = texture2D(u_color_texture, v_uv);
    vec4 normal_texture = texture2D(u_normal_texture,v_uv);


    vec3 vertexNormal = N;
    vec3 final_ka = Ka;
    vec3 final_kd = Kd;
    vec3 final_ks = Ks;
    vec3 final_normal = vertexNormal;

    if(flag.x ==1.0){
        final_ka = Ka *color_texture.rgb;

        final_kd = color_texture.rgb;

    }
    if(flag.y == 1.0){
        final_ks = vec3(color_texture.a)*Ks;
    }
    
   if(flag.z==1.0){
     vec3 textureLocalNormal = (2.0 * normal_texture - vec4(1.0)).xyz;
     vec3 textureWorldNormal = normalize((u_modelMatrix * vec4(textureLocalNormal,0.0)).xyz);
     float mix_factor = 1.0;
     final_normal = mix(vertexNormal, textureWorldNormal, mix_factor);
   }
    

    float dot_l_n = dot(L,final_normal);

    vec3 R = normalize(reflect(-L, final_normal));
    
    float dot_r_v = dot(R,V);

    vec3 ambient = final_ka*Ia;
    vec3 diffuse = final_kd*clamp(dot_l_n, 0.0, 1.0)*Id;
    vec3 specular = final_ks*pow(clamp(dot_r_v, 0.0, 1.0), shininess)*Is;

    vec3 Ip = ambient + (diffuse + specular)/pow(dist_,2.0); 

    vec4 finalColor =  vec4(Ip, 1.0);

    // Output final color
    gl_FragColor = finalColor;


}