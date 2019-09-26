#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <anagramderivation.h>
#include <iostream>
#include <stdio.h>
#include <QtWidgets>
Anagrams::Anagrams(QWidget *parent): QWidget (parent)

{
    creatGui();
    setLayout(gridGroupBox->layout());
    worker = new QThread();
    dictionary = new AnagramDerivation;
    dictionary->moveToThread(worker);
    worker->start();
    filedialog->setParent(this);
    connect(buttonLoad, &QPushButton::clicked,this, &Anagrams::onLoadButtonClicked );
    connect(buttonSearch,&QPushButton::clicked,this, &Anagrams::onSearchButtonClicked);
    connect(filedialog, &QFileDialog::rejected, this, &Anagrams::onCanceledButtonClicked);
    connect(buttonFind, &QPushButton::clicked, this, &Anagrams::onFindButtonClicked);
    connect(filedialog, &QFileDialog::fileSelected, this, &Anagrams::selectedFile);
    connect(this, &Anagrams::findAnagrams, dictionary, &AnagramDerivation::slFindAnagrams);
    connect(dictionary, &AnagramDerivation::sResults, this, & Anagrams::setResults);
}

Anagrams::~Anagrams()
{
    delete  dictionary;
    worker->quit();

    delete  gridGroupBox;
    delete  bigEditor;
    delete  labelDictionary;
    delete  labelWord;
    delete  labelSearching;
    delete  labelStatus;
    delete  lineEditDictionary;
    delete  lineEditWord;
    delete  buttonLoad;
    delete  buttonSearch;
    delete  buttonFind;
    delete  filedialog;
    delete  worker;
}

void Anagrams::onSearchButtonClicked()
{
    std::string word = lineEditWord->text().toStdString();
    if (dictionary->dictionarySize()==0)
        labelStatus->setText(tr("The dictionary does not exist !!"));
    else {
        labelStatus->setText(tr("Searching anagrams"));
        if(word.length() == 3) { // check word length
            uint index = static_cast<uint>(dictionary->getIndex(word));
            uint badIndex = static_cast<uint>(dictionary->badIndex());

            if(index != badIndex) { // check if the word exists in the dictionary
                labelSearching->setText(tr("Searching"));
                emit findAnagrams(tr(word.c_str()));


            } else {
                labelStatus->setText(tr("End searching anagrams"));
                bigEditor->setText(QStringLiteral("There is no word in the dictionary!!"));
            }
        } else {
            labelStatus->setText(tr("End searching anagrams"));
            bigEditor->setText(QStringLiteral("The word length is different than 3"));
        }
    }

}

void Anagrams::onLoadButtonClicked()
{
    this->labelStatus->setText(tr("Loading dictionary"));
    std::string path = this->lineEditDictionary->text().toStdString();

    if (dictionary->readDictionary(path)) {
        labelStatus->setText(tr("The dictionary is loaded."));
        bigEditor->setText(tr(""));
    }
    else {
        labelStatus->setText(tr("The dictionary does not exist !!"));
        bigEditor->setText(tr(""));

    }


}

void Anagrams::selectedFile(const QString& path)
{
    this->lineEditDictionary->setText(path);
    this->labelStatus->setText(tr("Selected dictionary"));
}

void Anagrams::onFindButtonClicked()
{
    filedialog->setVisible(true);
    labelStatus->setText(tr("Finding dictionary"));


}

void Anagrams::on_AcceptButton_clicked()
{
    lineEditDictionary->setText(filedialog->selectedFiles()[0]);
    labelStatus->setText(tr("Selected dictionary"));

}

void Anagrams::onCanceledButtonClicked()
{
    labelStatus->setText(tr("Status"));
}

void Anagrams::setResults(const QString &result)
{
    bigEditor->setText(result);
    labelStatus->setText(tr("End searching anagrams"));
    labelSearching->setText(tr(" "));

}

void Anagrams::creatGui()
{
    gridGroupBox = new QGroupBox();
    filedialog = new QFileDialog;
    filedialog->setFileMode(QFileDialog::AnyFile);
    QGridLayout *layout = new QGridLayout;
    labelDictionary = new QLabel(tr("Dictionary:"));
    labelWord = new QLabel(tr("Word: "));
    labelSearching = new QLabel(tr(" "));
    buttonLoad = new QPushButton(tr("Load"));
    buttonSearch = new QPushButton(tr("Search"));
    buttonFind = new QPushButton(tr("Find"));
    lineEditDictionary = new QLineEdit(tr("dictionary.txt"));
    lineEditWord = new QLineEdit(tr("abc"));
    bigEditor = new QTextEdit;
    labelStatus = new QLabel(tr("Status"));
    layout->addWidget(labelDictionary,1,1);
    layout->addWidget(lineEditDictionary,1,2);
    layout->addWidget(buttonLoad, 1,4);
    layout->addWidget(buttonFind,1,3);
    layout->addWidget(labelWord, 2,1);
    layout->addWidget(lineEditWord,2,2);
    layout->addWidget(labelSearching,2,3);
    layout->addWidget(buttonSearch,2,4);
    layout->addWidget(bigEditor,3,0,3,5);
    layout->addWidget(labelStatus,7,0,1,5);
    layout->setColumnStretch(2,200);
    layout->setColumnStretch(3,10);
    filedialog->setVisible(false);
    filedialog->update(this->geometry().x(),this->geometry().y(),150,300);
    layout->addWidget(filedialog,5,5);
    gridGroupBox->setLayout(layout);
}
