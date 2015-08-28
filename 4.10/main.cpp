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
  std::promise<int> prom;                      // ������ŵ 

  std::future<int> fut = prom.get_future();    // ��ȡ���� 

  std::thread th1 (print_int, std::ref(fut));  // ����������һ���µ��߳� 

  prom.set_value (10);                         // ���г�ŵ 
                                               // ���������get()����ͬ�� 
  th1.join();
  return 0;
}
