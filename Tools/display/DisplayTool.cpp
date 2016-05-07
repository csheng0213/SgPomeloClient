//
//  DisplayTool.cpp
//  LuaSanGuo
//
//  Created by 常胜 on 15/7/16.
//
//

#include "DisplayTool.h"

void DisplayTool::graySprite(cocos2d::Node *sp, bool isGray)
{
    if (isGray)
    {
        if(sp)
        {
            GLProgram * p = new GLProgram();
            p->initWithFilenames("res/shader/gray.vsh", "res/shader/gray.fsh");
            
            p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
            p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
            p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
            p->link();
            
            p->updateUniforms();
            sp->setGLProgram(p);
        }
    }else
    {
        if(sp)
        {
            sp->setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
        }

    }
}