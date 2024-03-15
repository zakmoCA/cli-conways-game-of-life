#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>

#define EMPTY_CELL " "
#define FILLED_CELL "█"

typedef enum
{
  CELL_DEAD,
  CELL_ALIVE
} CellState;

int gridWidth, gridHeight;
CellState **currentGrid, **nextGrid;

void clearTerminalScreen()
{
  printf("\033[2J");
}

void hideTerminalCursor()
{
  printf("\033[?25l");
}

void showTerminalCursor()
{
  printf("\033[?25h");
}

void resetCursorPosition()
{
  printf("\033[H");
}

void initializeGrid()
{
  for (int i = 0; i < gridHeight; ++i)
  {
    for (int j = 0; j < gridWidth; ++j)
    {
      currentGrid[i][j] = (rand() % 2) ? CELL_ALIVE : CELL_DEAD;
    }
  }
}

void displayGrid()
{
  resetCursorPosition();
  clearTerminalScreen();
  for (int i = 0; i < gridHeight; ++i)
  {
    for (int j = 0; j < gridWidth; ++j)
    {
      printf(currentGrid[i][j] == CELL_ALIVE ? FILLED_CELL FILLED_CELL : EMPTY_CELL);
    }
    printf("\n");
  }
  fflush(stdout);
}

int countAliveNeighbors(int row, int col)
{
  int aliveNeighbors = 0;
  for (int dy = -1; dy <= 1; ++dy)
  {
    for (int dx = -1; dx <= 1; ++dx)
    {
      if (dy == 0 && dx == 0)
        continue;
      int newY = row + dy, newX = col + dx;
      if (newY >= 0 && newY < gridHeight && newX >= 0 && newX < gridWidth &&
          currentGrid[newY][newX] == CELL_ALIVE)
      {
        aliveNeighbors++;
      }
    }
  }
  return aliveNeighbors;
}

void updateGrid()
{
  for (int i = 0; i < gridHeight; ++i)
  {
    for (int j = 0; j < gridWidth; ++j)
    {
      int aliveNeighbors = countAliveNeighbors(i, j);
      if (currentGrid[i][j] == CELL_ALIVE)
      {
        nextGrid[i][j] = (aliveNeighbors == 2 || aliveNeighbors == 3) ? CELL_ALIVE : CELL_DEAD;
      }
      else
      {
        nextGrid[i][j] = (aliveNeighbors == 3) ? CELL_ALIVE : CELL_DEAD;
      }
    }
  }
  CellState **temp = currentGrid;
  currentGrid = nextGrid;
  nextGrid = temp;
}

void getTerminalSize()
{
  struct winsize ws;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
  gridWidth = ws.ws_col / 2;
  gridHeight = ws.ws_row;
}

void allocateMemoryForGrids()
{
  currentGrid = malloc(gridHeight * sizeof(CellState *));
  nextGrid = malloc(gridHeight * sizeof(CellState *));
  for (int i = 0; i < gridHeight; ++i)
  {
    currentGrid[i] = malloc(gridWidth * sizeof(CellState));
    nextGrid[i] = malloc(gridWidth * sizeof(CellState));
  }
}

void freeMemoryForGrids()
{
  for (int i = 0; i < gridHeight; ++i)
  {
    free(currentGrid[i]);
    free(nextGrid[i]);
  }
  free(currentGrid);
  free(nextGrid);
}

int main()
{
  getTerminalSize();
  allocateMemoryForGrids();
  initializeGrid();
  hideTerminalCursor();
  atexit(freeMemoryForGrids);
  atexit(showTerminalCursor);

  while (1)
  {
    displayGrid();
    usleep(100 * 1000);
    updateGrid();
  }

  return 0;
}