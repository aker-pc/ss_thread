#include "ss_thread_wrapper.h"
#include <iostream>

int main() {

  auto func = []() {
    std::cout << "info running" << std::endl;
  };

  // avoid to use following statement 
  // ss::wrapper::ss_thread_wrapper<int> data(std::thread(func)); 
  // ss::wrapper::ss_thread_wrapper<int> data((std::thread(func))); 
  ss::wrapper::ss_thread_wrapper<ss::wrapper::ss_detach> data_detach((std::thread(func))); 
  ss::wrapper::ss_thread_wrapper<ss::wrapper::ss_join>   data_join((std::thread(func))); 


  return 0;
}

