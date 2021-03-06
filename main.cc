#include "bmc_exercise.h"
#include "mathsat_interface.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>

int main(int argc,char* argv[]) {
  try {
    if(argc < 2) {
      std::cerr << "Please supply an unwinding limit." << std::endl;
      return -1;
    }
    IMathSAT iface("QF_AUFLIA");
    BMC bmc(STDIN_FILENO,iface);
    int limit = atoi(argv[1]);
    for(int i = 0; i < limit; i++) {
      std::cout << "Level " << i << "..." << std::endl;
      bmc.extend();
      bool res = bmc.check();
      if(res) {
	std::cout << "Error found." << std::endl;
	return 1;
      }
    }
    IMathSAT::term reach;
    IMathSAT::term new_reach;
    check_result result;
    bmc.extend();
    if(bmc.check(nullptr,&reach)==INCOMPLETE) {
      std::cout << "Error found." << std::endl;
      return 1;
    }
    std::cout << "No error found." << std::endl;
    while(true) {
      switch(bmc.check(&reach,&new_reach)) {
      case INCOMPLETE:
	std::cout << "(Not complete)" << std::endl;
	return 2;
      case COMPLETE:
	std::cout << "(Complete)" << std::endl;
	return 0;
      default:
	reach = new_reach;
	break;
      }
    }
    return 0;
  } catch(std::string msg) {
    std::cerr << "Internal error: " << msg << std::endl;
    return -1;
  }
}
