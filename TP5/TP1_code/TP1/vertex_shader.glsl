#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 UV;
out vec3 textCoords;
out vec3 Normal;

uniform int mode;
uniform sampler2D heightmap;

void main(){
        
        UV = uv;

        if(mode == 1){
                vec3 pos = vec3(vertices_position_modelspace.x, vertices_position_modelspace.y, vertices_position_modelspace.z);
                gl_Position = projection * view * model * vec4(pos, 1);
        }
        else if(mode == 2){
                textCoords = vertices_position_modelspace;
                gl_Position = projection * view * model * vec4(vertices_position_modelspace.x, vertices_position_modelspace.y, vertices_position_modelspace.z, 1.0);
        }
        else{
                gl_Position = projection * view * model * vec4(vertices_position_modelspace, 1);
        }

        Normal = normal;

}

