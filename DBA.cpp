#include <fstream>
#include <math.h>

class DynamicBlockArray {
private:
	int bitCount;
	int mask;
	int blockSize;
	int tableSize;
	int size;
	int** blockTable;
public:
	DynamicBlockArray(unsigned blockSize = 256, unsigned tableSize = 1) : blockSize(blockSize), tableSize(tableSize) {
		// The usage of bit shifts and bit mask require the `blockSize` to be a power of two
		if(blockSize <= 0 || (blockSize & (blockSize-1))) {
			printf("`blockSize` must be a power of two!\n");
			return;
		}

		// Create a mask for the bottom bits, this will be the position inside a block
		bitCount = log2(blockSize);
		mask = (1<<(bitCount))-1;
		// Allocate memory
		size = blockSize*tableSize;
		blockTable = new int*[tableSize];
		*blockTable = new int[size];
		for(int i = 1; i<tableSize; i++) {
			*(blockTable + i) = *(blockTable) + i*blockSize;
		}
	}
	int GetSize() {
		return size;
	}
	int GetBlockSize() {
		return blockSize;
	}
	int GetBlockCount() {
		return tableSize;
	}
	int* GetElem(int index) {
		if(index >= size) return nullptr;
		int offset = index&mask;
		int segment = index>>bitCount;
		// return blockTable[segment][offset];
		return *(blockTable + segment) + offset;
	}
	int** GetBlock(int segment) {
		if(segment >= tableSize) return nullptr;
		return blockTable+segment;
	}
	int* GetElemInBlock(int offset, int segment) {
		if(offset >= blockSize) return nullptr;
		if(segment >= tableSize) return nullptr;
		return *(blockTable + segment) + offset;
	}
	int** GetTable() {
		return blockTable;
	}
	void TestFill() {
		for(int i = 0; i<tableSize; i++) {
			for(int j = 0; j<blockSize; j++) {
				blockTable[i][j] = i*blockSize + j;
			}
		}
	}
	void PrintAll() {
		printf(" --- START ---\n");
		for(int i = 0; i<tableSize; i++) {
			for(int j = 0; j<blockSize; j++) {
				printf("%4d\n", blockTable[i][j]);
			}
		}
		printf(" --- END ---\n\n");
	}
	void Extend() {
		// Double DBA size

		// Allocate the new table and copy the old one
		int** tmp = new int*[tableSize*2];
		memcpy(tmp, blockTable, tableSize*2*sizeof(int));
		delete[] blockTable;
		blockTable = tmp;
		// Allocate the new blocks with one allocation
		blockTable[tableSize] = new int[blockSize*tableSize];
		// Set the table pointers to the blocks
		// First one is already set
		// (head of blocks) + (next block offset)
		for(int i = 1; i<tableSize; i++) { 
			*(blockTable + tableSize + i) = *(blockTable + tableSize) + i*blockSize;
		}
		tableSize *= 2;
	}
};

int main() {

	DynamicBlockArray DBA(4096, 32);

	// Initialization and Extend() test
	// DBA.PrintAll();
	DBA.Extend();
	DBA.Extend();
	DBA.Extend();
	DBA.Extend();
	DBA.Extend();
	DBA.Extend();
	DBA.Extend();
	DBA.TestFill();
	// DBA.TestFill();
	// DBA.PrintAll();

	// Testing element access
	printf("> GetElem() test\n\t");
	int* elem = DBA.GetElem(10056);
	if(elem == nullptr) {
		printf("Error: out of index!\n");
	} else {
		printf("%d\n", *elem);
	}

	printf("> GetBlock() test\n\t");
	int** block = DBA.GetBlock(85);
	if(block == nullptr) {
		printf("Error: out of index!\n");
	} else {
		printf("%d\n", (*block)[2]);
	}

	printf("> GetElemInBlock() test\n\t");
	int* elemInBlock = DBA.GetElemInBlock(534, 1034);
	if(elemInBlock == nullptr) {
		printf("Error: out of index!\n");
	} else {
		printf("%d\n", *elemInBlock);
	}

	getchar();
	printf("\n[EOF]\n");

	return 0;
}
