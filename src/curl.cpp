#include <iostream>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>

int main(int argc, char** argv)
{
  if (argc < 3) {
    fprintf(stderr, "%s: Mauvais nombre d'arguments\n", argv[0]);
    return -1;
  }
  CURL *curl;
  CURLcode res = CURLE_OK;
 
  curl = curl_easy_init();
  if(curl) {
    /* Set username and password */ 
    curl_easy_setopt(curl, CURLOPT_USERNAME, argv[1]);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, argv[2]);
 
    /* This will fetch message 1 from the user's inbox */ 
    curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.sfr.fr:993/INBOX/;UID=*/");
 
    /* Perform the fetch */ 
    res = curl_easy_perform(curl);
 
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* Always cleanup */ 
    curl_easy_cleanup(curl);
  }
 
  return (int)res;
}
