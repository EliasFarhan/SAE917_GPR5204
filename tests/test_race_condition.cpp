#include <vector>
#include <thread>
#include <iostream>
#include <benchmark/benchmark.h>
#include <misc.h>

const size_t total = 10000;

class Wallet
{
    int mMoney;
public:
    Wallet()
        : mMoney(0)
    {
    }
    int GetMoney()
    {
        return mMoney;
    }
    NOINLINE void IncMoney();

    void AddMoney(size_t money)
    {
        for (size_t i = 0; i < money; ++i)
        {
            IncMoney();
        }
    }
};
void Wallet::IncMoney()
{
mMoney++;
}

int TestMultithreadedWallet(int totalThread)
{
    Wallet walletObject;
    std::vector<std::thread> threads;
    for (int i = 0; i < totalThread; ++i)
    {
        threads.push_back(std::thread(&Wallet::AddMoney, &walletObject, total));
    }

    for (int i = 0; i < threads.size(); i++)
    {
        threads.at(i).join();
    }
    return walletObject.GetMoney();
}

int main()
{

    int val = 0;

    for(int totalThread = 1; totalThread < 8; totalThread++)
    {
        int errorCount = 0;
        for (size_t k = 0; k < total; k++)
        {
            if ((val = TestMultithreadedWallet(totalThread)) != total*totalThread)
            {
                std::cout << "Error at count = " << k << " Money in Wallet = " << val << std::endl;
                errorCount++;
            }

        }

        std::cout << "Total error count: " << errorCount << " over 1000 with threads number: "<<totalThread<<"\n";
    }
    return 0;
}
