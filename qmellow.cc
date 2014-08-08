#include <iostream>
#include "translate.h"
#include "utils.h"

using namespace std;
using namespace qmellow;

int main(int argc, char *argv[]) {
  translate(read_whole_file(argv[1]).c_str())->pretty_print(cout);
  cout << endl;
  return 0;
}
