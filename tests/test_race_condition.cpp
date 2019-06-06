//
// Created by efarhan on 1/10/19.
//

#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>

static std::mutex mtx;

void MrPlus(int* prout)
{
  for(int i = 0; i < 1000; i++)
    {
      std::lock_guard<std::mutex> lock(mtx);
      (*prout) += 4;
    }

}


void MrMinus(int* prout)
{
  for(int i = 0; i < 1000; i++)
    {

      std::lock_guard<std::mutex> lock(mtx);
      (*prout) -= 4;
    }
}

int main()
{
  int prout = 0;
  std::thread t1(MrPlus, &prout);
  std::thread t2(MrMinus, &prout);
  std::thread t3(MrPlus, &prout);
  std::thread t4(MrMinus, &prout);
  std::thread t5(MrPlus, &prout);
  std::thread t6(MrMinus, &prout);

  t1.join ();
  t2.join ();
  t3.join ();
  t4.join ();
  t5.join ();
  t6.join ();

  std::cout << "Prout: " << (prout) <<"\n";
}

