#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

using namespace std;

void enterLetterPts(vector<int>& letterPts){
    ifstream ifs("Letter_point_value.txt");
    if (ifs.is_open())
    {
        string pointsString;
        string letterString;
        int points;
        char letter;
        int letterVal;
        
        while(getline(ifs, pointsString, ' ')){
            getline(ifs, letterString);
            
            points = atoi(pointsString.c_str());
            letter = letterString[0];
            
            letterVal = letter;
            letterPts[letterVal - 65] = points;
        }
        
        ifs.close();
    }
    else
        cout << "Could not open the file letter_point_value.txt" << endl;
}

int calcPointValue(const vector<int>& letterPts, string word ){
    int points = 0;
    int charVal;
    for(string::iterator i = word.begin(); i != word.end(); ++i){
        charVal = toupper(*i);
        points += letterPts[charVal - 65];
    }
    return points;
    
}

void enterWords(unordered_map<string, int>& theMap, const vector<int>& letterPts){
    ifstream ifs("ENABLE.txt");
    if (ifs.is_open())
    {
        string word;
        int pointVal;
        while(getline(ifs, word)){
            theMap.insert(pair<string, int>(word, calcPointValue(letterPts, word)));
            
        }
    }
    else
        cout << "Could not open file ENABLE.txt" << endl;
}

void print_valid_words( unordered_map<string, int> & word_list, string newstr, string oldstr)
{
    unordered_map<string, int>::const_iterator itr;
    for (int i = 0; i < oldstr.size(); ++i)
    {
        string nextnewstr =  newstr + oldstr[i];

        itr = word_list.find(nextnewstr);
        if (itr!=word_list.end())
            cout << nextnewstr << ": " << word_list[nextnewstr] << endl;

        print_valid_words(word_list, nextnewstr, oldstr.substr(0,i) + oldstr.substr(i+1));
    }
    
}
void print_valid_words(unordered_map<string,int> wordList, string letters)
{
    if (!letters.empty())
        print_valid_words(wordList, "", letters);
}

int main(int argc, char** argv) {
    
    vector<int> letterPts(26);
    enterLetterPts(letterPts);
    
    unordered_map<string, int> theMap;  
    enterWords(theMap, letterPts);
    
    print_valid_words(theMap, "father");
    
    return 0;
}

template <class ForwardIterator>
void advance(ForwardIterator& it, int n) {
    while (n-- && it++);
}

