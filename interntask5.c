#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

#define FILENAME "products.csv"

// Structure to hold webpage content
struct MemoryStruct {
    char *memory;
    size_t size;
};

// Callback function for libcurl to write data
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        // out of memory!
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// Function to fetch webpage content
char *fetchURL(const char *url) {
    CURL *curl_handle;
    CURLcode res;

    struct MemoryStruct chunk;

    chunk.memory = malloc(1);  // will be grown as needed by the realloc above
    chunk.size = 0;            // no data at this point

    curl_global_init(CURL_GLOBAL_ALL);

    curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl_handle);

    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        free(chunk.memory);
        chunk.memory = NULL;
    }

    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();

    return chunk.memory;
}

// Function to extract product information using XPath
void extractProducts(const char *html, const char *xpath_expr) {
    htmlDocPtr doc = htmlReadMemory(html, strlen(html), NULL, NULL, HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse HTML\n");
        return;
    }

    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if(xpathCtx == NULL) {
        fprintf(stderr, "Failed to create XPath context\n");
        xmlFreeDoc(doc);
        return;
    }

    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar *)xpath_expr, xpathCtx);
    if(xpathObj == NULL) {
        fprintf(stderr, "Failed to evaluate XPath expression\n");
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return;
    }

    FILE *fp = fopen(FILENAME, "w");
    if(fp == NULL) {
        perror("Failed to open file");
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return;
    }

    fprintf(fp, "Name,Price,Rating\n");

    xmlNodeSetPtr nodes = xpathObj->nodesetval;
    for(int i = 0; i < nodes->nodeNr; i++) {
        xmlNodePtr node = nodes->nodeTab[i];

        // Example: Assuming the structure of the product details and using relative XPath
        xmlChar *name = xmlXPathCastNodeToString(node);
        xmlChar *price = xmlXPathCastNodeToString(node);
        xmlChar *rating = xmlXPathCastNodeToString(node);

        fprintf(fp, "%s,%s,%s\n", name ? (char *)name : "", price ? (char *)price : "", rating ? (char *)rating : "");

        if(name) xmlFree(name);
        if(price) xmlFree(price);
        if(rating) xmlFree(rating);
    }

    fclose(fp);
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
}

int main() {
    const char *url = "https://example.com/products";  // Replace with the actual URL
    const char *xpath_expr = "//div[@class='product']";  // Replace with the actual XPath expression

    char *html = fetchURL(url);
    if(html) {
        extractProducts(html, xpath_expr);
        free(html);
    }

    return 0;
}
