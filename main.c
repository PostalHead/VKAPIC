#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "vec.h"
#include "json.h"
#include "vk_api.h"

#define PATH_LEN 256  // TODO: Dynamic allocation for path building

#define DEFAULT_FILE_PATH_USER "user.json"
#define DEFAULT_FILE_PATH_FOLLOWERS "followers.json"
#define DEFAULT_FILE_PATH_SUBSCRIPTIONS "subscriptions.json"

#define DEFAULT_USER_ID "161166919"
#define USER_FIELDS "screen_name,sex,city,home_town"
#define GROUP_FIELDS "&extended=1"
#define COUNT "100"
#define LANG "0"
#define API_VERSION "5.199"

int write_str_to_file(const char* filename, const char* jsonData) {
    FILE* file = fopen(filename, "w");

    if (file == NULL) {
        fprintf(stderr, "fopen() failed: %s\n", filename);
        return -1;
    }

    fprintf(file, "%s", jsonData);

    fclose(file);

    return 0;
}

// [user_id, dir_path]
int parse_argv(int argc, char** argv, char** xargv) {
    struct option long_options[] = {
        {"user-id", required_argument, NULL, 'u'},
        {"dir-path", required_argument, NULL, 'p'},
        {NULL, 0, NULL, 0} 
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "u:p:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'u':
                xargv[0] = optarg;
                break;
            case 'p':
                xargv[1] = optarg; 
                break;
            case '?':
                fprintf(stderr, "Usage: %s [--user-id <id>] [--dir-path <path>]\n", argv[0]);
                return -1;
                break;
        }
    }

    return 0;
}

int main(int argc, char* argv[]) {
    
    char* access_token = getenv("ACCESS_TOKEN");
    if (access_token == NULL) {
        fprintf(stderr, "getenv() failed: couldn't read ACCESS_TOKEN\n");
        return -1;
    }

    char* xargv[] = {NULL, NULL};
    char* user_id = DEFAULT_USER_ID;
    char* dir_path = "";
    char path[PATH_LEN] = {0};

    if (parse_argv(argc, argv, xargv) == -1) { return -1; }

    if (xargv[0]) {
        user_id = xargv[0];
    }

    if (xargv[1]) {
        dir_path = xargv[1];
    }

    if (curl_global_init(CURL_GLOBAL_DEFAULT) != 0) {
        fprintf(stderr, "curl_global_init() failed\n");
        return -1;
    }

    CURL* curl = curl_easy_init();
    if (curl == NULL) {
        fprintf(stderr, "curl_easy_init() failed\n");
        return -1;
    }

    //get_users
    VKQueryParams args = {
        user_id,
        USER_FIELDS,
        COUNT,
        LANG,
        access_token,
        API_VERSION
    };

    char* response = get_users(curl, &args);

    if (response == NULL) {
        fprintf(stderr, "Couldn't get a response for the user: %s", user_id);
        return -1;
    }

    Vec* pretty = prettify_json(response);
    if (pretty == NULL) {
        fprintf(stderr, "Couldn't prettify a json for the user: %s", user_id);
        return -1;
    }

    snprintf
    (
        path,
        sizeof(path),
        "%s%s_%s",
        dir_path,
        user_id,
        DEFAULT_FILE_PATH_USER
    );
    
    if (write_str_to_file(path, pretty->data) == 0) {
        printf("Successfully saved the user info as %s\n", path);
    }

    vec_free(pretty);
    memset(path, 0, sizeof(path));


    //get_user_followers
    response = get_user_followers(curl, &args);

    if (response == NULL) {
        fprintf(stderr, "Couldn't get a response for the user's followers: %s", user_id);
        return -1;
    }

    pretty = prettify_json(response);
    if (pretty == NULL) {
        fprintf(stderr, "Couldn't prettify a json for the user: %s", user_id);
        return -1;
    }

    snprintf
    (
        path,
        sizeof(path),
        "%s%s_%s",
        dir_path,
        user_id,
        DEFAULT_FILE_PATH_FOLLOWERS
    );

    if (write_str_to_file(path, pretty->data) == 0) {
        printf("Successfully saved %s user followers as %s\n", COUNT, path);
    }

    vec_free(pretty);
    memset(path, 0, sizeof(path));


    //get_user_subscriptions
    args.fields = GROUP_FIELDS;
    response = get_user_subscriptions(curl, &args);

    if (response == NULL) {
        fprintf(stderr, "Couldn't get a response for the user subscriptions: %s", user_id);
        return 1;
    }

    pretty = prettify_json(response);
    if (pretty == NULL) {
        fprintf(stderr, "Couldn't prettify a json for the user: %s", user_id);
        return -1;
    }

    snprintf
    (
        path,
        sizeof(path),
        "%s%s_%s",
        dir_path,
        user_id,
        DEFAULT_FILE_PATH_SUBSCRIPTIONS
    );

    if (write_str_to_file(path, pretty->data) == 0) {
        printf("Successfully saved %s user subscriptions as %s\n", COUNT, path);
    }

    vec_free(pretty);
    memset(path, 0, sizeof(path));

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}
