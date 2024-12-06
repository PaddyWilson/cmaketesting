#pragma once
#include <string>
#include <vector>

class Downloader
{
public:
	static int Download(std::string URL, std::string saveName);
	static int DownloadList(std::vector<std::string> URL, std::vector<std::string> saveName);

	static void DownloadAsync(std::string URL, std::string saveName);
	static void DownloadListAsync(std::vector<std::string> URL, std::vector<std::string> saveName);

private:

};