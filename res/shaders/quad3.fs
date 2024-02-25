precision mediump float;
varying vec2 v_uv;
uniform sampler2D u_texture;
uniform int subTask;
uniform vec2 texture_size;

void main() {

    vec4 color = texture2D(u_texture, v_uv);
    if(subTask == 0) {
        gl_FragColor = vec4(color);
    } else if(subTask == 1) {
         // Calculate the pixelated UV coordinates
        vec2 u_pixelSize = vec2(100);
        vec2 pixelatedUV = floor(v_uv * u_pixelSize) / u_pixelSize;

    // Fetch the color from the texture at the pixelated UV
        vec4 color = texture2D(u_texture, pixelatedUV);

    // Set the output color
        gl_FragColor = color;
    } else if(subTask == 2) {
        // Calculate the rotation matrix
        float rotationAngle = 3.1415926; //rotation angle in radians
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
