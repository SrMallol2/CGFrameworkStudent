
varying vec2 v_uv;
uniform sampler2D u_texture1;
uniform int subTask;


void main() {

    vec4 color = texture2D(u_texture1, v_uv);

    if(subTask == 0) {
        gl_FragColor = vec4(color);
        
    } else if(subTask == 1) {
        float luminance = dot(color.rgb, vec3(0.2, 0.7, 0.01));

    // Grayscale
        vec3 actual_luminance = vec3(luminance);
        gl_FragColor = vec4(actual_luminance, color.a);

    } else if(subTask == 2) {
       
        vec3 invertedColor = 1.0 - color.rgb;

    
        gl_FragColor = vec4(invertedColor, color.a);

    } else if(subTask == 3) {

    // Yellow-scale
    float intensity_factor =0.5;
    color.rgb = vec3(color.r + color.g, color.r + color.g, 0.0);
    color.rgb = color.rgb*intensity_factor;

  
    gl_FragColor = color;

    } else if(subTask == 4) {

        float threshold = 0.5;

    // Grayscale
        float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722)); // Luminance calculation

    // Thresholding to create black and white image
        vec3 blackAndWhite = step(threshold, brightness) * vec3(1.0); // Set pixels above threshold to white, below to black

    // Output
        gl_FragColor = vec4(blackAndWhite, 1.0);

    } 
    else if(subTask == 5) {

 
        vec2 center = vec2(0.5, 0.5); 
        float distance = distance(v_uv, center);

   
        float illuminationFactor = 1.0 - distance; // More illumination at the center
        illuminationFactor = clamp(illuminationFactor, 0.0, 1.0); // Clamping to ensure it stays within valid range

        illuminationFactor = pow(illuminationFactor, 2.0);

        vec3 illuminatedColor = color.rgb * illuminationFactor;

        gl_FragColor = vec4(illuminatedColor, color.a);

    } else if(subTask == 6) {
         // Blur size (how far from the current UV to sample)
    float blurSize = 1.0 /256.0;


    vec4 accumulatedColor = vec4(0.0);

    // The total number of samples around the current UV coordinate
    int samplesPerDirection = 5; // Including the center sample
    int totalSamples = samplesPerDirection * samplesPerDirection;

    
    for(int dx = -samplesPerDirection / 2; dx <= samplesPerDirection / 2; ++dx) {
        for(int dy = -samplesPerDirection / 2; dy <= samplesPerDirection / 2; ++dy) {
            // Offset for the current sample
            vec2 offset = vec2(float(dx), float(dy)) * blurSize;
            
            accumulatedColor += texture2D(u_texture1, v_uv + offset);
        }
    }

    // Average color by dividing by the total number of samples
    vec4 averageColor = accumulatedColor / float(totalSamples);

    // Set the output color to the average color
    gl_FragColor = averageColor;
    }
}