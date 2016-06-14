//
//  SimpleShader.h
//  demo
//
//  Created by sbxfc on 16/6/2.
//  Copyright © 2016年 me.rungame.sbxfc. All rights reserved.
//

#ifndef SimpleShader_h
#define SimpleShader_h

const char* vert_shader =
"\
#version 330\n\
\n\
layout (location = 1) in vec3 position;\n\
layout (location = 2) in vec3 color;\n\
out vec3 ver_color;\n\
\n\
void main(void) { \n\
ver_color = color;\n\
gl_Position = vec4(position, 1.0);\n\
}\n\
";

const char* frag_shader =
"\
#version 330\n\
\n\
in vec3 ver_color;\n\
out vec4 frag_color;\n\
\n\
void main(void) { \n\
frag_color = vec4(ver_color,1.0);\n\
}\n\
";

#endif /* SimpleShader_h */
