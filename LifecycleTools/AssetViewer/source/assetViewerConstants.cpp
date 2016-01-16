#include "assetViewerConstants.h"

#if defined (LCT_WINDOWS) || defined(LCT_OSX)
const lct::foun::Vector2 PRESS_SIZE = { 0.0f, 0.0f };
#else
const lct::foun::Vector2 PRESS_SIZE = { 64.0f, 64.0f };
#endif
