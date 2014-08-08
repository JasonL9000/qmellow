#include <iostream>
#include "translate.h"

using namespace std;
using namespace qmellow;

int main(int, char *[]) {
  translate("#baz or 'foo'")->pretty_print(cout);
  cout << endl;
  return 0;
}
