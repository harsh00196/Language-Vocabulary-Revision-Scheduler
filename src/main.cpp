// Language Vocabulary Revision Scheduler

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstring>
#include <filesystem>

using namespace std;

const int MAX_RECORDS = 20;
const string DATA_FILE = "data/vocabulary.dat";

// This structure stores one record in the binary file.
// Fixed-size character arrays keep every saved record the same size.
struct VocabularyFileRecord {
    int id;
    char word[100];
    char meaning[250];
    char language[60];
    char exampleSentence[250];
};

class Vocabulary {
private:
    int vocabularyId;
    string word;
    string meaning;
    string language;
    string exampleSentence;

    static int totalRecords;

public:
    // CA2 constructors and destructor.
    Vocabulary() {
        vocabularyId = 0;
        word = "";
        meaning = "";
        language = "";
        exampleSentence = "";
    }

    Vocabulary(int id, string w, string m, string lang, string sentence) {
        setDetails(id, w, m, lang, sentence);
    }

    Vocabulary(const Vocabulary& other) {
        vocabularyId = other.vocabularyId;
        word = other.word;
        meaning = other.meaning;
        language = other.language;
        exampleSentence = other.exampleSentence;
    }

    ~Vocabulary() {
        // No manual memory cleanup is needed because string manages its memory.
    }

    void setDetails(int id, string w, string m, string lang, string sentence) {
        // 'this' points to the current object.
        this->vocabularyId = id;
        this->word = w;
        this->meaning = m;
        this->language = lang;
        this->exampleSentence = sentence;
    }

    // Inline function retained from CA1.
    inline int getId() const {
        return vocabularyId;
    }

    // Convert this object into a fixed-size record for the binary file.
    void makeFileRecord(VocabularyFileRecord& record) const {
        record = {};

        record.id = vocabularyId;

        strncpy(record.word, word.c_str(), sizeof(record.word) - 1);
        strncpy(record.meaning, meaning.c_str(), sizeof(record.meaning) - 1);
        strncpy(record.language, language.c_str(), sizeof(record.language) - 1);
        strncpy(record.exampleSentence, exampleSentence.c_str(),
                sizeof(record.exampleSentence) - 1);
    }

    static int getTotalRecords() {
        return totalRecords;
    }

    static void setTotalRecords(int count) {
        totalRecords = count;
    }

    static void incrementTotalRecords() {
        totalRecords++;
    }

    // Friend function can display the private data of a Vocabulary object.
    friend void displayVocabulary(const Vocabulary& record);
};

// One shared count for all Vocabulary objects.
int Vocabulary::totalRecords = 0;

// Display one vocabulary record.
void displayVocabulary(const Vocabulary& record) {
    cout << "Vocabulary ID    : " << record.vocabularyId << '\n';
    cout << "Word             : " << record.word << '\n';
    cout << "Meaning          : " << record.meaning << '\n';
    cout << "Language         : " << record.language << '\n';
    cout << "Example Sentence : " << record.exampleSentence << '\n';
}

void showMenu() {
    cout << "\n===== Language Vocabulary Revision Scheduler =====\n";
    cout << "1. Add Vocabulary\n";
    cout << "2. Display All Vocabulary\n";
    cout << "3. Search Vocabulary by ID\n";
    cout << "4. Update Vocabulary\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

// Read the saved records when the program starts.
void loadRecords(Vocabulary records[]) {
    ifstream file(DATA_FILE, ios::binary);

    if (!file) {
        cout << "No saved records found. Starting with an empty list.\n";
        Vocabulary::setTotalRecords(0);
        return;
    }

    VocabularyFileRecord fileRecord;
    int count = 0;

    while (file.read(reinterpret_cast<char*>(&fileRecord), sizeof(fileRecord))) {
        if (count >= MAX_RECORDS) {
            cout << "Maximum record limit reached. Extra records were not loaded.\n";
            break;
        }

        // Rebuild a Vocabulary object from the saved record.
        records[count].setDetails(fileRecord.id, fileRecord.word,
                                fileRecord.meaning, fileRecord.language,
                                fileRecord.exampleSentence);
        count++;
    }

    Vocabulary::setTotalRecords(count);
    cout << count << " record(s) loaded from the file.\n";
    file.close();
}

// Check whether an ID is already present in the array.
bool idExists(Vocabulary records[], int id) {
    for (int i = 0; i < Vocabulary::getTotalRecords(); i++) {
        if (records[i].getId() == id) {
            return true;
        }
    }
    return false;
}

// Check whether a text field contains only spaces or tabs.
bool isBlank(const string& text) {
    return text.find_first_not_of(" \t") == string::npos;
}

// Read a complete vocabulary record from the user.
bool getVocabularyDetails(int& id, string& word, string& meaning,
                        string& language, string& sentence) {
    cout << "Enter Vocabulary ID: ";

    if (!(cin >> id)) {
        cout << "Please enter a valid number for the ID.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (id <= 0) {
        cout << "ID must be greater than zero.\n";
        return false;
    }

    cout << "Enter Word: ";
    getline(cin, word);

    cout << "Enter Meaning: ";
    getline(cin, meaning);

    cout << "Enter Language: ";
    getline(cin, language);

    cout << "Enter Example Sentence: ";
    getline(cin, sentence);

    if (isBlank(word) || isBlank(meaning) ||
        isBlank(language) || isBlank(sentence)) {
        cout << "Fields cannot be left empty.\n";
        return false;
    }

    // These limits make sure the text fits inside the binary record.
    if (word.length() >= 100 || meaning.length() >= 250 ||
        language.length() >= 60 || sentence.length() >= 250) {
        cout << "One of the fields is too long. Please enter shorter text.\n";
        return false;
    }

    return true;
}

// Add a record to the array and save it in the binary file.
void addVocabulary(Vocabulary records[]) {
    if (Vocabulary::getTotalRecords() >= MAX_RECORDS) {
        cout << "The vocabulary list is full.\n";
        return;
    }

    int id;
    string word, meaning, language, sentence;

    if (!getVocabularyDetails(id, word, meaning, language, sentence)) {
        return;
    }

    if (idExists(records, id)) {
        cout << "This ID already exists. Please use a different ID.\n";
        return;
    }

    // Create an object using the parameterized constructor.
    Vocabulary newRecord(id, word, meaning, language, sentence);

    // Make a copy using the copy constructor.
    Vocabulary copiedRecord(newRecord);

    VocabularyFileRecord fileRecord{};
    copiedRecord.makeFileRecord(fileRecord);

    ofstream file(DATA_FILE, ios::binary | ios::app);

    if (!file) {
        cout << "Unable to open the file for saving.\n";
        return;
    }

    file.write(reinterpret_cast<char*>(&fileRecord), sizeof(fileRecord));
    file.close();

    if (!file) {
        cout << "There was a problem saving the record.\n";
        return;
    }

    records[Vocabulary::getTotalRecords()].setDetails(
        id, word, meaning, language, sentence);
    Vocabulary::incrementTotalRecords();

    cout << "Vocabulary added and saved successfully.\n";
}

// Display all records currently loaded from the file.
void displayAllVocabulary(Vocabulary records[]) {
    int count = Vocabulary::getTotalRecords();

    if (count == 0) {
        cout << "No vocabulary records to display.\n";
        return;
    }

    for (int i = 0; i < count; i++) {
        cout << "\nRecord " << i + 1 << '\n';
        displayVocabulary(records[i]);
    }
}

// Search the binary file using seekg().
int searchRecord(int id, VocabularyFileRecord& foundRecord) {
    ifstream file(DATA_FILE, ios::binary);

    if (!file) {
        return -1;
    }

    int position = 0;
    VocabularyFileRecord currentRecord;

    while (true) {
        // Move to the beginning of the next fixed-size record.
        file.seekg(position * sizeof(VocabularyFileRecord), ios::beg);

        if (!file.read(reinterpret_cast<char*>(&currentRecord), sizeof(currentRecord))) {
            break;
        }

        if (currentRecord.id == id) {
            foundRecord = currentRecord;
            file.close();
            return position;
        }

        position++;
    }

    file.close();
    return -1;
}

void searchVocabulary() {
    int id;

    cout << "Enter ID to search: ";

    if (!(cin >> id)) {
        cout << "Please enter a valid number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    VocabularyFileRecord foundRecord;
    int position = searchRecord(id, foundRecord);

    if (position == -1) {
        cout << "Vocabulary record not found.\n";
        return;
    }

    Vocabulary result(foundRecord.id, foundRecord.word, foundRecord.meaning,
                    foundRecord.language, foundRecord.exampleSentence);

    cout << "\nRecord found:\n";
    displayVocabulary(result);
}

// Update an existing record in the binary file using seekp().
void updateVocabulary(Vocabulary records[]) {
    int id;

    cout << "Enter ID of the record to update: ";

    if (!(cin >> id)) {
        cout << "Please enter a valid number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    VocabularyFileRecord oldRecord;
    int position = searchRecord(id, oldRecord);

    if (position == -1 || position >= Vocabulary::getTotalRecords()) {
        cout << "Record not found. Nothing to update.\n";
        return;
    }

    // Copy constructor makes a copy of the existing object.
    Vocabulary updatedRecord(records[position]);

    cout << "Enter the new details below. The ID will stay the same.\n";

    string word, meaning, language, sentence;

    cout << "Enter new Word: ";
    getline(cin, word);

    cout << "Enter new Meaning: ";
    getline(cin, meaning);

    cout << "Enter new Language: ";
    getline(cin, language);

    cout << "Enter new Example Sentence: ";
    getline(cin, sentence);

    if (isBlank(word) || isBlank(meaning) ||
        isBlank(language) || isBlank(sentence) ||
        word.length() >= 100 || meaning.length() >= 250 ||
        language.length() >= 60 || sentence.length() >= 250) {
        cout << "Invalid or empty details. Record was not updated.\n";
        return;
    }

    updatedRecord.setDetails(id, word, meaning, language, sentence);

    VocabularyFileRecord newFileRecord{};
    updatedRecord.makeFileRecord(newFileRecord);

    fstream file(DATA_FILE, ios::in | ios::out | ios::binary);

    if (!file) {
        cout << "Unable to open the file for updating.\n";
        return;
    }

    // Move the write position to the record that was found.
    file.seekp(position * sizeof(VocabularyFileRecord), ios::beg);
    file.write(reinterpret_cast<char*>(&newFileRecord), sizeof(newFileRecord));
    file.close();

    if (!file) {
        cout << "There was a problem updating the record.\n";
        return;
    }

    records[position].setDetails(id, word, meaning, language, sentence);
    cout << "Vocabulary record updated successfully.\n";
}

int main() {
    // Create the data folder if it does not already exist.
    filesystem::create_directories("data");

    // CA1 array of objects and pointer are retained.
    Vocabulary records[MAX_RECORDS];
    Vocabulary* recordPointer = records;

    // Load saved records before showing the menu.
    loadRecords(recordPointer);

    int choice = 0;

    do {
        showMenu();

        if (!(cin >> choice)) {
            cout << "Please enter a number from 1 to 5.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            addVocabulary(recordPointer);
        } else if (choice == 2) {
            displayAllVocabulary(recordPointer);
        } else if (choice == 3) {
            searchVocabulary();
        } else if (choice == 4) {
            updateVocabulary(recordPointer);
        } else if (choice == 5) {
            cout << "Your records are saved. Goodbye!\n";
        } else {
            cout << "Invalid choice. Please select from 1 to 5.\n";
        }

    } while (choice != 5);

    return 0;
}
