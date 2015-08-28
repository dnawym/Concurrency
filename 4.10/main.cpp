// promise example
#include <iostream>       // std::cout
#include <functional>     // std::ref
#include <thread>         // std::thread
#include <future>         // std::promise, std::future

void print_int (std::future<int>& fut) {
  int x = fut.get();
  std::cout << "value: " << x << '\n';
}

int main ()
{
  std::promise<int> prom;                      // 创建承诺 

  std::future<int> fut = prom.get_future();    // 获取期望 

  std::thread th1 (print_int, std::ref(fut));  // 将期望交给一个新的线程 

  prom.set_value (10);                         // 履行承诺 
                                               // 与对期望的get()操作同步 
  th1.join();
  return 0;
}
