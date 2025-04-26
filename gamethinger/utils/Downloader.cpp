#include "Downloader.h"

#include <iostream>
#include <thread>
#include <curl/curl.h>
// #ifdef __unix__
// #include <cstdlib>
// #elif defined(_WIN32) || defined(WIN32)
// #include <tchar.h>
// #include <Windows.h>
// #include <Urlmon.h>
// #pragma comment(lib, "urlmon.lib")
// #endif

using namespace std;

int Downloader::Download(std::string URL, std::string saveName)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;
    // char *url = URL.c_str();
    // char outfilename[FILENAME_MAX] = "purple.png";
    curl = curl_easy_init();
    if (curl)
    {
        fp = fopen(saveName.c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    return 0;
    // #ifdef __unix__
    // 	std::string command = "wget -O "+ saveName + " " + URL;

    // 	// giving system command and storing return value
    //     int returnCode = system(command.c_str());

    //     // checking if the command was executed successfully
    //     if (returnCode == 0) {
    //         cout << "Command executed successfully." << endl;
    //     }
    //     else {
    //         cout << "Command execution failed or returned "
    //                 "non-zero: "
    //              << returnCode << endl;
    //     }
    // #elif defined(_WIN32) || defined(WIN32)

    // 	std::wstring temp_url = std::wstring(URL.begin(), URL.end());
    // 	std::wstring temp_saveName = std::wstring(saveName.begin(), saveName.end());

    // 	// URL of the file to download
    // 	const wchar_t* sURL = temp_url.c_str();// URL.c_str();//  L"https://cards.scryfall.io/png/front/9/4/94eea6e3-20bc-4dab-90ba-3113c120fb90.png?1670031714";
    // 	// Name of the Destination file
    // 	const wchar_t* dFile = temp_saveName.c_str(); // L"image.png";

    // 	//this only works on windows
    // 	if (S_OK == URLDownloadToFile(NULL, URL.c_str(), saveName.c_str(), 0, NULL))
    // 	{
    // 		cout << "The file is saved to " << saveName << endl;
    // 		return 0;
    // 	}
    // 	else
    // 	{
    // 		cout << "Unable to Download the file." << endl;
    // 		return -1;
    // 	}
    // #endif
}

int Downloader::DownloadList(std::vector<std::string> URL, std::vector<std::string> saveName)
{
    if (URL.size() != saveName.size())
        return -1;

    for (size_t i = 0; i < URL.size(); i++)
        Download(URL[i], saveName[i]);

    return 0;
}

void Downloader::DownloadAsync(std::string URL, std::string saveName)
{
    thread thread(Download, URL, saveName);
}

void Downloader::DownloadListAsync(std::vector<std::string> URL, std::vector<std::string> saveName)
{
    // has to have matching sizes
    if (URL.size() != saveName.size())
        return;

    thread thread(DownloadList, URL, saveName);
}
