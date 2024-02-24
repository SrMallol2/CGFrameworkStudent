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
    // Convert the color to grayscale by calculating luminance
        float luminance = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));

    // Set the output color to grayscale
        vec3 actual_luminance = vec3(luminance);
        gl_FragColor = vec4(actual_luminance, color.a);

    } else if(subTask == 2) {
        // Invert the colors by subtracting each color channel from 1.0
        vec3 invertedColor = 1.0 - color.rgb;

    // Set the output color to the inverted color with the original alpha value
        gl_FragColor = vec4(invertedColor, color.a);

    } else if(subTask == 3) {
        // Convert the color to grayscale by calculating luminance
        //(0.2126, 0.7152, 0.0722)
        float luminance = dot(color.rgb, vec3(0.2, 0.5, 0.1));
        vec3 yellow = vec3(1.0, 1.0, 0.0);

    // Set the output color to grayscale
        vec3 actual_luminance = vec3(luminance);
        vec3 final_color = mix(actual_luminance, yellow, 0.3);
        gl_FragColor = vec4(final_color, color.a);
    } else if(subTask == 4) {


    } else if(subTask == 5) {

    // Calculate the distance from the center of the image
        vec2 center = vec2(0.5, 0.5); // Assuming UV coordinates are normalized [0, 1]
        float distance = distance(v_uv, center);

    // Calculate illumination factor based on distance
        float illuminationFactor = 1.0 - distance; // More illumination at the center
        illuminationFactor = clamp(illuminationFactor, 0.0, 1.0); // Clamping to ensure it stays within valid range

    // Enhance the illumination factor to control the effect intensity
    // You can adjust the power to control how quickly the illumination falls off
        illuminationFactor = pow(illuminationFactor, 2.0); // Squaring to make the effect more pronounced

    // Apply the illumination factor to the color
        vec3 illuminatedColor = color.rgb * illuminationFactor;

    // Set the output color
        gl_FragColor = vec4(illuminatedColor, color.a);

    } else if(subTask == 6) {
         // Define the blur size (how far from the current UV to sample)
    float blurSize = 1.0 /256.0; // Adjust this based on your texture's dimensions for desired blur amount

    // Initialize a variable to accumulate sampled colors
    vec4 accumulatedColor = vec4(0.0);

    // The total number of samples around the current UV coordinate
    // Adjust the number of samples for a more or less pronounced blur effect
    int samplesPerDirection = 5; // Including the center sample
    int totalSamples = samplesPerDirection * samplesPerDirection;

    // Loop to sample around the current UV coordinate
    for(int dx = -samplesPerDirection / 2; dx <= samplesPerDirection / 2; ++dx) {
        for(int dy = -samplesPerDirection / 2; dy <= samplesPerDirection / 2; ++dy) {
            // Calculate the offset for the current sample
            vec2 offset = vec2(float(dx), float(dy)) * blurSize;
            // Accumulate the color from the texture at the offset position
            accumulatedColor += texture2D(u_texture, v_uv + offset);
        }
    }

    // Calculate the average color by dividing by the total number of samples
    vec4 averageColor = accumulatedColor / float(totalSamples);

    // Set the output color to the average color
    gl_FragColor = averageColor;
    }
}