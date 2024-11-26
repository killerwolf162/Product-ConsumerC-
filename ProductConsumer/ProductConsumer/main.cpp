#include <queue>
#include <thread>
#include <chrono>
#include <iostream>
#include <condition_variable>

std::condition_variable flag;
std::mutex mutex;

int counter = 0;
bool notified = false;

std::queue<int> goods;

void producer()
{
	std::cout << "Starting producer..." << std::endl;

	std::this_thread::sleep_for(std::chrono::seconds(4));

	for (int i = 0; i < 500; ++i)
	{
		goods.push(i);
		counter++;
		std::cout << "produced good number: " << counter << std::endl;
	}

	flag.notify_all();
	notified = true;


	std::cout << "Finished producer..." << std::endl;
}

void consumer()
{
	std::unique_lock<std::mutex> lock(mutex);		
	while (!notified)
	{		
		std::cout << "Starting consumer..." << std::endl;
		while (!goods.empty())
		{
			goods.pop();
			counter--;
			std::cout << "consumed good number: " << counter + 1 << std::endl;
		}
		
	}	
	std::cout << "Finished consumer..." << std::endl;	
}

int main()
{

	counter = 0;
	std::thread producerThread(producer);
	std::thread consumerThread(consumer);	
	
	producerThread.join();
	consumerThread.join();
	
	std::cout << "Net: " << counter << " " << goods.size() << std::endl;
}
