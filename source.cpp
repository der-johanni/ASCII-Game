#include <iostream>
#include <string>

const int VALLEY_HEIGHT = 24;
const int VALLEY_LENGTH = 50;

const char MOVE_UP    = 'w';
const char MOVE_DOWN  = 's';
const char MOVE_LEFT  = 'a';
const char MOVE_RIGHT = 'd';
const char game_exit  = 'e';
const char game_reset = 'r';

std::string readMap();

class Player{
private:
  int X_POS;
  int Y_POS;
  char PREVIOUS_MOVE;
public:
  Player(){
      X_POS = 0;
      Y_POS = 0;
    }
  Player(int x, int y){
    X_POS = x;
    Y_POS = y;
  }

  int get_x_pos(){return X_POS;}
  int get_y_pos(){return Y_POS;}
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
  Space(){X = 0; Y = 0; dot = '#';}
  int get_x() {return X;}
  int get_y() {return Y;}
  void set(int x, int y, char wall){X = x; Y = y; dot = (wall == '#') ? '#' : ' ';}
  int isWall(){return dot == '#' ? 1 : 0;}
  char isWall_char() {return dot == '#' ? '#' : ' ';}
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
      std::cout << "File problem: check the file you input and size of the map: 24x50" << std::endl;
      exit(1);
    }
    for (int Y = 0; Y < VALLEY_HEIGHT; Y++){
      for (int X = 0; X < VALLEY_LENGTH; X++){
        dots[VALLEY_LENGTH * Y + X].set(X, Y, GAME_MAP[VALLEY_LENGTH * Y + X]);
      }
    }
  }
  void update(char userMove){
    while (!dots[VALLEY_LENGTH * player.get_y_pos() + player.get_x_pos()].isWall()){
      switch (userMove){
        case MOVE_UP: player.move_up(); break;
        case MOVE_LEFT: player.move_left(); break;
        case MOVE_DOWN: player.move_down(); break;
        case MOVE_RIGHT: player.move_right(); break;
        default: break;
      }
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
            case 2: std::cout << " x: " << player.get_x_pos() << ", y: " << player.get_y_pos(); break;
            case 5: std::cout << " Control: "; break;
            case 6: std::cout << " w - move up."; break;
            case 7: std::cout << " a - move left."; break;
            case 8: std::cout << " s - move down."; break;
            case 9: std::cout << " d - move right."; break;
            default: std::cout << ' '; break;
          }
          continue;
        }
        if (player.get_x_pos() == X && player.get_y_pos() == Y) std::cout << '@';
        else std::cout << dots[VALLEY_LENGTH * Y + X].isWall_char();
      }
      
    }
    std::cout << " Your move: ";
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