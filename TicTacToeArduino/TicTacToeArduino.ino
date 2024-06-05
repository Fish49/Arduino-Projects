/*
Tic-Tac-Toe Arduino v1.0
PaiShoFish49

This project is for an arduino uno connected to a joystick and lcd display to play tic tac toe (on a long car ride for example)
the project is on https://github.com/Fish49/Arduino-Projects for more information about wiring, the code or for other arduino projects.

*/

#include <LCD_I2C.h>

// Initialize the library with the numbers of the interface pins
LCD_I2C lcd(0x27, 16, 2);

// Joystick pins
const int VRx = A0;
const int VRy = A1;
const int SW = 7;

// Game variables
char board[3][3];
int playerX = 0;
int playerO = 0;
int cursorX = 0;
int cursorY = 0;
int viewOffset = 0;
bool playerTurn = true; // true for player 'X', false for player 'O'
bool change = true;

void setup() {
  lcd.begin();
  lcd.backlight();
  pinMode(SW, INPUT_PULLUP);
  resetGame();
}

void loop() {
  handleJoystick();
  if (change) {
    drawBoard();
    change = false;
  }
}

void resetGame() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      board[i][j] = ' ';
    }
  }
  cursorX = 0;
  cursorY = 0;
  viewOffset = 0;
  playerTurn = true;
  lcd.clear();
  lcd.print("Tic-Tac-Toe!");
  delay(1000);
}

void handleJoystick() {
  int xValue = analogRead(VRx);
  int yValue = analogRead(VRy);
  int buttonState = digitalRead(SW);

  if (xValue < 400) { // Joystick left
    change = true;
    cursorX = (cursorX + 3) % 4;
    delay(200); // Debounce
  } else if (xValue > 600) { // Joystick right
    change = true;
    cursorX = (cursorX + 1) % 4;
    delay(400); // Debounce
  }

  if (yValue < 400) { // Joystick up
    change = true;
    cursorY = (cursorY + 2) % 3;
    delay(200); // Debounce
  } else if (yValue > 600) { // Joystick down
    change = true;
    cursorY = (cursorY + 1) % 3;
    delay(400); // Debounce
  }

  if (cursorY == 0){
    viewOffset = 0;
  } else if (cursorY == 2) {
    viewOffset = 1;
  }

  if (buttonState == LOW) { // Button pressed
    change = true;
    if (cursorX == 3) {
      resetGame();

    } else if (board[cursorY][cursorX] == ' ') {
      board[cursorY][cursorX] = playerTurn ? 'X' : 'O';
      playerTurn = !playerTurn;

      if (checkWin()) {
        lcd.clear();
        lcd.print(board[cursorY][cursorX]);
        lcd.print(" wins!");
        if (playerTurn) {
          playerO ++;
        } else {
          playerX ++;
        }
        delay(1000);
        resetGame();
      } else if (isBoardFull()) {
        lcd.clear();
        lcd.print("Draw!");
        delay(1000);
        resetGame();
      }
    }
    delay(400); // Debounce
  }
}

void drawBoard() {
  lcd.clear();
  for (int i = 0; i < 2; i++) {
    lcd.setCursor(0, i);
    if (i == viewOffset) {
      lcd.print('#');
    }
    for (int j = 0; j < 3; j++) {
      lcd.setCursor((j * 3)+1, i);
      char cell = board[viewOffset + i][j];
      if (viewOffset + i == cursorY && j == cursorX) {
        lcd.print('[');
        lcd.print(cell);
        lcd.print(']');
      } else {
        lcd.print(' ');
        lcd.print(cell);
        lcd.print(' ');
      }
    }
    if (i == 0) {
      lcd.setCursor(10, i);
      if (cursorX == 3) {
        lcd.print('[');
        lcd.print('R');
        lcd.print(']');
      } else {
        lcd.print(' ');
        lcd.print('R');
        lcd.print(' ');
      }
    } else {
      lcd.print(playerX);
      lcd.print('-');
      lcd.print(playerO);
    }
  }
}

bool checkWin() {
  // Check rows and columns
  for (int i = 0; i < 3; i++) {
    if ((board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) ||
        (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i])) {
      return true;
    }
  }
  // Check diagonals
  if ((board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) ||
      (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])) {
    return true;
  }
  return false;
}

bool isBoardFull() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == ' ') {
        return false;
      }
    }
  }
  return true;
}
