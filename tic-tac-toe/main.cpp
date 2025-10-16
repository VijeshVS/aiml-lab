// Tic Tac Toe Game Implementation
// Multiple Players Game (Human + Bot)

#include <iostream>
using namespace std;

class Board {
private:
    vector<vector<char>> status;
public:
    int size;
    Board(int n) : size(n) {
        status.assign(n,vector<char>(n,'.'));
    }
    void placeMove(int row,int col,char sign){
        status[row][col] = sign;
    }
    void printBoard(){
        cout << endl;
        cout << "[Board]" << endl;
        for(auto row: status){
            cout << " ";
            for(auto ch: row){
                cout << ch << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    bool checkIfCellEmpty(int row,int col){
        return status[row][col] == '.';
    }

    bool checkIfBoardFull(){
        for(auto row: status){
            for(auto ch: row){
                if(ch == '.') return false;
            }
        }
        return true;
    }

    char getWinner(){
        // check row
        for(auto row : status){
            char ch = row[0];
            int cnt = 0;
            for(auto ele : row){
                if(ele == ch) cnt++;
            }
            if(cnt == row.size() && ch != '.') return ch;
        }

        // check col
        for(int col = 0; col < size; col++){
            char ch = status[0][col];
            int cnt = 0;
            for(int row = 0; row < size; row++){
                if(status[row][col] == ch) cnt++;
            }
            if(cnt == size && ch != '.') return ch;
        }

        // check diagonal
        char ch = status[0][0];
        int cnt = 0;
        for(int i = 0; i < size; i++){
            if(status[i][i] == ch) cnt++;
        }
        if(cnt == size && ch != '.') return ch;

        ch = status[0][size-1];
        cnt = 0;
        for(int i = 0; i < size; i++){
            if(status[i][size-1-i] == ch) cnt++;
        }
        if(cnt == size && ch != '.') return ch;

        return '.';
    }
};

class Player {
protected:
    string name;
    char sign;
public:
    Player(string name,char sign) : name(name), sign(sign) {}
    virtual bool makeMove(Board *board) = 0;
    string getName(){
        return name;
    }
    char getSign(){
        return sign;
    }
};

class DumbBot : public Player {
public:
    DumbBot(string name,char sign) : Player(name,sign) {}
    bool makeMove(Board* board) {
        // Implement bot logic using DFS to determine the best move
        // For simplicity, let's make a random valid move for now
        for(int i = 0; i < board->size; i++){
            for(int j = 0; j < board->size; j++){
                if(board->checkIfCellEmpty(i,j)){
                    board->placeMove(i,j,this->sign);
                    cout << "Bot-" << this->name << " has placed " << this->sign << " at (" << i << "," << j << ")" << endl;
                    return true;
                }
            }
        }
        return false; // No valid moves left
    }
};

class SmartBot : public Player {
private:
    Player *enemy;
public:
    SmartBot(string name,char sign,Player *enemy) : Player(name,sign), enemy(enemy) {}

    int minmax(Board board,bool turn){
        if(board.getWinner() == this->sign){
            return 1;
        }
        else if(board.getWinner() == this->enemy->getSign()){
            return -1;
        }
        else if(board.checkIfBoardFull()){
            return 0;
        }

        if(turn){
            int bestScore = -1e9;
            for(int i = 0; i < board.size; i++){
                for(int j = 0; j < board.size; j++){
                    if(board.checkIfCellEmpty(i,j)){
                        board.placeMove(i,j,this->sign);
                        int score = minmax(board,false);
                        board.placeMove(i,j,'.'); // backtrack
                        bestScore = max(bestScore,score);
                    }
                }
            }
            return bestScore;
        }
        else {
            int bestScore = 1e9;
            for(int i = 0; i < board.size; i++){
                for(int j = 0; j < board.size; j++){
                    if(board.checkIfCellEmpty(i,j)){
                        board.placeMove(i,j,this->enemy->getSign());
                        int score = minmax(board,true);
                        board.placeMove(i,j,'.'); // backtrack
                        bestScore = min(bestScore,score);
                    }
                }
            }
            return bestScore;
        }
    }

    bool makeMove(Board* board){
        int bestRow, bestCol;
        int bestScore = -1e9;
        for(int i = 0; i < board->size; i++){
            for(int j = 0; j < board->size; j++){
                if(board->checkIfCellEmpty(i,j)){
                    board->placeMove(i,j,this->sign);
                    int score = minmax(*board,false);
                    board->placeMove(i,j,'.'); // backtrack
                    if(score > bestScore){
                        bestScore = score;
                        bestRow = i;
                        bestCol = j;
                    }
                }
            }
        }
        board->placeMove(bestRow,bestCol,this->sign);
        cout << "SmartBot-" << this->name << " has placed " << this->sign << " at (" << bestRow << "," << bestCol << ")" << endl;
        return true;
    }
};

class Human : public Player {
public:
    Human(string name,char sign) : Player(name,sign) {}
    bool makeMove(Board* board){
        int row,col;
        cout << "Player-" << this->name << " has to make the move" << endl;
        cout << "Please enter the valid row & col to make ur move" << endl;
        cin>>row>>col;

        if(row >= 0 && row < board->size && col >= 0 && col < board->size){
            board->placeMove(row,col,this->sign);
            cout << "Player-" << this->name << " has placed " << this->sign << " at (" << row << "," << col << ")" << endl;
            return true;
        }
        else {
            cout << "[Error] Invalid Move : Out of bounds" << endl;
            return false;
        }
    }
};

class Game {
private:
    Board *board;
    vector<Player*> players;
public:
    Game(Board *board) : board(board) {}
    void addPlayer(Player *player){
        players.push_back(player);
    }
    void conductGame(){
        int turn = 0;
        while(true){
            board->printBoard();
            Player* currentPlayer = players[turn % players.size()];
            bool validMove = currentPlayer->makeMove(board);
            if(!validMove) continue;

            char winner = board->getWinner();
            if(winner != '.'){
                board->printBoard();
                cout << "Player-" << currentPlayer->getName() << " has won the game!" << endl;
                break;
            }

            turn++;
            if(turn == board->size * board->size){
                board->printBoard();
                cout << "Game Draw!" << endl;
                break;
            }
        }
    }
};

int main(){
    Board *board = new Board(3);
    Player* vijesh = new Human("Vijesh",'X');
    Player* jivesh = new SmartBot("Jivesh",'O',vijesh);

    Game* game = new Game(board);
    game->addPlayer(vijesh);
    game->addPlayer(jivesh);

    game->conductGame();
}