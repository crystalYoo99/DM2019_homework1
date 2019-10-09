#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main (int argc, const char* argv[])
{
    FILE * f = fopen(argv[1], "r");
    FILE * fp = fopen("formula", "w");

    if(f == NULL) {
        printf("No input file\n");
        return 0;
    }
    
    char* ptr;
    int x=0, y=0; //x, y는 판 선언 시 카운터
	int** input;    //input은 위, 오른쪽 수 배제한 알맹이만
	char buffer[100];   //buffer는 한 줄 씩 임시로 읽어오는 것
	int col=0, row=0;   //col, row는 가로, 세로 수
    int* column_label;  //위의 수
    int* row_label; //오른쪽의 수
    int i;
    int rl = 0;
    int cl = 0;

    //row, col 알아내기
    fgets(buffer, sizeof(buffer), f);
    ptr = strtok(buffer, " ");
    while(ptr != NULL) {
        col++;
        ptr = strtok(NULL, " ");
    }
    while(1){
        fgets(buffer, sizeof(buffer), f);
        row++;
        if(feof(f)) break;
    }

    //row, col만큼 dynamic allocation
    input = (int**)malloc(row*sizeof(int*));
    for(i = 0; i < row; i++)
        input[i] = (int*)malloc(col*sizeof(int));
    column_label = (int*)malloc(col*sizeof(int));
    row_label = (int*)malloc(row*sizeof(int));

    //파일 포인터 처음으로
    fseek(f, 0L, SEEK_SET ); 

    //제일 위 column_label 받기
    buffer[0] = '\0';
    fgets(buffer, sizeof(buffer), f);   //한 줄 읽어와서 buffer에 저장
    ptr = strtok(buffer, " ");
    while(ptr != NULL) {
        if(*ptr != ' ') {
            column_label[cl] = atoi(ptr);
            cl++;
        }
        ptr = strtok(NULL, " ");
    }

    //파일에서 읽어와서 배열에 저장
    while(1) {
        buffer[0] = '\0';
	    fgets(buffer, sizeof(buffer), f);   //한줄 읽기
        ptr = strtok(buffer, " ");
		
		y++;  //로우++
		x=0;  //칼럼 초기화
        while(ptr != NULL && x < col) {
            if(*ptr != ' ') {
                x++;
                input[y-1][x-1] = atoi(ptr);
            }
            ptr = strtok(NULL, " ");
        }
        
        //만약 한줄 읽으면 제일 끝의 row_label 읽기
        row_label[rl] = atoi(ptr);
        rl++;
        if(feof(f)) break;  //파일 끝이면 브레이크
	}
    fclose(f);
    
    //판 짜기
    for (y = 1 ; y <= row ; y++)    //로우 동안
        for (x = 1 ; x <= col ; x++)    //컬럼 동안
            fprintf(fp, "(declare-const a%d_%d Bool)\n", y, x) ; //판 선언
    
    for(y = 1; y <= row; y++) 
        for( x= 1; x <= col; x++)
            fprintf(fp, "(declare-const b%d_%d Int)\n", y, x);

    for(y =1; y <= row; y++)
        for(x =1; x <= col; x++)
            fprintf(fp, "(declare-const w%d_%d Int)\n", y, x);

    for(y =1 ; y <= row; y++)
        for(x = 1; x<=col; x++)
            fprintf(fp, "(assert (or (= b%d_%d %d) (= b%d_%d 0)))\n", y, x, input[y-1][x-1], y, x);
    for(y =1 ; y <= row; y++)
        for(x = 1; x<=col; x++)
            fprintf(fp, "(assert (or (= w%d_%d %d) (= w%d_%d 0)))\n", y, x, input[y-1][x-1], y, x);
    for(y = 1; y<=row; y++)
        for(x = 1; x <=col; x++)
            fprintf(fp, "(assert (not (= b%d_%d w%d_%d)))\n", y, x,y, x);
    

    for(y =1; y <= row; y++) {
        fprintf(fp, "(assert(=");
        for(i = 1; i <= row; i++) 
            fprintf(fp, "(+ (ite (= a%d_%d false) w%d_%d 0)", y, i, y, i);
        for(i = 1; i <= row; i++)
            fprintf(fp, ")");
        fprintf(fp, " %d))\n", row_label[y-1]);
    }
    
    for(x =1; x <= col; x++) {
        fprintf(fp, "(assert(=");
        for(i = 1; i <= col; i++) 
            fprintf(fp, "(+ (ite (= a%d_%d true) b%d_%d 0) ", i, x, i, x);
        for(i = 1; i <= col; i++)
            fprintf(fp, ")");
        fprintf(fp, " %d))\n", column_label[x-1]);
    }
    fprintf(fp, "(check-sat)\n(get-model)\n") ;
    fclose(fp) ;

    free(input);
    free(column_label);
    free(row_label);

    char s[128];
    char t[128];
    int j, k;
    char board[row+1][col+1];
    
    FILE * fin = popen("z3 formula", "r") ; //FIXME
    
    char buf[128] ;
    fscanf(fin, "%s", buf) ;
   
    if (strcmp(buf, "unsat") == 0)
    {
        printf("No solution\n");
        return 0;
    }
    
    fscanf(fin, "%s", buf) ;
 
    while (!feof(fin)) {
        fscanf(fin, "%*s %s %*s %*s %s", s, t);
        if(s[0]=='b'|| s[0] == 'w') continue;
        char *p = strtok(s+1, "_");
        int temp = 0;
        while(p)
        {
            if(temp == 0)
                i = atoi(p);
            else
                j = atoi(p);
            p = strtok(NULL, " ");
            temp++;
        }
        
        if(strcmp(t,"true)") == 0 ){
            board[i][j] = 1;
        }
        else{
            board[i][j] = 0;
        }
    }
    
    for (i = 1; i <=row; i++)
    {
        for (j = 1; j <= col; j++)
        {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    
    pclose(fin) ;

}