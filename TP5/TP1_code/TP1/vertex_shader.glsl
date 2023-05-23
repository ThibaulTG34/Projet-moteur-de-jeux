#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 uv_terrain;
out vec2 uv_roche;
out vec2 uv_peau;
out vec2 uv_bois;
out vec2 uv_arbre;
out vec3 textCoords_skybox;
out vec3 Normal;

uniform int mode;
uniform sampler2D heightmap;

void main(){
        
        if(mode == 1){
                uv_terrain = uv;
                gl_Position = projection * view * model * vec4(vertices_position_modelspace, 1);
        }
        else if(mode == 2){
                textCoords_skybox = vertices_position_modelspace;
                gl_Position = projection * view * model * vec4(vertices_position_modelspace.x, vertices_position_modelspace.y, vertices_position_modelspace.z, 1.0);
        }
        else if(mode==3){
                uv_roche = uv;
                gl_Position = projection * view * model * vec4(vertices_position_modelspace, 1);
        }
        else if(mode==4){
                uv_peau = uv;
                gl_Position = projection * view * model * vec4(vertices_position_modelspace, 1);
        }
        else if(mode==5){
                uv_bois = uv;
                gl_Position = projection * view * model * vec4(vertices_position_modelspace, 1);
        }
        else if(mode==6){
                uv_arbre = uv;
                gl_Position = projection * view * model * vec4(vertices_position_modelspace, 1);
        }
        else{
                gl_Position = projection * view * model * vec4(vertices_position_modelspace, 1);
        }

        Normal = normal;

}

