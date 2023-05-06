#version 330 core

// Ouput data
out vec4 FragColor;
out vec3 Color;

in vec2 UV;
in float Y;
in vec3 textCoords;

in vec3 o_positionWorld;
in vec3 tang_viewPos;
in vec3 tang_lightPos;
in vec3 tang_pos;

uniform sampler2D textureID;
uniform sampler2D normal_texture;
uniform samplerCube sktext;
uniform int mode;

void main(){

        if(mode==1){
                FragColor = texture(textureID, UV);
                // vec3 normal = texture(normal_texture, UV).rgb;
                // normal = normalize(normal * 2.0 - 1.0);
                // vec3 color_text = texture(textureID, UV).rgb;

                // vec3 ambient = color_text * 1.0;
                // vec3 lightDir = normalize(tang_lightPos - o_positionWorld);
                // float diff = max(dot(normal, lightDir), 0.0);
                // vec3 diffuse = color_text * (diff * 1.0);

                // vec3 viewDir = normalize(tang_viewPos - tang_pos);
                // vec3 reflectDir = reflect(-lightDir, normal);
                // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
                // vec3 specular = (spec * vec3(0.2));

                // vec3 result = ambient + diffuse + specular;

                // FragColor = vec4(result, 1.0); 
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
