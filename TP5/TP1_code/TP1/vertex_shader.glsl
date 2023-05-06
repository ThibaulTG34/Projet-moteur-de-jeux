#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;

uniform vec3 viewPos;
uniform vec3 lightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 UV;
out vec3 textCoords;

out vec3 o_positionWorld;
out vec3 tang_viewPos;
// out vec2 o_uv0;
out vec3 tang_lightPos;
out vec3 tang_pos;

uniform int mode;
uniform sampler2D heightmap;

void main(){

        
        
        UV = uv;

        // o_positionWorld = vec3(model * vec4(vertices_position_modelspace, 1.0));
        // mat3 normalMatrix = mat3(transpose(inverse(model)));
        // // o_uv0 = uv0;
        // vec3 tang = normalize(normalMatrix * tangent);
        // vec3 norm = normalize(normalMatrix * normal);
        // tang = normalize(tang - dot(tang, norm) * norm);
        // vec3 bitan = cross(norm, tang);

        // mat3 tan_bitan_norm = transpose(mat3(tang, bitan, norm));
        // tang_lightPos = tan_bitan_norm * lightPos;
        // tang_viewPos = tan_bitan_norm * viewPos;
        // tang_pos = tan_bitan_norm * o_positionWorld;

        float Height = 0;  //texture(heightmap, UV).r;

        if(mode == 1){
                vec3 pos = vec3(vertices_position_modelspace.x, Height, vertices_position_modelspace.z);
                gl_Position = projection * view * model * vec4(pos, 1);
        }
        else if(mode==2){
                textCoords = vertices_position_modelspace;
                gl_Position = projection * view * model * vec4(vertices_position_modelspace.x, vertices_position_modelspace.y, vertices_position_modelspace.z, 1.0);
               
        }
        else{
                 gl_Position = projection * view * model * vec4(vertices_position_modelspace, 1);
        }

}

