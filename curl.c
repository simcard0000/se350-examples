#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <memory.h>
#include <stdlib.h>

// When building, you must link with the external CURL library: for example, 'gcc curl.c -lcurl'

const char data[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

struct data {
    char *readptr;
    size_t size_left;
};

size_t read_callback(void *dest, size_t size, size_t nmemb, void *userp) {
    struct data *d = (struct data *) userp;
    size_t buffer_size = size * nmemb;
    if (d->size_left > 0) {
        // copy as much as possible from the source to the destination
        size_t copy_this_much = d->size_left;
        if (copy_this_much > buffer_size) {
            copy_this_much = buffer_size;
        }
        memcpy(dest, d->readptr, copy_this_much);
        d->readptr += copy_this_much;
        d->size_left -= copy_this_much;
        return copy_this_much;
    }
    return 0; // no more data left to deliver
}

int main(int argc, char** argv) {
    CURL *curl;
    CURLcode res;
    struct data *d = malloc(sizeof(struct data));

    d->readptr = data;
    d->size_left = strlen(data);

    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_global_init() function failed: %s\n.", curl_easy_strerror(res));
        return 1;
    }
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://example.com/index.cgi");
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl, CURLOPT_READDATA, d);
        res = curl_easy_perform(curl);
        // Check for errors from the result:
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() function failed: %s\n.", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
    free(d);
    curl_global_cleanup();
    return 0;
}
