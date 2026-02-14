#ifndef ARDUINO
#ifndef SPECIAL_HTTP_CLIENT_H
#define SPECIAL_HTTP_CLIENT_H

#include <StandardDefines.h>
#include "ISpecialHttpClient.h"

#include <curl/curl.h>
#include <sstream>
#include <map>
#include <iostream>

/**
 * Implementation of ISpecialHttpClient using libcurl
 * 
 * Response format (JSON string):
 * {
 *   "statusCode": 200,
 *   "headers": {
 *     "Content-Type": "application/json",
 *     ...
 *   },
 *   "body": "..."
 * }
 */
/* @Component */
class SpecialHttpClient : public ISpecialHttpClient {
    Private
        // Initialize curl globally (called once)
        static bool InitializeCurl() {
            static bool initialized = false;
            if (!initialized) {
                curl_global_init(CURL_GLOBAL_DEFAULT);
                initialized = true;
            }
            return initialized;
        }
        // Callback function to write response data
        static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
            size_t totalSize = size * nmemb;
            StdString* str = static_cast<StdString*>(userp);
            str->append(static_cast<char*>(contents), totalSize);
            return totalSize;
        }

        // Callback function to write response headers
        static size_t HeaderCallback(char* buffer, size_t size, size_t nitems, void* userp) {
            size_t totalSize = size * nitems;
            StdMap<StdString, StdString>* headers = static_cast<StdMap<StdString, StdString>*>(userp);
            
            StdString headerLine(buffer, totalSize);
            // Remove trailing \r\n
            while (!headerLine.empty() && (headerLine.back() == '\r' || headerLine.back() == '\n')) {
                headerLine.pop_back();
            }
            
            // Parse header line: "Key: Value"
            size_t colonPos = headerLine.find(':');
            if (colonPos != StdString::npos && colonPos > 0) {
                StdString key = headerLine.substr(0, colonPos);
                StdString value = headerLine.substr(colonPos + 1);
                
                // Trim whitespace
                while (!key.empty() && (key.front() == ' ' || key.back() == ' ')) {
                    if (key.front() == ' ') key.erase(0, 1);
                    if (key.back() == ' ') key.pop_back();
                }
                while (!value.empty() && (value.front() == ' ' || value.back() == ' ')) {
                    if (value.front() == ' ') value.erase(0, 1);
                    if (value.back() == ' ') value.pop_back();
                }
                
                if (!key.empty()) {
                    (*headers)[key] = value;
                }
            }
            
            return totalSize;
        }

        // Helper method to perform HTTP request
        StdString PerformRequest(
            const StdString& method,
            const StdString& url,
            const optional<StdString>& jsonBody,
            const optional<StdMap<StdString, StdString>>& customHeaders
        ) {
            #ifndef ARDUINO
            std::cout << "[HTTP_CLIENT] Starting " << method << " request to: " << url << std::endl;
            #endif
            
            // Initialize curl globally
            InitializeCurl();
            
            #ifndef ARDUINO
            std::cout << "[HTTP_CLIENT] Curl initialized" << std::endl;
            #endif
            
            CURL* curl = curl_easy_init();
            if (!curl) {
                #ifndef ARDUINO
                std::cout << "[HTTP_CLIENT] ERROR: Failed to initialize curl" << std::endl;
                #endif
                return CreateErrorResponse(500, "Failed to initialize curl");
            }

            #ifndef ARDUINO
            std::cout << "[HTTP_CLIENT] Curl handle created" << std::endl;
            #endif

            StdString responseBody;
            StdMap<StdString, StdString> responseHeaders;
            long statusCode = 0;

            // Set URL
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            #ifndef ARDUINO
            std::cout << "[HTTP_CLIENT] URL set: " << url << std::endl;
            #endif

            // Set timeout (10 seconds for connection, 30 seconds for total request)
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
            #ifndef ARDUINO
            std::cout << "[HTTP_CLIENT] Timeouts set: connect=10s, total=30s" << std::endl;
            #endif

            // Set write callback for response body
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);

            // Set header callback
            curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &responseHeaders);

            // Set HTTP method
            if (method == "POST") {
                curl_easy_setopt(curl, CURLOPT_POST, 1L);
            } else if (method == "PUT") {
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
            } else if (method == "DELETE") {
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
            } else if (method == "PATCH") {
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
            }

            // Set request body if provided
            if (jsonBody.has_value() && !jsonBody.value().empty()) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBody.value().c_str());
            }

            // Build header list
            struct curl_slist* headerList = nullptr;
            
            // Add Content-Type header for requests with body
            if (jsonBody.has_value() && !jsonBody.value().empty()) {
                headerList = curl_slist_append(headerList, "Content-Type: application/json");
            }
            
            // Add custom headers
            if (customHeaders.has_value()) {
                for (const auto& pair : customHeaders.value()) {
                    StdString header = pair.first + ": " + pair.second;
                    headerList = curl_slist_append(headerList, header.c_str());
                }
            }
            
            if (headerList) {
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
            }

            // Perform request
            #ifndef ARDUINO
            std::cout << "[HTTP_CLIENT] About to perform curl_easy_perform()..." << std::endl;
            #endif
            CURLcode res = curl_easy_perform(curl);
            #ifndef ARDUINO
            std::cout << "[HTTP_CLIENT] curl_easy_perform() returned with code: " << res << std::endl;
            #endif

            // Get status code
            if (res == CURLE_OK) {
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
                #ifndef ARDUINO
                std::cout << "[HTTP_CLIENT] Request successful, status code: " << statusCode << std::endl;
                std::cout << "[HTTP_CLIENT] Response body length: " << responseBody.length() << std::endl;
                #endif
            } else {
                #ifndef ARDUINO
                std::cout << "[HTTP_CLIENT] ERROR: Curl error code: " << res << ", message: " << curl_easy_strerror(res) << std::endl;
                #endif
                curl_easy_cleanup(curl);
                if (headerList) {
                    curl_slist_free_all(headerList);
                }
                return CreateErrorResponse(0, "Curl error: " + StdString(curl_easy_strerror(res)));
            }

            // Cleanup
            if (headerList) {
                curl_slist_free_all(headerList);
            }
            curl_easy_cleanup(curl);

            // Build JSON response
            return CreateResponse(statusCode, responseHeaders, responseBody);
        }

        // Helper to create JSON response
        StdString CreateResponse(
            long statusCode,
            const StdMap<StdString, StdString>& headers,
            const StdString& body
        ) {
            // Use ArduinoJson to build response JSON
            JsonDocument doc;
            doc["statusCode"] = static_cast<int>(statusCode);
            
            JsonObject headersObj = doc["headers"].to<JsonObject>();
            for (const auto& pair : headers) {
                headersObj[pair.first.c_str()] = pair.second.c_str();
            }
            
            doc["body"] = body.c_str();
            
            StdString result;
            serializeJson(doc, result);
            return result;
        }

        // Helper to create error response
        StdString CreateErrorResponse(long statusCode, const StdString& errorMessage) {
            JsonDocument doc;
            doc["statusCode"] = static_cast<int>(statusCode);
            doc["headers"] = JsonObject();
            doc["body"] = errorMessage.c_str();
            
            StdString result;
            serializeJson(doc, result);
            return result;
        }

    Public
        SpecialHttpClient() = default;
        Virtual ~SpecialHttpClient() override = default;

        Public Virtual StdString Get(
            CStdString& url,
            const optional<StdMap<StdString, StdString>>& headers = std::nullopt
        ) override {
            return PerformRequest("GET", StdString(url), std::nullopt, headers);
        }

        Public Virtual StdString Post(
            CStdString& url,
            CStdString& jsonBody,
            const optional<StdMap<StdString, StdString>>& headers = std::nullopt
        ) override {
            return PerformRequest("POST", StdString(url), optional<StdString>(StdString(jsonBody)), headers);
        }

        Public Virtual StdString Put(
            CStdString& url,
            CStdString& jsonBody,
            const optional<StdMap<StdString, StdString>>& headers = std::nullopt
        ) override {
            return PerformRequest("PUT", StdString(url), optional<StdString>(StdString(jsonBody)), headers);
        }

        Public Virtual StdString Delete(
            CStdString& url,
            const optional<StdMap<StdString, StdString>>& headers = std::nullopt
        ) override {
            return PerformRequest("DELETE", StdString(url), std::nullopt, headers);
        }

        Public Virtual StdString Patch(
            CStdString& url,
            CStdString& jsonBody,
            const optional<StdMap<StdString, StdString>>& headers = std::nullopt
        ) override {
            return PerformRequest("PATCH", StdString(url), optional<StdString>(StdString(jsonBody)), headers);
        }
};

#endif // SPECIAL_HTTP_CLIENT_H
#endif // ARDUINO