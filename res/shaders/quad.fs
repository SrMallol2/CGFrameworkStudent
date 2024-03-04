

varying vec2 v_uv;
uniform float u_time;
uniform vec2 framebuffer_size;
uniform int subTask;
uniform float PI;

void main() {
	float motion = abs(sin(u_time));

	float framebufferWidth = framebuffer_size.x;
	float framebufferHeight = framebuffer_size.y;

    // Aspect ratio of the framebuffer
	float framebuffer_aspect_ratio = framebufferWidth / framebufferHeight;

	if(subTask == 1) {

		gl_FragColor = vec4(v_uv.x, 0.0, 1.0 - v_uv.x, 0.0);
		
	} else if(subTask == 2) {
		vec2 center = vec2(0.5);
		float dist_ = distance(center, v_uv);
		vec3 final_color = vec3(dist_);
		gl_FragColor = vec4(final_color, 0.0);

	} 
	else if(subTask == 3) {

	vec2 aspectUVs = gl_FragCoord.xy/framebuffer_aspect_ratio;
    vec2 grid = fract(aspectUVs/80.0); 
    vec3 color = vec3(0.0); //Background

    float lineH = 0.5 * (1.0 + sin(2.0*PI * (grid.x-0.5))); // Horizontal blur
    float lineV = 0.5 * (1.0 + sin(2.0*PI * (grid.y-0.5))); // Vertical blur

	
    color +=  (1.0 - lineH) * vec3(0.0, 0.0, 1.0); // Blue color
    color += (1.0 - lineV) * vec3(1.0, 0.0, 0.0); // Red Color

    gl_FragColor = vec4(color, 1.0);
	} 

	else if(subTask == 4) {

    	//UV coordinates of each fragment
		vec2 uv = gl_FragCoord.xy / framebuffer_size;

    	// Number of squares per row and column based on the screen size
		int numSquaresX = int(framebuffer_size.x / 50.0); 
		int numSquaresY = int(framebuffer_size.y / 50.0);

    	// Size of each square in UV space based on the number of squares
		float squareSizeX = 1.0 / float(numSquaresX);
		float squareSizeY = 1.0 / float(numSquaresY);

    
		int row = int(uv.y / squareSizeY);
		int col = int(uv.x / squareSizeX);

   		 // Color for the entire screen based on the row and column indices
		vec4 color = vec4(float(col) / float(numSquaresX), float(row) / float(numSquaresY), 0.0, 0.0);


		gl_FragColor = color;

	} else if(subTask == 5) {

			
		const int num_rows = 12;
    		
		vec2 cell_size;
		cell_size.y = 1.0 / float(num_rows);
		cell_size.x = cell_size.y / framebuffer_aspect_ratio;

    		
		ivec2 cell_index = ivec2(v_uv / cell_size);

    	// Alternatiion if colors based cell index
		vec3 final_color = mix(vec3(1.0, 1.0, 1.0), vec3(0.0, 0.0, 0.0), mod(float(cell_index.x + cell_index.y), 2.0));

		gl_FragColor = vec4(final_color, 1.0);

	} else if(subTask == 6) {
		
		float frequency = 10.0;
		float amplitude = 0.2;

    	// Wave displacement
		float waveDisplacement = amplitude * sin(v_uv.x * frequency);

    
		vec2 displacedUV = v_uv + vec2(0.0, waveDisplacement);

    
		vec3 upperColor = vec3(0.0, 1.0 - v_uv.y, 0.0); // Dark green
		vec3 lowerColor = vec3(0.0, v_uv.y, 0.0); // Light green


		vec3 gradientColor = mix(lowerColor, upperColor, step(0.5, displacedUV.y));

   
		gl_FragColor = vec4(gradientColor, 1.0);
	}

}
