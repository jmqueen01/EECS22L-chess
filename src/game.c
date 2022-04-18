#include "game.h"
#include "board.h"
#include <stdio.h>


const int VALID_MOVE_SIZE = 63;

//move the piece at (f0r0) to (f1r1) on the board b
//returns chess algebraic notation of the move if valid
char *Move(int f0, int r0, int f1, int r1, Board *b) {
	if (IsValid(f0, r0, f1, r1, b)) {
		//store the moving piece temporarily
		Piece *p = b->board[f0][r0];
		
		//normal move
		if (b->board[f1][r1]->piece == EMPTY) {
			b->board[f0][r0] = b->board[f1][r1];
			b->board[f1][r1] = p;
			//Castling (f0, r0, f1, r1, b);
		}

		//implement promotion
		//implement en passant
		
		//capturing move
		else {
			Capture(f1, r1, b);
			b->board[f1][r1] = p;
			b->board[f0][r0] = malloc(sizeof(Piece));
			b->board[f0][r0]->piece = EMPTY;
			b->board[f0][r0]->color = NO_COLOR;
		}
	}else{
		printf("Invalid move!\n");
	}
	return "";
}



void Capture(int f0, int r0, Board *b) {
	free(b->board[f0][r0]);
}



//checks if the move from (f0r0) to (f1r1) is valid
bool IsValid(int f0, int r0, int f1, int r1, Board *b) {
	//pointer to array of pointers (stores the address of our array of move pointers)
	//(since this array will be dynamically allocated)
	MOVE * valid[VALID_MOVE_SIZE];

	for (int i = 0; i < VALID_MOVE_SIZE; ++i) valid[i] = NULL;
	
	
	//get the valid moves array for the corresponding piece
	switch (b->board[f0][r0]->piece) {
		case QUEEN:
			getValidMovesQueen(f0, r0, b, valid);
			break;
		case ROOK:
			getValidMovesRook(f0, r0, b, valid);
			break;
		case KNIGHT:
			getValidMovesKnight(f0, r0, b, valid);
			break;
		case BISHOP:
			getValidMovesBishop(f0, r0, b, valid);
			break;
		case PAWN:
			getValidMovesPawn(f0, r0, b, valid);
			break;
		case KING:
			getValidMovesKing(f0, r0, b, valid);
			break;
		default:
			return true;
	}

	for(int i = 0; i < VALID_MOVE_SIZE; i++){
		if(valid[i]){
			if(valid[i]->r1 == r1 && valid[i]->f1 == f1){
				return true;
			}
		}else{
			return false;
		}
	}

	//if the given move is not found in the valid moves
	return false;
}

void getValidMovesKnight(int f0, int r0, Board *b, MOVE *moves[]) {
	//TODO: WE NEED TO TEST THIS
	//MOVE *moves[63];
	int knights_move[8];
	int curr_move = 0;
	knights_move[0] = f0 + 2;
	knights_move[1] = r0 - 1;

	knights_move[2] = f0 + 1;
	knights_move[3] = r0 - 2;

	knights_move[4] = f0 - 2;
	knights_move[5] = r0 + 1;

	knights_move[6] = f0 - 1;
	knights_move[7] = r0 + 2;

	for (int i=0; i < 7; i+=2) {
		if (knights_move[i]>=0 && knights_move[i]<=7 && knights_move[i+1]>=0 && knights_move[i+1]<=7) {
			if (b->board[knights_move[i]][knights_move[i+1]]->piece == EMPTY 
			|| b->board[knights_move[i]][knights_move[i+1]]->color != b->board[f0][r0]->color) {
				moves[curr_move] = malloc(sizeof(MOVE));
				moves[curr_move]->r0 = r0;
				moves[curr_move]->f0 = f0;
				moves[curr_move]->f1= knights_move[i];
				moves[curr_move]->r1 = knights_move[i+1];
				curr_move++;
			}
		}
	}
	//return moves;
}

void getValidMovesQueen(int f, int r, Board *b, MOVE *moves[]){
	//MOVE *moves[63];
	int curr_move = 0;
	//check vertical line up
	for(int i = r+1; i < 8; i++){
		if(b->board[f][i]->color != NO_COLOR && b->board[f][i]->color == b->board[f][r]->color){
			break;
		}
		moves[curr_move] = malloc(sizeof(MOVE));
		moves[curr_move]->r0 = r;
		moves[curr_move]->f0 = f;
		moves[curr_move]->r1 = i;
		moves[curr_move]->f1 = f;
		//b->board[f][i]->hl = 1;
		if(!(b->board[f][i]->piece == EMPTY)){
			break;
		}
		curr_move++;
	}
	//check vertical line down
	for(int i = r-1; i >= 0; i--){
		if(b->board[f][i]->color != NO_COLOR && b->board[f][i]->color == b->board[f][r]->color){
			break;
		}
		moves[curr_move] = malloc(sizeof(MOVE));
		moves[curr_move]->r0 = r;
		moves[curr_move]->f0 = f;
		moves[curr_move]->r1 = i;
		moves[curr_move]->f1 = f;
		//b->board[f][i]->hl = 1;
		if(!(b->board[f][i]->piece == EMPTY)){
			break;
		}
		curr_move++;
	}
	//check horizontal line right
	for(int i = f+1; i < 8; i++){
		if(b->board[i][r]->color != NO_COLOR && b->board[i][r]->color == b->board[f][r]->color){
			break;
		}
		moves[curr_move] = malloc(sizeof(MOVE));
		moves[curr_move]->r0 = r;
		moves[curr_move]->f0 = f;
		moves[curr_move]->r1 = r;
		moves[curr_move]->f1 = i;
		//b->board[i][r]->hl = 1;
		if(!(b->board[i][r]->piece == EMPTY)){
			break;
		}
		curr_move++;
	}
	//check horizontal line left
	for(int i = f-1; i >= 0; i--){
		if(b->board[i][r]->color != NO_COLOR && b->board[i][r]->color == b->board[f][r]->color){
			break;
		}
		moves[curr_move] = malloc(sizeof(MOVE));
		moves[curr_move]->r0 = r;
		moves[curr_move]->f0 = f;
		moves[curr_move]->r1 = r;
		moves[curr_move]->f1 = i;
		//b->board[i][r]->hl = 1;
		if(!(b->board[i][r]->piece == EMPTY)){
			break;
		}
		curr_move++;
	}
	//check diagonal top right
	for(int i = f+1, j=r+1; i<8 && j<8; i++,j++){
		if(b->board[i][j]->color != NO_COLOR && b->board[i][j]->color == b->board[f][r]->color){
			break;
		}
		moves[curr_move] = malloc(sizeof(MOVE));
		moves[curr_move]->r0 = r;
		moves[curr_move]->f0 = f;
		moves[curr_move]->r1 = j;
		moves[curr_move]->f1 = i;
		//b->board[i][j]->hl = 1;
		if(!(b->board[i][j]->piece == EMPTY)){
			break;
		}
		curr_move++;
	}
	//check diagonal bottom right
	for(int i = f+1, j=r-1; i<8 && j>=0; i++,j--){
		if(b->board[i][j]->color != NO_COLOR && b->board[i][j]->color == b->board[f][r]->color){
			break;
		}
		moves[curr_move] = malloc(sizeof(MOVE));
		moves[curr_move]->r0 = r;
		moves[curr_move]->f0 = f;
		moves[curr_move]->r1 = j;
		moves[curr_move]->f1 = i;
		//b->board[i][j]->hl = 1;
		if(!(b->board[i][j]->piece == EMPTY)){
			break;
		}
		curr_move++;
	}
	//check diagonal bottom left
	for(int i = f-1, j=r-1; i>=0 && j>=0; i--,j--){
		if(b->board[i][j]->color != NO_COLOR && b->board[i][j]->color == b->board[f][r]->color){
			break;
		}
		moves[curr_move] = malloc(sizeof(MOVE));
		moves[curr_move]->r0 = r;
		moves[curr_move]->f0 = f;
		moves[curr_move]->r1 = j;
		moves[curr_move]->f1 = i;
		//b->board[i][j]->hl = 1;
		if(!(b->board[i][j]->piece == EMPTY)){
			break;
		}
		curr_move++;
	}
	//check diagonal top left
	for(int i = f-1, j=r+1; i>=0 && j<8; i--,j++){
		if(b->board[i][j]->color != NO_COLOR && b->board[i][j]->color == b->board[f][r]->color){
			break;
		}
		moves[curr_move] = malloc(sizeof(MOVE));
		moves[curr_move]->r0 = r;
		moves[curr_move]->f0 = f;
		moves[curr_move]->r1 = j;
		moves[curr_move]->f1 = i;
		//b->board[i][j]->hl = 1;
		if(!(b->board[i][j]->piece == EMPTY)){
			break;
		}
		curr_move++;
	}
}

void getValidMovesRook(int f0, int r0, Board *b, MOVE *moves[]) {
	//MOVE *moves[VALID_MOVE_SIZE];
	
	int f = f0, r = r0 + 1, moveCount = 0;
	
	//Castling First White Rook
	if (f0 == 7 && r0 == 0 && b->board[f0][r0]->color == WHITE && b->board[f0-1][r0]->piece == EMPTY && b->board[f0-2][r0]->piece == EMPTY)
    	{
        moves[moveCount] = malloc(sizeof(MOVE));
        moves[moveCount]->f0 = f0;
        moves[moveCount]->r0 = r0;
        moves[moveCount]->f1 = 5;
        moves[moveCount]->r1 = 1;
        //b->board[5][0]->hl = 1;
        moveCount++;
    	}

	//Castling Second White Rook
	if (f0 == 0 && r0 == 0 && b->board[f0][r0]->color == WHITE && b->board[f0+1][r0]->piece == EMPTY && b->board[f0+2][r0]->piece == EMPTY && b->board[f0+3][r0]->piece == EMPTY)
    	{
        moves[moveCount] = malloc(sizeof(MOVE));
        moves[moveCount]->f0 = f0;
        moves[moveCount]->r0 = r0;
        moves[moveCount]->f1 = 3;
        moves[moveCount]->r1 = 0;
        //b->board[3][0]->hl = 1;
        moveCount++;
    	}
	
	//Castling First Black Rook
        if (f0 == 7 && r0 == 7 && b->board[f0][r0]->color == BLACK && b->board[f0-1][r0]->piece == EMPTY && b->board[f0-2][r0]->piece == EMPTY)
    	{
        moves[moveCount] = malloc(sizeof(MOVE));
        moves[moveCount]->f0 = f0;
        moves[moveCount]->r0 = r0;
        moves[moveCount]->f1 = 5;
        moves[moveCount]->r1 = 7;
        //b->board[5][7]->hl = 1;
        moveCount++;
    	}
 
	//Castling Second Black Rook
        if (f0 == 0 && r0 == 7 && b->board[f0][r0]->color == BLACK && b->board[f0+1][r0]->piece == EMPTY && b->board[f0+2][r0]->piece == EMPTY && b->board[f0+3][r0]->piece == EMPTY)
    	{
        moves[moveCount] = malloc(sizeof(MOVE));
        moves[moveCount]->f0 = f0;
        moves[moveCount]->r0 = r0;
        moves[moveCount]->f1 = 3;
        moves[moveCount]->r1 = 7;
        //b->board[3][7]->hl = 1;
        moveCount++;
    	} 

	//check above
	for (; r < 8; ++r) {
		//if (!IsInCheck(f0, r0, f, r, b)) {
		if (b->board[f][r]->color != b->board[f0][r0]->color) {
			moves[moveCount] = malloc(sizeof(MOVE));
			moves[moveCount]->f0 = f0;
			moves[moveCount]->r0 = r0;
			moves[moveCount]->f1 = f;
			moves[moveCount]->r1 = r;

			//b->board[f][r]->hl = 1;
			if (b->board[f][r]->piece != EMPTY)
				break;
		}
		else { 
			break;
		}
		moveCount++;
		//}				
	}

	//check below
	f = f0, r = r0 - 1;	
	for (; r >= 0; --r) {
		//if (!IsInCheck(f0, r0, f, r, b)) {
		if (b->board[f][r]->color != b->board[f0][r0]->color) {
			moves[moveCount] = malloc(sizeof(MOVE));
			moves[moveCount]->f0 = f0;
			moves[moveCount]->r0 = r0;
			moves[moveCount]->f1 = f;
			moves[moveCount]->r1 = r;

			//b->board[f][r]->hl = 1;
			if (b->board[f][r]->piece != EMPTY)
				break;
		}
		else {
			break;
		}

		moveCount++;
		//}				
	}

	//check right
	f = f0 + 1, r = r0;	
	for (; f < 8; ++f) {
		//if (!IsInCheck(f0, r0, f, r, b)) {
		if (b->board[f][r]->color != b->board[f0][r0]->color) {
			moves[moveCount] = malloc(sizeof(MOVE));
			moves[moveCount]->f0 = f0;
			moves[moveCount]->r0 = r0;
			moves[moveCount]->f1 = f;
			moves[moveCount]->r1 = r;

			//b->board[f][r]->hl = 1;
			if (b->board[f][r]->piece != EMPTY)
				break;
		}
		else break;

		moveCount++;
		//}				
	}

	//check left
	f = f0 - 1, r = r0;	
	for (; f >= 0; --f) {
		//if (!IsInCheck(f0, r0, f, r, b)) {
		if (b->board[f][r]->color != b->board[f0][r0]->color) {
			moves[moveCount] = malloc(sizeof(MOVE));
			moves[moveCount]->f0 = f0;
			moves[moveCount]->r0 = r0;
			moves[moveCount]->f1 = f;
			moves[moveCount]->r1 = r;

			//b->board[f][r]->hl = 1;
			if (b->board[f][r]->piece != EMPTY)
				break;
		}
		else break;

		moveCount++;
		//}				
	}
	
	//return moves;
}

//SEGMENTATION FAULT NEEDS MORE DEBUGGING
void getValidMovesBishop(int f, int r, Board *b, MOVE * moves[]){
	int curr_move = 0;

	//check diagonal top right
	for (int i = f+1, j= r+1; i < 8 && j < 8; i++, j++){
		if (b->board[f][r]->color != b->board[i][j]->color){

			moves[curr_move] = malloc(sizeof(MOVE));
			moves[curr_move]->r0 = r; 
			moves[curr_move]->f0 = f;
			moves[curr_move]->r1 = j;
			moves[curr_move]->f1 = i;
			//b->board[i][j]->hl = 1;

			if (b->board[i][j]->piece != EMPTY){
				break;
			}

		}else break;

		curr_move++;

	}
	//check diagonal bottom right
	for (int i = f+1, j= r-1; i < 8 && j>=0; i++, j--){
		if (b->board[f][r]->color != b->board[i][j]->color){

			moves[curr_move] = malloc(sizeof(MOVE));
			moves[curr_move]->r0 = r;
			moves[curr_move]->f0 = f;
			moves[curr_move]->r1 = j;
			moves[curr_move]->f1 = i;
			//b->board[i][j]->hl = 1;

			if (b->board[i][j]->piece != EMPTY){
				break;
			}

		}else break;
		
		curr_move++;
	}
	//check diagonal bottom left
	for (int i = f-1, j= r-1; i >= 0 && j >= 0; i--, j--){
		if (b->board[f][r]->color != b->board[i][j]->color){

			moves[curr_move] = malloc(sizeof(MOVE));
			moves[curr_move]->r0 = r;
			moves[curr_move]->f0 = f;
			moves[curr_move]->r1 = j;
			moves[curr_move]->f1 = i;
			//b->board[i][j]->hl = 1;

			if (b->board[i][j]->piece != EMPTY){
				break;
			}
			
		}else break;

		curr_move++;
	}
	//check diagonal top left
	for (int i = f-1, j= r+1; i>=0 && j < 8; i--, j++){
		if (b->board[f][r]->color != b->board[i][j]->color){

			moves[curr_move] = malloc(sizeof(MOVE));
			moves[curr_move]->r0 = r;
			moves[curr_move]->f0 = f;
			moves[curr_move]->r1 = j;
			moves[curr_move]->f1 = i;
			//b->board[i][j]->hl = 1;

			if (b->board[i][j]->piece != EMPTY){
				break;
			}

		}else break;

		curr_move++;
	}
}

void getValidMovesKing(int f,int r, Board *b, MOVE *moves[]){
	//MOVE *moves[63];
	int current_move = 0;
	int Castling;
	
	//Castling
	if (f == 4 && r == 0 && b->board[f][r]->color == WHITE && b->board[f+1][r]->piece == EMPTY && b->board[f+2][r]->piece == EMPTY)
	{
		moves[current_move] = malloc(sizeof(MOVE));
		moves[current_move]->f0 = f;
		moves[current_move]->r0 = r;
		moves[current_move]->f1 = 6;
		moves[current_move]->r1 = 0;
		//b->board[6][0]->hl = 1;
		current_move++;
	} 

	//Castling
	if (f == 4 && r == 0 && b->board[f][r]->color == WHITE && b->board[f-1][r]->piece == EMPTY && b->board[f-2][r]->piece == EMPTY && b->board[f-3][r]->piece == EMPTY)
	{
		moves[current_move] = malloc(sizeof(MOVE));
		moves[current_move]->f0 = f;
		moves[current_move]->r0 = r;
		moves[current_move]->f1 = 2;
		moves[current_move]->r1 = 0;
		//b->board[2][0]->hl = 1;
		current_move++;
	}
	
	//Castling
	if (f == 4 && r == 7 && b->board[f][r]->color == BLACK && b->board[f+1][r]->piece == EMPTY && b->board[f+2][r]->piece == EMPTY)
	{
		moves[current_move] = malloc(sizeof(MOVE));
		moves[current_move]->f0 = f;
		moves[current_move]->r0 = r;
		moves[current_move]->f1 = 6;
		moves[current_move]->r1 = 7;
		//b->board[6][7]->hl = 1;
		current_move++;
	}

	//Castling
	if (f == 4 && r == 7 && b->board[f][r]->color == BLACK && b->board[f-1][r]->piece == EMPTY && b->board[f-2][r]->piece == EMPTY && b->board[f-3][r]->piece == EMPTY)
	{
		moves[current_move] = malloc(sizeof(MOVE));
		moves[current_move]->f0 = f;
		moves[current_move]->r0 = r;
		moves[current_move]->f1 = 2;
		moves[current_move]->r1 = 7;
		//b->board[2][7]->hl = 1;
		current_move++;
	}

	//Check 1 space above
	moves[current_move] = malloc(sizeof(MOVE));
	moves[current_move]->f0 = f;
	moves[current_move]->r0 = r;
	moves[current_move]->f1 = f;
	moves[current_move]->r1 = r+1;
	
	if (b->board[f][r+1]->piece == EMPTY && r+1 <= 7)
	{ 
		current_move ++;
		//b->board[f][r+1]->hl = 1;
	}

	//Check 1 space below
	moves[current_move] = malloc(sizeof(MOVE));
	moves[current_move]->f0 = f;
	moves[current_move]->r0 = r;
	moves[current_move]->f1 = f;
	moves[current_move]->r1 = r-1;
		
	if (r-1 >= 0)
	{
		if (b->board[f][r-1]->piece == EMPTY)
		{ 
			current_move ++;
			//b->board[f][r-1]->hl = 1;
		}
	}
	
	//Check 1 space to the left
	moves[current_move] = malloc(sizeof(MOVE));
	moves[current_move]->f0 = f;
	moves[current_move]->r0 = r;
	moves[current_move]->f1 = f-1;
	moves[current_move]->r1 = r;

	if (f-1 >= 0)
	{
		if (b->board[f-1][r]->piece == EMPTY)
		{ 
			current_move ++;
			//b->board[f-1][r]->hl = 1;
		}
	}
	
	//Check 1 space to the right
	moves[current_move] = malloc(sizeof(MOVE));
	moves[current_move]->f0 = f;
	moves[current_move]->r0 = r;
	moves[current_move]->f1 = f+1;
	moves[current_move]->r1 = r;
	
	if (f+1 <= 7)
	{
		if (b->board[f+1][r]->piece == EMPTY)
		{ 
			current_move ++;
			//b->board[f+1][r]->hl = 1;
		}
	}
	
	//Check 1 space to the Upper Right
	moves[current_move] = malloc(sizeof(MOVE));
	moves[current_move]->f0 = f;
	moves[current_move]->r0 = r;
	moves[current_move]->f1 = f + 1;
	moves[current_move]->r1 = r + 1;

	if (r+1 <= 7 && f+1 <= 7)
	{
		if (b->board[f+1][r+1]->piece == EMPTY)
		{ 
			current_move ++;
			//b->board[f+1][r+1]->hl = 1;
		}
	}

	//Check 1 space to the Upper Left
	moves[current_move] = malloc(sizeof(MOVE));
	moves[current_move]->f0 = f;
	moves[current_move]->r0 = r;
	moves[current_move]->f1 = f - 1;
	moves[current_move]->r1 = r + 1;
	
	if (r+1 <= 7 && f-1 >=0)
	{
		if (b->board[f-1][r+1]->piece == EMPTY)
		{ 
			current_move ++;
			//b->board[f-1][r+1]->hl = 1;
		}
	}

	//Check 1 space to the Lower Right
	moves[current_move] = malloc(sizeof(MOVE));
	moves[current_move]->f0 = f;
	moves[current_move]->r0 = r;
	moves[current_move]->f1 = f + 1;
	moves[current_move]->r1 = r - 1;
	
	if (r-1 >= 0 && f+1 <= 7)
	{
		if (b->board[f+1][r-1]->piece == EMPTY)
		{ 
			current_move ++;
			//b->board[f+1][r-1]->hl = 1;
		}
	}
	
	//Check 1 space to the Lower Left
	moves[current_move] = malloc(sizeof(MOVE));
	moves[current_move]->f0 = f;
	moves[current_move]->r0 = r;
	moves[current_move]->f1 = f - 1;
	moves[current_move]->r1 = r - 1;
	
	if (r-1 >= 0 && f-1 >= 0)
	{
		if (b->board[f-1][r-1]->piece == EMPTY)
		{ 
			current_move ++;
			//b->board[f-1][r-1]->hl = 1;
		}
	}
//return moves;	
}

void getValidMovesPawn(int f0, int r0, Board *b, MOVE *moves[]){
	//MOVE *moves[VALID_MOVE_SIZE];
	//FOR WHITE PAWN
	//check to see if white pawn can move forward 2 spaces
	if(b-> board[f0][r0]->color == WHITE){
		int f = f0, r = r0 + 2, moveCount = 0;
		if(r0 == 1 && b->board[f0][r0+2]->piece == EMPTY){
			//if(!IsInCheck(f0, r0, f, r, b)){
			moves[moveCount] = malloc(sizeof(MOVE));
			moves[moveCount] -> f0 = f0;
			moves[moveCount] -> r0 = r0;
			moves[moveCount] -> f1 = f;
			moves[moveCount] -> r1 = r;
			//b->board[f][r]->hl = 1;
			moveCount++;	
			//}
		}
		//check to see if white pawn can move forward a space
		f = f0, r = r0 + 1;
		if(r < 8 && b->board[f0][r0+1]->piece == EMPTY){
			//if(!IsInCheck(f0, r0, f, r, b)){
			moves[moveCount] = malloc(sizeof(MOVE));
			moves[moveCount] -> f0 = f0;
			moves[moveCount] -> r0 = r0;
			moves[moveCount] -> f1 = f;
			moves[moveCount] -> r1 = r;
			//b->board[f][r]->hl = 1;
			moveCount++;	
			//}
		
		}
		//check to see if white pawn can capture right diagonally
		f = f0+1, r = r0 +1; //right diagonal
		//if(!IsInCheck(f0, r0, f, r, b)){//check to see if move to be made will put King in check
		if(f < 8 && r < 8 && b->board[f][r]->piece != WHITE && b->board[f][r]->piece != EMPTY){//piece should not be white and not empty to capture
			moves[moveCount] = malloc(sizeof(MOVE));
			moves[moveCount] -> f0 = f0;
			moves[moveCount] -> r0 = r0;
			moves[moveCount] -> f1 = f;
			moves[moveCount] -> r1 = r;
			//b->board[f][r]->hl = 1; //highlight space for user to see as valid move
			moveCount++;	
		}
		//}
		//check to see if white pawn can capture left diagonally
		f = f0 - 1, r = r0 + 1; //left diagonal
		//if(!IsInCheck(f0, r0, f, r, b)){ //check to see if move to be made will put King in check
		if(f >= 0 && r < 8 && b->board[f][r]->piece != WHITE && b->board[f][r]->piece != EMPTY){ //piece should not be white and not empty to capture
			moves[moveCount] = malloc(sizeof(MOVE));
			moves[moveCount] -> f0 = f0;
			moves[moveCount] -> r0 = r0;
			moves[moveCount] -> f1 = f;
			moves[moveCount] -> r1 = r;
			//b->board[f][r]->hl = 1; //highlight space for user to see valid move
			moveCount++;	
		}
		//}
	}
	//FOR BLACK PAWN
	if(b-> board[f0][r0]->color == BLACK){
		//check to see if black pawn can move forward two spaces 
		int f = f0, r = r0 - 2, moveCount = 0;
		if(r0 == 6 && b->board[f0][r0-2]->piece == EMPTY){
			int f = f0, r = r0 - 2, moveCount = 0;
			//if(!IsInCheck(f0, r0, f, r, b)){
			moves[moveCount] = malloc(sizeof(MOVE));
			moves[moveCount] -> f0 = f0;
			moves[moveCount] -> r0 = r0;
			moves[moveCount] -> f1 = f;
			moves[moveCount] -> r1 = r;
			//b->board[f][r]->hl = 1;
			moveCount++;	
			
			//}
		}
		//check to see if black pawn can move forward a space
		f = f0, r = r0 - 1;
		if(r >= 0 && b->board[f0][r0-1]->piece == EMPTY){
			//if(!IsInCheck(f0, r0, f, r, b)){
			moves[moveCount] = malloc(sizeof(MOVE));
			moves[moveCount] -> f0 = f0;
			moves[moveCount] -> r0 = r0;
			moves[moveCount] -> f1 = f;
			moves[moveCount] -> r1 = r;
			//b->board[f][r]->hl = 1;
			moveCount++;	
		
			//}
		}
		//check to see if black pawn can capture right diagonally
		f = f0 - 1, r = r0 - 1; //right diagonal
		if(f >= 0 && r >= 0 && b->board[f0 -1][r0-1]->piece != EMPTY && b->board[f0-1][r0-1]->piece != WHITE){//piece should not be white and not empty to capture
			//if(!IsInCheck(f0, r0, f, r, b)){//check to see if move to be made will put King in check
			moves[moveCount] = malloc(sizeof(MOVE));
			moves[moveCount] -> f0 = f0;
			moves[moveCount] -> r0 = r0;
			moves[moveCount] -> f1 = f;
			moves[moveCount] -> r1 = r;
			//b->board[f][r]->hl = 1; //highlight space for user to see as valid move
			moveCount++;	
				
			//}
		}
		//check to see if black pawn can capture left diagonally
		f = f0 + 1, r = r0 - 1; //left diagonal
		//if(!IsInCheck(f0, r0, f, r, b)){ //check to see if move to be made will put King in check
		if(f < 8 && r >= 0 && b->board[f0 + 1][r0 - 1]->piece != EMPTY && b->board[f0 + 1][r0 - 1]->piece != WHITE){ //piece should not be white and not empty to capture
			moves[moveCount] = malloc(sizeof(MOVE));
			moves[moveCount] -> f0 = f0;
			moves[moveCount] -> r0 = r0;
			moves[moveCount] -> f1 = f;
			moves[moveCount] -> r1 = r;
			//b->board[f][r]->hl = 1; //highlight space for user to see valid move
			moveCount++;	
		}
		//}
	}
}

bool IsInCheck(int f0, int r0, int f1, int r1, Board *b) {
	/*
	MOVE *valid[63];
	for(int f = 0; f < 8; f++){
		for(int r = 0; r < 8; r++){
			if(b[f][r]->Piece != EMPTY){
				switch (b->board[f0][r0]->piece) {
				case QUEEN:
					getValidMovesQueen(f0, r0, b, valid);
				case ROOK:
					getValidMovesRook(f0, r0, b, valid);
				case KNIGHT:
					getValidMovesKnight(f0, r0, b, valid);
				case BISHOP:
					getValidMovesBishop(f0, r0, b, valid);
				case PAWN:
					getValidMovesPawn(f0, r0, b, valid);
				case KING:
					getValidMovesKing(f0, r0, b, valid);
				default:
					return false;
				}

				for(int i = 0; i < VALID_MOVE_SIZE; i++){
					if(valid[i]){
						if(b->board[valid[i]->f1][valid[i]->r1]->Piece == KING){
							return true;
						}
					}
				}
			}
		}
	}
	*/
	return false;
}


void Castling (int f0, int r0, int f1, int r1, Board *b)
{
	if (f1 == 6 && r1 == 0 && f0 == 4 && r0 == 0)
	{
		Piece *x = b->board[7][0];
		
		b->board[7][0] = b->board[5][0];
		b->board[5][0] = x;
	}

	if (f1 == 2 && r1 == 0 && f0 == 4 && r0 == 0)
	{
		Piece *x = b->board[0][0];
		
		b->board[0][0] = b->board[3][0];
		b->board[3][0] = x;
	}

	if (f1 == 6 && r1 == 7 && f0 == 4 && r0 == 7)
	{
		Piece *x = b->board[7][7];
		
		b->board[7][7] = b->board[5][7];
		b->board[5][7] = x;
	}

	if (f1 == 2 && r1 == 7 && f0 == 4 && r0 == 7)
	{
		Piece *x = b->board[0][7];
		
		b->board[0][7] = b->board[3][7];
		b->board[3][7] = x;
	}
	
	if (f1 == 5 && r1 == 0 && f0 == 7 && r0 == 0)
	{
		Piece *x = b->board[6][0];
		
		b->board[6][0] = b->board[4][0];
		b->board[4][0] = x;
	}

	if (f1 == 3 && r1 == 0 && f0 == 0 && r0 == 0)
	{
		Piece *x = b->board[2][0];
		
		b->board[2][0] = b->board[4][0];
		b->board[4][0] = x;
	}

	if (f1 == 5 && r1 == 7 && f0 == 7 && r0 == 7)
	{
		Piece *x = b->board[6][7];
		
		b->board[6][7] = b->board[4][7];
		b->board[4][7] = x;
	}

	if (f1 == 3 && r1 == 7 && f0 == 0 && r0 == 7)
	{
		Piece *x = b->board[2][7];
		
		b->board[2][7] = b->board[4][7];
		b->board[4][7] = x;
	}


}