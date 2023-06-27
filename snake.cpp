#include<iostream>
#include<conio.h>
#include<windows.h>
#include<iostream>
#include<conio.h>
#include<windows.h>
#include<ctime>
#include<stack>

//powered by Viktoriia Dziadukh 2023
//thanks
w

using namespace std;
struct Position {
    int x;
    int y;
};

bool GameOver;
const int height = 20;
const int width = 50;

int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100], nTail;

enum eDirection { STOP = 0, UP, DOWN, LEFT, RIGHT };
eDirection dir;
eDirection prevDir = STOP;

void AutoPlay() {
    int prevX = x;
    int prevY = y;

    stack<Position> path;
    bool visited[height][width] = { false };

    // Tworzenie poczatkowej pozycji
    Position startPos;
    startPos.x = x;
    startPos.y = y;

    // Tworzenie pozycji docelowej
    Position targetPos;
    targetPos.x = fruitX;
    targetPos.y = fruitY;

    path.push(startPos);

    if ((dir == LEFT && prevDir != RIGHT) ||
    (dir == RIGHT && prevDir != LEFT) ||
    (dir == UP && prevDir != DOWN) ||
    (dir == DOWN && prevDir != UP)) {
    prevDir = dir;
} else {
    dir = prevDir;
}

    while (!path.empty()) {
        Position currentPos = path.top();
        path.pop();

        // Sprawdz, czy aktualna pozycja jest owocem
        if (currentPos.x == targetPos.x && currentPos.y == targetPos.y) {
            // Znaleziono droge do owocu, zaktualizuj kierunek
            if (currentPos.x < x && dir != RIGHT)
                dir = LEFT;
            else if (currentPos.x > x && dir != LEFT)
                dir = RIGHT;
            else if (currentPos.y < y && dir != DOWN)
                dir = UP;
            else if (currentPos.y > y && dir != UP)
                dir = DOWN;

            break;
        }

        // Sprawdz sasiednie pozycje i dodaj je do sciezki
        if (currentPos.x > 0 && !visited[currentPos.y][currentPos.x - 1]) {
            Position leftPos;
            leftPos.x = currentPos.x - 1;
            leftPos.y = currentPos.y;
            path.push(leftPos);
            visited[currentPos.y][currentPos.x - 1] = true;
        }
        if (currentPos.x < width - 1 && !visited[currentPos.y][currentPos.x + 1]) {
            Position rightPos;
            rightPos.x = currentPos.x + 1;
            rightPos.y = currentPos.y;
            path.push(rightPos);
            visited[currentPos.y][currentPos.x + 1] = true;
        }
        if (currentPos.y > 0 && !visited[currentPos.y - 1][currentPos.x]) {
            Position upPos;
            upPos.x = currentPos.x;
            upPos.y = currentPos.y - 1;
            path.push(upPos);
            visited[currentPos.y - 1][currentPos.x] = true;
        }
        if (currentPos.y < height - 1 && !visited[currentPos.y + 1][currentPos.x]) {
            Position downPos;
            downPos.x = currentPos.x;
            downPos.y = currentPos.y + 1;
            path.push(downPos);
            visited[currentPos.y + 1][currentPos.x] = true;
        }
    }

    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
    }

    if (x >= width) x = 0;
    else if (x < 0) x = width - 1;
    if (y >= height) y = 0;
    else if (y < 0) y = height - 1;

    if (prevX != x || prevY != y) {
        for (int i = 0; i < nTail; i++) {
            int tempX = tailX[i];
            int tempY = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = tempX;
            prevY = tempY;
        }
    }
}

void Setup() {
    dir = STOP;
    GameOver = false;
    x = width / 2;
    y = height / 2;
    srand(static_cast<unsigned>(time(0)));
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
}

void Draw() {
    system("CLS");

    for (int i = 0; i < width; i++) {
        cout << "#";
    }
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0 || j == width - 1) {
                cout << "#";
            } else if (i == y && j == x) {
                cout << "o";
            } else if (i == fruitY && j == fruitX) {
                cout << "F";
            } else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (i == tailY[k] && j == tailX[k]) {
                        cout << "o";
                        print = true;
                    }
                }
                if (!print) {
                    cout << " ";
                }
            }
        }
        cout << endl;
    }

    for (int i = 0; i < width; i++) {
        cout << "#";
    }
    cout << endl;

    cout << "Score: " << score << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'w':
                dir = UP;
                break;
            case 'a':
                dir = LEFT;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'd':
                dir = RIGHT;
                break;
            default:
                break;
        }
    }
}

void Logic() {
       int prevX = tailX[0];
    int prevY = tailY[0];
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        int prev2X = tailX[i];
        int prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        default:
            break;
    }

    if (x < 0 || x >= width || y < 0 || y >= height) {
        GameOver = true;
    }

    for (int i = 0; i < nTail; i++) {
        if (x == tailX[i] && y == tailY[i]) {
            GameOver = true;
        }
    }

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main() {
    Setup();

    while (!GameOver) {
        Draw();
        Input();
        Logic();

        Sleep(60);
    }
    return 0;
}
