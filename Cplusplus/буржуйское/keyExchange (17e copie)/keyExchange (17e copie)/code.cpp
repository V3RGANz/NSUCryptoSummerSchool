#include <iostream>
#define MAXROWS 9
#define MAXVALUES 9

using namespace std;
char display[] = {'1','2','3','4','5','6','7','8','9'};

int main() {

 int arrs[MAXROWS];  // represent the different variables in the for loops

 bool status = false;

 for (int r=0;r<MAXROWS;r++)
  arrs[r] = 0;  // Initialize values

 while (!status) {

  int total = 0;
  // calculate total for exit condition
  for (int r=0;r<MAXROWS;r++)
   total +=arrs[r];
  // test for exit condition
  if (total == (MAXVALUES-1)*MAXROWS)
   status = true;

  // printing
  for (int r=0;r<MAXROWS;r++)
   cout << display[arrs[r]]; // print(arrs[r])
  cout << endl;  // print(endline)

  // increment loop variables
  bool change = true;
  int r = MAXROWS-1;  // start from innermost loop
  while (change && r>=0) {
   // increment the innermost variable and check if spill overs
   if (++arrs[r] > MAXVALUES-1) {
	arrs[r] = 0;  // reintialize loop variable
	// Change the upper variable by one
	// We need to increment the immediate upper level loop by one
	change = true;
   }
   else
	change = false; // Stop as there the upper levels of the loop are unaffected

   // We can perform any inner loop calculation here arrs[r]

   r=r-1;  // move to upper level of the loop

  }

 }
}
