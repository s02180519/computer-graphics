#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float blur;

const float offset = 1.0 / 300.0; 

void main()
{ 
		if (blur == 7)
			FragColor = texture(screenTexture, TexCoords);
		else
		{
			vec2 offsets[9] = vec2[](
	        vec2(-offset,  offset), 
	        vec2( 0.0f,    offset), 
	        vec2( offset,  offset), 
	        vec2(-offset,  0.0f),   
			vec2( 0.0f,    0.0f),   
	        vec2( offset,  0.0f),   
	        vec2(-offset, -offset), 
	        vec2( 0.0f,   -offset), 
	        vec2( offset, -offset)     
		   );

			float kernel[9];
			kernel[0] = 1.0f;
			kernel[2] = 1.0f;
			kernel[6] = 1.0f;
			kernel[8] = 1.0f;
			kernel[1] = blur;
			kernel[3] = blur;
			kernel[5] = blur;
			kernel[7] = blur;
			kernel[4] = blur * blur;

			vec3 sampleTex[9];
		   for(int i = 0; i < 9; i++)
		    {
		        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
		    }
		   vec3 col = vec3(0.0);
		   for(int i = 0; i < 9; i++)
		      col = col + sampleTex[i] * kernel[i];
    
			col = col/(4 + 4 * blur + blur * blur);

		  FragColor = vec4(col, 1.0);
		}
}