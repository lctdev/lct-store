#ifndef ASSET_VIEWER_MESSAGES
#define ASSET_VIEWER_MESSAGES

enum MessageType
{
	MESSAGE_TYPE_PROGRAM_CLOSE_MENU_OVERLAY = 0,
	MESSAGE_TYPE_PROGRAM_CHANGE_MODE
};

struct ProgramChangeModeContent
{
	const char* pNextModeName;
};

#endif//ASSET_VIEWER_MESSAGES
