#include "anagramderivation.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <QSet>
auto compareString = [](std::string a, std::string b){
    return a.length() < b.length();
};




nodeList AnagramDerivation::findTheLongestAnagrams(const std::string &word) const {
    nodeList list;
    std::vector<node> temp;
    temp.clear();
    uint currentLenght{static_cast<uint>(word.length())};
    uint firstIndexLenght{0};
    bool allElements{false};
    uint iLenght{0}, iWord, index;
    std::string firstWord, biggerWord;
    list.push_back(temp);
    iLenght = static_cast<uint>(this->getIndex(word));

    while (this->sortedDictionary_[iLenght].length() == currentLenght)
    {
        biggerWord = this->sortedDictionary_[iLenght];
        if(word == biggerWord)
        {
            list[0].push_back(node(iLenght, iLenght));
        }
        iLenght++;
        if(firstIndexLenght+iLenght == this->sortedDictionary_.size())
        {
            iLenght--;
            allElements= true;
            break;
        }
    }
    if (list[0].size() != 1)

        std::cerr<<"In dictionary is few this same basis word !!"<<std::endl;
    currentLenght = static_cast<uint>(this->sortedDictionary_[firstIndexLenght+iLenght].length());
    firstIndexLenght +=iLenght;

    while (!allElements) //loop to search all items in the dictionary
    {
        list.push_back(temp);
        for (iWord = 0; iWord < list[list.size() - 2].size(); iWord++) // loop for all anagrams of previous length
        {
            iLenght=0;
            while (this->sortedDictionary_[firstIndexLenght + iLenght].length() == currentLenght) //A loop for each word with the current length
            {
                index = list[list.size() - 2][iWord].index_;
                firstWord = this->sortedDictionary_[index];
                biggerWord = this->sortedDictionary_[firstIndexLenght+iLenght];
                if(AnagramDerivation::isInclude(firstWord, biggerWord))
                {
                    list[list.size() - 1].push_back(node(firstIndexLenght + iLenght, iWord));
                }
                iLenght++;
                uint tempSize = static_cast<uint>(this->sortedDictionary_.size());
                if(firstIndexLenght + iLenght == tempSize) // check if this is the end of the dictionary
                {
                    iLenght--;
                    break;
                }
            }
        }
        currentLenght = static_cast<uint>(this->sortedDictionary_[firstIndexLenght + iLenght].length());
        firstIndexLenght += iLenght;
        if(list[list.size()-1].empty())// check if a larger lenght anagram was found in the previous search.
                                       //  If no such anagram is found, the algorithm terminates.
        {
            allElements= true;
            list.pop_back();
        }
        if (firstIndexLenght == this->sortedDictionary_.size() - 1)// if we check all the words we end the algorithm
        {
            allElements = true;
        }
    }
    return list;
}

bool AnagramDerivation::isInclude( std::string word,  std::string biggerWord)
{
    std::size_t found = biggerWord.find_first_of(word);
    while (found != std::string::npos)
    {
        word[word.find_first_of(biggerWord[found])] = '!';
        biggerWord[found]='*';
        found=biggerWord.find_first_of(word);
    }
    std::string all(word.length(),'!');

    if (word == all)
        return true;
    else
        return false;
}

std::size_t AnagramDerivation::getIndex(const std::string &word) const
{
    size_t index{this->sortedDictionary_.max_size()};
    for (std::size_t i{0};i<this->sortedDictionary_.size();i++)
        if (this->sortedDictionary_[i] == word)
        {
            index = i ;
            break;
        }
    return index;
}

std::string AnagramDerivation::displayResult(const nodeList &lists) const
{
    std::string result{""};
    uint numberOfList{static_cast<uint>(lists.size())};
    uint numberOfWord{static_cast<uint>(lists[lists.size() - 1].size())};

    for(uint i = 0; i< numberOfWord;i++){
        node upperNode = lists[numberOfList - 1][i];
        result = sortedDictionary_[upperNode.index_] + result;

        if (numberOfList > 1)
            for (uint j = 1; j < numberOfList; j++)
            {
                upperNode = lists[numberOfList - 1 - j][upperNode.numberOfPreviousNode_];
                result =sortedDictionary_[upperNode.index_] + "->" +  result;
            }
        result = "\n"+ result;
    }
    if (numberOfWord == 0)
        result += " ";
    return result.substr(1);
}

std::size_t AnagramDerivation::badIndex() const
{
    return this->sortedDictionary_.max_size();
}

void AnagramDerivation::slFindAnagrams(const QString &word)
{
    std::vector<std::vector<node>> lists = this->findTheLongestAnagrams(word.toStdString());
    std::string result = this->displayResult(lists);

    emit sResults(tr(result.c_str()));
}

AnagramDerivation::AnagramDerivation(QObject *parent): QObject (parent)
{

}

bool AnagramDerivation::readDictionary(std::string file_name)
{
    sortedDictionary_.clear();
    QSet<QString> dictionary;
    std::ifstream file(file_name);

    if (!file){
        std::perror("File opening failed");

        return false;
    }
    std::string rawDictionary((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::smatch match;
    std::regex reg ("[a-z,A-Z]+");
    while(std::regex_search(rawDictionary,match,reg)) {
        std::string word = match.str();
        std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c){return std::tolower(c);});
        dictionary.insert(tr(word.c_str()));
        rawDictionary = match.suffix();
    }
   for(const QString &word: dictionary){
   sortedDictionary_.push_back(word.toStdString());
   }

    sort(sortedDictionary_.begin(),sortedDictionary_.end(), compareString );

    return  true;
}
size_t AnagramDerivation::dictionarySize() const
{
    return this->sortedDictionary_.size();
}

bool AnagramDerivation::isDictionary() const
{
    if (sortedDictionary_.size() == 0)
        return false;
    else
        return true;

}
