#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform vec3 cameraPos;

out vec2 texCoord;

void main()                                                                         
{       
    mat4 gVP = projMatrix * viewMatrix;                                                                            
    vec3 pos = gl_in[0].gl_Position.xyz;                                            
    vec3 CameraToPoint = pos - cameraPos; 
    CameraToPoint = normalize(vec3(CameraToPoint.x, 0, CameraToPoint.z));                                  
    vec3 up = vec3(0.0, 1.0, 0.0);                                                  
    vec3 right = cross(up, CameraToPoint);                                               
         
    // bottom left
    pos -= 0.5*right;
    gl_Position = gVP * vec4(pos, 1.0);
    texCoord = vec2(0.0, 0.0);                                                      
    EmitVertex(); 
                  
    // top left
    pos.y += 1.0;                                                                   
    gl_Position = gVP * vec4(pos, 1.0);                                             
    texCoord = vec2(0.0, 1.0);                                                      
    EmitVertex();                                                                   
    
    // bottom right
    pos.y -= 1.0;
    pos += right;    
    gl_Position = gVP * vec4(pos, 1.0);
    texCoord = vec2(1.0, 0.0);
    EmitVertex();

    // top right
    pos.y += 1.0;
    gl_Position = gVP * vec4(pos, 1.0);
    texCoord = vec2(1.0, 1.0);
    EmitVertex();
                                                                                    
    EndPrimitive();                                                                 
}   