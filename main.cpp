#include <stdio.h>
#include <string>
#include <iostream>
#include <curl/curl.h>

std::string url = "http://www.google.com.vn/search?q=";

std::string encode_query(std::string &q);

int main(int argc, char* argv[])
{
  std::string query;

  if(argc == 2)
  {
    // https://curl.haxx.se/libcurl/c/simple.html
    query = argv[1];
  }
  else if(argc == 1)
  {
    while(query.empty())
    {
      std::cout << "Type search query: " << std::flush;
      getline(std::cin, query);
    }
  }

  std::cout << "Please wait..." << std::flush;

  url += encode_query(query);

  std::cout << url << std::endl;

  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    res = curl_easy_perform(curl);

    std::cout << std::endl;
    if(res != CURLE_OK)
    {
      std::cerr << "curl_easy_perform() error: " << std::endl 
        << curl_easy_strerror(res) << std::endl;
    }

    curl_easy_cleanup(curl);
  }

  return 0;
}

std::string encode_query(std::string &s)
{
  std::size_t pos = 0;
  // Replace all space
  std::string r(" ");
  while(pos < s.size())
  {
    pos = s.find(r, pos);
    if(pos != std::string::npos)
    {
      std::cout << pos << std::endl;
      s.replace(pos, r.length(), "+");
    }
  }

  return s;
}
