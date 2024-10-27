#include <curl/curl.h>
#include "vec.h"

#define BASE_URL "https://api.vk.com/method"
#define URL_LEN 256 // TODO: Dynamic allocation for URL building 

typedef struct 
{
    char* user_id;
    char* fields;
    char* count;
    char* lang;
    char* access_token;
    char* api_version;
} VKQueryParams;

size_t write_callback(void* contents, size_t size, size_t nmemb, Vec** vec) {
    for (size_t i = 0; i < nmemb; i++) {
        char* memb = (char*)contents + i * size;
        vec_push(vec, memb, size);
    }

    return size * nmemb;
}

CURLcode curl_request(CURL* curl, char* url, Vec** storage) {
    CURLcode res;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, storage);
    
    res = curl_easy_perform(curl);

    return res;
}

Vec* get_users(CURL* curl, VKQueryParams* args) {
    CURLcode res;

    Vec* response = vec_new(0, sizeof(char));
    if (response == NULL) {
        fprintf(stderr, "vec_new() failed\n");
        return NULL;
    }

    char url[URL_LEN];
    snprintf
    (
        url,
        sizeof(url),
        "%s/users.get?user_ids=%s&fields=%s&lang=%s&access_token=%s&v=%s",
        BASE_URL,
        args->user_id,
        args->fields,
        args->lang,
        args->access_token,
        args->api_version
    );

    res = curl_request(curl, url, &response);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_request() failed: %s\n", curl_easy_strerror(res));
        return NULL;
    } 


    return response;
}

Vec* get_user_followers(CURL* curl, VKQueryParams* args) {
    CURLcode res;

    Vec* response = vec_new(0, sizeof(char));
    if (response == NULL) {
        fprintf(stderr, "vec_new() failed\n");
        return NULL;
    }

    char url[URL_LEN];
    snprintf
    (
        url,
        sizeof(url),
        "%s/users.getFollowers?user_id=%s&fields=%s&count=%s&lang=%s&access_token=%s&v=%s",
        BASE_URL,
        args->user_id,
        args->fields,
        args->count,
        args->lang,
        args->access_token,
        args->api_version
    );

    res = curl_request(curl, url, &response);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_request() failed: %s\n", curl_easy_strerror(res));
        return NULL;
    } 


    return response;
}

Vec* get_user_subscriptions(CURL* curl, VKQueryParams* args) {
    CURLcode res;

    Vec* response = vec_new(0, sizeof(char));
    
    if (response == NULL) {
        fprintf(stderr, "vec_new() failed\n");
        return NULL;
    }

    char url[URL_LEN];
    snprintf
    (
        url,
        sizeof(url),
        "%s/users.getSubscriptions?user_id=%s&fields=%s&count=%s&lang=%s&access_token=%s&v=%s",
        BASE_URL,
        args->user_id,
        args->fields,
        args->count,
        args->lang,
        args->access_token,
        args->api_version
    );

    res = curl_request(curl, url, &response);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_request() failed: %s\n", curl_easy_strerror(res));
        return NULL;
    } 


    return response;
}
