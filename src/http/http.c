#include "http.h"

#include "../oidc_error.h"
#include "../utils/pass.h"
#include "http_handler.h"
#include "http_postHandler.h"

#include <curl/curl.h>

#include <stdlib.h>
#include <syslog.h>

/** @fn char* httpsGET(const char* url, const char* cert_path)
 * @brief does a https GET request
 * @param url the request url
 * @param cert_path the path to the SSL certs
 * @return a pointer to the response. Has to be freed after usage. If the Https
 * call failed, NULL is returned.
 */
char* httpsGET(const char* url, struct curl_slist* headers,
               const char* cert_path) {
  syslog(LOG_AUTHPRIV | LOG_DEBUG, "Https GET to: %s", url);
  CURL* curl = init();
  setUrl(curl, url);
  struct string s;
  if (setWriteFunction(curl, &s) != OIDC_SUCCESS) {
    return NULL;
  }
  setSSLOpts(curl, cert_path);
  setHeaders(curl, headers);
  oidc_error_t err = perform(curl);
  if (err != OIDC_SUCCESS) {
    if (err >= 200 && err < 600 && strValid(s.ptr)) {
      pass;
    } else {
      clearFreeString(s.ptr);
      return NULL;
    }
  }
  cleanup(curl);
  syslog(LOG_AUTHPRIV | LOG_DEBUG, "Response: %s\n", s.ptr);
  return s.ptr;
}

/** @fn char* httpsPOST(const char* url, const char* data, const char*
 * cert_path)
 * @brief does a https POST request
 * @param url the request url
 * @param cert_path the path to the SSL certs
 * @param data the data to be posted
 * @return a pointer to the response. Has to be freed after usage. If the Https
 * call failed, NULL is returned.
 */
char* httpsPOST(const char* url, const char* data, struct curl_slist* headers,
                const char* cert_path, const char* username,
                const char* password) {
  syslog(LOG_AUTHPRIV | LOG_DEBUG, "Https POST to: %s", url);
  CURL* curl = init();
  setUrl(curl, url);
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  struct string s;
  if (setWriteFunction(curl, &s) != OIDC_SUCCESS) {
    return NULL;
  }
  setPostData(curl, data);
  setSSLOpts(curl, cert_path);
  setHeaders(curl, headers);
  if (username && password) {
    setBasicAuth(curl, username, password);
  }
  oidc_error_t err = perform(curl);
  if (err != OIDC_SUCCESS) {
    if (err >= 200 && err < 600 && strValid(s.ptr)) {
      pass;
    } else {
      clearFreeString(s.ptr);
      cleanup(curl);
      return NULL;
    }
  }
  cleanup(curl);
  syslog(LOG_AUTHPRIV | LOG_DEBUG, "Response: %s\n", s.ptr);
  return s.ptr;
}

char* sendPostDataWithBasicAuth(const char* endpoint, const char* data,
                                const char* cert_path, const char* username,
                                const char* password) {
  return httpsPOST(endpoint, data, NULL, cert_path, username, password);
}

char* sendPostDataWithoutBasicAuth(const char* endpoint, const char* data,
                                   const char* cert_path) {
  return httpsPOST(endpoint, data, NULL, cert_path, NULL, NULL);
}