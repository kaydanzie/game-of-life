#include "RunGame.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <time.h>
#include <limits>
using namespace std;

RunGame::RunGame(){}
RunGame::~RunGame(){}


bool RunGame::askInitial(){
	string in;
	bool ans;

	cout << "Which configuration would you like to use?" << endl << "1. Use random assignment configuration." << endl << "2. Specify a flat file configuration." << endl;
	cin >> in;

	if(in=="2" || in=="2." || in=="2. Specify a flat file configuration." || in=="Specify a flat file configuration."){
		ans=true;
		cout<< "You've selcted to specify a flat file configuration." << endl << endl;

		//still need to ask for file path but do it in main because you can't return bool and string
		//plus making its own method for 2 lines looks sloppy and inefficient in this circumstance
		
	}
	else{
		ans=false;
		cout << "You've selected to use a random assignment configuration." << endl << endl;
	}

	return ans;

}

bool RunGame::askMode(){
	bool ans;
	string in;

	cout << "What boundary mode would you like to use?" <<endl << "1. Classic mode" << endl <<"2. Doughnut mode" <<endl;
	cin >> in;

	if(in=="1" || in=="1." || in=="1. Classic mode" || in=="Classic" || in=="Classic mode"){
		ans = true;
		cout << "You've chosen classic mode." <<endl << endl;
		
	}
	else{
		ans = false;
		cout << "You've chosen doughnut mode." <<endl << endl;
	}

	//no follow up questions regardless of response, move on to end behavior

	return ans;
}



void RunGame::setRowsFile(string fileName){

	fstream fs;
	fs.open(fileName);
	int lineNumbers =0;

	while(fs){
		string l;
		getline(fs, l);

		++lineNumbers;

	}
	
	rows = lineNumbers-3;//don't count first 2 rows that are dimensions

}


void RunGame::setColsFile(string fileName){//sets global cols variable

	fstream f;
	f.open(fileName);
	int tempCols =0;

	while(f){
		
		string line;
		getline(f, line);

		if(line.size() <= 1){

		}

		else{
			for(int p=0; p<line.size(); p++){//chars in line is same as cols
				tempCols++;
			}
		}
	}

	cols = tempCols/rows;//because cols is incremented in every row
}

void RunGame::setGens(){
	generation = 0;
}

int RunGame::getGens(){
	return generation;
}




char** RunGame::initialFile(string path){
	char** configuration;

	fstream fs;
	fs.open(path);


	int lineNum=0;

	configuration = new char*[rows];

	//fills array
	while(fs){

		string fsLine;
		getline(fs,fsLine);

		if(lineNum==0 || lineNum==1){
		}


		else{
			configuration[lineNum-2] = new char[cols];//lineNum is the index since not using for loop
			
			for(int h=0; h< cols; ++h){//cols is the same as fsLine.size
				configuration[lineNum-2][h]=fsLine[h];
			}
		}
		
		

		lineNum++;
	
	}//end while for file
	
	fs.close();

	return configuration;
}


char** RunGame::initialRandom(){
	char** configuration;

	double density;
	string in= "";

	while(true){//get user rows
		cout << "How many rows will the grid have? ";
		cin >> in;

		stringstream myStream(in);
		if(myStream >> rows && rows>0){//put into pre-existing variable
			break;
		}
		else{
			cout << "Invalid number. Try again." << endl;
		}
	}

	while(true){//get user columns
		cout << "How many columns will the grid have? ";
		cin >> in;

		stringstream myStream(in);
		if(myStream >> cols && cols>0){
			break;
		}
		else{
			cout << "Invalid number. Try again." << endl;
		}
	}


	while(true){//get user density as decimal
		cout << "Enter the initial population density (decimal between 0 and 1). ";
		cin >> in;

		stringstream myStream(in);
		if(myStream >> density && (density>0.0) && density<1.0){
			break;
		}
		else{
			cout << "Invalid decimal. Try again." << endl;
		}
	}



	
	configuration = new char*[rows];
	

	int numFilled = density*(rows*cols);//proportion of total grid spots to be filled initially
	srand(time(NULL));
	int randomFill;//will be 0 or 1 (- or X)

	for(int f=0; f<rows; ++f){// f= rows

		configuration[f]= new char[cols];

		for(int d=0; d<cols; ++d){// d= columns

			
			randomFill = rand() % 2;

			if(randomFill==1 && numFilled>0){
				configuration[f][d]='X';
				numFilled--;
			}
			else{
				configuration[f][d]='-';
			}

			
		}//end for loop- columns

		//don't do anything in this for loop (rows)
	}
	while(numFilled>0){
		for(int f=0; f<rows; ++f){
			for(int w=0; w<cols; ++w){
				randomFill= rand() % 2;

				if(randomFill==1 && configuration[f][w]=='-' && numFilled>0){//need numFilled so it doesn't go into negatives before getting back out to while loop
					configuration[f][w]='X';
					numFilled--;
				}
				else{
					//either random int is - or this spot already has an x
				}
			}//end col for
		}//end row for
	}//end while numFilled

	return configuration;

}






char** RunGame::runClassicMode(char** initial){

	char** newConfig = new char*[rows];


	char up, down, left, right, rightUp, rightDown, leftUp, leftDown;
	int neighbors=0;

	for(int w=0; w< rows; ++w){

		newConfig[w] = new char[cols];


		for(int q=0; q<cols; ++q){ //q=columns
			

			//if it doesnt go into the if it means the cell youre looking at doesn't have that position in reference to itself, the position doesn't exist

			if((w-1)>=0){//just need to check that there is a row at all
				up = initial[w-1][q];
			}
			else{//so the last iteration value doesn't persist, other option to reset all at end of loop
				up = '-';
			}
			
			if((w+1)<=(rows-1)){//needs to be <=, means after this iteration there won't be a down
				down = initial[w+1][q];
			}
			else{
				down= '-';
			}
			if((q-1)>=0){
				left = initial[w][q-1];
			}
			else{
				left = '-';
			}
			if((q+1)<=(cols-1)){
				right = initial[w][q+1];
			}
			else{
				right='-';
			}

			if((w+1)<=(rows-1) && (q+1)<=(cols-1)){
				rightUp = initial[w+1][q+1];
			}
			else{
				rightUp='-';
			}
			if((w-1)>=0 && (q+1)<=(cols-1)){
				rightDown = initial[w-1][q+1];
			}
			else{
				rightDown = '-';
			}
			if((w-1)>=0 && (q-1)>=0){
				leftUp = initial[w-1][q-1];
			}
			else{
				leftUp= '-';
			}
			if((w+1)<=(rows-1) && (q-1)>=0){
				leftDown = initial[w+1][q-1];
			}
			else{
				leftDown = '-';
			}


			if(up=='X'){
				neighbors++;
			}
			if(down=='X'){
				neighbors++;
			}
			if(left=='X'){
				neighbors++;
			}
			if(right=='X'){
				neighbors++;
			}
			if(rightDown=='X'){
				neighbors++;
			}
			if(rightUp=='X'){
				neighbors++;
			}
			if(leftUp=='X'){
				neighbors++;
			}
			if(leftDown=='X'){
				neighbors++;
			}



			if(neighbors <=1 ){//lonely
				newConfig[w][q] = '-';
			}
			else if(neighbors==2){//stable
				newConfig[w][q] = initial[w][q];//was empty, stays empty
			}
			else if (neighbors >=4){//overcrowded
				newConfig[w][q] = '-';
			}
			else if(neighbors == 3){//grows
				newConfig[w][q] = 'X'; //no matter what was in it before, now it has one
			}
			

			neighbors=0;//every spot has a unique number of neighbors so has to go back to 0 in every col iteration (q)

		}//end column for loop


	}//end row for loop

	generation++;

	return newConfig;

}



char** RunGame::runDoughnutMode(char** initial){
	
	char** newConfig = new char*[rows];

	char up, down, left, right, rightUp, rightDown, leftUp, leftDown;
	int neighbors=0;

	for(int w=0; w< rows; ++w){

		newConfig[w] = new char[cols];


		for(int q=0; q<cols; ++q){ //q=columns
			
			
			//for up, down, left, right all will act same even corners, only wrap around one edge at most
			if((w-1)>=0){
				up = initial[w-1][q];//previous row in same column q
			}
			else{
				up = initial[rows-1][q];//last row in column q
			}
			
			if((w+1)<=(rows-1)){
				down = initial[w+1][q];
			}
			else{
				down= initial[0][q];
			}

			if((q-1)>=0){
				left = initial[w][q-1];
			}
			else{
				left = initial[w][(cols-1)];
			}

			if((q+1)<=(cols-1)){
				right = initial[w][q+1];
			}
			else{
				right = initial[w][0];
			}

			//the 4 diagonals:
			//for each diagonal there are 2 of the 4 corners that don't need to be explicity 

			if((w-1)>=0 && (q+1)<=(cols-1)){//if it's possible to go up and right without wrapping it goes in this if
				rightUp = initial[w-1][q+1];
			}
			else{//no bottom left or top left 
				if(w==0 && q==(cols-1)){//top right
					rightUp = initial[rows-1][0];
				}
				else if(w==(rows-1) && q==(cols-1)){//bottom right, don't need bottom left because it won't go into the else
					rightUp = initial[w-1][0];
				}
				else{
					if(q==(cols-1)){//it's in the furthest column
						rightUp = initial[w-1][0];//shoudln't be out of bounds because if it's here it means w-1 
					}
					else{
						rightUp = initial[rows-1][q+1];//this cell is in top row so neighbor is in bottom row

					}
				}
			}


			if((w+1)<=(rows-1) && (q+1)<=(cols-1)){
				rightDown = initial[w+1][q+1];
			}
			else{//no top left or bottom left
				if(w==0 && q==(cols-1)){//top right
					rightDown = initial[w+1][0];
				}
				else if(w==(rows-1) && q==(cols-1)){
					rightDown = initial[0][0];
				}
				else{
					if(q==(cols-1)){
						rightDown = initial[w+1][0];
					}
					else{
						rightDown = initial[0][q+1];
					}
					
				}
			}

			

			if((w-1)>=0 && (q-1)>=0){
				leftUp = initial[w-1][q-1];
			}
			else{//no top right or bottom right
				if(w==0 && q==0){
					leftUp = initial[rows-1][cols-1];
				}
				else if(w==(rows-1) && q==0){//bottom left
					leftUp = initial[w-1][cols-1];
				}
				else if(w==0 && q==(cols-1)){//top right
					leftUp = initial[rows-1][q-1];
				}
				else{
					if(q==0){//leftmost column
						leftUp = initial[w-1][cols-1];
					}
					else{
						leftUp = initial[rows-1][q-1];
					}
					
				}
			}


			if((w+1)<=(rows-1) && (q-1)>=0){
				leftDown = initial[w+1][q-1];
			}
			else{//no top right or bottom right
				if(w==0 && q==0){//top left
					leftDown = initial[w+1][cols-1];
				}
				else if(w==(rows-1) && q==0){//bottom left
					leftDown = initial[0][cols-1];
				}
				else{
					if(q==0){//leftmost column
						leftDown = initial[w+1][cols-1];
					}
					else{//top row
						leftDown = initial[0][q-1];
					}
				}
			}

			
			if(up=='X'){
				neighbors++;
			}
			if(down=='X'){
				neighbors++;
			}
			if(left=='X'){
				neighbors++;
			}
			if(right=='X'){
				neighbors++;
			}
			if(rightDown=='X'){
				neighbors++;
			}
			if(rightUp=='X'){
				neighbors++;
			}
			if(leftUp=='X'){
				neighbors++;
			}
			if(leftDown=='X'){
				neighbors++;
			}



			if(neighbors <=1 ){//lonely
				newConfig[w][q] = '-';
			}
			else if(neighbors==2){//stable
				newConfig[w][q] = initial[w][q];//was empty, stays empty
			}
			else if (neighbors >=4){//overcrowded
				newConfig[w][q] = '-';
			}
			else if(neighbors == 3){//grows
				newConfig[w][q] = 'X'; //no matter what was in it before, now it has one
			}
			
			
			//reset all so the last setting doesn't persist for every cell
			up= '-';
			down='-';
			left='-';
			right='-';
			leftUp='-';
			leftDown='-';
			rightDown='-';
			rightUp='-';

			neighbors=0;

		}//end column for loop


	}//end row for loop


	generation++;
	return newConfig;
}






void RunGame::briefPause(){
	usleep(2500000);
}

void RunGame::promptNext(){
	cout << "Press Enter to continue to next generation." << endl;
	cin.ignore();
}

void RunGame::outputFile(char** thisConfig, string name){
	ofstream newFile;
	newFile.open(name, std::ios_base::app);
	newFile << "Generation #" << generation << endl;
	for(int x= 0; x< rows; ++x){
		newFile << thisConfig[x] << endl;
	}

	newFile << endl;

	newFile.close();

}

void RunGame::printRows(char** con){
	for(int r=0; r< rows; ++r){
		cout << con[r] << endl;
	}
	cout << endl;
}


bool RunGame::isEmpty(char** config){
	int numEmpty = 0;
	bool e;
	for(int r=0; r < rows; ++r){
		for(int c=0; c< cols; ++c){
			if(config[r][c]=='-'){
				numEmpty++;
			}
			//otherwise don't do anything
		}
	}

	if(numEmpty==(rows*cols)){
		e=true;
	}
	else{
		e=false;
	}
	return e;
}

bool RunGame::isStable(char** prev, char** next){
	//both will have same dimensions
	int equalCells =0;//if all their cells are equal to their counterpart then it's stable
	for(int r=0; r< rows; ++r){
		for(int c=0; c< cols; ++c){
			if(prev[r][c]==next[r][c]){
				equalCells++;
			}
		}//end cols
	}//end rows
	bool stable;
	if(equalCells==(rows*cols)){
		stable=true;
	}
	else{
		stable=false;
	}

	return stable;
}
