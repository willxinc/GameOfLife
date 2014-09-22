#include "winallegdialogs.h"

// Wrapper functions for GetOpenFileName and GetSaveFileName
// see http://msdn.microsoft.com/en-us/library/windows/desktop/ms646928(v=vs.85).aspx
// for more details
void loadDialog(char (&fname)[256]){
    OPENFILENAME ofn;

	ZeroMemory( &ofn , sizeof( ofn));
	ofn.lStructSize = sizeof ( ofn );
	ofn.lpstrFile = fname;
	ofn.nMaxFile = sizeof( fname );
	ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn)){
		fname[0] = '\0';
	}
}
void saveDialog(char (&fname)[256]){
    OPENFILENAME sfn;

	ZeroMemory( &sfn , sizeof( sfn));
	sfn.lStructSize = sizeof ( sfn );
	sfn.lpstrFile = fname ;
	sfn.nMaxFile = sizeof( fname );
	sfn.Flags = OFN_PATHMUSTEXIST;

	if (!GetSaveFileName(&sfn)){
		fname[0] = '\0';
	}
}
