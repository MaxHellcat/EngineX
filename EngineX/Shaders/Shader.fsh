//
//  Shader.fsh
//  EngineX
//
//  Created by Maxim Reshetey on 18.03.13.
//  Copyright (c) 2013 Maxim Reshetey. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
