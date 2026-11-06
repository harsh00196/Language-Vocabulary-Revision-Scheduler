// main.cpp
// CA1 menu. Records are stored in memory and are discarded on exit.

#include "Vocabulary.h"

#include <climits>
#include <iostream>
#include <string>

const int MAX_RECORDS = 20;

void showMenu() {
    std::cout << "\nLanguage Vocabulary Revision Scheduler\n";
    std::cout << "1. Add Vocabulary\n";
    std::cout << "2. Display All Vocabulary\n";
    std::cout << "3. Exit\n";
    std::cout << "Enter your choice: ";
}

bool isBlank(const std::string& text) {
    return text.find_first_not_of(" \t") == std::string::npos;
}

std::string trimSpaces(const std::string& text) {
    const std::size_t first = text.find_first_not_of(" \t");
    if (first == std::string::npos) {
        return "";
    }

    const std::size_t last = text.find_last_not_of(" \t");
    return text.substr(first, last - first + 1);
}

bool readText(const char* prompt, std::string& value) {
    std::cout << prompt;
    return static_cast<bool>(std::getline(std::cin, value));
}

// 1: vocabularyId holds a whole number greater than 0.
// 0: text is not a whole number.
// -1: text is a whole number, but it is not greater than 0.
int classifyVocabularyId(const std::string& text, int& vocabularyId) {
    if (text.empty()) {
        return 0;
    }

    long value = 0;
    for (std::size_t index = 0; index < text.size(); ++index) {
        const char digit = text[index];
        if (digit < '0' || digit > '9') {
            return 0;
        }

        if (value > INT_MAX / 10) {
            return 0;
        }
        value = value * 10 + (digit - '0');
        if (value > INT_MAX) {
            return 0;
        }
    }

    if (value <= 0) {
        return -1;
    }

    vocabularyId = static_cast<int>(value);
    return 1;
}

bool idAlreadyStored(const Vocabulary* records, int vocabularyId) {
    const Vocabulary* current = records;
    const int total = Vocabulary::getTotalRecords();

    for (int index = 0; index < total; ++index) {
        if (current->matchesId(vocabularyId)) {
            return true;
        }
        ++current;
    }

    return false;
}

// Read one record. Store it only after every check has passed.
void addVocabulary(Vocabulary* records) {
    if (records == nullptr) {
        std::cout << "Vocabulary list is not available.\n";
        return;
    }

    if (Vocabulary::getTotalRecords() >= MAX_RECORDS) {
        std::cout << "The vocabulary list is full. No more records can be added.\n";
        return;
    }

    std::string idText;
    std::string word;
    std::string meaning;
    std::string language;
    std::string exampleSentence;

    if (!readText("Enter Vocabulary ID: ", idText) ||
        !readText("Enter Word: ", word) ||
        !readText("Enter Meaning: ", meaning) ||
        !readText("Enter Language: ", language) ||
        !readText("Enter Example Sentence: ", exampleSentence)) {
        std::cin.clear();
        std::cout << "Input could not be read. Record was not added.\n";
        return;
    }

    int vocabularyId = 0;
    const int idStatus = classifyVocabularyId(idText, vocabularyId);
    if (idStatus == 0) {
        std::cout << "Vocabulary ID must be a whole number. Record was not added.\n";
        return;
    }
    if (idStatus == -1) {
        std::cout << "Vocabulary ID must be greater than 0. Record was not added.\n";
        return;
    }
    if (idAlreadyStored(records, vocabularyId)) {
        std::cout << "That Vocabulary ID already exists. Record was not added.\n";
        return;
    }
    if (isBlank(word)) {
        std::cout << "Word cannot be empty. Record was not added.\n";
        return;
    }
    if (isBlank(meaning)) {
        std::cout << "Meaning cannot be empty. Record was not added.\n";
        return;
    }
    if (isBlank(language)) {
        std::cout << "Language cannot be empty. Record was not added.\n";
        return;
    }
    if (isBlank(exampleSentence)) {
        std::cout << "Example sentence cannot be empty. Record was not added.\n";
        return;
    }

    Vocabulary* slot = records + Vocabulary::getTotalRecords();
    slot->setDetails(vocabularyId, word, meaning, language, exampleSentence);
    Vocabulary::incrementTotalRecords();

    std::cout << "Vocabulary added successfully.\n";
}

// Print every stored record. The static counter marks the end of the used slots.
void displayAllVocabulary(const Vocabulary* records) {
    if (records == nullptr) {
        std::cout << "Vocabulary list is not available.\n";
        return;
    }

    const int total = Vocabulary::getTotalRecords();
    if (total == 0) {
        std::cout << "No vocabulary records to display.\n";
        return;
    }

    std::cout << "\nStored vocabulary records: " << total << '\n';

    const Vocabulary* current = records;
    for (int index = 0; index < total; ++index) {
        std::cout << "\nRecord " << (index + 1) << '\n';
        displayVocabulary(current);
        ++current;
    }
}

int main() {
    Vocabulary records[MAX_RECORDS] = {};
    Vocabulary* recordList = records;

    std::cout << "Records stay in memory and are removed when the program ends.\n";

    while (true) {
        showMenu();

        std::string choiceText;
        if (!std::getline(std::cin, choiceText)) {
            if (std::cin.eof()) {
                std::cout << "\nGoodbye.\n";
                return 0;
            }
            std::cin.clear();
            std::cout << "Invalid choice. Enter 1, 2, or 3.\n";
            continue;
        }

        const std::string choice = trimSpaces(choiceText);
        if (choice == "1") {
            addVocabulary(recordList);
        } else if (choice == "2") {
            displayAllVocabulary(recordList);
        } else if (choice == "3") {
            std::cout << "Goodbye.\n";
            return 0;
        } else {
            std::cout << "Invalid choice. Enter 1, 2, or 3.\n";
        }
    }
}
