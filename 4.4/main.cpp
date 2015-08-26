#include <iostream>

#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>

template<typename T>
class threadsafe_queue
{
private:
    std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
public:
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }
    
    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=data_queue.front();
        data_queue.pop();
    }
};


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
	
	data_chunk() : m_data('q') {
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

threadsafe_queue<data_chunk> data_queue;	// 用于线程间通信的队列 
// mutex和条件变量都已放入到threadsafe_queue中，去掉相应的全局变量 

void data_preparation_thread()
{
    while(more_data_to_prepare())
    {
    	std::cout << "data_preparation_thread while" << std::endl;
        data_chunk const data=prepare_data();
        // 数据准备好后，将数据插入队列之中，不在需要额外的同步操作 
        data_queue.push(data);
    }
}

void data_processing_thread()
{
    while(true)
    {
    	std::cout << "data_processing_thread while" << std::endl;
        data_chunk data;
        // wait_and_pop实现了相应的“等待”操作 
        data_queue.wait_and_pop(data);
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

