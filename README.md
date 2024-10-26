# VKAPIC 

`VK API` requests for a user in `C` using `libcurl`. 

## Prerequisites

- `libcurl` should installed on your system. It is required for making HTTP requests.
  - **Windows**: You can download the precompiled binaries from the [cURL website](https://curl.se/windows/). Either add it to your `PATH` environment variable or use `-L` flag.
  - **Linux**: You can install `libcurl` using your package manager. 

- `ACCESS_TOKEN` environment variable should be set to your access token which you can get from [vk dev website](https://dev.vk.com/).
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

```bash
./main [--user-id <id>] [--dir-path <path>]
```

Make sure that the `ACCESS_TOKEN` environment variable is set before running the program.
