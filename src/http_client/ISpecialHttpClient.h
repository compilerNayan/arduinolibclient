#ifndef ISPECIAL_HTTP_CLIENT_H
#define ISPECIAL_HTTP_CLIENT_H

#include <StandardDefines.h>
#include <optional>

/**
 * Interface for HTTP client operations
 * Provides methods for making HTTP requests (GET, POST, PUT, DELETE, PATCH)
 * Returns response as JSON string containing status code, headers, and body
 */
DefineStandardPointers(ISpecialHttpClient)
class ISpecialHttpClient {
    Public Virtual ~ISpecialHttpClient() = default;

    /**
     * Perform HTTP GET request
     * 
     * @param url The target URL
     * @param headers Optional map of custom headers (key-value pairs)
     * @return JSON string containing response with statusCode, headers, and body
     */
    Public Virtual StdString Get(
        CStdString& url,
        const optional<Map<StdString, StdString>>& headers = std::nullopt
    ) = 0;

    /**
     * Perform HTTP POST request
     * 
     * @param url The target URL
     * @param jsonBody JSON string to send as request body
     * @param headers Optional map of custom headers (key-value pairs)
     * @return JSON string containing response with statusCode, headers, and body
     */
    Public Virtual StdString Post(
        CStdString& url,
        CStdString& jsonBody,
        const optional<Map<StdString, StdString>>& headers = std::nullopt
    ) = 0;

    /**
     * Perform HTTP PUT request
     * 
     * @param url The target URL
     * @param jsonBody JSON string to send as request body
     * @param headers Optional map of custom headers (key-value pairs)
     * @return JSON string containing response with statusCode, headers, and body
     */
    Public Virtual StdString Put(
        CStdString& url,
        CStdString& jsonBody,
        const optional<Map<StdString, StdString>>& headers = std::nullopt
    ) = 0;

    /**
     * Perform HTTP DELETE request
     * 
     * @param url The target URL
     * @param headers Optional map of custom headers (key-value pairs)
     * @return JSON string containing response with statusCode, headers, and body
     */
    Public Virtual StdString Delete(
        CStdString& url,
        const optional<Map<StdString, StdString>>& headers = std::nullopt
    ) = 0;

    /**
     * Perform HTTP PATCH request
     * 
     * @param url The target URL
     * @param jsonBody JSON string to send as request body
     * @param headers Optional map of custom headers (key-value pairs)
     * @return JSON string containing response with statusCode, headers, and body
     */
    Public Virtual StdString Patch(
        CStdString& url,
        CStdString& jsonBody,
        const optional<Map<StdString, StdString>>& headers = std::nullopt
    ) = 0;
};

#endif // ISPECIAL_HTTP_CLIENT_H

