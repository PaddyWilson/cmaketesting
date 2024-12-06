#pragma once

#include <string>

void StringReplace(std::string* in, std::string find, std::string replace);

bool OpenFileDialog(std::string cwd, std::string& outPath);
bool SaveFileDialog(std::string cwd, std::string& outPath);