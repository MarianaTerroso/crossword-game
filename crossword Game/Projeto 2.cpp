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

// Criação de um tipo correspondente à cor e orientação.
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

// Função para associar as letras "h" e "v" à orientação.
Orientation charToOrientation(char c) {
  if (c == 'h' || c == 'H') {
    return Orientation::HORIZONTAL;
  }
  if (c == 'v' || c == 'V') {
    return Orientation::VERTICAL;
  }
  return Orientation::INVALID;
}

// Imprimir cores.
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

//--------------------------------------------------------------------------------------------------------------------------------
// Classe Player
class Player
{
public:

  Player(int num, const std::string& name, Color color);
  void setScore(int score);
  void setGiveUp(bool giveUp);
  void setPass(bool pass);
  std::string getName() const;
  Color getColor() const;
  int getScore() const;
  bool getGiveUp() const;
  bool getPass() const;
  int getNumber() const;
  int totalScore(const vector <vector <int>>& positionSave) const;
  void positionSave(vector <vector<int>>& positionSave, const std::string& word, int row, int column, Orientation orientation) const;
  void writeColor() const;
  void writeScore() const;

private:
  std::string name;
  Color color;
  int score;
  bool giveUp;
  bool pass;
  int number;
};

// Construtor da classe Player.
Player::Player(int num, const std::string& name, Color color) :
  name(name),
  color(color),
  score(0),
  giveUp(false),
  pass(false),
  number(num)
{}

// Escolha do valor para a variável (função set).
void Player::setScore(int score)
{
  this->score = score;
}

void Player::setGiveUp(bool giveUp)
{
  this->giveUp = giveUp;
}

void Player::setPass(bool pass)
{
  this->pass = pass;
}

// Retorno do valor da variável (função get).
std::string Player::getName() const
{
  return name;
}

Color Player::getColor() const
{
  return color;
}

int Player::getScore() const
{
  return score;
}

bool Player::getGiveUp() const
{
  return giveUp;
}

bool Player::getPass() const
{
  return pass;
}

int Player::getNumber() const
{
  return number;
}

// Guarda num vetor a posição onde foi colocada a letra pelo jogador. 
void Player::positionSave(vector <vector<int>>& positionSave, const std::string& word, int row, int column, Orientation orientation) const
{
  if (orientation == Orientation::VERTICAL) {
    for (size_t i = row; i < row + word.size(); i++) positionSave.at(i).at(column) = number;
  }
  else {
    for (size_t i = column; i < column + word.size(); i++) positionSave.at(row).at(i) = number;
  }
}

// Calcula a pontuação total de determinado jogador.
int Player::totalScore(const vector <vector <int>>& positionSave) const
{
  int totalScore = 0;
  for (int i = 0; i < 13; i++) {
    for (int j = 0; j < 13; j++) {
      if (positionSave.at(i).at(j) == number) totalScore = totalScore + 1;
    }
  }
  return totalScore;
}

// Imprime a cor correspondente a cada jogador.
void Player::writeColor() const
{
  printColor(color);
  switch (color) {
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
}

// Imprime no ecrã a pontuação total.
void Player::writeScore() const
{
  cout << "Score: " << score << endl << endl;
}

//---------------------------------------------------------------------------------------------------------------
// Converte o tipo char no tipo inteiro.
int charToInt(char character) {
  if (character >= 'A' && character <= 'M') return character - 'A';
  if (character >= 'a' && character <= 'm') return character - 'a';
  return -1;
}


//---------------------------------------------------------------------------------------------------------------
//Classe Board
class Board
{
public:
  Board();
  ostream& printBoard(const vector <vector<int>>& position_save) const;
  char getCell(int line, int column) const;
  void setCell(int line, int column, char letter);
  bool isEmpty(int row1 = 0, int row2 = 12, int col1 = 0, int col2 = 12) const;


private:
  char board[13][13];
};

// Contrutor da classe Board.
Board::Board() {
  for (int i = 0; i < 13; i++) {
    for (int j = 0; j < 13; j++) {
      this->board[i][j] = ' ';
    }
  }
}

// Imprime o tabuleiro no ecrã.
ostream& Board::printBoard(const vector <vector<int>>& position_save) const {
  cout << "  ";
  for (char c = 'a'; c <= 'm'; c++) {
    cout << c << " ";
  }
  cout << endl;
  for (char i = 'A'; i <= 'M'; i++) {
    cout << i << ' ';
    printColor(Color::GREY);
    for (int j = 0; j < 13; j++) {
      if (position_save.at(charToInt(i)).at(j) == 0) printColor(Color::RED);
      else if (position_save.at(charToInt(i)).at(j) == 1) printColor(Color::GREEN);
      else if (position_save.at(charToInt(i)).at(j) == 2) printColor(Color::BLUE);
      else if (position_save.at(charToInt(i)).at(j) == 3) printColor(Color::MAGENTA);
      cout << this->board[charToInt(i)][j];
      if (j < 12) cout << ' ';
    }
    resetColors() << endl;
  }
  return cout;
}

// Retorno do valor da variável (função get).
char Board::getCell(int line, int column) const {
  return this->board[line][column];
}

// Escolha do valor para a variável (função set).
void Board::setCell(int line, int column, char letter) {
  this->board[line][column] = letter;
}

// Retorna verdadeiro se o tabuleiro estiver vazio.
bool Board::isEmpty(int row1, int row2, int col1, int col2) const
{
  for (int i = row1; i <= row2; i++) {
    for (int j = col1; j <= col2; j++) {
      if (this->board[i][j] != ' ') return false;
    }
  }
  return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Class Bag
class Bag
{
public:
  Bag(istream& config);
  char take();
  bool isEmpty() const;


private:
  vector<char> bag;
};

// Construtor da classe Bag.
Bag::Bag(istream& config) {
  std::string title;
  getline(config, title);

  char c;
  int n;
  while (config >> c >> n)
  {
    for (int i = 1; i <= n; i++)
    {
      this->bag.push_back(c);
    }
  }

  // Shuffle bag
  unsigned seed = (unsigned)chrono::system_clock::now().time_since_epoch().count();
  shuffle(bag.begin(), bag.end(), default_random_engine(seed));
}

// Retorna verdadeiro se o bag estiver vazio.
bool Bag::isEmpty() const {
  return this->bag.empty();
}

// Retira letras do bag.
char Bag::take() {
  char letter = this->bag.back();
  this->bag.pop_back();
  return letter;

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Classe Rack
class Rack
{
public:
  Rack();
  Rack(const Rack& rack);
  ostream& printRack() const;
  void fillRack(Bag& bag);
  void dropRack();
  bool remove(char letter);

private:
  char rack[7];
};

//  Construtor da classe Rack.
Rack::Rack() {}

Rack::Rack(const Rack& rack) {
  for (int i = 0; i < 7; i++) {
    this->rack[i] = rack.rack[i];
  }
}

// Imprime a prateleira no ecrã.
ostream& Rack::printRack() const {
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

// Preenche a prateleira com novas letras, se estiver vazia.
void Rack::fillRack(Bag& bag) {
  int count = 0;
  for (int i = 0; i < 7; i++) {
    if (bag.isEmpty()) {
      break;
    }
    if (rack[i] == NULL) {
      rack[i] = bag.take();
    }
  }
  sort(rack, rack + 7);
}

// Esvazia a prateleira.
void Rack::dropRack() {
  for (int i = 0; i < 7; i++) {
    rack[i] = NULL;
  }
}

// Remove da prateleira a letra escolhida pelo jogador.
bool Rack::remove(char letter) {
  for (int k = 0; k < 7; k++) {
    if (letter == this->rack[k]) {
      this->rack[k] = NULL;
      return true;
    }
  }
  return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------
// Classe Game
class Game
{
public:
  Game(size_t numberOfPlayers, Bag bag,vector<Player> players,Rack rack, Board board);
  size_t getCurrentPlayer() const;
  void nextPlayer();
  bool isOver(int winnerScore, int numPass) const;
  void declareWinner() const;
  void resetPass(vector<Player>& players) const;
  bool allPlayersPassed() const;
  static std::string getConfigValue(istream& config);
  static size_t defineNumberOfPlayers();

private:
  size_t numberOfPlayers;
  size_t currentPlayer;
  Bag bag;
  vector<Player> players;
  Rack rack;
  Board board;

};

// Construtor da classe Game.
Game::Game(size_t numberOfPlayers,Bag bag, vector<Player> players, Rack rack, Board board) :
  numberOfPlayers(numberOfPlayers),
  currentPlayer(0),
  bag(bag),
  players (players),
  rack(rack),
  board(board)
{};

// Retorno do valor da variável (função get).
size_t Game::getCurrentPlayer() const {
  return this->currentPlayer;
}

// Dá a vez ao próximo jogador.
void Game::nextPlayer() {
  this->currentPlayer = (this->currentPlayer + 1) % numberOfPlayers;
}

// Retorna verdadeiro quando o jogo acaba, ou seja, quando um jogador atinge a pontuação máxima, quando todos passam em três rondas ou quando apenas um dos jogadores não desiste.
bool Game::isOver(int winnerScore, int numPass) const {
  for (size_t i = 0; i < players.size(); i++) {
    if (players[i].getScore() >= winnerScore) {
      return true;
    }
  }

  int numGiveUp = 0;
  for (size_t i = 0; i < players.size(); i++) {
    if (players[i].getGiveUp()) {
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

// Imprime no ecrã o jogador vencedor.
void Game::declareWinner() const {
  int winner = -1;
  int winnerScore = -1;
  for (size_t i = 0; i < players.size(); i++) {
    if (!players[i].getGiveUp() && players[i].getScore() > winnerScore) {
      winner = i;
      winnerScore = players[i].getScore();
    }
  }

  cout << players[winner].getName() << ", you are the winner!!" << endl;
}

// Atribui valor falso a todos os jogadores.
void Game::resetPass(vector<Player>& players) const {
  for (size_t i = 0; i < players.size(); i++) {
    players[i].setPass(false); 
  }
}

// Retorna verdadeiro se todos os jogadores passarem.
bool Game::allPlayersPassed() const {
  for (size_t i = 0; i < players.size(); i++) {
    if (!players[i].getGiveUp() && !players[i].getPass()) {
      return false;
    }
  }
  return true;
}

// Obtém a partir do ficheiro CONFIG o valor da pontuação máxima.
std::string Game::getConfigValue(istream& config) {
  std::string value;
  getline(config, value);
  cout << value << endl;
  return value.substr(value.find_last_of(' ') + 1);
}

// Pergunta o número de jogadores; apenas permite entre 2 a 4.
size_t Game::defineNumberOfPlayers()
{
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

  return numberOfPlayers;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
// Tranforma as minúsculas de uma string em maiúsculas.
std::string lowerToUpper(std::string& w)
{
  for (size_t i = 0; i < w.size(); i++) w[i] = toupper((int)w[i]);
  return w;
}

// Lista das palavras válidas.
void getWords(istream& wordsFile, vector<std::string>& words) {
  std::string w;
  while (wordsFile >> w)
  {
    w = lowerToUpper(w);
    words.push_back(w);
  }
}

// Coloca uma palavra no tabuleiro.
void putWordInBoard(Board& board, Rack& rack, const std::string& word, int row, int col, Orientation orientation) {
  for (size_t i = 0; i < word.size(); i++) {
    if (board.getCell(row, col) == ' ') {
      rack.remove(word[i]);
    }
    board.setCell(row, col, word[i]);

    if (orientation == Orientation::VERTICAL) {
      row++;
    }
    else {
      col++;
    }
  }
}

//Palavras inválidas.
bool invalidWord1(const std::string& word, const vector <std::string>& words)
{
  for (size_t i = 0; i < words.size(); i++)
  {
    if (word == words[i]) {
      return false;
    }
  }

  return true;
}

// Posição inválida.
bool invalidWord2(const std::string& word, int line, int column, Orientation orientation)
{
  int numberOfLetters = word.size();

  if (14 - column <= numberOfLetters) return true;
  else if (14 - line <= numberOfLetters) return true;

  return false;
}

// As letras colocadas não fazem parte da prateleira, nem das que já estão no tabuleiro.
bool invalidWord3(const Board& board, const Rack& rack, const std::string& word, int line, int column, Orientation orientation)
{
  Rack rackCopy(rack);


  for (size_t i = 0; i < word.size(); i++) {

    if (board.getCell(line, column) == ' ') {
      if (!rackCopy.remove(word[i])) {
        return true;
      }
    }
    else {
      if (board.getCell(line, column) != word[i]) {
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

// Não pode ter letras imediatamente antes ou depois da palavra.
bool invalidWord4(const Board& board, const vector<std::string>& words, const std::string& word, int line, int column, Orientation orientation) {
  if (orientation == Orientation::HORIZONTAL)
  {
    if ((column - 1 >= 0 && board.getCell(line, column - 1) != ' ') || (column + word.size() <= 12 && board.getCell(line, column + word.size()) != ' ')) {
      return true;
    }
  }
  else
  {
    if ((line - 1 >= 0 && board.getCell(line - 1, column) != ' ') || (line + word.size() <= 12 && board.getCell(line + word.size(), column) != ' ')) {
      return true;
    }
  }
  return false;
}

// Se o board não estiver vazio, a palavra tem de usar uma letra já existente ou estar adjacente a uma letra.
bool invalidWord5(const Board& board, const vector<std::string>& words, const std::string& word, int line, int column, Orientation orientation) {
  if (board.isEmpty()) {
    return false;
  }

  if (orientation == Orientation::HORIZONTAL)
  {
    for (size_t j = column; j < column + word.size(); j++)
    {
      if ((line + 1 <= 12 && board.getCell(line + 1, j) != ' ') || (line - 1 >= 0 && board.getCell(line - 1, j) != ' '))
      {
        return false;
      }
    }
  }
  else
  {
    for (size_t j = line; j < line + word.size(); j++)
    {
      if ((column + 1 <= 12 && board.getCell(line, j + 1) != ' ') || (column - 1 >= 0 && board.getCell(line, j - 1) != ' '))
      {
        return false;
      }
    }
  }

  bool usesExistingLetter = false;
  for (size_t i = 0; i < word.size(); i++) {
    if (board.getCell(line, column) != ' ') {
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

int findAdjacentWordLimit(const Board& board, int line, int column, Orientation orientation, bool start) {
  while (true) {
    if (orientation == Orientation::HORIZONTAL) {
      line += start ? -1 : 1;
    }
    else {
      column += start ? -1 : 1;
    }

    if (line < 0 || column < 0 || line > 12 || column > 12 || board.getCell(line, column) == ' ') {
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

std::string createNewWord(const Board& board, int line, int column, Orientation orientation, const std::string& word)
{
  int pos1 = findAdjacentWordLimit(board, line, column, orientation, true);
  int pos2 = findAdjacentWordLimit(board, line, column, orientation, false);

  std::string newWord;
  for (int k = pos1; k < pos2; k++) {
    if (orientation == Orientation::HORIZONTAL) newWord.push_back(board.getCell(k, column));
    else if (orientation == Orientation::VERTICAL) newWord.push_back(board.getCell(line, k));
  }

  return newWord;
}

// Se estiver adjacente a outras palavras, tem de formar palavras válidas com esta.
bool invalidWord6(Board& board, const vector<std::string>& words, const std::string& word, int line, int column, Orientation orientation)
{

  if (orientation == Orientation::HORIZONTAL)
  {
    for (size_t j = 0; j < word.size(); j++) {
      if ((line + 1 <= 12 && board.getCell(line + 1, column) != ' ') || (line - 1 >= 0 && board.getCell(line - 1, column) != ' '))
      {
        std::string newWord;
        char old_letter = board.getCell(line, column);
        board.setCell(line, column, word[j]);
        newWord = createNewWord(board, line, column, orientation, word);
        board.setCell(line, column, old_letter);
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
      if ((column + 1 <= 12 && board.getCell(line, column + 1) != ' ') || (column - 1 >= 0 && board.getCell(line, column - 1) != ' '))
      {
        std::string newWord;
        char old_letter = board.getCell(line, column);
        board.setCell(line, column, word[j]);
        newWord = createNewWord(board, line, column, orientation, word);
        board.setCell(line, column, old_letter);
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

// Retorna verdadeiro se a palavra for válida.
bool tryPutWord(Board& board, Rack& rack, const vector<std::string>& words, const std::string& word, int line, int column, Orientation orientation,
  vector <vector <int>>& position_save, int currentPlayer) {
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

// Imprime no ecrã o nome e a pontuação de todos os jogadores.
void printLeaderboard(const vector<Player>& players) {
  cout << endl;
  for (size_t i = 0; i < players.size(); i++) {
    cout << "Player: " << players[i].getName() << ", place: " << i + 1 << ", score: " << players[i].getScore() << endl;
  }
  cout << endl << endl;
}

//-----------------------------------------------------------------------------------------------------------------------------------

int main()
{
  // Abre o ficheiro CONFIG.
  ifstream config("CONFIG.txt");
  if (config.fail())
  {
    cerr << "Input file opening failed.\n";
    exit(1);
  }

  // Obtenção da pontuação máxima, da lista de palavras válidas e das letras disponíveis.
  int winnerScore = stoi(Game::getConfigValue(config));

  ifstream validWordsFile(Game::getConfigValue(config));
  if (validWordsFile.fail())
  {
    cerr << "Input file opening failed.\n";
    exit(1);
  }

  vector<std::string> words;
  getWords(validWordsFile, words);

  Bag bag(config);

  // Criação de uma prateleira.
  Rack rack;
  rack.fillRack(bag);

  Board board;

  // Definição dos jogadores.
  size_t numberOfPlayers = Game::defineNumberOfPlayers();
  vector<Player> players;

  Game game(numberOfPlayers,bag,players,rack,board);

  for (size_t i = 0; i < numberOfPlayers; i++) {

    std::string nameOfPlayer;
    cout << "Player " << i << ": ";
    cin >> nameOfPlayer;
    players.push_back(Player(i, nameOfPlayer, (Color)i));

    if (cin.fail()) {
      return 0;
    }

    players[i].writeColor();
    players[i].writeScore();
  }

  int numPass = 0;
  vector <vector <int>> position_save(13, vector < int>(13));
  for (int i = 0; i < 13; i++) {
    for (int j = 0; j < 13; j++) {
      position_save.at(i).at(j) = ' ';
    }
  }

  // Rondas.
  while (!game.isOver(winnerScore, numPass))
  {
    size_t currentPlayer = game.getCurrentPlayer();
    if (players[currentPlayer].getGiveUp()) {
      game.nextPlayer();
      continue;
    }

    board.printBoard(position_save);
    cout << endl;
    rack.printRack();
    cout << players[currentPlayer].getName() << ", it's your turn!" << endl;
    cout << "Input a word or one of the following commands:" << endl;
    cout << "\t/giveUp (give up the game)" << endl;
    cout << "\t/next (pass your turn)" << endl;
    cout << "Word: ";
    std::string word;
    cin >> word;
    if (cin.fail()) {
      return 0;
    }

    if (word == "/giveUp")
    {
      players[currentPlayer].setGiveUp(true);
      players[currentPlayer].setScore(0);
    }
    else if (word == "/next")
    {
      players[currentPlayer].setPass(true);
    }
    else
    {
      word = lowerToUpper(word);
      game.resetPass(players);
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

      if (!tryPutWord(board, rack, words, word, row, col, orientation, position_save, currentPlayer)) {
        continue;
      }

      putWordInBoard(board, rack, word, row, col, orientation);
      players[currentPlayer].positionSave(position_save, word, row, col, orientation);
    }
    for (size_t i = 0; i < numberOfPlayers; i++)
    {
      players[i].setScore(players[i].totalScore(position_save));
    }

    if (game.allPlayersPassed()) {
      game.resetPass(players);
      numPass++;
      rack.dropRack();
    }

    rack.fillRack(bag);
    game.nextPlayer();
    printLeaderboard(players);
  }
  // Declarar o jogador vencedor.
  game.declareWinner();
}

