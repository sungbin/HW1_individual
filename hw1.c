#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#define ARRAY_MAX 2500

int main() {

	int N = 0, M = 0;
	
	int temp;
	int row[50];
	int col[50];
	int* arr;
	arr = (int*)malloc(sizeof(int)*ARRAY_MAX);
	int arr_size = 0;

	while(scanf("%d",&temp) != EOF) {
		col[M] = temp;
		M ++;
		if(getchar() == '\n') break;
	}

	
	while(scanf("%d",&temp) != EOF) {
		arr[arr_size] = temp;
		arr_size ++;
		char c = getchar();
		if(c == '\n' || c == EOF) {
			arr_size --;
			row[N] = temp;
			N++ ;
		}
	}

	int** board;
	board = (int**) malloc(sizeof(int*) * N);
	for(int i = 0; i < N; i++) board[i] = (int*)malloc(sizeof(int) * M);

	for(int i = 0; i < arr_size; i++) board[i/M][i%M] = arr[i];

	FILE * fp = fopen("formula", "w") ;	

	for(int y = 0; y < N; y++) for(int x = 0; x < M; x++) {
		fprintf(fp, "(declare-const a%d_%d Int)\n", y, x) ;
		fprintf(fp, "(declare-const b%d_%d Int)\n", y, x) ;
	}

	for(int y = 0; y < N; y++) for(int x = 0; x < M; x++) {
		fprintf(fp, "(assert (= a%d_%d %d))\n", y, x, board[y][x]) ;
		fprintf(fp, "(assert (and (<= b%d_%d 1) (<= 0 b%d_%d)))\n", y, x, y, x) ;
			
	}
	
	for(int x = 0; x < M; x++) {
		fprintf(fp, "(assert (= (+  ");
		for(int y = 0; y < N; y++) {
			// ite
			fprintf(fp, "(ite (= b%d_%d 1) a%d_%d 0) ",y,x,y,x);
		}
		fprintf(fp, ") %d)) ",col[x]);
	}

	for(int y = 0; y < N; y++) {
		fprintf(fp, "(assert (= (+  ");
		for(int x = 0; x < M; x++) {
			// ite
			fprintf(fp, "(ite (= b%d_%d 0) a%d_%d 0) ",y,x,y,x);
		}
		fprintf(fp, ") %d)) ",row[y]);
	}
	fprintf(fp, "(check-sat)\n(get-model)\n") ;
	
	fclose(fp);

	int** answer;
	answer = (int**) malloc(sizeof(int*) * N);
	for(int i = 0; i < N; i++) answer[i] = (int*)malloc(sizeof(int) * M);

	FILE * fin = popen("z3 formula", "r");
	int i, j, k;
    	char satis[128];
    	char a[128] ;
    	char b[128] ;
    	char s[128] ;
    	char t[128] ;
    
	while(!feof(fin)) {

        // Check satisfying
    	fscanf(fin,"%s",satis);
	if(!strcmp("unsat",satis)) {
		fscanf(fin,"%s",b); // error message absort
		printf("No solution\n");
		exit(1);
	}
        else if(!strcmp("sat",satis)) {
			fscanf(fin,"%s",b); // get-model message
			while(true) {
				int val, col, row;
				char c;

				fscanf(fin,"%s", a) ;
				if(!strcmp(")",a)) break;
				fscanf(fin," %c%d_%d %s %s %s", &c,&col,&row, b, b, t) ;
				if(c == 'b') {
					t[strlen(t)-1] = 0x0;
					val = atoi(t);
					answer[col][row] = val;		
				}
			}
		}
	}

	for(int y = 0; y < N; y++) {
		for(int x = 0; x < M; x++) {
			printf("%d ",answer[y][x]);
		}
		printf("\n");
	}

}
