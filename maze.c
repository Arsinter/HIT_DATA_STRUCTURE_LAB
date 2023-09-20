#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define TRUE 1
#define FALSE 0
#define numRow 10
#define numCol 10
#define DST_ROW 5
#define DST_COL 6
#define stack_size 100

//记录迷宫中的位置
typedef struct {
    int row;
    int col;
    int dir_vector;
} element;

//记录迷宫中的偏移
typedef struct {
    short int vert; //竖直
    short int horiz;//水平
} offsets;

void setup_maze();                     //建立迷宫+外圈建墙
void set_move();                       //移动，共八个方向
void path();                           //找路
int init_dir_vector(int row, int col); //初始化方向
int get_next_dir(int dir_vector);      //下一步怎么走
void push(element position);           //入栈
element pop();                         //出栈
void findPath(int row, int col);       //递归找路

int top = -1;
offsets move[8];
short int maze[numRow + 2][numCol + 2];
element stack[stack_size];
int flag;

int main() {
    setup_maze();
    set_move();
    path();
    //findPath(1,1); //此处为了避免两种方法的冲突而注释了此行
    return 0;
}

void setup_maze() {
    int i, j;
    short int maze0[numRow][numCol] = {
            { 0,0,1,0,1,1,1,0,1,0 },
            { 1,0,0,1,1,1,0,1,0,1 },
            { 1,1,0,1,1,0,1,0,1,1 },
            { 0,0,1,0,1,1,1,0,0,0 },
            { 0,1,1,0,1,0,1,0,1,0 },
            { 1,0,1,1,1,1,0,0,1,0 },
            { 1,1,0,1,0,1,0,0,1,0 },
            { 1,0,0,0,1,0,1,0,0,0 },
            { 0,1,0,1,1,1,0,1,1,0 },
            { 1,0,0,1,1,1,0,0,0,0 }
    };

    if (DST_ROW == 1 && DST_COL == 1) {
        printf("\nThe coordinate of destination should be different from that of entrance (1,1).");
        exit(0);
    }

    if (maze0[DST_ROW - 1][DST_COL - 1] == 1) {
        printf("\nmaze[DST_ROW][DST_COL] should be 0.");
        exit(0);
    }

    for (i = 0; i < numCol + 2; i++) maze[0][i] = 1;
    for (i = 0; i < numCol + 2; i++) maze[numRow + 1][i] = 1;
    for (i = 0; i < numRow + 2; i++) maze[i][0] = 1;
    for (i = 0; i < numRow + 2; i++) maze[i][numCol + 1] = 1;
    for (i = 1; i <= numRow; i++)
        for (j = 1; j <= numCol; j++) maze[i][j] = maze0[i - 1][j - 1];
    for(int k = 0;k < numCol + 2;k++){
        printf("\n");
        for (int l = 0; l < numRow + 2; ++l) {
            printf("%d ", maze[k][l]);

        }
    }
}

void set_move() {
    move[0].vert = -1; move[0].horiz = 0;
    move[1].vert = -1; move[1].horiz = 1;
    move[2].vert = 0; move[2].horiz = 1;
    move[3].vert = 1; move[3].horiz = 1;
    move[4].vert = 1; move[4].horiz = 0;
    move[5].vert = 1; move[5].horiz = -1;
    move[6].vert = 0; move[6].horiz = -1;
    move[7].vert = -1; move[7].horiz = -1;
}

void push(element position) {
    stack[++top] = position;
}

element pop() {
    element position;

    position = stack[top--];
    return position;
}

void path() {
    int i, row, col, nextRow, nextCol, dir, dir_vector, found = FALSE;
    element position;

    row = 1; col = 1;
    maze[1][1] = -1;
    dir_vector = init_dir_vector(1, 1);
    if (dir_vector == -1) found = TRUE;
    else {
        top = 0;
        stack[0].row = 1; stack[0].col = 1; stack[0].dir_vector = dir_vector;
    }
    while (top > -1 && !found) {
        position = pop();

        row = position.row;
        col = position.col;
        dir_vector = position.dir_vector;
        dir = get_next_dir(init_dir_vector(row,col));

        while (dir < 8 && !found) {
            nextRow = row + move[dir].vert;
            nextCol = col + move[dir].horiz;

            if (nextRow == DST_ROW && nextCol == DST_COL)
            {
                found = TRUE;
            }
            else {
                maze[nextRow][nextCol] = -1;
                position.row = row; position.col = col;
                position.dir_vector = get_next_dir(init_dir_vector(row, col));
                push(position);
                row = nextRow; col = nextCol;
                dir_vector = init_dir_vector(row, col);
                dir = get_next_dir(dir_vector);
            }
        }
    }
    if (found) {
        printf("The path is:\n");
        printf("row  col\n");
        for (i = 0; i <= top; i++) printf("%2d%5d\n", stack[i].row, stack[i].col);
        printf("%2d%5d\n", row, col);
        printf("%2d%5d\n", DST_ROW, DST_COL);
    }
    else printf("The maze does not have a path\n");
}

int init_dir_vector(int row, int col) {

    int dir, nextRow, nextCol, dir_vector;

    dir_vector = ((int)pow(2.0, 8.0)) - 1;
    for (dir = 0; dir < 8; dir++) {
        nextRow = row + move[dir].vert;
        nextCol = col + move[dir].horiz;
        if (nextRow == DST_ROW && nextCol == DST_COL) return -1; // to report that a path is found
        if (!maze[nextRow][nextCol]) dir_vector -= (int)pow(2.0, (double)dir);
    }
    return dir_vector;
}

int get_next_dir(int dir_vector) {

    int dir, d[8], count, k;

    for (dir = 0; dir < 8; dir++) {
        d[dir] = dir_vector % 2;
        dir_vector /= 2;
    }


    count = 0;
    for (dir = 0; dir < 8; dir++) if (d[dir] == 0) count++;

    if (count == 0) return 8;


    k = rand() % count + 1;
    for (dir = 0; dir < 8; dir++) {
        if (d[dir] == 0) k--;
        if (k == 0) return dir;
    }
}

int st[1001][0];
int ptr = 0;
void findPath(int row, int col) {

    if (row == DST_ROW && col == DST_COL) {
        printf("(%d, %d)\n", row, col);
        for(int k = 0;k < numCol + 2;k++){
            printf("\n");
            for (int l = 0; l < numRow + 2; ++l) {
                printf("%d ", maze[k][l]);

            }
        }
        flag = 1;
        return;
    }

    for (int i = 0; i < 8; i++) {
        int nextRow = row + move[i].vert;
        int nextCol = col + move[i].horiz;

        if (maze[nextRow][nextCol] == 0) {
            maze[nextRow][nextCol] = -1;
            ptr++;
            st[ptr][0] = nextRow;
            st[ptr][1] = nextCol;
            findPath(nextRow, nextCol);
            if (flag) {
                return;
            }
            ptr--;
            maze[nextRow][nextCol] = 0; // 回溯
        }
    }
}







