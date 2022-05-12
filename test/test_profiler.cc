#include "Profiler.h"

#include <iostream>

void function() {
  std::cout << "Function" << std::endl;
}

void blocking(){
  for (int i = 0; i < 100000; i++){
    std::cout << i << std::endl;
  }
}

void blocking2(){
  for (int i = 0; i < 100000; i++){
    std::cout << i << "\n";
  }
}


int main(){

  PROFILER_START("Test")
  // Same as
  // Instrumentor::Get().BeginSession("Test");

  {
      PROFILE("all")


    {
      PROFILE("timer1")
      // Same as
      //InstrumentationTimer timer("timer1");

      blocking();
    }

    {
      PROFILE("timer1")
      // Same as
      //InstrumentationTimer timer("timer1");

      blocking2();
    }
  }
  PROFILE_END
  // Same as
  // Instrumentor::Get().EndSession();


  return 1;
}
