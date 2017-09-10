#include <iostream>

using namespace std;
int main()
{
  cout << "saída do arquivo main1.cc" << endl;

  #ifdef __MINGW__
  cout << "usando o mingw\n";
  #endif

}
