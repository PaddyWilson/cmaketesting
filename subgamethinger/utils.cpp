#include "utils.h"

#include <curl/curl.h>
void DownloadHTTP(std::string address, std::string filename)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;
    // char *url = URL.c_str();
    // char outfilename[FILENAME_MAX] = "purple.png";
    curl = curl_easy_init();
    if (curl)
    {
        fp = fopen(filename.c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    return 0;
}