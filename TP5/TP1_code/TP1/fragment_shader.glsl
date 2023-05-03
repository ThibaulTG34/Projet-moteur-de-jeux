#version 330 core

// Ouput data
out vec4 FragColor;
out vec3 Color;

in vec2 UV;
in float Y;
in vec3 textCoords;

uniform sampler2D textureID;
uniform samplerCube sktext;
uniform int mode;

void main(){

        if(mode==1){
                FragColor = texture(textureID, UV);
        }
        else if (mode==2){
                FragColor = texture(sktext, textCoords);
        }
        else if (mode==3){
                FragColor = vec4(255.0, 0.0, 0.0, 1.0);
        }
        else{
                FragColor = vec4(0.0, 255.0, 0.0, 1.0);
        }
        
        
}
