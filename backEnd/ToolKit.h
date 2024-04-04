#pragma once

#include <vector>
#include <string>

using namespace std;

namespace EodToolKit
{
class ToolKit
{
public:
  vector<string> split ( const string &s, char delim );
  void displayVector ( const vector<string> &vec );
  string removeSpaces ( const string inString );
  string removePunctuation ( const string inString );
  string removeChar ( const string inString, const char inChar );
  string replaceChar ( const string inString, const char searchChar, const char replaceChar );
private:
  void split ( const string &s, char delim, vector<string> &elems );
};
}
