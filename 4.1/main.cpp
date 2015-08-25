#include <iostream>

#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>

static bool more = true;

bool more_data_to_prepare()
{
	return more;
}

struct data_chunk
{
	char m_data = 'q';
	data_chunk(char c) : m_data(c) {
	}
};

data_chunk prepare_data()
{
	std::cout << "data_preparation_thread prepare_data"<< std::endl;
	char x = 'q';
	std::cin >> x;
	if (x == 'q')
	{
		more = false;
	}
    return data_chunk(x);
}

void process(data_chunk& data)
{
	std::cout << "process data: " << data.m_data << std::endl;
}

bool is_last_chunk(data_chunk& data)
{
	if (data.m_data == 'q')
    {
    	return true;
	}
	
	return false;
}

std::mutex mut;
std::queue<data_chunk> data_queue;
std::condition_variable data_cond;

void data_preparation_thread()
{
    while(more_data_to_prepare())
    {
    	std::cout << "data_preparation_thread while" << std::endl;
        data_chunk const data=prepare_data();
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);
        std::cout << "data_preparation_thread notify_one" << std::endl;
        data_cond.notify_one();
    }
}

void data_processing_thread()
{
    while(true)
    {
    	std::cout << "data_processing_thread while" << std::endl;
        std::unique_lock<std::mutex> lk(mut);
        std::cout << "data_processing_thread before wait" << std::endl;
        data_cond.wait(lk,[]{return !data_queue.empty();});
        std::cout << "data_processing_thread pass wait" << std::endl;
        data_chunk data=data_queue.front();
        data_queue.pop();
        lk.unlock();
        std::cout << "data_processing_thread process data" << std::endl;
        process(data);
        if(is_last_chunk(data))
            break;
    }
}

int main()
{
	std::cout << "main" << std::endl;
    std::thread t1(data_preparation_thread);
    std::thread t2(data_processing_thread);
    
    t1.join();
    t2.join();
}

