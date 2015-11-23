#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

using namespace std;

struct colloid{
    double x,y;
};

//meine Funktionen-------------------------------------
void condition(int* rx, int* ry, int N);
void pusher(int* rx, int* ry, int N, colloid* const c, const double step);
void statistic(colloid* const c, double& meanx, double& meany, double& var, int N);
//-----------------------------------------------------

void init(colloid* const c, const int N);
void print(const colloid* const c, const int N, const string fname);

int main(void){
  
    int N  = 50;
    cout << "Right now # of colloids is set to " << N << ", how many do you want?" << endl;
    cin  >> N;
    
    colloid*  c  = new colloid[N];	// dynamically allocate memory for N particles
    int*      rx = new int[N];		// dynamically allocate memory for movement-conditions in x direction
    int*      ry = new int[N];		// dynamically allocate memory for movement-conditions in y direction
    
    double meanx, meany, var;		// statistics
    ofstream stat("statistics.dat");	// ... to be printed out here
    
    const double step = 0.1;		// step size
    
    init(c, N);				// initialize all colloids
    
    stringstream s;			// set up stringstream -> multiple output files
    string       fname = "rwalk";	// basic name tag for multiple output files
    
    const int Nsteps = 100;		// total # of steps
    const int Nfiles = 10;		// total # of output files
    int Nsubsteps    = Nsteps / Nfiles; // resulting substeps
    
    s.str("");				// empty stringstream
    s << fname << "_0.dat";		// fill in string for initial file
    print(c, N, s.str());		// print initial positions
    
    srand(time(NULL));			// randomize random numbers
    
    for(int i = 1; i <= Nfiles; i++){
	for(int j = 0; j < Nsubsteps; j++){
	    // call to function which randomly sets up rx and ry
	    condition(rx,ry,N);
	    // call to function which pushes all colloids according to rx and ry
	    pusher(rx,ry,N,c,step);
	    // call to function which evaluates statistics
	    statistic(c,meanx,meany,var,N);
	    
	    stat << (i-1)*Nsubsteps+j << "\t" << meanx << "\t";
	    stat << meany << "\t" << var << endl;
	}
	s.str("");						// preparations for printing...
	s << fname << "_" << i << ".dat";			// ... colloid positions
	print(c, N, s.str());					
    }
    
    stat.close();			// close statistics file
    delete[] rx;
    delete[] ry;
    delete[] c;				// delete dynamically allocated arrays
    return 0;
}

void init(colloid* const c, const int N){
    for(int i = 0; i < N; i++){
	c[i].x = 0;
	c[i].y = 0;
    }
}

void print(const colloid* const c, const int N, const string fname){
    ofstream out(fname.c_str());
    for(int i = 0; i < N; i++)
	out << c[i].x << "\t" << c[i].y << endl;
    out.close();
}
//Meine Unterunfunktionen
void condition(int* rx, int* ry, int N){
  for(int i = 0; i < N; i++){
    rx[i] = rand() % 3 + 1;
    ry[i] = rand() % 3 + 1;
    }//1 := Bewegung in - Richtung; 2:= keine Bewegung; 3:= Bewegung in + Richtung
}
void pusher(int* rx, int* ry, int N, colloid* const c, const double step){
  for(int i = 0; i < N; i++){
    if(rx[i] == 1){
      c[i].x -= step;
    }
    else if(rx[i] == 3){
      c[i].x += step;
    }//rx = 2 -->keine Bewegung
  }
  for(int i = 0; i < N; i++){
    if(ry[i] == 1){
      c[i].y -= step;
    }
    else if(ry[i] == 3){
      c[i].y += step;
    }//ry = 2 -->keine Bewegung
  }
}
void statistic(colloid* const c, double& meanx, double& meany, double& var, int N){
  double sumx = 0;
  double sumy = 0;
  double varx = 0;
  double vary = 0;
  for(int i = 0; i < N; i++){
    sumx += c[i].x;
    sumy += c[i].y;
  }
  meanx = sumx/N;
  meany = sumy/N;
  for(int i = 0; i < N; i++){
    varx += (c[i].x - meanx)*(c[i].x - meanx);
    varx += (c[i].y - meany)*(c[i].y - meany);
  }
  var = (varx/N)+(vary/N);
}