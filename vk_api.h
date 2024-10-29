#include <curl/curl.h>

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

typedef struct 
{
    size_t size;
    char* data;
} Buffer;


size_t write_callback(void* contents, size_t size, size_t nmemb, Buffer* storage) {
    size_t total_size = size * nmemb;

    char* reallocated_data = realloc(storage->data, storage->size + total_size + 1);
    if (reallocated_data == NULL) {
        fprintf(stderr, "realloc() returned NULL\n");
        return 0;
    }

    storage->data = reallocated_data;
    memcpy(&(storage->data[storage->size]), contents, total_size);
    storage->size += total_size;
    storage->data[storage->size] = 0;

    return total_size;
}

char* curl_request(CURL* curl, char* url) {
    CURLcode res;

    Buffer* storage = malloc(sizeof(Buffer));
    storage->data = malloc(1);
    storage->size = 0;
    if (storage == NULL) {
        fprintf(stderr, "malloc() failed: couldn't allocate buffer\n");
        return NULL;
    }
    else if (storage->data == NULL) {
        fprintf(stderr, "malloc() failed: couldn't allocate buffer\n");
        return NULL;
    }


    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, storage);
    
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_request() failed: %s\n", curl_easy_strerror(res));
        return NULL;
    } 

    char* data = storage->data;
    free(storage);

    return data;
}

char* get_users(CURL* curl, VKQueryParams* args) {
    CURLcode res;

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

    char* response = curl_request(curl, url);

    return response;
}

char* get_user_followers(CURL* curl, VKQueryParams* args) {
    CURLcode res;

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

    char* response = curl_request(curl, url);

    return response;
}

char* get_user_subscriptions(CURL* curl, VKQueryParams* args) {
    CURLcode res;

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

    char* response = curl_request(curl, url);

    return response;
}
