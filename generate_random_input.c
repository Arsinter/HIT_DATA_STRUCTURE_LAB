#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file = fopen("C:\\Users\\Fanful\\DSlab4Rank\\input.txt", "w");
    if (file == NULL) {
        printf("fAILED\n");
        return 1;
    }

    int i;
    for (i = 0; i < 50000; i++) {
        int num = rand() % 1000;  // 生成一个随机整数，范围在0到999之间
        fprintf(file, "%d ", num);
    }

    fclose(file);
    printf("success\n");
    return 0;
}
