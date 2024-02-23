precision mediump float; // Add this line
varying vec2 v_uv;
uniform float u_time;
uniform vec2 framebuffer_size;
uniform int subTask;

void main() {
	float motion = abs(sin(u_time));

	float framebufferWidth = framebuffer_size.x;
	float framebufferHeight = framebuffer_size.y;

// Aspect ratio of the framebuffer
	float framebuffer_aspect_ratio = framebufferWidth / framebufferHeight;


		if(subTask == 1) {
			//gl_FragColor = vec4(v_uv.x, 0.0, 1.0 - v_uv.x, 0.0);
			// Define the number of vertical lines
			// Define the position of the horizontal line
			float linePositionHorizontal = 0.5; // Adjust this value to change the position of the horizontal line (range: 0.0 to 1.0)

    // Define the number of vertical lines
			const int num_vertical_lines = 1;
    // Define the color for the lines
			vec3 horizontalLineColor = vec3(0.1, 0.0, 0.98); // Blue
			vec3 verticalLineColor = vec3(0.0, 0.0, 0.0); // Red

    // Define the positions of the vertical lins
			float verticalLinePositions[num_vertical_lines];

    // Set the positions of the vertical lines
			for(int i = 0; i < num_vertical_lines; i++) {
				verticalLinePositions[i] = float(i + 1) / float(num_vertical_lines + 1);
			}

    // Calculate the distance from the horizontal line
			float distanceToLineHorizontal = abs(fract(v_uv.y) - linePositionHorizontal);

    // Calculate the color of the horizontal line based on the distance
			float edgeWidth = 0.05; // Adjust this value to change the width of the diffused edge
			float blendFactorHorizontal = smoothstep(0.0, edgeWidth, distanceToLineHorizontal); // Blend factor for the diffused edge
			vec3 lineColorHorizontal = mix(horizontalLineColor, vec3(0.0), blendFactorHorizontal); // Blend with a light color

    // Calculate the color of the vertical lines
			vec3 lineColorVertical = vec3(1.0, 0.0, 0.0); // Initialize with black background color

    // Check if the fragment is on any of the vertical lines
			for(int i = 0; i < num_vertical_lines; i++) {
				float distanceToLineVertical = abs(fract(v_uv.x) - verticalLinePositions[i]);
				float blendFactorVertical = smoothstep(0.0, 0.05, distanceToLineVertical); // Adjust the width of the diffused edge
				lineColorVertical += mix(vec3(0.0), verticalLineColor, blendFactorVertical);
			}

    // Combine the colors of the horizontal and vertical lines
			vec3 finalColor = lineColorHorizontal + lineColorVertical;

    // Output the final color
			gl_FragColor = vec4(finalColor, 1.0);

		} else if(subTask == 2) {
			vec2 center = vec2(0.5);
			float dist_ = distance(center, v_uv);
			vec3 final_color = vec3(dist_);
			gl_FragColor = vec4(final_color, 0.0);

		} else if(subTask == 3) {
			 // Define the number of horizontal lines and their positions
			const int num_lines = 5;
			float linePositions[num_lines];
			linePositions[0] = 0.1; // Example line position
			linePositions[1] = 0.3; // Example line position
			linePositions[2] = 0.5; // Example line position
			linePositions[3] = 0.7; // Example line position
			linePositions[4] = 0.9; // Example line position

    // Define the color for the lines
			vec3 lineColor = vec3(0.0, 0.0, 1.0); // Blue

    // Define the width of the diffused edge
			float edgeWidth = 0.05; // Adjust this value to change the width of the diffused edge

    // Calculate the color of the lines
			vec3 finalColor = vec3(0.0); // Initialize with black background color

    // Loop through each line
			for(int i = 0; i < num_lines; ++i) {
        // Calculate the distance from the current line
				float distanceToLine = abs(fract(v_uv.y) - linePositions[i]);

        // Calculate the blend factor for the diffused edge
				float blendFactor = smoothstep(0.0, edgeWidth, distanceToLine);

        // Blend the line color with the background color
				finalColor += mix(vec3(0.0), lineColor, blendFactor);
			}

    // Output the final color
			gl_FragColor = vec4(finalColor, 1.0);

		} else if(subTask == 4) {

			// Define the number of rows and columns in the grid
			const int num_rows = 16;
			const int num_cols = 16;

    // Calculate the size of each cell
			float cell_size = min(1.0 / float(num_cols), 1.0 / float(num_rows));

    // Calculate the position within each cell (ranging from 0 to 1)
			vec2 posInCell = fract(v_uv / cell_size);

    // Calculate the distance from the nearest edge of the cell
			float distToEdgeX = min(posInCell.x, 1.0 - posInCell.x);
			float distToEdgeY = min(posInCell.y, 1.0 - posInCell.y);
			float distToEdge = min(distToEdgeX, distToEdgeY);

    // Calculate the darkness of the grid lines based on the distance from the edges
			float darkness = mix(v_uv.x, v_uv.y, distToEdge + 1.0); // Adjust the range as needed

    // Set the base color for the grid lines (slightly darker than the cell color)
			vec3 baseLineColor = vec3(v_uv.x, v_uv.y, 0.0); // Darker grey

    // Calculate the final color of the grid lines
			vec3 lineColor = baseLineColor * darkness;

    // Output the color
			gl_FragColor = vec4(lineColor, 1.0);

		} else if(subTask == 5) {

			//Rows and columns in the grid
			const int num_rows = 12;
			const int num_cols = 12;

    		// Calculate the size of each cell in UV space
			vec2 cell_size;
			cell_size.y = 1.0 / float(num_rows);
			cell_size.x = cell_size.y / framebuffer_aspect_ratio;

    		// Calculate the index of the cell based on the UV coordinates
			ivec2 cell_index = ivec2(v_uv / cell_size);

    		// Calculate the alternating color based on the cell index
			vec3 final_color = mix(vec3(1.0, 1.0, 1.0), vec3(0.0, 0.0, 0.0), mod(float(cell_index.x + cell_index.y), 2.0));

			gl_FragColor = vec4(final_color, 1.0);
		} else if(subTask == 6) {
			// Define the position of the sine wave
			float wavePosition = 0.5; // Adjust this value to change the position of the wave (range: 0.0 to 1.0)

    // Calculate the position within each cell (ranging from 0 to 1)
			vec2 posInCell = fract(v_uv);

    // Calculate the distance from the sine wave
			float distanceToWave = abs(posInCell.y - wavePosition);

    // Calculate the amplitude and frequency of the sine wave
			float amplitude = 0.1; // Adjust this value to change the height of the wave
			float frequency = 10.0; // Adjust this value to change the frequency of the wave

    // Calculate the sine wave value
			float waveValue = sin(posInCell.x * frequency * 3.14159) * amplitude;

    // Determine the direction of the gradient based on the position relative to the wave
			bool belowWave = posInCell.y < wavePosition;
			bool aboveWave = posInCell.y > wavePosition;

    // Calculate the gradient color based on the position relative to the wave
			vec3 gradientColor;
			if(belowWave) {
				gradientColor = mix(vec3(0.1, 0.4, 0.1), vec3(0.7, 1.0, 0.7), posInCell.y);
			} else if(aboveWave) {
				gradientColor = mix(vec3(0.7, 1.0, 0.7), vec3(0.1, 0.4, 0.1), posInCell.y);
			} else {
				gradientColor = vec3(0.1, 0.4, 0.1);
			}
			vec3 color = vec3(0.7, 1.0, 0.7);
			vec3 color2 = vec3(0.1, 0.4, 0.1);

    // Apply the sine wave to the gradient color
			gradientColor *= 1.0 - smoothstep(-0.02, 0.02, waveValue - distanceToWave);

    // Output the final color
			gl_FragColor = vec4(gradientColor, 1.0);
		}

}
