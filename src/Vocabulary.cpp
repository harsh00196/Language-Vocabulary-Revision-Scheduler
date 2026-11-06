// Vocabulary.cpp
// Definitions for the Vocabulary entity. Storage stays in memory for CA1.

#include "Vocabulary.h"

#include <iostream>

// One shared counter for the whole class. It starts at zero records.
int Vocabulary::totalRecords = 0;

void Vocabulary::setDetails(int vocabularyId,
                            const std::string& word,
                            const std::string& meaning,
                            const std::string& language,
                            const std::string& exampleSentence) {
    // The parameter names match the members, so this-> selects the object.
    this->vocabularyId = vocabularyId;
    this->word = word;
    this->meaning = meaning;
    this->language = language;
    this->exampleSentence = exampleSentence;
}

bool Vocabulary::matchesId(int vocabularyId) const {
    // getVocabularyId() is the inline accessor. this-> selects this object.
    return this->getVocabularyId() == vocabularyId;
}

int Vocabulary::getTotalRecords() {
    return totalRecords;
}

void Vocabulary::incrementTotalRecords() {
    totalRecords = totalRecords + 1;
}

void displayVocabulary(const Vocabulary* record) {
    if (record == nullptr) {
        return;
    }

    // A friend may read private members through the pointer.
    std::cout << "Vocabulary ID    : " << record->vocabularyId << '\n';
    std::cout << "Word             : " << record->word << '\n';
    std::cout << "Meaning          : " << record->meaning << '\n';
    std::cout << "Language         : " << record->language << '\n';
    std::cout << "Example Sentence : " << record->exampleSentence << '\n';
}
