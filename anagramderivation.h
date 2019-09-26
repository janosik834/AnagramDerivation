#ifndef ANAGRAMDERIVATION_H
#define ANAGRAMDERIVATION_H
#include<vector>
#include<stdint.h>
#include<string>
#include<QThread>
struct node;
typedef std::vector<std::vector<node>> nodeList ;
/*!
 * \brief The node struct which help to keep connection betwen word and bigger anagram
 */
struct node{
    /*!
     * \brief index_ - anagram index
     */
    uint index_;
    /*!
     * \brief numberOfPreviousNode_ - index number that represents the previously designated anagram from which the current word was created
     */
    uint numberOfPreviousNode_;
    node(uint index, uint numberOfPreviousNode){
        this->index_ = index;

        this->numberOfPreviousNode_ = numberOfPreviousNode;
    }
};
/*!
 * \brief The AnagramDerivation - the class provides the main function, which is to find larger anagrams
 */
class AnagramDerivation : public QObject
{
    Q_OBJECT
    /*!
     * \brief sorted_dictionary_ attribute - sorted (by word length) dictionary
     */
    std::vector<std::string> sortedDictionary_;
public:
    AnagramDerivation(QObject *parent = nullptr);
    virtual ~AnagramDerivation(){}
    bool readDictionary(std::string file_name);
    size_t dictionarySize() const;
    bool isDictionary() const;
    nodeList findTheLongestAnagrams(const std::string& word) const;

    /*!
      * \brief isInclude static function which check if 'biggerWord' is containing 'word'
      * \param word -basic - word
      * \param biggerWord - checked word
      * \return
      */
    static bool isInclude(std::string word, std::string biggerWord);
    /*!
      * \brief getIndex - function returning index of word in loaded dictionary
      * \param word
      * \return
      */
    std::size_t getIndex(const std::string &word) const;
    /*!
      * \brief displayResult - function rendering all founded anagrams path
      * \param lists - table of connection between word
      * \return
      */
    std::string displayResult(const nodeList &lists) const;
    /*!
      * \brief badIndex - method that returns a number representing the fact that the word is not in the 'this.sorted_dictionary_'
      * \return
      */
    std::size_t badIndex() const;
signals:
    void sResults(const QString& result);
public slots:
    void slFindAnagrams(const QString& word);
};


#endif // ANAGRAMDERIVATION_H
