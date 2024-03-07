precision mediump float;
uniform sampler2D u_color_texture;
uniform sampler2D u_normal_texture;
uniform vec3 useTextures;


varying vec2 v_uv;
varying vec3 position;
varying vec3 N;
varying vec3 V;
varying vec3 L;


varying float dist_;
varying vec3 inv_L;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float shininess;
uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;





void main() {

//Pepi 1
    // Sample textures
    //vec4 diffuseColor = texture2D(u_diffuseTexture, v_uv);
    //vec4 specularColor = texture2D(u_specularTexture, v_uv);
    //vec3 normal_texture = texture2D(u_normalTexture, v_uv).rgb * 2.0 - 1.0;
    //normal_texture = normalize(normal_texture);

    // Calculate modified ambient color
    //vec3 modifiedKa = Ka * diffuseColor.rgb;
    //vec3 ambient = modifiedKa;

    // Calculate specular coefficient
    //float specularCoefficient = specularColor.a;

    // Calculate specular reflection
    //vec3 specular = specularCoefficient * Ks;

    // Calculate final lighting color
    //vec3 Ip = useTextures.x * (ambient + Id * max(dot_l_n, 0.0) + specular / pow(dist_, 2.0));

    // Output final color
    //gl_FragColor = vec4(Ip, 1.0);


//Pepi 2 ( en teoria es el mateix que la meva versio pero netejat ( t'ho posso per si no entens lo meu))


    //vec4 specularColor = texture2D(u_specularTexture, v_uv);

    // Sample the diffuse color texture
    //vec4 diffuseColor = texture2D(u_diffuseTexture, v_uv);

    // Extract the RGB components of the diffuse color
    //vec3 diffuseRGB = diffuseColor.rgb;

    // Use the alpha component of the diffuse color as the specular coefficient (Ks)
    //float specularCoefficient = diffuseColor.a;

    // Calculate ambient term
    //vec3 ambient = Ka * diffuseRGB;

    // Calculate diffuse term
    //vec3 diffuse = clamp(dot_l_n, 0.0, 1.0) * Id * diffuseRGB;

    // Calculate specular term using the extracted specular coefficient
    //vec3 specular = pow(clamp(dot_r_v, 0.0, 1.0), shininess) * Is * specularCoefficient;

    // Combine ambient, diffuse, and specular terms to get final lighting contribution
    //vec3 Ip = Ia * ambient + Id * diffuse + specular;
    //vec4 finalColor =  vec4(Ip, 1.0);

    // Output final color
    //gl_FragColor = finalColor;

// versio meva
// el que no entenc del 3.5 és tot el tema de lo de normal_texture i mix ( mirat'ho m'ho expliques i ja faré)

    vec4 color_texture = texture2D(u_color_texture, v_uv);
    vec4 normal_texture = texture2D(u_normal_texture,v_uv);

    // Remapping  from [0, 1] to [-1, 1] //
    /*
    Remember that normals are stored as colors, and colors can only go from 0 to 1. 
    To store normals (-1 to 1) they were encoded by adapting its range to 0..1 so
     you must apply the opposite operation before manipulating the normals
    */
    vec3 textureLocalNormal = normalize(normal_texture.rgb * 2.0 - 1.0);

    
    // Calculate ambient reflection using modified Ka
    vec3 ambient = Ka *color_texture.rgb;

    // Calculate specular reflection using specular color and constant Ks
    vec3 specular = color_texture.a * Ks;

    // Calculate reflection (not sure about this, but assuming it's reflection color)
    vec3 reflection = color_texture.rgb;

    float mix_factor = 0.5;

    vec3 vertexNormal = N;
 
    vec3 final_normal = mix(vertexNormal, textureLocalNormal, mix_factor);

    float dot_l_final_n = dot(L,final_normal);


    vec3 final_R = reflect(inv_L, final_normal);
    normalize(final_R);

     
    float dot_r_v = dot(final_R,V);
   

    vec3 Ip = (ambient)*Ia +((reflection)*(clamp(dot_l_final_n,0.0,1.0))*Id+
    specular*pow(clamp(dot_r_v,0.0,1.0),shininess)*Is)/pow(dist_,2.0);

    vec4 finalColor =  vec4(Ip, 1.0);

    // Output final color
    gl_FragColor = finalColor;

    





//Works (mostly) but not formula provided
    //vec3 diffuseColor = texture2D(u_diffuseTexture, v_uv).rgb;
    //vec3 Ip = (Ka * diffuseColor)*Ia +((Kd * diffuseColor)*(clamp(dot_l_n,0.0,1.0))*Id+
    //Ks*pow(clamp(dot_r_v,0.0,1.0),shininess)*Is)/pow(dist_,2.0);

    //vec4 finalColor =  vec4(Ip, 1.0);

    // Output final color
    //gl_FragColor = finalColor;

//No texture
    //vec3 Ip = (Ka)*Ia +((Kd )*(clamp(dot_l_n,0.0,1.0))*Id+
    //Ks*pow(clamp(dot_r_v,0.0,1.0),shininess)*Is)/pow(dist_,2.0);

    //vec4 finalColor =  vec4(Ip, 1.0);

    // Output final color
    //gl_FragColor = finalColor;
}