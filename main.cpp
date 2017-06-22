#include <stdio.h>
#include <iostream>
#include <string>
#include <regex>
#include <curl/curl.h>

// using namespace std;

std::string url = "http://www.google.com.vn/search?q=";
std::string query;
std::string data;

bool input();
std::string encode_query(std::string &q);
size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up);
bool curl(std::string url);

int main(int argc, char* argv[])
{
  if (argc == 2)
  {
    query = argv[1];
  }
  else
  {
    if (input() == false)
    {
      return 0;
    }
  }

  url += encode_query(query);
  std::cout << url << std::endl;

  curl(url); 
  // Regex
  const std::regex r("<a.*?href=\"(.*?)\".*?>(.*?)</a>");
  std::match_results<std::string::const_iterator> m;
  std::string::const_iterator searchStart = data.begin();
  int i = 0;
  std::string uri[30];
  while (std::regex_search(searchStart, data.cend(), m, r))
  {
    searchStart = m[0].second;
    if (std::string(m[1]).find("/url?q=") == std::string::npos)
    {
      continue;
    }
    i++;
    uri[i] = std::string(m[1]);
    std::cout << "-[" << i << "]- " << m[2] << std::endl;
    if (std::string(m[1]).find("webcache.googleusercontent.com") == std::string::npos)
    {
      std::cout << "\tDirect: " << m[1] << std::endl;
    }
    else
    {
      std::cout << "\tCache: " << m[1] << std::endl;
    }
  }

  if (i > 0)
  {
    std::string choose;
    std::cout << "Choose a number: ";
    while (
      !(std::cin >> choose) ||
      choose > std::to_string(i) ||
      choose <= std::to_string(0) ||
      choose == "\n"
      )
    {
      std::cout << "Number must be > 0 and < " << (i+1) << ": ";
    }
    curl("http://google.com.vn" + uri[std::stoi(choose)]);
    std::cout << data << std::endl;
  }

  return 0;
}

bool input()
{
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

bool curl(std::string url)
{
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
    //list = curl_slist_append(list, "User-Agent: Mozilla/5.001 (windows; U; NT4.0; en-US; rv:1.0) Gecko/25250101");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
    
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
      std::cerr << "curl_easy_perform() error: " << std::endl 
        << curl_easy_strerror(res) << std::endl;
      return false;
    }
    else
    {
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
      if (http_code != 200)
      {
        std::cerr << "HTTP Code error: " << http_code << std::endl;
        return false;
      }
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return true;
  }

  return false;
}

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{
  for (int c = 0; c<size*nmemb; c++)
  {
    data += buf[c];
  }

  return size*nmemb;
}
