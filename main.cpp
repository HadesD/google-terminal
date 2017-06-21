#include <stdio.h>
#include <iostream>
// #include <curl/curl.h>

using namespace std;

int main(int argc, char* argv[])
{
  cout << "There are " << argc << " Arg(s)" << endl;
  for(int i = 0; i < argc; i++)
  {
    cout << argv[i] << endl;
  }
  
  return 0;
}
