#ifndef INPU_MOUSE_READER_OBJC_H
#define INPU_MOUSE_READER_OBJC_H

namespace lct
{
namespace inpu
{

bool IsTouchDownEvent(void* uiEvent);
bool IsTouchUpEvent(void* uiEvent);
bool IsTouchMoveEvent(void* uiEvent);
int GetXParam(void* uiEvent);
int GetYParam(void* uiEvent);

void LogEvent(void* uiEvent);
    
//namespace inpu
}
//namespace lct
}

#endif//INPU_MOUSE_READER_OBJC_H
