//Jeffrey Lin
#include "dialog.h"
#include "ui_dialog.h"

#include <QFileDialog>

Dialog::Dialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::Dialog)
{
    // Setup components
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    
    // Initialize radix tree
    myTree.parentNode->letters="";
    
    // Initialize UI behavior
    ui->displaySuggestions->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->displaySuggestions->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->displaySuggestions->setSizeAdjustPolicy(QListWidget::AdjustToContents);
    ui->displaySuggestions->setResizeMode(QListView::Adjust);
    ui->displaySuggestions->hide();
    
    ui->delete_2->setIcon(QIcon("/Users/jeffles/Desktop/MyPlayer/minus.png"));
    
    connect(player, &QMediaPlayer::positionChanged, this, &Dialog::on_positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &Dialog::on_durationChanged);
}

Dialog::~Dialog()
{
    delete ui;
}

/**
	SortByArtist is a method which overloads the sort method from vector. Sorts by artist name alphabetically
	@param const mp3 &left: the left mp3 object which will be compared to right
	@param const mp3 &right: the right mp3 object which will be compared to left
	@return bool to tell if left.get_artist() is before the right artist
*/
bool SortByArtist(const mp3 &left, const mp3 &right)
{
    return left.get_artist()<right.get_artist();
}
/**
	SortByTitle is a method which overloads the sort method from vector. Sorts by song title alphabetically
	@param const mp3 &left: the left mp3 object which will be compared to right
	@param const mp3 &right: the right mp3 object which will be compared to left
	@return bool to tell if left.get_title() is before the right title
*/
bool SortByTitle(const mp3 &left, const mp3 &right)
{
    return left.get_title()<right.get_title();
}
/**
	The searching_deep method is a method which adds strings of words with similar beginnings to the search input 
	of user. EX. if B is typed in, Bat, Batter, and Battle will show up. 
	@param vector<Node*> pointer is the branches under the Node. It leads to the next possible set of strings
	@param string base: the root Node's string. Concatenate the base with the string in an element of pointer 
	to get the full string
	@param string key_pl: the text user enters into search bar

*/
void Dialog::searching_deep(vector<Node*>pointer,string base, string key_pl)
{
    for(int i=0; i<pointer.size(); i++)/ going to iterate through
    {
        string word= base+pointer[i]->letters;// starting the word
        if((word[0]==key_pl[0]))// if first letter matches
        {
            if((word.find(key_pl)==0)&&(pointer[i]->isWord==true))// if the search is in beginning of word
																  // and the Node is a Word
            {
                Data d;// create a new Data object
                for(int i=0; i<myPlayList.size(); i++)
                {
					// get the lowercase ver of words
                    string my_word_lower=RadixTree3::lowerCase(word);
                    string my_playlist=RadixTree3::lowerCase(myPlayList[i].get_title());
					
                    if(my_playlist==my_word_lower)// compare them and see if match
                    {
						// if true, add info to the Data obj
                        d.fromNode=myPlayList[i].get_title();
                        d.is_it_art=pointer[i]->isArtist;
                        possible_searches.push_back(d);// add in search entry
                    }
                }
            }
            searching_deep(pointer[i]->myPointers,word,key_pl);// continue searching/ returning
        }
    }
}
/**
	The search method is a helper method. Just calls the searching_deep method 
	@param vector<Node*> myP is the branches under the Node. It leads to the next possible set of strings
	@param string myS: the text user enters into search bar
*/
void Dialog::search (vector<Node *> myP, string mys)
{
    possible_searches.clear ();// if the search changes, clear possible_searches to update the list
    searching_deep (myP, "", mys);//call searching_deep to get searches
}

// Adjust player position when progress slider is moved
void Dialog::on_sliderProgress_sliderMoved(int position)
{
    player->setPosition(position);
}

// Adjust player volume when volume slider is moved
void Dialog::on_sliderVolume_sliderMoved(int position)
{
    player->setVolume(position);
}

// Play the QMediaPlayer on button press
void Dialog::on_pushButton_clicked()
{
    player->play();
}

// Stop the QMediaPlayer on button press
void Dialog::on_pushButton_2_clicked()
{
    player->stop();
}

// Update the progress slider when player position progresses
void Dialog::on_positionChanged(qint64 position)
{
    ui->sliderProgress->setValue(position);
}

// Update the progress slider maximum for each new song
void Dialog::on_durationChanged(qint64 position)
{
    ui->sliderProgress->setMaximum(position);
}

// Retrieve the filename of playlist item to play
void Dialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    player->setMedia(QUrl::fromLocalFile(item->data(78).toString()));
    player->play();
}

// Import MP3 File
void Dialog::on_importFile_clicked()
{
    // Open file manager dialog
    QString file = QFileDialog::getOpenFileName(this, tr("Select Music File"));
    
    // Check if a file is selected
    if(file != NULL)
    {
        // Create new MP3 object
        string utf8_text = file.toUtf8().constData();
        mp3 mp3File = mp3(utf8_text);
        
        // Insert MP3 object into myPlaylist vector
        myPlayList.emplace_back(mp3File);
        
        // Insert MP3 title and object
        songMap.insert(pair<string, mp3>(RadixTree3::lowerCase(mp3File.get_title()), mp3File));
    }
    
    updateList(ui->songButton->isChecked());
}

/*Updates the playlist using the global myPlaylist variable
 @param song - whether we are sorting by song
 */
void Dialog::updateList(bool song)
{
    // clear the list UI
    ui->listWidget->clear();
    
    // Sort the myPlayList vector
    if(song)
    {
        sort(myPlayList.begin(),myPlayList.end(),SortByTitle);//if sort by Title
    }
    
    else
    {
        sort(myPlayList.begin(),myPlayList.end(),SortByArtist);//if sort by artist
    }
    
    // Use a range-based for loop to generate the list
    for(auto&&x : myPlayList)
    {
        // Create a new list item
        QListWidgetItem *item = new QListWidgetItem();
        
        // Insert the file name and title in unused data fields
        item->setData(78, QString::fromStdString(x.get_filename()));
        item->setData(79, QString::fromStdString(x.get_title()));
        
        // Determine the order to list song objects
        if(song)
        {
            item->setText(QString::fromStdString(x.get_title() + ", " + x.get_artist()));
        }
        
        else
        {
            item->setText(QString::fromStdString(x.get_artist() + ", " + x.get_title()));
        }
        
        // Add the tiem onto the list
        ui->listWidget->addItem(item);
        
        // Add the title into the tree
        myTree.addNode(x.get_title(),myTree.parentNode,true,false);
    }
}

// Set to sort by song title
void Dialog::on_songButton_clicked()
{
    ui->artistButton->setChecked(false);
    updateList(ui->songButton->isChecked());
}

// Set to sort by artist name
void Dialog::on_artistButton_clicked()
{
    ui->songButton->setChecked(false);
    updateList(ui->songButton->isChecked());
}

/*
 Function to display autocomplete of song names
 */
void Dialog::on_lineEdit_textEdited(const QString &arg1)
{
    // If the text is empty, hind the suggestions list
    if(ui->lineEdit->text() == NULL)
    {
        ui->displaySuggestions->hide();
    }
    
    else
    {
        ui->displaySuggestions->show();
        
        // Search the tree using the text converted to lower case
        search (myTree.parentNode->myPointers, RadixTree3::lowerCase(arg1.toUtf8().constData()));
        
        // Clear previous suggestions
        ui->displaySuggestions->clear();
        
        // Adapt the suggestions list to fit the number of elements
        // in the possible_searches vector
        ui->displaySuggestions->setFixedHeight(18.5*possible_searches.size());
        
        // Range-based for loop to dynamically create and insert suggestions
        for(auto &&x : possible_searches)
        {
            QListWidgetItem *item = new QListWidgetItem();
            item->setText(QString::fromStdString(x.fromNode));
            ui->displaySuggestions->addItem(item);
        }
    }
}

// Playback the song if one of the suggestions are double clicked
void Dialog::on_displaySuggestions_itemDoubleClicked(QListWidgetItem *item)
{
    // Retrieve the song's name and convert it to lower case
    string currentSongName = RadixTree3::lowerCase(item->text().toUtf8().constData());
    
    // Use the map to find the corresponding mp3 object and it's file location
    QString fileLocation = QString::fromStdString(songMap.find(currentSongName)->second.get_filename());
    
    // Change the loaded song for the MediaPlayer
    player->setMedia(QUrl::fromLocalFile(fileLocation));
    player->play();
}

// Delete a song from the playlist
void Dialog::on_delete_2_clicked()
{
    // Retrieve the selected item on the playlist
    QListWidgetItem *item = ui->listWidget->currentItem();
    string itemString = item->data(79).toString().toUtf8().constData();
    
    // Create a new vector without the to-be deleted song
    vector<mp3> temp = myPlayList;
    myPlayList.clear();
    for(auto &&x: temp)
    {
        if(x.get_title().compare(itemString) != 0)
        {
            myPlayList.emplace_back(x);
        }
    }
    
    // Remove the song name from the tree
    myTree.deleteNode(itemString, myTree.parentNode);
    
    // Remove the song from the map
    songMap.erase(itemString);
    
    updateList(ui->songButton->isChecked());
}

// Empty functions to appease the compiler
void Dialog::on_displaySuggestions_indexesMoved(const QModelIndexList &indexes)
{
    
}

void Dialog::on_displaySuggestions_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    
}
