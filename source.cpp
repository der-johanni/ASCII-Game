#include <iostream>
#include <string>
#include <unistd.h>

const int VALLEY_HEIGHT = 24;
const int VALLEY_LENGTH = 50;

const char MOVE_UP    = 'w';
const char MOVE_DOWN  = 's';
const char MOVE_LEFT  = 'a';
const char MOVE_RIGHT = 'd';
const char game_exit  = 'e';
const char game_reset = 'r';

const char gameWalls  = '#';
const char gameSpace  = ' ';
const char gameCoins  = '$';
const char gamePlayer = '@';

std::string readMap();

class Player{
private:
  int X_POS;
  int Y_POS;
  int COINS;
  char PREVIOUS_MOVE;
public:
  Player(){
      X_POS = 0;
      Y_POS = 0;
      COINS = 0;
    }

  int get_x(){return X_POS;}
  int get_y(){return Y_POS;}
  int get_coins(){return COINS;}
  void coins_up(){COINS++;}
  void set(int x, int y){X_POS = x; Y_POS = y;}

  void move_up(){
      Y_POS--;
      PREVIOUS_MOVE = MOVE_UP;
    }
  void move_down(){
      Y_POS++;
      PREVIOUS_MOVE = MOVE_DOWN;
    }
  void move_left(){
      X_POS--;
      PREVIOUS_MOVE = MOVE_LEFT;
    }
  void move_right(){
      X_POS++;
      PREVIOUS_MOVE = MOVE_RIGHT;
    }
  void move_back(){
    switch (PREVIOUS_MOVE){
      case MOVE_UP: move_down(); break;
      case MOVE_DOWN: move_up(); break;
      case MOVE_LEFT: move_right(); break;
      case MOVE_RIGHT: move_left(); break;
    }
  }
};

class Space{
private:
  char dot;
  int X;
  int Y;
public:
  Space(){X = 0; Y = 0; dot = gameWalls;}
  int get_x() {return X;}
  int get_y() {return Y;}
  int isWall() {return (dot == '#')? 1 : 0;}
  char getChar() {return dot;}
  void set(int x, int y, char wall){X = x; Y = y; dot = wall;}
  
};

class Board{
private:
  Space dots[VALLEY_HEIGHT * VALLEY_LENGTH];
  Player player;
  unsigned int frame_stat;
public:
  Board(){
    player.set(1, 1);
    frame_stat = 1;
    std::string GAME_MAP = readMap();
    if (GAME_MAP.length() != VALLEY_LENGTH * VALLEY_HEIGHT){
      std::cout << "File problem: check the file you input and size of the map: ";
      std::cout << VALLEY_HEIGHT * VALLEY_LENGTH  << ": " << GAME_MAP.length() << std::endl;
      exit(1);
    }
    for (int Y = 0; Y < VALLEY_HEIGHT; Y++){
      for (int X = 0; X < VALLEY_LENGTH; X++){
        dots[VALLEY_LENGTH * Y + X].set(X, Y, GAME_MAP[VALLEY_LENGTH * Y + X]);
      }
    }
  }
  void update(char userMove){
    int dotCoord = VALLEY_LENGTH * player.get_y() + player.get_x();
    while (!dots[dotCoord].isWall()){
      switch (userMove){
        case MOVE_UP: player.move_up(); break;
        case MOVE_LEFT: player.move_left(); break;
        case MOVE_DOWN: player.move_down(); break;
        case MOVE_RIGHT: player.move_right(); break;
        default: break;
      }
      if (dots[dotCoord].getChar() == gameCoins){
        dots[dotCoord].set(player.get_x(), player.get_y(), gameSpace);
        player.coins_up();
      }
      dotCoord = VALLEY_LENGTH * player.get_y() + player.get_x();
    }
    player.move_back();
    this->show();
    frame_stat++;
  }
  void show(){
    for (int Y = 0; Y < VALLEY_HEIGHT; Y++){
      std::cout << std::endl;
      for (int X = 0; X < VALLEY_LENGTH + 1; X++){
        if (X == 50){
          switch (Y){
            case 0: std::cout << " Step: " << frame_stat; break;
            case 2: std::cout << " x: " << player.get_x() << ", y: " << player.get_y(); break;
            case VALLEY_HEIGHT - 9: std::cout << " Control: "; break;
            case VALLEY_HEIGHT - 8: std::cout << " w - move up."; break;
            case VALLEY_HEIGHT - 7: std::cout << " a - move left."; break;
            case VALLEY_HEIGHT - 6: std::cout << " s - move down."; break;
            case VALLEY_HEIGHT - 5: std::cout << " d - move right."; break;
            case VALLEY_HEIGHT - 3: std::cout << " Coins: " << player.get_coins(); break;
            default: std::cout << ' '; break;
          }
          continue;
        }
        if (player.get_x() == X && player.get_y() == Y) std::cout << '@';
        else std::cout << dots[VALLEY_LENGTH * Y + X].getChar();
      }
      
    }
    std::cout << "Your move: ";
  }
  void reset(){
    player.set(1, 1);
    frame_stat = 1;
    std::string GAME_MAP = readMap();
    for (int Y = 0; Y < VALLEY_HEIGHT; Y++){
      for (int X = 0; X < VALLEY_LENGTH; X++){
        dots[VALLEY_LENGTH * Y + X].set(X, Y, GAME_MAP[VALLEY_LENGTH * Y + X]);
      }
    }
    this->show();
  }
};

int main(){
  Board game;
  game.show();
  while(1){
    char userCommand;
    std::cin >> userCommand;
    if (userCommand == game_exit) break;
    if (userCommand == game_reset) {game.reset(); continue;}
    game.update(userCommand);
  }
  std::cout << "You have exit the game. Goodbuy!" << std::endl;
  for (int SCREENS = 1; SCREENS < VALLEY_HEIGHT - 1; SCREENS++) std::cout << std::endl;
  return 0;
}

std::string readMap(){
  std::string MAP;
  FILE* map = fopen("map.txt", "r");
  if (map == NULL) return "0";
  while (!feof(map)){
    char Character = (char)fgetc(map);
    if (Character == '\n') continue;
    MAP += Character;
  }
  MAP.pop_back();
  return MAP;
}