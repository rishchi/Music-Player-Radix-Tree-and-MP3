// Rishabh Chittaranjan Andrew Wang Jeffrey Lin Drake Lin
// Period 1
// The Radix Tree Class makes the Radix Tree data structure into an object. It stores Node structs
// through the use of strings. For example, if the tree stores th words Batter and Battle, the tree
// will be stored like this:
//								Batt
//							  /      \
//							 le       er
// So, basically it splits strings apart and stores the common part in a root, then it creates
// two branches and stores the suffixes. We used the Radix Tree Class to store the names of songs
// and artists. 



#include <iostream>
#include <bits/stdc++.h> 
#include <vector>

using namespace std;

// Node struct which contains all the data of the mp3: is able to tell whether a string stored isArtist or isWord
//string letters contains the string- can be substring of word trying to add
// vector myPointers: depending on what strings have extra prefixes to word being stored, vector pointers
// 					  point to the next Nodes to build full string
//bool isWord: tells if a certain Node is a Word so the output method can rint the full word. 
//			  EX. Batt-er and Batt-le. 'Batt' is not a word, but 'er' and 'le' are words
//                   So concatenate a string until er/le and print. 
//bool isArtist: tells if the string is an Artist name
				  
struct Node
	{
		string letters;
		vector<Node*> myPointers;
		bool isWord;
		bool isArtist;
	
	};


class RadixTree3
{
	public:
	
	
	Node* parentNode;// the rootNode of the tree
	
	
	// constructor to make the RadixTree3; just makes a new node
	RadixTree3()
	{
		parentNode= new Node;
		
	}
	
	// Node object
	//letters- string component of Node
	//vector<Node*>- holds all the pointers to new Nodes



/**
 The lowerCase method makes all of the string lower case
 @param: getLower= string which was previously uppercase
 @return: a lowercase string
*/

string lowerCase(string getLower)
{
	string adding="";
	for(int i=0;i<getLower.length();i++)
	{
		if(isupper(getLower[i]))// boolean to check if the char is uppercase
			adding+=tolower(getLower[i]);//add lowercase character
		else
			adding+=getLower[i];// add other characters
			
	}
	return adding;// string w/ lowercase
}

/**
	The addNode method adds all the strings which are made to be lowerCase
	@param: string newString- string you are adding
	@param: Node* pointerVar- the Node in which you are going to be adding. Start from root and go to new one
	@param: bool is_true- is the string you adding a word?
	@param: bool is_artist- is the string you are adding a name of an artist?
	@reutrn: None
**/
void addNode(string newString, Node* pointerVar, bool is_true, bool is_art)
{
	string myLower=lowerCase(newString);// string to be added, just lowercase version
	Node* tempNode;// the Node which holds the myLower variable
	int situation=0;// defines situations(which loop to enter)
	int ind_mem=0;// saves the index where you are going to add under
	string compareString;// saves the value of the Node you are iterating through
	
	
	
	for(int i=0;i<pointerVar->myPointers.size();i++)
	{
		compareString= pointerVar->myPointers[i]->letters;// storing value 
		
		if(compareString[0]==myLower[0])
		{
			ind_mem=i;// save index value
			if(compareString==myLower)
			{
				return;// don't really do anything, no situation if same string
			}
			else if((compareString.length()!=myLower.length())||(compareString.length()==myLower.length())) // any string can be added under
																											// no more 3 cases
			{
				situation=1;// where u have to put 2 substrings- all cases if you have same beginning letter
				break;// have to exit: already found situation. else it would still continue. 
				
			}
		}
			
	}
	if(situation==0)// basic situation; just adding a new node
	{
		tempNode= new Node;// starting new Node
		
		//adding the proper characteristics
		tempNode->isWord=is_true;
		tempNode->isArtist=is_art;
		tempNode->letters=myLower;// have new node's string equal to myLower
		pointerVar->myPointers.push_back(tempNode);//add the new pointer under the root/branch

		return;// exit
	}
	if(situation==1)
	{
		tempNode=pointerVar->myPointers[ind_mem];// starting from a common place EX. s[0]==p[0];same start letter
		int index=0;
		while(myLower[index]==compareString[index])// find index where the letters are not the same
		{
			index++;
		}
		string mySubstring= myLower.substr(index);// getting the substring which starts when strings are not the same
		string otherSubstring=compareString.substr(index);//EX. Battle & Batter-> Batt same
		
		// changing the root/branch node to accommodate both strings. EX. tempNode now contains Batt and not Batter
		string newComponent=myLower.substr(0,index);
		tempNode->letters=newComponent;
		
		if(otherSubstring.length()==0)// don't do anything- don't want to add an empty space
		{
			
		}
		else
		{
			// save previous bools- EX. Batt is now root: have to change bools to false- not word/not artist
			bool prevbool= tempNode->isWord;
			bool prevArt=tempNode->isArtist;
			
			// changing to false
			tempNode->isWord=false;
			tempNode->isArtist=false;
			
			addNode(otherSubstring,tempNode,prevbool,prevArt);// adding under the same root with proper bool values
		}
		
		addNode(mySubstring,tempNode,is_true,is_art);// adding the new Substring/ component you want to add
		
		return;
	}
	


}

/**
	Method deleteNode deletes a node based on the name of the string
	@param: string deleting- string you want to delete
	@param: Node* pointerVar- Node you are starting to iterate from/ location of the string
	@return: full string of the node you deleted
**/
string deleteNode(string deleting, Node* pointerVar)
{
	
	string lowercase_delete= lowerCase(deleting);// first making a lowercase deleting string since everything in tree
												 // is lowerCase
	int situation=-1;// determines which if statement to go to
	int index=0;
	string node_letters;//holds string in the Node you are iterating through
	Node* tempNode;// place holder for Node you are in
	for(int i=0;i<pointerVar->myPointers.size();i++)
	{
		
		node_letters= pointerVar->myPointers[i]->letters;// storing value
		if(lowercase_delete.substr(0,node_letters.length())==node_letters)
		{
			
			index=i;// if the delete_string's substring matches node_letters, save the index of the Node
			
			 if(lowercase_delete.compare(node_letters)==0)// if exactly the same
			{
				situation=0;
				break;
			}
			else if((pointerVar->myPointers.size()>0)&&lowercase_delete==node_letters)// if there are strings under
			// the same Node which contains the string you want to delete
			{
				situation=2;
				break;
			}
			 
			else if(lowercase_delete!=node_letters)// if it is not completely the same
			{
				situation=1;
				break;
			}
		}
	}
	
	tempNode=pointerVar->myPointers[index];// storing all the values in Node
	node_letters=tempNode->letters;// need this line. pls double check
	
	if(situation==-1)// can't find part of string
	{
		cout<<"String is not within the tree"<<endl;
		return "";
	}
	
	if(situation==0)
	{
		node_letters=tempNode->letters;// saving the string before deleting-> probably don't need this line
		
		// cleaning up the Node before deleting
		tempNode->letters="";
		pointerVar->myPointers.erase(pointerVar->myPointers.begin()+index);
		delete tempNode;// deleting it 
		
		return node_letters;// return string	
	}
	if(situation==1)
	{
		string substring_delete = lowercase_delete.substr(0,node_letters.length());// getting the beginning part of string
		return substring_delete+deleteNode(lowercase_delete.substr(node_letters.length()),tempNode);
		// return beginning part plus deeper part of string
	}
	if(situation==2)
	{
		return "";// don't delete anything: can delete everything under Node if you do
	}
}


/**
	The method PrintRadixTree2 is the main output method. Gives the index which it is stored in also
	@param vector<Node*> myMiniVector is the vector which contains all the strings in one branch
	@param string base is the string stored in the root node
	@return nothing just output

**/
void PrintRadixTree2(vector<Node*> myMiniVector,string base)
{
	
	for(int i=0;i<myMiniVector.size();i++)
	{
		if(myMiniVector[i]->isWord==true)// print if word is true. don't want to print a prefix
		{
			cout<<i<<" : "<<base+myMiniVector[i]->letters<<endl;// printing word
		}
		PrintRadixTree2(myMiniVector[i]->myPointers,base+myMiniVector[i]->letters);// go further down the rabbit hole
	}
	return;// return out
}

/**
	The method PrintRadixTree is the helper method. Makes calling PrintRadixTree2 easier
	@param: none
	@return: none
**/
void PrintRadixTree()
{
	Node* tempNode= parentNode;// starting point of output
	PrintRadixTree2(tempNode->myPointers,tempNode->letters);// print from the 1st level
}
private:// no private vars

};



