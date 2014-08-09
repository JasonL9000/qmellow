#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include "repl.h"

using namespace std;
using namespace qmellow;

/* The application itself. */
class app_t final {
  public:

  /* Process the command line and prepare to run. */
  app_t(int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) {
      paths.emplace_back(argv[i]);
    }
    cout << "loaded " << (argc - 1) << " file(s)" << endl;
  }

  /* Run. */
  int operator()() {
    return (!paths.empty()) ? (repl_t(paths)()) : EXIT_SUCCESS;
  }

  private:

  /* The paths of the subject files to load. */
  vector<string> paths;

};  // app_t

int main(int argc, char *argv[]) {
  int result;
  try {
    result = app_t(argc, argv)();
  } catch (const exception &ex) {
    cerr << ex.what() << endl;
    result = EXIT_FAILURE;
  }
  return result;
}
