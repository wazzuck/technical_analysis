#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <regex>

#include "ToolKit.h"
#include "Logger.h"
#include <bits/stdc++.h>

using namespace std;

namespace EodToolKit
{
//http://stackoverflow.com/questions/236129/split-a-string-in-c
void ToolKit::split ( const string &s, char delim, vector<string> &elems )
{
  stringstream ss;
  ss.str ( s );
  string item;

  while ( getline ( ss, item, delim ) ) {
    elems.push_back ( item );
  }
}

// Function to remove spaces
string ToolKit::removeSpaces ( const string inString )
{
  string str = inString;
  stringstream ss;
  string temp;

  // Storing the whole string
  // into string stream
  ss << str;

  // Making the string empty
  str = "";

  // Running loop till end of stream
  while ( !ss.eof() ) {
    // Extracting word by word from stream
    ss >> temp;

    // Concatenating in the string to be
    // returned
    str = str + temp;

    temp = "";
  }

  return str;
}

string ToolKit::removeChar ( const string inWord, const char ch )
{
  string word = inWord;

  for ( int i = 0; i < word.length(); i++ ) {
    if ( word[i] == ch ) {
      word.erase ( word.begin()+i );
      i--;
    }
  }

  return word;
}

string ToolKit::removePunctuation ( const string inWord )
{
  string str = inWord;

  for ( int i = 0, len = str.size(); i < len; i++ ) {
    // check whether parsing character is punctuation or not
    if ( ispunct ( str[i] ) ) {
      str.erase ( i--, 1 );
      len = str.size();
    }
  }

  return str;
}


//https://www.geeksforgeeks.org/cpp-program-to-replace-a-character-in-a-string/
string ToolKit::replaceChar ( const string inString, const char searchChar, const char replaceChar )
{
  string outString = inString;
  int len = outString.length();

  // loop to traverse in the string
  for ( int i = 0; i < len; i++ ) {
    // check for searchString and replace
    if ( outString[i] == searchChar )
      outString[i] = replaceChar;

    //TODO I have no idea what this is for
    /*
    // check for replaceChar and replace
    else if (outString[i] == replaceChar)
      outString[i] = searchChar;
    */
  }

  return outString;
}

vector<string> ToolKit::split ( const string &s, char delim )
{

  //DLOG("String to split " << s);

  int startingPlace = 0;
  int endingPlace = 0;

  regex startreg ( "^\".*" );
  regex endreg ( ".*\"$" );

  vector<string> elems;
  split ( s, delim, elems );

  bool insideQuotes = 0;
  string stringToMerge = "";

  for ( long unsigned int iteratorCount = 0; iteratorCount < elems.size(); iteratorCount++ ) {
    string entry = elems[iteratorCount];
    //DLOG("Processing entry " << iteratorCount << " " << entry << " ");

    if ( insideQuotes == 0 ) {
      // flag type for determining the matching behavior (in this case on string objects)
      smatch m;

      // regex_search that searches pattern regexp in the string entry
      if ( regex_search ( entry, m, startreg ) ) {
        //DLOG("Found the start !!!");
        if ( regex_search ( entry, m, endreg ) ) {
          //DLOG("Also found the end!");
          continue;
        }
        else {
          //DLOG"Element at the START of a merge " << entry);
          stringToMerge += entry + ",";
          insideQuotes = 1;
        }

        startingPlace = iteratorCount;
        //DLOG"Starting place " << startingPlace);
      }
    }
    else if ( insideQuotes == 1 ) {
      // flag type for determining the matching behavior (in this case on string objects)
      smatch m;

      if ( regex_search ( entry, m, endreg ) ) {
        //DLOG("Found the end !!!");
        //DLOG("Element at the END of a merge " << entry);
        stringToMerge += entry;

        //DLOG("Completed string to merge " << stringToMerge);
        endingPlace = iteratorCount;
        //DLOG("ending place " << endingPlace);

        //DLOG("Started at " << startingPlace << " and ended at " << endingPlace);

        //Remove quotes
        stringToMerge.erase ( remove ( stringToMerge.begin(), stringToMerge.end(), '\"' ),stringToMerge.end() );

        elems[startingPlace] = stringToMerge;

        stringToMerge = "";

        //DLOG("removing " << startingPlace + 1 << " " << elems[startingPlace + 1] << " to " << endingPlace << " " << elems[endingPlace]);

        //DLOG("Size of vector before erase " << elems.size());

        elems.erase ( elems.begin() + startingPlace + 1, elems.begin() + endingPlace + 1 );

        //DLOG("Size of vector after erase " << elems.size());

        iteratorCount = startingPlace;

        startingPlace = 0;
        endingPlace = 0;

        insideQuotes = 0;
      }
      else {
        //DLOG("Element in MIDDLE of a merge " << entry);
        stringToMerge += entry + ",";
      }
    }
  }

  /*
  if (startingPlace != 0)
  {
      DLOG("starting place " << startingPlace << " " << "ending place " << endingPlace);
      elems.erase(elems.begin() + (startingPlace) + 1, elems.begin() + endingPlace + 1);

      startingPlace = 0;
      endingPlace = 0;
  }
  */
  //DLOG("Started at " << startingPlace << " and ended at " << endingPlace);
  return elems;
}

void ToolKit::displayVector ( const vector<string> &v )
{
  DLOG ( "################################################" );
  DLOG ( "# Print split vector" );
  DLOG ( "################################################" );

  for ( const auto &elem : v ) {
    DLOG ( << elem );
  }
}
}
