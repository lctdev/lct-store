//
//  Shader.fsh
//  AssetViewer_IOS
//
//  Created by Michael Kron on 10/23/15.
//  Copyright © 2015 Michael Kron. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
