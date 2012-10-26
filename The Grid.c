#include <stdio.h>
#include <stdlib.h>



#define ON 1
#define OFF 0
#define POP(x) {Press(grd2,(x));bits^=(1U<<(x-1));}

struct Light {
	int on;
	char s1[8];
	char s2[8];
	char s3[8];
};

int  Press(struct Light grid[], int i);
int  PrintGrid(struct Light grid[]);
void Flip(struct Light grid[], int index);
void Randomize(struct Light grid[]);
int  BitsOn(int flags);
void Solve(struct Light grid[]);
int  CheckSolved(struct Light grid[]);


int main(void)
{
	struct Light grid[25];
	auto int userNum;
	auto char charbok;

	srand((unsigned)time(NULL));
	Randomize(grid);

	do  {
		PrintGrid(grid);
		printf("Enter a #: ");
		if ( 1 == scanf("%d", &userNum)) {
			if ( userNum > 0 && userNum < 26 ) Press(grid, userNum);
			else if ( 99 == userNum ) Solve(grid);
			else if ( 77 == userNum ) Randomize(grid);
			else if ( 42 == userNum ) break; //exit success 
			}
		else { puts("Invalid input"); exit(EXIT_FAILURE); }
		if (1 == CheckSolved(grid)){
			printf("You won! Play again? (y/n): "); getchar();
			charbok = getchar();
			if (toupper(charbok) == 'Y') Randomize(grid);	
			else if (toupper(charbok) == 'N') break; //exit success
			else { puts("Bad Character"); exit(EXIT_FAILURE); }
			}
		} while (1);
	printf("GoodBye!!\n");
	return 0;
}



int PrintGrid(struct Light grid[]){
	auto int i, j;
	printf("\n");
	printf("====================================\n");
	printf("= Enter 1 - 25 to flip lightswitch =\n");
	printf("= 77-Reset   42-Quit   99-Solution =\n");
	printf("====================================\n");
	
	for (i = 0; i < 5; i++){
		for (j = 0; j < 5; j++) printf("%s", grid[(i*5+j)].s1);printf("\n");
		for (j = 0; j < 5; j++) printf("%s", grid[(i*5+j)].s2);printf("\n");
		for (j = 0; j < 5; j++) printf("%s", grid[(i*5+j)].s3);printf("\n");
		printf("\n");
		}
	return 1;
}


void Flip(struct Light grid[], int index){
	auto int i;
	auto char chew = grid[index].on ? ' ' : '#';

	for (i=1; i<7; i++) {
		grid[index].s1[i] = chew;
		grid[index].s3[i] = chew; }
	grid[index].s2[1] = chew;
	grid[index].s2[6] = chew;

	grid[index].on = grid[index].on ? OFF : ON;
}


void Randomize(struct Light grid[]){
	auto int ctr, j;
	
	for (ctr = 0; ctr < 25; ctr++) {
		grid[ctr].on = OFF;
		for (j=0; j<7; j++){ 
			grid[ctr].s1[j] = ' ';
			grid[ctr].s2[j] = ' ';
			grid[ctr].s3[j] = ' '; }
		grid[ctr].s1[7] = 0;
		grid[ctr].s2[7] = 0;
		grid[ctr].s3[7] = 0;
		grid[ctr].s2[3] = (ctr+1)/10 + '0';
		grid[ctr].s2[4] = (ctr+1)%10 + '0'; }
	
	for (ctr = 1; ctr <= 25; ctr++) if (rand()%2) Press(grid, ctr);
}

int BitsOn(int flags){
	auto int ctr;
	auto int numBits = 0;
	auto int intSize = sizeof(int) * 8;
	
	for (ctr=0; ctr<intSize; ctr++) if (1U==(1U&(flags>>ctr))) numBits++;
	return numBits;
}	


void Solve(struct Light grid[]){
	struct Light grd2[25];
	auto unsigned int ctr;
	auto unsigned int bits = 0;
	auto unsigned int bottom = 0;
	auto unsigned int a, b, c, d;
	
	for (ctr = 0; ctr < 25; ctr++) grd2[ctr].on = grid[ctr].on;
	
	for (ctr = 0; ctr < 20; ctr++)
		if (grd2[ctr].on) { Press(grd2, ctr+6); bits ^= (1U << (ctr+5)); }
	
	for (ctr = 0; ctr < 5; ctr++) if (grd2[ctr+20].on) bottom ^= (1U << ctr);
	switch (bottom){
		case  0: break;
		case  7: POP(2) break;
		case 13: POP(5) break;
		case 22: POP(1) break;
		case 28: POP(4) break;
		case 10: POP(1) POP(4) break;
		case 17: POP(1) POP(2) break;
		case 27: POP(2) POP(4) break;
		default: puts("Switch error"); exit(EXIT_FAILURE); }
	
	for (ctr = 0; ctr < 20; ctr++)
		if (grd2[ctr].on) { Press(grd2, ctr+6); bits ^= (1U << (ctr+5)); }
	
	a = BitsOn(bits);
	b = BitsOn(bits ^ 0x15A82B5);
	c = BitsOn(bits ^ 0x1B06C1B);
	d = BitsOn(bits ^ 0x0EAEEAE);
	
	if (b<c && b<a && b<d) bits ^= 0x15A82B5;
	else if   (c<a && c<d) bits ^= 0x1B06C1B;
	else if   (d<a)        bits ^= 0x0EAEEAE;

	printf("\nTo solve, press ");
	for (ctr = 0; ctr < 25; ctr++){
		if (1U == (1U & (bits >> ctr))) printf("%d ", ctr+1); }
}


int Press(struct Light grid[], int i){
	Flip(grid, i-1);
	if ( i > 5 ) Flip(grid, i-6);
	if ( i < 21) Flip(grid, i+4);
	if ( 0 != i%5) Flip(grid, i);
	if ( 1 != i%5) Flip(grid, i-2);
}


int CheckSolved(struct Light grid[]){
	auto int i;
	for (i=0; i<25; i++) if (grid[i].on) return 0;
	return 1;
}
