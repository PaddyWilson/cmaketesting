#include "Utils.h"

#include <NativeFileDialog/nfd.h>

void StringReplace(std::string* in, std::string find, std::string replace)
{
	size_t pos = in->find(find);
	if (pos != std::string::npos) {
		in->replace(pos, find.size(), replace);   // 5 = length( $name )
	}
}

bool OpenFileDialog(std::string cwd, std::string& outPath)
{
	nfdchar_t* outP = NULL;
	nfdresult_t result = NFD_OpenDialog(NULL, cwd.c_str(), &outP);

	if (result == NFD_OKAY) {
		//puts("Success!");
		//puts(outPath);		

		outPath = outP;

		free(outP);
		return true;
	}
	else if (result == NFD_CANCEL) {
		puts("User pressed cancel.");
		return false;
	}
	else {
		printf("OpenFileDialog() Error: %s\n", NFD_GetError());
		return false;
	}
}

bool SaveFileDialog(std::string cwd, std::string& outPath)
{
	nfdchar_t* outP = NULL;
	nfdresult_t result = NFD_SaveDialog(NULL, cwd.c_str(), &outP);

	if (result == NFD_OKAY) {
		outPath = outP;
		free(outP);
		return true;
	}
	else if (result == NFD_CANCEL) {
		puts("User pressed cancel.");
		return false;
	}
	else {
		printf("SaveFileDialog() Error: %s\n", NFD_GetError());
		return false;
	}
}