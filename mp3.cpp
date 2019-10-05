//MP3 Class, constructed using the mp3 file's filename
//Created by Andrew Wang on 1/4/19.
//Copyright © 2019 Andrew Wang. All rights reserved.

#include "mp3.hpp"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

//Tag which identifies the type of data, size of data, and extra bytes,
//packed to keep the tag a specific size (7 bytes)
typedef struct __attribute__ ((packed)) Tag
{
    char key[4]; //Indicates the what data follows (eg artist)
    int size; //Indicates length of the data
    char buffer[2]; //Offset to reach data
} Tag_t;

    //Constructer
mp3::mp3(string filename)
    {
        if (!analyze_file(filename))
        {
            title = "-----";
            artist = "-----";
        }
    }
    
    //Getter methods
string mp3::get_title() const
    {
        return title;
    }
string mp3::get_artist() const
    {
        return artist;
    }

    /*Takes in a filename and scans it for the tags that indicate title and artist
    Once it finds them, it assigns them to the object's instance variables
     @param filename = Name of the mp3 file being analyzed
     @return true if successfully analyzed, false otherwise
    */
bool mp3::analyze_file(string filename)
    {
        fstream strm(filename.c_str(), ios::in);
        if (strm.fail())
        {
            cout<<"Error: File not found"<<endl;
            return false;
        }
        int HEADER_SIZE = 10;
        int NUM_VARIABLES_REQUESTED = 2; //only title and artist
        
        Tag_t id_tag;
        char label[3];
        strm.read((char*)&label, sizeof(label));
        if (strncmp(label,"ID3",3) != 0)
        {
            cout<<"Incompatible File: "<<filename<<" does not have ID3v2"<<endl;
            return false;
        }
        strm.seekg(HEADER_SIZE);
        int counter = 0;
        while (!strm.fail() && counter<NUM_VARIABLES_REQUESTED)
        {
            strm.read((char*) &id_tag,sizeof(Tag));
            long x = strm.tellg();
            strm.seekg(x + 1);
            int conv_size = ntohl(id_tag.size); //Converts endian forms
            char text[conv_size];
            strm.read((char*)&text,sizeof(text)-1);
            text[conv_size-1] = '\0';
            if (strncmp(id_tag.key,"TIT2",4) == 0)
            {
                title = text;
                counter++;
            }
            else if (strncmp(id_tag.key,"TPE1",4) == 0)
            {
                artist = text;
                counter++;
            }
        }
        strm.close();
        return true;
    }

