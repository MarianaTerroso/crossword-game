#include <iostream>
#include <string>
#include <fstream>
#include <cstddef>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <random>
#include <chrono>
#include <ios>
#include <limits>
#include <iomanip>

using namespace std;
vector<char> bag;

enum class Color {
    RED = 0,
    BLUE = 1,
    GREEN = 2,
    MAGENTA = 3,
    GREY
};

enum class Orientation {
    HORIZONTAL,
    VERTICAL,
    INVALID
};

Orientation charToOrientation(char c) {
    if (c == 'h' || c == 'H') {
        return Orientation::HORIZONTAL;
    }
    if (c == 'v' || c == 'V') {
        return Orientation::VERTICAL;
    }
    return Orientation::INVALID;
}

// Colors
ostream& printColor(Color c) {
    switch (c) {
    case Color::RED:
        cout << "\x1B[31m";
        break;
    case Color::GREEN:
        cout << "\x1B[32m";
        break;
    case Color::BLUE:
        cout << "\x1B[34m";
        break;
    case Color::MAGENTA:
        cout << "\x1B[35m";
        break;
    case Color::GREY:
        cout << "\x1B[100m";
        break;
    }
    return cout;
}

ostream& resetColors() {
    return cout << "\x1B[0m";
}

// Players definition
struct Player {
    string name;
    Color color;
    int score;
    bool giveUp;
    bool pass;

    Player(string name, Color color) :
        name(name), color(color), score(0), giveUp(false), pass(false) {}
};

int charToInt(char character) {
    if (character >= 'A' && character <= 'M') return character - 'A';
    if (character >= 'a' && character <= 'm') return character - 'a';
    return -1;
}

//Board
ostream& printBoard(char board[13][13], vector <vector<int>> position_Save) {
    cout << "  ";
    for (char c = 'a'; c <= 'm'; c++) {
        cout << c << " ";
    }
    cout << endl;
    for (char i = 'A'; i <= 'M'; i++) {
        cout << i << ' ';
        printColor(Color::GREY);
        for (int j = 0; j < 13; j++) {
            if (position_Save.at(charToInt(i)).at(j) == 0) printColor(Color::RED);
            else if (position_Save.at(charToInt(i)).at(j) == 1) printColor(Color::GREEN);
            else if (position_Save.at(charToInt(i)).at(j) == 2) printColor(Color::BLUE);
            else if (position_Save.at(charToInt(i)).at(j) == 3) printColor(Color::MAGENTA);
            cout << board[charToInt(i)][j];
            if (j < 12) cout << ' ';
        }
        resetColors() << endl;
    }
    return cout;
}


bool emptyBoard(char board[13][13], int row1 = 0, int row2 = 12, int col1 = 0, int col2 = 12)
{
    for (int i = row1; i <= row2; i++) {
        for (int j = col1; j <= col2; j++) {
            if (board[i][j] != ' ') return false;
        }
    }
    return true;
}

// Rack
ostream& printRack(char rack[7]) {
    cout << "RACK: ";
    for (int i = 0; i < 7; i++) {
        if (rack[i] == NULL) {
            continue;
        }
        cout << rack[i];

        if (i != 6) {
            cout << ", ";
        }
    }
    cout << endl;
    return cout;
}

void fillRack(char rack[7], vector<char>& bag) {
    int count = 0;
    for (int i = 0; i < 7; i++) {
        if (bag.empty()) {
            break;
        }
        if (rack[i] == NULL) {
            rack[i] = bag.back();
            bag.pop_back();
        }
    }
    sort(rack, rack + 7);
}

void dropRack(char rack[7]) {
    for (int i = 0; i < 7; i++) {
        rack[i] = NULL;
    }
}

// Bag
void createBag(istream& config, vector<char>& bag) {
    string title;
    getline(config, title);

    char c;
    int n;
    while (config >> c >> n)
    {
        for (int i = 1; i <= n; i++)
        {
            bag.push_back(c);
        }
    }

    // Shuffle bag
    unsigned seed = (unsigned)chrono::system_clock::now().time_since_epoch().count();
    shuffle(bag.begin(), bag.end(), default_random_engine(seed));
}

string getConfigValue(istream& config) {
    string value;
    getline(config, value);
    cout << value << endl;
    return value.substr(value.find_last_of(' ') + 1);
}

// List of Valid Words
string lowerToUpper(string& w)
{
    for (size_t i = 0; i < w.size(); i++) w[i] = toupper((int)w[i]);
    return w;
}

void getWords(istream& wordsFile, vector<string>& words) {
    string w;
    while (wordsFile >> w)
    {
        w = lowerToUpper(w);
        words.push_back(w);
    }
}

//Game Over and Winner
bool isGameOver(int winnerScore, const vector<Player>& players, int numPass) {
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i].score >= winnerScore) {
            return true;
        }
    }

    int numGiveUp = 0;
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i].giveUp) {
            numGiveUp = numGiveUp + 1;
        }
    }
    if (numGiveUp == players.size() - 1) {
        return true;
    }

    if (numPass == 3) {
        return true;
    }

    return false;

}

void declareWinner(const vector<Player>& players) {
    int winner = -1;
    int winnerScore = -1;
    for (size_t i = 0; i < players.size(); i++) {
        if (!players[i].giveUp && players[i].score > winnerScore) {
            winner = i;
            winnerScore = players[i].score;
        }
    }

    cout << players[winner].name << ", you are the winner!!" << endl;
}

// Pass
void resetPass(vector<Player>& players) {
    for (size_t i = 0; i < players.size(); i++) {
        players[i].pass = false;
    }
}

bool allPlayersPassed(vector<Player>& players) {
    for (size_t i = 0; i < players.size(); i++) {
        if (!players[i].giveUp && !players[i].pass) {
            return false;
        }
    }
    return true;
}

void putWordInBoard(char board[13][13], char rack[7], string word, int row, int col, Orientation orientation) {
    for (size_t i = 0; i < word.size(); i++) {
        if (board[row][col] == ' ') {
            for (size_t j = 0; j < 7; j++) {
                if (rack[j] == word[i]) {
                    rack[j] = NULL;
                    break;
                }
            }
        }
        board[row][col] = word[i];

        if (orientation == Orientation::VERTICAL) {
            row++;
        }
        else {
            col++;
        }
    }
}

void positionSave(vector <vector<int>>& positionSave, string word, int row, int column, Orientation orientation, int number) //number = number of the player
{
    if (orientation == Orientation::VERTICAL) {
        for (size_t i = row; i < row + word.size(); i++) positionSave.at(i).at(column) = number;
    }
    else {
        for (size_t i = column; i < column + word.size(); i++) positionSave.at(row).at(i) = number;
    }
}

int totalScore(vector <vector <int>> positionSave, int number)
{
    int totalScore = 0;
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            if (positionSave.at(i).at(j) == number) totalScore = totalScore + 1;
        }
    }
    return totalScore;
}


//Invalid words
bool invalidWord1(string word, vector <string> words) //  Não faz parte da lista de palavras
{
    for (size_t i = 0; i < words.size(); i++)
    {
        if (word == words[i]) {
            return false;
        }
    }

    return true;
}

bool invalidWord2(string word, int line, int column, Orientation orientation) // Posição inválida 
{
    int numberOfLetters = word.size();

    if (14 - column <= numberOfLetters) return true;
    else if (14 - line <= numberOfLetters) return true;

    return false;
}

bool inRackCopy(char letter, char rackCopy[7]) {
    for (int k = 0; k < 7; k++) {
        if (letter == rackCopy[k]) {
            rackCopy[k] = NULL;
            return true;
        }
    }
    return false;
}

bool invalidWord3(char board[13][13], char rack[7], string word, int line, int column, Orientation orientation) // As letras colocadas não fazem parte da prateleira, nem das que já estão no tabuleiro
{
    char rackCopy[7];
    for (int i = 0; i < 7; i++) {
        rackCopy[i] = rack[i];
    }

    for (size_t i = 0; i < word.size(); i++) {

        if (board[line][column] == ' ') {
            if (!inRackCopy(word[i], rackCopy)) {
                return true;
            }
        }
        else {
            if (board[line][column] != word[i]) {
                return true;
            }
        }
        if (orientation == Orientation::HORIZONTAL) {
            column++;
        }
        else {
            line++;
        }
    }

    return false;
}

bool invalidWord4(char board[13][13], vector<string> words, string word, int line, int column, Orientation orientation) { // Nao pode ter letras imediatamente antes ou depois da palavra
    if (orientation == Orientation::HORIZONTAL)
    {
        if ((column - 1 >= 0 && board[line][column - 1] != ' ') || (column + word.size() <= 12 && board[line][column + word.size()] != ' ')) {
            return true;
        }
    }
    else
    {
        if ((line - 1 >= 0 && board[line - 1][column] != ' ') || (line + word.size() <= 12 && board[line + word.size()][column] != ' ')) {
            return true;
        }
    }
    return false;
}

bool invalidWord5(char board[13][13], vector<string> words, string word, int line, int column, Orientation orientation) { // Se o board não estiver vazio, a palavra tem de usar uma letra ja existente ou estar adjacente a uma letra
    if (emptyBoard(board)) {
        return false;
    }

    if (orientation == Orientation::HORIZONTAL)
    {
        for (size_t j = column; j < column + word.size(); j++)
        {
            if ((line + 1 <= 12 && board[line + 1][j] != ' ') || (line - 1 >= 0 && board[line - 1][j] != ' '))
            {
                return false;
            }
        }
    }
    else
    {
        for (size_t j = line; j < line + word.size(); j++)
        {
            if ((column + 1 <= 12 && board[j][column + 1] != ' ') || (column - 1 >= 0 && board[j][column - 1] != ' '))
            {
                return false;
            }
        }
    }

    bool usesExistingLetter = false;
    for (size_t i = 0; i < word.size(); i++) {
        if (board[line][column] != ' ') {
            return false;
        }

        if (orientation == Orientation::HORIZONTAL) {
            column++;
        }
        else {
            line++;
        }
    }

    return true;
}

int findAdjacentWordLimit(char board[13][13], int line, int column, Orientation orientation, bool start) {
    while (true) {
        if (orientation == Orientation::HORIZONTAL) {
            line += start ? -1 : 1;
        }
        else {
            column += start ? -1 : 1;
        }

        if (line < 0 || column < 0 || line > 12 || column > 12 || board[line][column] == ' ') {
            if (orientation == Orientation::HORIZONTAL) {
                if (start) {
                    line += 1;
                }
                return line;
            }
            else {
                if (start) {
                    column += 1;
                }
                return column;
            }
        }
    }
}

string createNewWord(char board[13][13], int line, int column, Orientation orientation, string word)
{
    int pos1 = findAdjacentWordLimit(board, line, column, orientation, true);
    int pos2 = findAdjacentWordLimit(board, line, column, orientation, false);

    string newWord;
    for (int k = pos1; k < pos2; k++) {
        if (orientation == Orientation::HORIZONTAL) newWord.push_back(board[k][column]);
        else if (orientation == Orientation::VERTICAL) newWord.push_back(board[line][k]);
    }

    return newWord;
}

bool invalidWord6(char board[13][13], vector<string> words, string word, int line, int column, Orientation orientation) // Se estiver adjacente a outras palavras, tem de formar palavras validas com estas 
{

    if (orientation == Orientation::HORIZONTAL)
    {
        for (size_t j = 0; j < word.size(); j++) {
            if ((line + 1 <= 12 && board[line + 1][column] != ' ') || (line - 1 >= 0 && board[line - 1][column] != ' '))
            {
                string newWord;
                char old_letter = board[line][column];
                board[line][column] = word[j];
                newWord = createNewWord(board, line, column, orientation, word);
                board[line][column] = old_letter;
                if (invalidWord1(newWord, words)) {
                    cout << "Word " << newWord << " is not valid" << endl;
                    return true;
                }
            }

            column += 1;
        }
    }
    else
    {
        for (size_t j = 0; j < word.size(); j++) {
            if ((column + 1 <= 12 && board[line][column + 1] != ' ') || (column - 1 >= 0 && board[line][column - 1] != ' '))
            {
                string newWord;
                char old_letter = board[line][column];
                board[line][column] = word[j];
                newWord = createNewWord(board, line, column, orientation, word);
                board[line][column] = old_letter;
                if (invalidWord1(newWord, words)) {
                    cout << "Word " << newWord << " is not valid" << endl;
                    return true;
                }
            }

            line += 1;
        }
    }
    return false;
}

bool tryPutWord(char board[13][13], char rack[7], vector<string>& words, string word, int line, int column, Orientation orientation,
    vector <vector <int>>& position_Save, int currentPlayer) {
    if (invalidWord1(word, words)) {
        cout << "The given word is not valid." << endl;
        return false;
    }

    if (invalidWord2(word, line, column, orientation)) {
        cout << "The word would go out of bounds." << endl;
        return false;
    }

    if (invalidWord3(board, rack, word, line, column, orientation)) {
        cout << "You don't have the necessary letters to make that word." << endl;
        return false;
    }

    if (invalidWord4(board, words, word, line, column, orientation)) {
        cout << "There are letters immediately before or after the word that would be played." << endl;
        return false;
    }

    if (invalidWord5(board, words, word, line, column, orientation)) {
        cout << "The word must either use an existing letter or be adjacent to an existing letter" << endl;
        return false;
    }

    if (invalidWord6(board, words, word, line, column, orientation)) {
        cout << "The word must form valid words with every letter it is adjacent to." << endl;
        return false;
    }

    return true;
}

void printLeaderboard(const vector<Player>& players) {
    cout << endl;
    for (size_t i = 0; i < players.size(); i++) {
        cout << "Player: " << players[i].name << ", place: " << i + 1 << ", score: " << players[i].score << endl;
    }
    cout << endl << endl;
}

int main()
{
    ifstream config("CONFIG.txt");
    if (config.fail())
    {
        cerr << "Input file opening failed.\n";
        exit(1);
    }

    int winnerScore = stoi(getConfigValue(config));

    ifstream validWordsFile(getConfigValue(config));
    if (validWordsFile.fail())
    {
        cerr << "Input file opening failed.\n";
        exit(1);
    }

    vector<string> words;
    getWords(validWordsFile, words);

    vector<char> bag;
    createBag(config, bag);

    char rack[7] = { 0 };
    fillRack(rack, bag);

    char board[13][13];
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            board[i][j] = ' ';
        }
    }

    // PLayers
    size_t numberOfPlayers;
    do {
        cout << "How many players (2-4) ? ";
        cin >> numberOfPlayers;
        if (cin.fail()) {
            if (cin.eof()) {
                return 0;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (numberOfPlayers < 2 || numberOfPlayers > 4);

    vector<Player> players;
    for (size_t i = 0; i < numberOfPlayers; i++) {
        string name;
        cout << "Player " << i + 1 << ": ";
        cin >> name;
        if (cin.fail()) {
            return 0;
        }
        players.push_back(Player(name, (Color)i));
        printColor(players[i].color);
        switch (players[i].color) {
        case Color::RED:
            cout << "Color: Red";
            break;
        case Color::GREEN:
            cout << "Color: Green";
            break;
        case Color::BLUE:
            cout << "Color: Blue";
            break;
        case Color::MAGENTA:
            cout << "Color: Magenta";
            break;
        }
        resetColors() << endl;
        cout << "Score: " << players[i].score << endl << endl;
    }

    int numPass = 0;
    int currentPlayer = 0;
    vector <vector <int>> position_Save(13, vector < int>(13));
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            position_Save.at(i).at(j) = ' ';
        }
    }


    //     Play rounds
    while (!isGameOver(winnerScore, players, numPass))
    {
        if (players[currentPlayer].giveUp) {
            currentPlayer = (currentPlayer + 1) % players.size();
            continue;
        }

        printBoard(board, position_Save);
        cout << endl;
        printRack(rack);
        cout << players[currentPlayer].name << ", it's your turn!" << endl;
        cout << "Input a word or one of the following commands:" << endl;
        cout << "\t/giveUp (give up the game)" << endl;
        cout << "\t/next (pass your turn)" << endl;
        cout << "Word: ";
        string word;
        cin >> word;
        if (cin.fail()) {
            return 0;
        }

        if (word == "/giveUp")
        {
            players[currentPlayer].giveUp = true;
            players[currentPlayer].score = 0;
        }
        else if (word == "/next")
        {
            players[currentPlayer].pass = true;
        }
        else
        {
            word = lowerToUpper(word);
            resetPass(players);
            numPass = 0;
            char position[3];
            cout << "Position: ";
            cin >> position;
            if (cin.fail()) {
                return 0;
            }

            if (position[0] < 'A' || position[0] > 'M') {
                cout << "Invalid row" << endl;
                continue;
            }
            if (position[1] < 'a' || position[1] > 'm') {
                cout << "Invalid column" << endl;
                continue;
            }
            int row = charToInt(position[0]);
            int col = charToInt(position[1]);
            Orientation orientation = charToOrientation(position[2]);
            if (orientation == Orientation::INVALID) {
                cout << "Invalid orienation" << endl;
                continue;
            }

            if (!tryPutWord(board, rack, words, word, row, col, orientation, position_Save, currentPlayer)) {
                continue;
            }

            putWordInBoard(board, rack, word, row, col, orientation);
            positionSave(position_Save, word, row, col, orientation, currentPlayer);
        }
        for (int i = 0; i < numberOfPlayers; i++)
        {
            players[i].score = totalScore(position_Save, i);
        }

        if (allPlayersPassed(players)) {
            resetPass(players);
            numPass++;
            dropRack(rack);
        }

        fillRack(rack, bag);
        currentPlayer = (currentPlayer + 1) % players.size();
        printLeaderboard(players);
    }
    declareWinner(players);
}

