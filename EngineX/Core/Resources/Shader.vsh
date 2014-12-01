//
//  Shader.vsh
//  EngineX
//
//  Created by Maxim Reshetey on 18.11.12.
//  Copyright (c) 2012 Maxim Reshetey. All rights reserved.
//

attribute vec4 attrPosition;

uniform mat4 modelViewProjectionMatrix;

void main()
{
    gl_Position = modelViewProjectionMatrix * attrPosition;
}
