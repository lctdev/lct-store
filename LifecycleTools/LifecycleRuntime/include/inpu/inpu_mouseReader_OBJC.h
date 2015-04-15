#ifndef INPU_MOUSE_READER_OBJC_H
#define INPU_MOUSE_READER_OBJC_H

namespace lct
{
namespace inpu
{

bool IsMouseDownEvent(void* nsEvent);
bool IsMouseUpEvent(void* nsEvent);
bool IsMouseMoveEvent(void* nsEvent);
int GetXParam(void* nsEvent);
int GetYParam(void* nsEvent);
    
//namespace inpu
}
//namespace lct
}

#endif//INPU_MOUSE_READER_OBJC_H
