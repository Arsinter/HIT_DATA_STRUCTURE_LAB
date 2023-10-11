#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//定义哈夫曼树的节点结构
typedef struct HuffmanNode {
	unsigned char value; //字符的ASCII值
	int frequency; //字符的频率
	struct HuffmanNode* lchild;
	struct HuffmanNode* rchild;
} HuffmanNode;

//定义哈夫曼编码表的节点结构
typedef struct HuffmanCode {
	unsigned char value; //字符的ASCII值
	char code[256]; //字符的哈夫曼编码
} HuffmanCode;

//文件中读取文本，统计字符频率
void Read(const char* filename, int* frequency)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("文件打开失败");
		exit(1);
	}

	unsigned char c;
	while (1) 
	{
		c = fgetc(file); //先逐个字符读入
		if (feof(file)) //光标后面没有字符就停止读取
			            //（只用fgetc判断会死循环，一直读ASCII中255的“ ”，只用feof会读到n+1位，这样写EOF被fgetc读取，feof判断后直接break，读到第n位停止）
			break;
		printf("%c", c);
		frequency[c]++;
	}

	fclose(file);
}

//创建哈夫曼树节点
HuffmanNode* newNode(unsigned char value, int frequency)
{
	HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
	node->value = value;
	node->frequency = frequency;
	node->lchild = NULL;
	node->rchild = NULL;

	return node;
}

//选频率最低的两个节点
void lowest(HuffmanNode** nodes, int size, int* min1, int* min2)
{
	*min1 = -1;
	*min2 = -1;
	for (int i = 0; i < size; i++)
	{
		if (nodes[i] != NULL)
		{
			if (*min1 == -1 || nodes[i]->frequency < nodes[*min1]->frequency)
			{
				*min2 = *min1; //确保左节点小于右节点
				*min1 = i;
			}
			else if (*min2 = -1 || nodes[i]->frequency < nodes[*min2]->frequency)
			{
				*min2 = i;
			}
		}
	}
}

//构建哈夫曼树
HuffmanNode* buildTree(int* frequency)
{
	HuffmanNode** nodes = (HuffmanNode**)malloc(256 * sizeof(HuffmanNode*));
	for (int i = 0; i < 256; i++)
	{
		if (frequency[i] > 0)
		{
			nodes[i] = newNode((unsigned char)i, frequency[i]);
		}
		else
		{
			nodes[i] = NULL; //未出现的字符设为空指针
		}
	}

	int min1, min2; //两个最小频率字符的下标

	while (1)
	{
		lowest(nodes, 256, &min1, &min2);
		if (min2 == -1) //说明已经构建好了
		{
			break;
		}

		HuffmanNode* newnode = newNode(0, nodes[min1]->frequency + nodes[min2]->frequency);
		newnode->lchild = nodes[min1];
		newnode->rchild = nodes[min2];

		nodes[min1] = newnode; //将父节点设为一个新的左节点
		nodes[min2] = NULL;
	}

	HuffmanNode* root = nodes[min1]; //把构建好的树给root
	free(nodes);
	return root;
}

//构建哈夫曼编码表 递归
void HuffmanCodeTable(HuffmanNode* node, HuffmanCode* table, char* code, int depth)
{
	if (node == NULL)
		return;

	if (node->lchild == NULL && node->rchild == NULL) //到叶子停止
	{
		table[node->value].value = node->value;
		strcpy(table[node->value].code, code);
		return;
	}

	code[depth] = '0';
	code[depth + 1] = '\0';
	HuffmanCodeTable(node->lchild, table, code, depth + 1);

	code[depth] = '1';
	code[depth + 1] = '\0';
	HuffmanCodeTable(node->rchild, table, code, depth + 1);

}

//编码+压缩
void encodeText(const char* filename, HuffmanCode* table) //用const char表明指针指向的文件不应被修改
{
	FILE* inputFile = fopen(filename, "r"); //打开文本文件
	FILE* outputFile = fopen("compressed.bin", "wb"); //二进制模式写入输出文件
	if (inputFile == NULL || outputFile == NULL)
	{
		printf("文件打开失败");
		exit(1);
	}

	unsigned char c, buffer = 0; //buffer作为缓冲区，共八位
	int count = 0; //计数器

	while (!feof(inputFile))
	{
		c = fgetc(inputFile);
		for (int i = 0; i < strlen(table[c].code); i++)
		{
			if (table[c].code[i] == '1')
			{
				buffer |= (1 << (7 - count)); //从高位到低位写入
			}
			count++;

			if (count == 8)
			{
				fwrite(&buffer, sizeof(unsigned char), 1, outputFile);
				buffer = 0; //重置缓冲区和计数器
				count = 0;
			}
		}
	}

	if (count > 0)
	{
		fwrite(&buffer, sizeof(unsigned char), 1, outputFile); //最后不满八位
	}

	fclose(inputFile);
	fclose(outputFile);
}

//计算压缩率
float compressionRate(const char* inputFilename, const char* outputFilename)
{
	FILE* inputFile = fopen(inputFilename, "r");
	FILE* outputFile = fopen(outputFilename, "r");
	if (inputFile == NULL || outputFile == NULL) {
		printf("文件打开失败\n");
		exit(1);
	}

	fseek(inputFile, 0, SEEK_END); //将字符指针移到最后一位
	long inputFileSize = ftell(inputFile); //计算长度，即字符总数
	fseek(outputFile, 0, SEEK_END);
	long outputFileSize = ftell(outputFile);

	fclose(inputFile);
	fclose(outputFile);

	float rate = ((float)outputFileSize / (float)inputFileSize) * 100;
	return rate;
}

//解码为文本文件+与原文件比较
void decodeFile(const char* filename, HuffmanNode* root, const char* outputFilename)
{
	FILE* inputFile = fopen(filename, "rb");
	FILE* outputFile = fopen(outputFilename, "w");
	if (inputFile == NULL || outputFile == NULL) {
		printf("文件打开失败\n");
		exit(1);
	}

	HuffmanNode* currentNode = root;
	unsigned char buffer;

	while (fread(&buffer, sizeof(unsigned char), 1, inputFile) > 0)
	{
		for (int i = 7; i >= 0; i--)
		{
			if (buffer & (1 << i)) //若buffer为1则右节点，0则左节点
			{
				currentNode = currentNode->rchild;
			}
			else
			{
				currentNode = currentNode->lchild;
			}

			if (currentNode->lchild == NULL && currentNode->rchild == NULL)
			{
				fputc(currentNode->value, outputFile);
				currentNode = root; //重置为根节点
			}
		}
	}

	fclose(inputFile);
	fclose(outputFile);
}



HuffmanCode table[256]; //用全局变量存放哈夫曼编码表，太大了，会爆栈

int main()
{
	int frequency[256] = { 0 }; //存放字符频率
	Read("input.txt", frequency);

	HuffmanNode* root = buildTree(frequency); //构建哈夫曼树

	char code[256] = { '\0' }; //存放哈夫曼编码表
	HuffmanCodeTable(root, table, code, 0);


	for (int i = 0; i < 256; i++) // 输出字符的哈夫曼编码
	{
		if (table[i].value != 0) {
			printf("%c: %s\n", table[i].value, table[i].code);
		}
	}


	encodeText("input.txt", table); // 压缩文件

	float rate = compressionRate("input.txt", "compressed.bin"); // 计算压缩率

	printf("压缩率: %.2f%%\n", rate);


	decodeFile("compressed.bin", root, "output.txt"); // 解码文件并与原文件比较

	return 0;
}