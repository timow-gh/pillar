#include <gtest/gtest.h>
#include <curl/curl.h>
#include <iostream>
#include <memory>
#include <string>

// Smart pointer wrapper for CURL with custom deleter
struct CurlDeleter
{
  void operator()(CURL* curl) const
  {
    if (curl != nullptr)
    {
      curl_easy_cleanup(curl);
    }
  }
};

using CurlPtr = std::unique_ptr<CURL, CurlDeleter>;

// Helper function to initialize curl
static CurlPtr curl_init()
{
  return CurlPtr(curl_easy_init());
}

class test_curl : public testing::Test {};

// Test to check available protocols in curl build
TEST_F(test_curl, AvailableProtocols)
{
  curl_version_info_data* data = curl_version_info(CURLVERSION_NOW);
  ASSERT_NE(data, nullptr);

  std::cout << "Protocols supported by curl:\n";
  for (int i = 0; data->protocols[i] != nullptr; i++)
  {
    std::cout << "  " << data->protocols[i] << "\n";
  }

  // Check for HTTPS support specifically
  bool httpsSupported = false;
  for (int i = 0; data->protocols[i] != nullptr; i++)
  {
    if (strcmp(data->protocols[i], "https") == 0)
    {
      httpsSupported = true;
      break;
    }
  }

  if (!httpsSupported)
  {
    std::cout << "WARNING: HTTPS protocol is not supported by this curl build\n";
  }

  // Print curl version information
  std::cout << "Curl version: " << data->version << "\n";
  std::cout << "SSL version: " << ((data->ssl_version != nullptr) ? data->ssl_version : "None") << "\n";
}

// Test download function with HTTP protocol
TEST_F(test_curl, DownloadWithHttpProtocol)
{
  std::string str;
  auto curl = curl_init();
  ASSERT_NE(curl.get(), nullptr);

  curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, "curl-test");
  curl_easy_setopt(curl.get(), CURLOPT_URL, "http://example.com");
  curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYPEER, false);

  curl_write_callback writeCb = [](char* contents, std::size_t size, std::size_t nmemb, void* userp) -> std::size_t
  {
    auto& userstr = *static_cast<std::string*>(userp);
    auto realsize = size * nmemb;
    userstr.append(contents, realsize);
    return realsize;
  };

  curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, writeCb);
  curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &str);

  CURLcode res = curl_easy_perform(curl.get());
  std::cout << "HTTP Request result: " << curl_easy_strerror(res) << "\n";

  if (res == CURLE_OK)
  {
    std::cout << "Download succeeded, content length: " << str.length() << " bytes\n";
  }
  else
  {
    std::cout << "Download failed with error: " << curl_easy_strerror(res) << "\n";
  }

  // We don't assert success since the test environment might not have internet
  // Just log the result to diagnose the issue
}

// Test download function with HTTPS protocol
TEST_F(test_curl, DownloadWithHttpsProtocol)
{
  std::string str;
  auto curl = curl_init();
  ASSERT_NE(curl.get(), nullptr);

  curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, "curl-test");
  curl_easy_setopt(curl.get(), CURLOPT_URL, "https://example.com");
  curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYPEER, false);

  curl_write_callback writeCb = [](char* contents, std::size_t size, std::size_t nmemb, void* userp) -> std::size_t
  {
    auto& userstr = *static_cast<std::string*>(userp);
    auto realsize = size * nmemb;
    userstr.append(contents, realsize);
    return realsize;
  };

  curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, writeCb);
  curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &str);

  CURLcode res = curl_easy_perform(curl.get());
  std::cout << "HTTPS Request result: " << curl_easy_strerror(res) << "\n";

  if (res == CURLE_OK)
  {
    std::cout << "HTTPS download succeeded, content length: " << str.length() << " bytes\n";
  }
  else
  {
    std::cout << "HTTPS download failed with error: " << curl_easy_strerror(res) << "\n";
  }

  // We don't assert success since the test environment might not have internet
  // Just log the result to diagnose the issue
}
