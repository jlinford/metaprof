#include <iostream>

#include "ForkExecChild.hpp"
#include "ProcStatProbe.hpp"

using namespace std;


int main(int argc, char ** argv)
{
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " cmd [args...]" << endl;
    return 0;
  }

  IChildProcess * child = new ForkExecChild;
  child->AddProbe<ProcStatProbe>();

  int retval = child->Create(argc, argv);

  child->ReportToCSVFile();

  return retval;
}
