#version 330 core

// Ouput data
out vec4 FragColor;
out vec3 Color;

in vec2 UV;
in float Y;
in vec3 textCoords;
in vec3 Normal;

uniform sampler2D textureID;
uniform samplerCube sktext;
uniform int texturemode;

void main(){

        if (texturemode==1){
                FragColor = texture(textureID, UV);
        }
        else{
                FragColor = texture(sktext, textCoords);
        }
              
        
        
}
