#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>

#define EMPTY_CELL " "
#define FILLED_CELL "█"

int gridWidth, gridHeight;

typedef enum
{
  CellAlive,
  CellDead
} CellState;
CellState **currentGrid, **nextGrid;

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

int CountAliveNeighbors(int row, int col)
{
  int aliveNeighbors = 0;
  for (int dy = -1; dy <= 1; ++dy)
  {
    for (int dx = -1; dx <= 1; ++dx)
    {
      if (dy == 0 && dx == 0)
        continue;
      int newY = row + dy, newX = col + dx;
      if (newY >= 0 && newY < gridHeight && newX >= 0 && newX < gridWidth && currentGrid[newY][newX] == CellAlive)
      {
        aliveNeighbors++;
      }
    }
  }
  return aliveNeighbors;
}

void AllocateMemoryForGrids()
{
  currentGrid = malloc(gridHeight * sizeof(CellState *));
  nextGrid = malloc(gridHeight * sizeof(CellState *));
  for (int i = 0; i < gridHeight; ++i)
  {
    currentGrid[i] = malloc(gridWidth * sizeof(CellState));
    nextGrid[i] = malloc(gridWidth * sizeof(CellState));
  }
}

void FreeMemoryForGrids()
{
  for (int i = 0; i < gridHeight; ++i)
  {
    free(currentGrid[i]);
    free(nextGrid[i]);
  }
  free(currentGrid);
  free(nextGrid);
}

void UpdateGrid()
{
  for (int i = 0; i < gridHeight; ++i)
  {
    for (int j = 0; j < gridWidth; ++j)
    {
      int aliveNeighbors = CountAliveNeighbors(i, j);
      if (currentGrid[i][j] == CellAlive)
      {
        nextGrid[i][j] = (aliveNeighbors == 2 || aliveNeighbors == 3) ? CellAlive : CellDead;
      }
      else
      {
        nextGrid[i][j] = (aliveNeighbors == 3) ? CellAlive : CellDead;
      }
    }
  }
  CellState **temp = currentGrid;
  currentGrid = nextGrid;
  nextGrid = temp;
}

void GetTerminalSize()
{
  struct winsize ws;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
  gridWidth = ws.ws_col / 2;
  gridHeight = ws.ws_row;
}

int main()
{
  GetTerminalSize();
  AllocateMemoryForGrids();
  InitializeGrid();
  HideTerminalCursor();
  atexit(FreeMemoryForGrids);
  atexit(ShowTerminalCursor);

  while (1)
  {
    DisplayGrid();
    usleep(100 * 1000);
    UpdateGrid();
  }

  return 0;
}