#include <iostream>
using namespace std;

class RunGame{
	public:

		RunGame();
		~RunGame();

//user questions for initial configuration and game mode
		bool askInitial();
		bool askMode();
		//can't really do an efficient method for end behavior because of 3 options

//global variables
		int rows;
		int cols;
		int generation;

//dimensions from file and generation variable
		void setRowsFile(string fileName);//set int rows
		void setColsFile(string fileName);// set int cols
		void setGens();
		int getGens();

//producing game boards
		char** initialFile(string filePath);
		char** initialRandom();//set dimensions within method, no parameters
		char** runClassicMode(char** initial);
		char** runDoughnutMode(char** initial);

//three end behaviors
		void briefPause();
		void promptNext();
		void outputFile(char** thisConfig, string name);

//simple nested for loop to print rows to console given a configuration
		void printRows(char** config);

//testing boards to stop when stable or empty
		bool isEmpty(char** c);
		bool isStable(char** prev, char** next);



};
