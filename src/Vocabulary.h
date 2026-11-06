// Vocabulary.h
// One vocabulary record. CA1 keeps records in memory only.

#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <string>

class Vocabulary;

// Friend of Vocabulary. Reads private fields and prints one record.
void displayVocabulary(const Vocabulary* record);

class Vocabulary {
private:
    int vocabularyId;
    std::string word;
    std::string meaning;
    std::string language;
    std::string exampleSentence;

    // Number of records currently stored. Shared by every Vocabulary object.
    static int totalRecords;

public:
    // Parameter names match the data members so the function body can use this->.
    void setDetails(int vocabularyId,
                    const std::string& word,
                    const std::string& meaning,
                    const std::string& language,
                    const std::string& exampleSentence);

    bool matchesId(int vocabularyId) const;

    int getVocabularyId() const;

    static int getTotalRecords();
    static void incrementTotalRecords();

    friend void displayVocabulary(const Vocabulary* record);
};

// Short accessor. Defined here so the compiler can inline it.
inline int Vocabulary::getVocabularyId() const {
    return vocabularyId;
}

#endif
