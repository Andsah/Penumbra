#version 150                                                                      
                                                                                    
uniform sampler2D textureMap;                                                        
                                                                                    
in vec2 texCoord;                                                                   
out vec4 out_Color;                                                                 
                                                                                    
void main()                                                                         
{                                                                                   
    out_Color = texture2D(textureMap, texCoord);                                     
                                                                                    
    if ((out_Color.r == 0 && out_Color.g == 0 && out_Color.b == 0) || out_Color.a == 0) {
        discard;                                                                    
    }
                                                                              
}