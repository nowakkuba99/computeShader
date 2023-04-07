// #version 330 core
// out vec4 FragColor;
  
// in vec3 ourColor;
// in vec2 TexCoord;

// uniform sampler2D texture1;
// uniform sampler2D texture2;

// void main()
// {
//     FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
// }
#version 430 core
out vec4 FragColor;
	
in vec2 TexCoords;
	
uniform sampler2D tex;


vec4 colormap(float x);

void main()
{             
    float texCol = texture(tex, TexCoords).r;    

    // Normalize
    //  scaledValue = (rawValue - min) / (max - min);
    // min = -1, max = 1
    texCol = (texCol - (-1)) / (1 - (-1));
    FragColor = vec4(colormap(texCol));
}

/* COLOR MAP SHADER
Credits to:
https://github.com/kbinani/colormap-shaders/blob/master/shaders/glsl/MATLAB_jet.frag */
float colormap_red(float x) {
    if (x < 0.7) {
        return 4.0 * x - 1.5;
    } else {
        return -4.0 * x + 4.5;
    }
}

float colormap_green(float x) {
    if (x < 0.5) {
        return 4.0 * x - 0.5;
    } else {
        return -4.0 * x + 3.5;
    }
}

float colormap_blue(float x) {
    if (x < 0.3) {
       return 4.0 * x + 0.5;
    } else {
       return -4.0 * x + 2.5;
    }
}

vec4 colormap(float x) {
    float r = clamp(colormap_red(x), 0.0, 1.0);
    float g = clamp(colormap_green(x), 0.0, 1.0);
    float b = clamp(colormap_blue(x), 0.0, 1.0);
    return vec4(r, g, b, 1.0);
}