#include <cstring>
#include <iomanip>
#include <iostream>

const int kRows = 5;
const int kCols = 10;

void Clear(char board[kRows][kCols]) {
  for (int y = 0; y < kRows; ++y) {
    for (int x = 0; x < kCols; ++x) {
      board[y][x] = ' ';
    }
  }
}

bool Inside(int x, int y) { return x >= 0 && x < kCols && y >= 0 && y < kRows; }

bool Put(char board[kRows][kCols], int x, int y, char c) {
  if (!Inside(x, y)) {
    return false;
  }
  board[y][x] = c;
  return true;
}

bool Text(char board[kRows][kCols], int x, int y, const char* word) {
  int len = strlen(word);
  // если влезают первая и последняя буквы, то влезает всё слово
  if (len == 0 || !Inside(x, y) || !Inside(x + len - 1, y)) {
    return false;
  }
  for (int i = 0; i < len; ++i) {
    Put(board, x + i, y, word[i]);
  }
  return true;
}

void Print(const char board[kRows][kCols]) {
  for (int y = 0; y < kRows; ++y) {
    for (int x = 0; x < kCols; ++x) {
      std::cout << board[y][x];
    }
    std::cout << "\n";
  }
}

int main() {
  char board[kRows][kCols];
  Clear(board);

  char cmd[32];
  while (std::cin >> std::setw(32) >> cmd) {
    if (strcmp(cmd, "quit") == 0) {
      break;
    }

    if (strcmp(cmd, "clear") == 0) {
      Clear(board);
    } else if (strcmp(cmd, "print") == 0) {
      Print(board);
    } else if (strcmp(cmd, "put") == 0) {
      int x, y;
      char c;
      std::cin >> x >> y >> c;
      if (!Put(board, x, y, c)) {
        std::cout << "error\n";
      }
    } else if (strcmp(cmd, "text") == 0) {
      int x, y;
      char word[32];
      std::cin >> x >> y >> std::setw(32) >> word;
      if (!Text(board, x, y, word)) {
        std::cout << "error\n";
      }
    } else {
      std::cout << "error\n";
    }
  }
  return 0;
}
