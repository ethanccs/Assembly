#include <iostream>
#include <vector>
#include <regex>
#include <string>
#include <fstream>
#include <cstdlib>
//todo
//ablity to dectect registers from strings
//ablity to dectect loops
//ablity to exicute loops of a certain size in parallel
//improve comments
//add stack
//remove global variables
//update perl code
//perl code will compile and this code will interpret

void (*func)(int,int,int);
int registers[8];
const int numberInstructions = 32;
decltype(func) instructions[numberInstructions];
int instructionArgs[numberInstructions][4]; //1,2 are used only if 4 or 5 are
                                            //0 respectivly
char instructionPointer = 0;
char start = 0;
int maxFunctorNumber = 5;

std::vector<decltype(func)> funcLookUp;

void add (int loc, int t, int u) {// add t with u, store result at loc
  registers[loc] = t + u;
  ++instructionPointer;
}

void bie (int loc, int t, int u) { //branch to loc if t == u
  if (t == u)                       
    instructionPointer = loc;
}

void div (int loc, int t, int u) { //divide t by u, store result at loc
  registers[loc] = t / u;
  ++instructionPointer;
}

void mul (int loc, int t, int u) { //multiply t by u, store result at loc
  registers[loc] = t * u;
  ++instructionPointer;
}

void sub (int loc, int t, int u) { //subtract t by u, store result at loc
  registers[loc] = t - u;
  ++instructionPointer;
}

void initLookUp (void) {
  //this is safe because each function has the same:
  //return type and {(number, type, order) of arguments}
  funcLookUp.push_back(&add);
  funcLookUp.push_back(&bie);
  funcLookUp.push_back(&div);
  funcLookUp.push_back(&mul);
  funcLookUp.push_back(&sub);
}

//gets uses a register if it is supposed to, otherwise it uses immediate value
int useCorrectValue (int registerLoc) {
  if (instructionArgs[instructionPointer][registerLoc])
    return registers[registerLoc-1]; //registerLocations are offset by 1
  return instructionArgs[instructionPointer][registerLoc-2];
}

void execute (void) { //requires the instructions to move the instructionPointer
  instructions[instructionPointer](instructionArgs[instructionPointer][0],
				   useCorrectValue(3), useCorrectValue(4));
}

void applyByNumber (auto num, int loc, int t, int u) {  
  funcLookUp[num](loc,t,u);
  std::cout << registers[1] << std::endl;
}

//based off of a string, return the index of the function pointer
int funcStrNumber (std::string string) {
  if (regex_match(string, std::regex ("add")))
    return 0;
  if (regex_match(string, std::regex ("bie")))
    return 1;
  if (regex_match(string, std::regex ("div")))
    return 2;
  if (regex_match(string, std::regex ("mul")))
    return 3;
  if (regex_match(string, std::regex ("sub")))
    return 4;
}

//must be called before only before execution as it changes the stack
//pointer
void addInstructionFromStr (std::string func, int loc, std::string t,
			    std::string u, std::string rt, std::string ru) {
  //add instruction
  instructions[instructionPointer] = funcLookUp[funcStrNumber(func)];
  
  //add instruction arguments
  //only loc, (either t or rt), (either u or ru) are used
  instructionArgs[instructionPointer][0] = loc;
  instructionArgs[instructionPointer][1] = t[2];
  instructionArgs[instructionPointer][2] = u[2];
  instructionArgs[instructionPointer][3] = rt[2];
  instructionArgs[instructionPointer][4] = ru[2];

  //increment pointer so data is not overwritten
  instructionPointer++;
}

//reads the contents of the specified file
//adds the 
void readFile (const std::string& filename) {
  std::ifstream file(filename);

  if(!file) {
    std::cerr << "cannont open file:" << filename << std::endl;
    exit(EXIT_FAILURE);
  }

  size_t lineNumber = 0;
  for(std::string line; std::getline(file, line); ) {
    lineNumber++;
    std::istringstream iss(line);
    std::string op, arg1, arg2;
    int loc;
    
    if(!(iss >> op >> loc >> arg1 >> arg2)) {
      std::cerr << "error at: " << filename
	   << "at line: " << lineNumber
	   << "contents: " << line << std::endl;
      exit(EXIT_FAILURE);
    }

    addInstructionFromStr(op, loc, arg1, arg2, arg1, arg2);
  }
}
  

int RegsiterStrToMemLoc (std::string string) {
  //the numbers are offset by one
  if(string[0] == 'r') {
    if(string[1] < 58) {
      return string[1] - 48; //0 as char - 47 = 1 as number
    } else {
      return string[1] - 87; //a - 86 = 11
    }
  }
}

int main (int argc, char* argv[]) {
  initLookUp();
  //int x = funcStrNumber(argv[1]);
  //std::cout << argv[1] << std::endl;
  //std::cout << x << std::endl;
  //addInstruction(fsub);
  //applyByNumber(3, 1, 1, 1);
  return 0;
}
