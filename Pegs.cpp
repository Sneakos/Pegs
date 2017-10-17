#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

 template<class TYPE>
struct node{
    int sRow; //Source Row
    int sCol; //Source Column
	int dRow; //Destination Column
	int dCol; //Destination Column
    node<TYPE>* next;
};

 template<class TYPE>
struct Stack{
    node<TYPE>* top;
	int size;
};

/***********************************
 * Push: Adds a node to the stack
 * Inputs: A stack, the source row and column, and the destination row and col
 * Outputs: void
***********************************/
 template<class TYPE>
void push(Stack<TYPE>* stack, int sRow, int sCol, int dRow, int dCol)
{
    node<TYPE>* tmp = new node<TYPE>;
    tmp->sRow = sRow;
    tmp->sCol = sCol;
	tmp->dRow = dRow;
	tmp->dCol = dCol;
    tmp->next = stack->top;
    stack->top = tmp;
	stack->size++;
}

/***********************************
 * Pop: Removes a node from the top of the stack
 * Inputs: A stack, a reference to the source columnds and the destination columns
 * Outputs: void, stack values stored to reference variables
***********************************/
 template<class TYPE>
void pop(Stack<TYPE>* stack, TYPE *sRow, TYPE *sCol, TYPE *dRow, TYPE *dCol)
{
    if(stack->top == NULL)
        return;
    else
    {
        *sRow = stack->top->sRow;
		*sCol = stack->top->sCol;
		*dRow = stack->top->dRow;
		*dCol = stack->top->dCol;
        node<TYPE>* tmp = stack->top->next;
        delete(stack->top);
        stack->top = tmp;
		stack->size--;
    }
}

/***********************************
 * Initialize Stack: Creates a stack with NULL top and size 0
 * Inputs: A stack
 * Outsputs: void
***********************************/
 template<class TYPE>
void initStack(Stack<TYPE>* stack)
{
    stack->top = NULL;
	stack->size = 0;
}

/***********************************
 * Array Copy: Copies the contents of one array to another
 * Inputs: Source array and destination array
 * Outputs: void
***********************************/
void arrCpy(int src[][6], int dest[][6])
{
	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 6; j++)
			dest[i][j] = src[i][j];
	}
}

/***********************************
 * Check: Finds all valid positions the given coordinate can jump to
 * Inputs: The array to check, row of pegs, column of peg, a stack
 * Outputs: void, positions pushed to the stack
***********************************/
 template<class TYPE>
void check(int arr2[][6], int row, int col, Stack<TYPE>* jump)
{
	if(arr2[row][col] <= 0)
        return;
    int arr[6][6];
	arrCpy(arr2, arr);
	int count = 1; //This keeps track of which spot its checking, skipping 5 (since that is the original spot)
		
	for(int i = row - 1; i <= row + 1; i++)
	{
		for(int j = col -1; j <= col + 1; j ++)
		{
			if(i >= 0 && j >= 0 && arr[i][j] == 1)
			{
				if(count == 1 && i >= 1 && j >= 1 && arr[i-1][j-1] == 0) //Adds the coordinate to stack - Top Left
				{ 
					push(jump, row, col, i - 1, j - 1); 
					arr[i-1][j-1] = 1;
				}
				if(count == 2 && (i-1) >= 0 && arr[i-1][j] == 0)  //Adds the coordinate to stack - Above
				{
					push(jump, row, col, i - 1, j);
					arr[i-1][j] = 1; }
				if(count == 3 && i >= 1 && arr[i-1][j+1] == 0 && row < 4) //"" - Top Right
				{
					push(jump, row, col, i - 1, j + 1);
					arr[i-1][j+1] = 1; }
				if(count == 4 && (j-1) >= 0 && arr[i][j-1] == 0) //"" - Left
				{
					push(jump, row, col, i, j - 1);
					arr[i][j-1] = 1; }
				if(count == 6 && arr[i][j+1] == 0) //"" -Right
				{
					push(jump, row, col, i, j + 1);
					arr[i][j+1] = 1; }
				if(count == 7 && j >= 1 && arr[i+1][j-1] == 0) //"" - Bottom Left
				{
					push(jump, row, col, i + 1, j - 1);
					arr[i+1][j-1] = 1; }
				if(count == 8 && arr[i+1][j] == 0) //"" - Below
				{
					push(jump, row, col, i + 1, j);
					arr[i+1][j] = 1; }
				if(count == 9 && arr[i+1][j+1] == 0) //"" - Bottom Right
				{
					push(jump, row, col, i + 1, j + 1);
					arr[i+1][j+1] = 1; 
				}
			}
			count++;
		}
	}
}

/***********************************
 * Jump: Moves peg from source position to destination position, and removes peg from the position in between
 * Inputs: A stack, an array to make the jump in, source row and column, destination row and column
 * Outputs: void
***********************************/
 template<class TYPE>
void jump(Stack<TYPE>* jumps, int arr[][6], int sR, int sC, int dR, int dC)
{
	int mR, mC;
	mR = (sR + dR) / 2; //This gets the midpoint of the (sR, sC) and (dR, dC)
	mC = (sC + dC) / 2; //""
	arr[sR][sC] = 0;
	arr[mR][mC] = 0;
	arr[dR][dC] = 1;
	push(jumps, sR, sC, dR, dC); //adds the jump to the stack
}

/***********************************
 * Unjump: Similar to jump, adds peg to destination position, removes peg from source position, but adds peg to position in between
 * Inputs: The array to jump in, source row and column, destination row and column
 * Outputs: void
***********************************/
void unjump(int arr[][6], int sR, int sC, int dR, int dC)
{
	int mR, mC;
	mR = (sR + dR) / 2;
	mC = (sC + dC) / 2;
	arr[sR][sC] = 0;
	arr[mR][mC] = 1;
	arr[dR][dC] = 1;
}

/***********************************
 * Get Moves: Utilizes the check() function to get all possible moves in an array, and stores them in a certain row in an array
 * Inputs: A stack, the row to store the moves in, the array to check, the array to store the moves in
 * Outputs: void
***********************************/
 template<class TYPE>
void getMoves(Stack<TYPE>* jump, int open, int arr[][6], int path[][500])
{
	int sRow, sCol, dRow, dCol;
	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			check(arr, i, j, jump);
		}
	}
	//Adds values from stack to path array, in correct formatting
	for(int i = 0; jump->size > 0; i += 4)
	{
		pop(jump, &sRow, &sCol, &dRow, &dCol);
		path[open][i] = sRow;
		path[open][i+1] = sCol;
		path[open][i+2] = dRow;
		path[open][i+3] = dCol;
	}
}

/***********************************
 * Clear: Sets the top of the stack to NULL
 * Inputs: A stack
 * Outputs: void
***********************************/
 template<class TYPE>
void clear(Stack<TYPE> *stack)
{
	stack->top = NULL; //ewwwww memory leaks?
}

/***********************************
 * Print Stack: Prints the contents of the stack
 * Inputs: A stack
 * Outputs: void
***********************************/
 template<class TYPE>
void printStack(Stack<TYPE>* stack)
{
    node<TYPE>* current = stack->top;
    cout << "Stack: ";
    while(current != NULL)
    {
        cout << "i: " << current->sRow << "\t" << "j: " << current-> sCol << "\t";
		cout << "x: " << current->dRow << "\t" << "y: " << current-> dCol << endl;
        current = current->next;
    }
    cout << "\n";
}

/***********************************
 * Print Array: Prints the contents of the array
 * Inputs: An array
 * Outputs: void
***********************************/
void printArr(int arr[][6])
{
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 6; j++)
			cout << arr[i][j] << "\t";
		cout << endl;
	}
}

/***********************************
 * Print Array 2: For debugging - prints the array containing all possible moves
 * Inputs: An array
 * Outputs: void
***********************************/
void printArr2(int arr[][500])
{
	for(int i = 0; i < 15; i++){ //For readability, only prints first 15 values in array
		for(int j = 0; j < 15; j++)
			cout << arr[i][j] << " ";
		cout << endl;
	}
}

/***********************************
 * Fill Array: Fills the array with -1's
 * Inputs: An array
 * Outputs: void
***********************************/
void fillArray(int arr[][500])
{
	for(int i = 0; i < 500; i++){
		for(int j = 0; j < 500; j++)
			arr[i][j] = -1;
	}
}

/***********************************
 * Get Row Column: Gets the row and column of the starting position
 * Inputs: The starting position between 1 and 15, rerefence to row and column
 * Outputs: iRow and iCol will have the row and col
***********************************/
void getRowCol(int start, int *iRow, int *jCol)
{
	if(start > 10)
	{
		*iRow = 4;
		*jCol = start - 11;
	}
	else if (start > 6)
	{
		*iRow = 3;
		*jCol = start - 7;
	}
	else if (start > 3)
	{
		*iRow = 2;
		*jCol = start - 4;
	}
	else if (start > 1)
	{
		*iRow = 1;
		*jCol = start - 2;
	}
	else
	{
		*iRow = *jCol = 0;
	}
}

/***********************************
 * Get Spot: Gets the spot the row and column are referring to
 * Inputs: The starting position between 1 and 15, rerefence to row and column
 * Outputs: The spot
***********************************/
int getSpot(int i, int j)
{
	int spot = 1;
	if(i == 4)
		spot = j + 11;
	else if (i == 3)
		spot = j + 7;
	else if (i == 2)
		spot = j + 4;
	else if (i == 1)
		spot = j + 2;
	return spot;
}

/***********************************
 * Initialize Array: Initializes the array, with a given starting position
 * Inputs: An array, the starting position
 * Outputs: void
***********************************/
void initializeArray(int arr[][6], int start)
{
	int iRow, jCol;
	getRowCol(start, &iRow, &jCol);
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j <= i; j++)
		{
			if(i == iRow && j == jCol)
				arr[i][j] = 0;
			else
				arr[i][j] = 1;
		}
		for(int j = i+1; j < 6; j++)
		{
			arr[i][j] = -1;
		}
	}
	for(int j = 0; j < 6; j++)
	{
		arr[5][j] = -1;
	}
}

/***********************************
 * Array Count: Counts the number of 1's (pegs) in the array
 * Inputs: An array
 * Outputs: The numbers of 1's found
***********************************/
int arrCount(int arr[][6])
{
	int size = 0;
	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			if(arr[i][j] == 1)
				size++;
		}
	}
	return size;
}

/***********************************
 * Convert Row: Converts the row to an assigned letter equivalent (for user output)
 * Inputs: The row to convert
 * Outputs: The corresponding character
***********************************/
char convertR(int row)
{
	switch(row)
	{
		case 0: return 'A'; break;
		case 1: return 'B'; break;
		case 2: return 'C'; break;
		case 3: return 'D'; break;
		case 4: return 'E'; break;
		default: return 'Z'; break;
	}
}

/**********************************
 * Print Solution: Prints the user-translated position, as given by the stack
 * Inputs: A stack
 * Outputs: void
***********************************/
 template<class TYPE>
void printSol(Stack<TYPE> *jumps)
{
	int sRow1, sCol1, dRow1, dCol1;
	int i = 0;
	char sRow[13];
	char dRow[13];
	int sCol[13];
	int dCol[13];
	for(i = 0; jumps->size > 0; i++)
	{
		pop(jumps, &sRow1, &sCol1, &dRow1, &dCol1);
		sRow[i] = convertR(sRow1);
		sCol[i] = sCol1 + 1;
		dRow[i] = convertR(dRow1);
		dCol[i] = dCol1 + 1;
	}
	for(i -= 1; i >= 0; i--)
	{
		cout << sRow[i] << sCol[i] << " to " << dRow[i] << dCol[i] << endl;
	}
}

/**********************************
 * Print Solution 2: Prints the user-translated position, as given by the stack
 * Inputs: A stack
 * Outputs: void
***********************************/
 template<class TYPE>
void printSol2(Stack<TYPE> *jumps)
{
	int sRow1, sCol1, dRow1, dCol1;
	int i = 0;
	int sSpot[13];
	int dSpot[13];
	for(i = 0; jumps->size > 0; i++)
	{
		pop(jumps, &sRow1, &sCol1, &dRow1, &dCol1);
		sSpot[i] = getSpot(sRow1, sCol1);
		dSpot[i] = getSpot(dRow1, dCol1);
	}
	for(i -= 1; i >= 0; i--)
	{
		cout << setw(2) << sSpot[i] << " to " << dSpot[i] << endl;
	}
}

/***********************************
 * This initalizes the stacks and arrays, and brute forces the answer by checking every possible set of moves until there is
 * only one peg left in the array, and then prints out the path it took to get to that state
***********************************/
int main()
{
    Stack<int> stack;
    Stack<int> jumps;
    initStack(&stack);
	initStack(&jumps);

	int size, row;
	int sR, sC, dR, dC;
	int best = 15;
	size = 0;
	row = 0;
	
	int moves[500][500]; //The array to hold the moves generated by getMoves()
	/* FORMAT
	 * row: [sRow][sCol][dRow][dCol][sRow][sCol][dRow][dCol][etc.].....[-1]
	*/
	fillArray(moves);

	int start, x, y, iRow, jCol;
	cout << "Enter number of starting move: ";
	cin >> start;
	
	int arr[6][6];
	//if future me can think of a better way to do
	//the algorithm for getSpot and getRowCol, go for it
	initializeArray(arr, start);
					 
	getMoves(&stack, row, arr, moves);			 
	while(row >= 0)
	{
		while(size == 0 && row >= 0)
		{
			for(size = 0; moves[row][size] != -1; size++);
			if(size == 0)
			{
				int comp = arrCount(arr);
				if(comp < best) best = comp;
				if(best == 1) //Solution has been found
				{
					//printSol(&jumps); 
					printSol2(&jumps);
					return 0; 
				}
				row--; //Current row has no moves, check previous row in next iteration
				pop(&jumps, &sR, &sC, &dR, &dC); //The last jump was incorrect, remove it
				unjump(arr, dR, dC, sR, sC); //Restore the previous state in the array, as described by the values popped off the stack 'jump'
			}
		}
		if(row < 0)
		{
			cout << best << endl; //This only executes if no perfect solution (1) has been found
			return 0;
		}
		//Stores the values in the moves array, and then removes them from the array
		sR = moves[row][size - 4];
		sC = moves[row][size - 3];
		dR = moves[row][size - 2];
		dC = moves[row][size - 1];
		moves[row][size - 4] = -1;
		moves[row][size - 3] = -1;
		moves[row][size - 2] = -1;
		moves[row][size - 1] = -1;
		
		jump(&jumps, arr, sR, sC, dR, dC);
		clear(&stack);
		row++;
		getMoves(&stack, row, arr, moves); //Checks the next state of the array, and stores it in the next row of array 'moves'
		size = 0;
	}
	return 0; //Technically, an unreachable statement
}