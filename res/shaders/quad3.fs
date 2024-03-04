
varying vec2 v_uv;
uniform float u_time;
uniform sampler2D u_texture2;
uniform int subTask;

void main() {

    vec4 color = texture2D(u_texture2, v_uv);
    if(subTask == 0) {
        gl_FragColor = vec4(color);
    } else if(subTask == 1) {
     
        float maxPixelSize = 500.0; //max pixel size
        float minPixelSize = 10.0;  // minimum pixel size (normal resolution)
        float timeFactor = mod(u_time*0.75, 10.0) / 10.0; 

    // Interpolateing pixel size from maxPixelSize to minPixelSize over time
        float pixelSize = mix(maxPixelSize, minPixelSize, timeFactor);

    // Adjusting UV coordinates to create the pixelation effect
        vec2 pixelatedUV = floor(v_uv * pixelSize) / pixelSize;

        vec4 color = texture2D(u_texture2, pixelatedUV);
    // Output the color
        gl_FragColor = color;

    } else if(subTask == 2) {
        // Rotation matrix
        float rotationAngle = u_time * 0.5; //rotation angle in radians
        float cosAngle = cos(rotationAngle);
        float sinAngle = sin(rotationAngle);
        mat2 rotationMatrix = mat2(cosAngle, -sinAngle, sinAngle, cosAngle);

    // Rotated UV coordinates
        vec2 rotatedUV = rotationMatrix * (v_uv - 0.5) + 0.5;

        vec4 color = texture2D(u_texture2, rotatedUV);


        gl_FragColor = color;
    }
}
