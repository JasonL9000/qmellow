#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include "file.h"
#include "translate.h"
#include "utils.h"

using namespace std;
using namespace qmellow;

int main(int argc, char *argv[]) {
  int result = EXIT_SUCCESS;
  try {
    vector<file_t> files;
    for (int i = 1; i < argc; ++i) {
      files.emplace_back(argv[i]);
    }
    string line;
    for (;;) {
      cout << "qmellow: ";
      getline(cin, line);
      try {
        auto expr = translate(line.c_str());
        for (const auto &file: files) {
          cout << file.get_path() << ':' << endl;
          expr->eval(file).write(cout);
        }
      } catch (exception &ex) {
        cout << ex.what() << endl;
      }
    }
  } catch (exception &ex) {
    cerr << ex.what() << endl;
    result = EXIT_FAILURE;
  }
  return result;
}
