#include "CurlTest.h"

#include <curl/curl.h>

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

void CurlTest::CurlTest(std::string url)
{
	CURL* curl;
	    CURLcode res;

		const char* urls = url.c_str();
	    curl_global_init(CURL_GLOBAL_DEFAULT);
	
	    curl = curl_easy_init();
	    if (curl)
	    {
			curl_easy_setopt(curl, CURLOPT_URL, urls);
			//curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
			//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // this is needed if I use CURLOPT_USE_SSL othervise it is unable to connect to https if I use CURLOPT_USE_SSL

	#ifdef SKIP_PEER_VERIFICATION
	        /*
	         * If you want to connect to a site who is not using a certificate that is
	         * signed by one of the certs in the CA bundle you have, you can skip the
	         * verification of the server's certificate. This makes the connection
	         * A LOT LESS SECURE.
	         *
	         * If you have a CA cert for the server stored someplace else than in the
	         * default bundle, then the CURLOPT_CAPATH option might come handy for
	         * you.
	         */
	        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	#endif
	
	#ifdef SKIP_HOSTNAME_VERIFICATION
	        /*
	         * If the site you are connecting to uses a different host name that what
	         * they have mentioned in their server certificate's commonName (or
	         * subjectAltName) fields, libcurl refuses to connect. You can skip this
	         * check, but it makes the connection insecure.
	         */
	        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	#endif
	
	        /* cache the CA cert bundle in memory for a week */
	        curl_easy_setopt(curl, CURLOPT_CA_CACHE_TIMEOUT, 604800L);
	
	        /* Perform the request, res gets the return code */
	        res = curl_easy_perform(curl);
	        /* Check for errors */
	        if (res != CURLE_OK)
	            fprintf(stderr, "curl_easy_perform() failed: %s\n",
	                    curl_easy_strerror(res));
	
	        /* always cleanup */
	        curl_easy_cleanup(curl);
	    }
	
	    curl_global_cleanup();
}

void CurlTest::CurlDownload(std::string url)
{
	CURL* curl;
	FILE* fp;
	CURLcode res;
	const char* urls = url.c_str();
	char outfilename[FILENAME_MAX] = "padwilson.jpg";
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (curl) {
		fp = fopen(outfilename, "wb");
		curl_easy_setopt(curl, CURLOPT_URL, urls);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

		/* follow redirection */
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		/* SSL Options */
		//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1);
		//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1);
		/* Provide CA Certs from http://curl.haxx.se/docs/caextract.html */
		//curl_easy_setopt(curl, CURLOPT_CAINFO, "ca-bundle.crt");

		res = curl_easy_perform(curl);
		/* always cleanup */
		curl_easy_cleanup(curl);
		fclose(fp);
	}
}
