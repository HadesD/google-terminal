#include <stdio.h>
#include <iostream>
#include <string>
#include <regex>
#include <curl/curl.h>

using namespace std;

std::string url = "http://www.google.com.vn/search?hl=vi&q=";
std::string query;
std::string data;

bool input(int argc, char *argv[]);
std::string encode_query(std::string &q);
size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up);

int main(int argc, char* argv[])
{
  if (input(argc, argv) == false)
  {
    return 0;
  }

  url += encode_query(query);
  std::cout << url << std::endl;

  std::cout << "Please wait..." << std::endl;

  // https://curl.haxx.se/libcurl/c/simple.html
  CURL *curl;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if (curl)
  {
    CURLcode res;
    long http_code = 0;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    // Header
    struct curl_slist *list = NULL;
    list = curl_slist_append(list, "User-Agent: Mosilla/5.0 (Macintosh; Intel Mac OS X 10_7_4) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.57 Safari/536.11");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
    
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
      std::cerr << "curl_easy_perform() error: " << std::endl 
        << curl_easy_strerror(res) << std::endl;
    }
    else
    {
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
      if (http_code != 200)
      {
        std::cerr << "HTTP Code error: " << http_code << std::endl;
      }
    }

    // Regex
    const regex r("<a href=(.*)>(.*)</a>");
    smatch m;
    if (regex_search(data, m, r))
    {
      for (int i=1; i<m.size(); i++)
      {
        cout << m[i] << endl;
      }
    }
    // std::cout << data << std::endl;

    curl_easy_cleanup(curl);
    curl_global_cleanup();
  }

  return 0;
}

bool input(int argc, char *argv[])
{
  if (argc == 2)
  {
    query = argv[1];
    return true;
  }
  while (query.empty())
  {
    std::cout << "Type search query: ";
    getline(std::cin, query);
  }
  if (query == "q")
  {
    std::cout << "Exit..." << std::endl;
    return false;
  }
  return true;
}

std::string encode_query(std::string &s)
{
  std::size_t pos = 0;
  // Replace all space
  std::string r(" ");
  while (pos < s.size())
  {
    pos = s.find(r, pos);
    if (pos != std::string::npos)
    {
      s.replace(pos, r.length(), "+");
    }
  }

  return s;
}

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{
  for (int c = 0; c<size*nmemb; c++)
  {
    data += buf[c];
  }

  return size*nmemb;
}
