#include <queue>
#include <thread>
#include <iostream>
#include <condition_variable>

std::condition_variable consumerFlag;
std::condition_variable producerFlag;
std::mutex mutex;




int counter = 0;
bool done = false;

std::queue<int> goods;

void producer()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "Starting producer..." << std::endl;

	std::unique_lock<std::mutex> lock(mutex);

	for (int i = 0; i < 500; ++i)
	{
		goods.push(i);
		counter++;
		std::cout << "good number: " << counter << " produced" << std::endl;
		consumerFlag.notify_one();
		producerFlag.wait(lock);
	}
	consumerFlag.notify_one();
	std::cout << "Finished producer..." << std::endl;

	done = true;
}

void consumer()
{
	std::cout << "Starting consumer..." << std::endl;

	while (!done)
	{
		std::cout << "Consumer waiting..." << std::endl;
		producerFlag.notify_one();
		std::unique_lock<std::mutex> lock(mutex);
		while (!goods.empty())
		{
			goods.pop();
			counter--;
			std::cout << "good number: " << counter + 1 << " consumed" << std::endl;

			producerFlag.notify_one();
			consumerFlag.wait(lock);
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
