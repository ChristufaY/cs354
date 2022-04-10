//Christopher Yang

int arr2D[128][8];

int main() {

    for(int iteration = 0; iteration < 100; iteration++)
	for(int row = 0; row < 128; row = row + 64)
	    for(int col = 0; col < 8; col++)
		arr2D[row][col] = iteration + row + col;
    return 0;
}

