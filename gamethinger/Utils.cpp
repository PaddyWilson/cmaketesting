#include "Utils.h"

#include <nfd.h>
#include <stdio.h>
#include <stdlib.h>

void StringReplace(std::string* in, std::string find, std::string replace)
{
	size_t pos = in->find(find);
	if (pos != std::string::npos) {
		in->replace(pos, find.size(), replace);   // 5 = length( $name )
	}
}

bool OpenFileDialog(std::string cwd, std::string& outPath)
{
	/*nfdchar_t* outPath = NULL;
	nfdresult_t result = NFD_OpenDialogU8(&outPath, NULL, NULL, NULL);

	if (result == NFD_OKAY) {
		puts("Success!");
		puts(outPath);
		free(outPath);
	}
	else if (result == NFD_CANCEL) {
		puts("User pressed cancel.");
	}
	else {
		printf("Error: %s\n", NFD_GetError());
	}*/


	nfdchar_t* outP = NULL;
	nfdresult_t result = NFD_OpenDialogU8(&outP, NULL, NULL, NULL);

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
	nfdresult_t result = NFD_SaveDialogU8(&outP, NULL, NULL, cwd.c_str(), NULL);

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