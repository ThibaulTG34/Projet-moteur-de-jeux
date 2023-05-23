#version 330 core

// Ouput data
out vec4 FragColor;
out vec3 Color;

in vec2 uv_terrain;
in vec2 uv_roche;
in vec2 uv_peau;
in vec2 uv_bois;
in vec2 uv_arbre;
in float Y;
in vec3 textCoords_skybox;
in vec3 Normal;

uniform sampler2D texture_terrain;
uniform sampler2D texture_roche;
uniform sampler2D texture_peau;
uniform sampler2D texture_bois;
uniform sampler2D texture_arbre;
uniform samplerCube sktext;
uniform int mode;

void main(){

        if (mode==1){
                FragColor = texture(texture_terrain, uv_terrain);
        }
        else if(mode==2){
                FragColor = texture(sktext, textCoords_skybox);
        }
        else if(mode==3){
                FragColor = texture(texture_roche, uv_roche);
        }
        else if(mode==4){
                FragColor = texture(texture_peau, uv_peau);
        }
        else if(mode==5){
                FragColor = texture(texture_bois, uv_bois);
        }
        else if(mode==6){
                FragColor = texture(texture_arbre, uv_arbre);
        }
        else{
                FragColor = vec4(0, 0, 0, 1);
        }
              
        
        
}
