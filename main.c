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

void InitializeGrid()
{
  for (int i = 0; i < gridHeight; ++i)
  {
    for (int j = 0; j < gridWidth; ++j)
    {
      currentGrid[i][j] = (rand() % 2) ? CellAlive : CellDead;
    }
  }
}

void DisplayGrid()
{
  ResetCursorPosition();
  ClearTerminalScreen();
  for (int i = 0; i < gridHeight; ++i)
  {
    for (int j = 0; j < gridWidth; ++j)
    {
      printf(currentGrid[i][j] == CellAlive ? FILLED_CELL FILLED_CELL : EMPTY_CELL);
    }
    printf("\n");
  }
  fflush(stdout);
}