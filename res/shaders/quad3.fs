precision mediump float;
varying vec2 v_uv;
uniform float u_time;
uniform sampler2D u_texture;
uniform int subTask;

void main() {

    vec4 color = texture2D(u_texture, v_uv);
    if(subTask == 0) {
        gl_FragColor = vec4(color);
    } else if(subTask == 1) {
         // Calculate dynamic pixel size based on time
    // Start with larger "pixels" and decrease the size over time
        float maxPixelSize = 500.0; // Adjust this value as needed
        float minPixelSize = 10.0;  // The minimum pixel size (normal resolution)
        float timeFactor = mod(u_time*0.75, 10.0) / 10.0; // Modulate time to loop the effect every 10 seconds

    // Interpolate pixel size from maxPixelSize to minPixelSize over time
        float pixelSize = mix(maxPixelSize, minPixelSize, timeFactor);

    // Adjust UV coordinates to create the pixelation effect
        vec2 pixelatedUV = floor(v_uv * pixelSize) / pixelSize;

    // Sample the texture with the adjusted UVs
        vec4 color = texture2D(u_texture, pixelatedUV);

    // Output the color
        gl_FragColor = color;
    } else if(subTask == 2) {
        // Calculate the rotation matrix
        float rotationAngle = u_time * 0.5; //rotation angle in radians
        float cosAngle = cos(rotationAngle);
        float sinAngle = sin(rotationAngle);
        mat2 rotationMatrix = mat2(cosAngle, -sinAngle, sinAngle, cosAngle);

    // Calculate the rotated UV coordinates
        vec2 rotatedUV = rotationMatrix * (v_uv - 0.5) + 0.5;

    // Fetch the color from the texture at the rotated UV
        vec4 color = texture2D(u_texture, rotatedUV);

    // Set the output color
        gl_FragColor = color;
    }
}
