#include <iostream>
#include "RunGame.cpp"

using namespace std;



int main(int argc, char **argv){



	string answer;//used to hold the user input which is put into another variable depending on the question
	string path;//file path if using flat file initial config
	RunGame runner;
	char** configuration;

	runner.setGens();//sets generations to 0, happens once at the start of the file

	bool fileConfig= runner.askInitial();
	bool classicMode =runner.askMode();


	//three end behaviors, only one will be made true
	bool pause=false;
	bool prompt=false;
	bool out = false;
	string outName;//file name entered by user for output file if applicable


	if(fileConfig){
		cout << "Enter a path for the flat file initial configuration: ";
		cin >> path;

		cout << endl;
	}

	//need to clean up memory before going to a different configuration?


	cout << "What end behavior would you like to use? (1, 2, or 3)" <<endl << "Brief, 5 second pause between generations. (1)" << endl << "Be prompted to press the Enter key (2)" <<endl << "Output to a file (3)" <<endl;
	cin >> answer;
	if(answer=="1" || answer=="(1)" || answer=="1)" || answer=="1."){//just runner.briefPause()
		cout << "You've chosen a brief pause between generations." << endl << endl;
		pause = true;
	}
	else if(answer=="2" || answer=="(2)" || answer=="2)" || answer=="2."){
		//runner.promptStop();
		cout << "You've chosen to be prompted to hit the 'Enter' key." << endl << endl;
		prompt = true;
	}
	else if(answer=="3" || answer== "3)" || answer=="(3)" || answer=="3."){
		cout << "You've chosen to output to a file." << endl << endl;
		cout << "Enter a file name for the output: ";
		cin >> outName;
		out = true;
	}
	else{//in case they don't enter 1, 2, 3 this is default
		cout << "You've chosen a brief pause between generation." << endl << endl;
		pause = true;
	}


//now all user input is set so start generating configurations

int temp=0;//used once to change behavior on first iteration versus others
char** starting;
char** first;
char** next;
bool empty=false;

while(true){


	if(temp==0){//initial config methods only go here because you only do initial once
		
		if(fileConfig){//returns true if using file configuration as initial, not random
			runner.setRowsFile(path);
			runner.setColsFile(path);
			starting= runner.initialFile(path);

			if(out){
				runner.outputFile(starting, outName);
			}

			cout << "Generation #" << runner.getGens() << endl;
			runner.printRows(starting);
		}
		else{//random configuration
			starting = runner.initialRandom();

			if(out){
				runner.outputFile(starting, outName);
			}


			cout << "Generation #" << runner.getGens() << endl;
			runner.printRows(starting);
		}



		if(prompt){
			runner.promptNext();
		}



		if(classicMode){
			first = runner.runClassicMode(starting);

			cout << "Generation #" << runner.getGens() << endl;
			runner.printRows(first);
		}
		else{
			first = runner.runDoughnutMode(starting);

			cout << "Generation #" << runner.getGens() << endl;
			runner.printRows(first);
		}



		//don't need to check initial config
		if(runner.isEmpty(starting)){
			cout << "World is empty, no more living cells exist. Press Enter to exit." << endl;
			cin.ignore();

			_Exit(EXIT_FAILURE);
		}
		//don't really need an else because it would just mean the program isn't empty and needs to keep running
		//if you do isStable here then arg is (starting, first)
	}
	




	else{//after first iteration, need to use last generation not a new initial every time
		
		//now fileConfig doesn't matter
		if(classicMode){
			next = runner.runClassicMode(first);

			cout << "Generation #" << runner.getGens() << endl;
			runner.printRows(next);

			//isStable HAS to go here before first is set to next to move to next generation
			if(runner.isStable(first, next)){//new config is equal to last one
				cout << "World is stable and no longer changing. Press Enter to exit." << endl;
				cin.ignore();

				_Exit(EXIT_FAILURE);
			}
			first = next;//for the next iteration
		}
		else{
			next = runner.runDoughnutMode(first);

			cout << "Generation #" << runner.getGens() << endl;
			runner.printRows(next);

			if(runner.isStable(first, next)){//new config is equal to last one
				cout << "World is stable and no longer changing. Press Enter to exit." << endl;
				cin.ignore();
				_Exit(EXIT_FAILURE);
			}
			first = next;
		}

		if(runner.isEmpty(next)){
			cout << "World is empty, no more living cells exist. Press Enter to exit." << endl;
			cin.ignore();

			_Exit(EXIT_FAILURE);
		}

	}




	//end behaviors
	if(pause){
		runner.briefPause();
	}
	else if(prompt){
		runner.promptNext();
	}
	else if(out){
		runner.outputFile(first, outName);
	}


	temp++;
}
	


	return 0;
}
