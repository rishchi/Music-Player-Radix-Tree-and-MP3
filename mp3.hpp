//MP3 object initialized from the file's name.
//Only works with ID3 version 2
//Extracts title and artist information from the mp3 file's metadata

#ifndef mp3_hpp
#define mp3_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class mp3
{
public:
    //Constructer
    mp3(string filename);
    //Getter methods
    string get_title() const;
    string get_artist() const;
    
private:
    string title;
    string artist;
    string filename;
    
    /*Takes in a filename and scans it for the tags that indicate title and artist
     Once it finds them, it assigns them to the object's instance variables
     @param filename = Name of the mp3 file being analyzed
     @return whether the file was successfully analyzed
     */
    bool analyze_file(string filename);
};

#endif /* mp3_hpp */
