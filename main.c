#include <stdio.h>
#include <stdlib.h>

#define EMPTY_CELL " "
#define FILLED_CELL "█"

int gridWidth, gridHeight;

typedef enum
{
  CellAlive,
  CellDead
} CellState;
CellState **currentGrid, **nextGrid;

int main()
{
  return 0;
}

void ClearTerminalScreen()
{
  printf("\033[2J");
}

void HideTerminalCursor()
{
  printf("\033[?25l");
}

void ShowTerminalCursor()
{
  printf("\033[?25h");
}

void ResetCursorPosition()
{
  printf("\033[H");
}
