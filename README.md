# VKAPIC 

`VK API` requests for a user in `C` using `libcurl`. 

## Methods
### [Users](https://dev.vk.com/method/users)

| Method                   | Function                                                       |
| ------------------------ | -------------------------------------------------------------- |
| `users.get`              | `Vec* get_users(CURL* curl, VKQueryParams* args)`              |
| `users.getFollowers`     | `Vec* get_user_followers(CURL* curl, VKQueryParams* args)`     |
| `users.getSubscriptions` | `Vec* get_user_subscriptions(CURL* curl, VKQueryParams* args)` |

```C
typedef struct 
{
    char* user_id;
    char* fields;
    char* count;
    char* lang;
    char* access_token;
    char* api_version;
} VKQueryParams;
```

## Prerequisites

- `libcurl` should be installed on your system. It is used for making HTTP requests.
  - **Windows**: You can download the precompiled binaries from the [cURL website](https://curl.se/windows/). Either add binaries to your `PATH` environment variable or use the `-L` flag.
  - **Linux**: You can install `libcurl` using your package manager. 

- `ACCESS_TOKEN` environment variable should be set to your access token which you can get from the [vk dev website](https://dev.vk.com/).
  - **Linux/Mac**:
    ```bash
    export ACCESS_TOKEN=your_access_token_here
    ```
  - **Windows**:
    ```cmd
    set ACCESS_TOKEN=your_access_token_here
    ```

## Compilation

```bash
gcc main.c -o main -lcurl
```

or

```bash
gcc main.c -o main -L/path/to/libcurl -lcurl
```

## Usage

```C
curl_global_init(CURL_GLOBAL_DEFAULT);
CURL* curl = curl_easy_init();

VKQueryParams args = {
    user_id,
    USER_FIELDS,
    COUNT,
    LANG,
    access_token,
    API_VERSION
};

Vec* response = get_users(curl, &args);

curl_easy_cleanup(curl);
curl_global_cleanup();
```

```bash
./main [--user-id <id>] [--dir-path <path>]
```

Make sure that the `ACCESS_TOKEN` environment variable is set before running the program.
