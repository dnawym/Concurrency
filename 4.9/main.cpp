#include <iostream>
#include <deque>
#include <mutex>
#include <future>
#include <thread>
#include <utility>
#include <chrono>

std::mutex m;
std::deque<std::packaged_task<void()> > tasks;

static int shutdown = 3;

bool gui_shutdown_message_received()
{
	// 执行3次则结束GUI线程 
	return ((shutdown--) < 0);
}
void get_and_process_gui_message()
{
	
}

// 倒计时3秒 
void countdown()
{
	int from = 3, to = 0;
  	for (int i=from; i!=to; --i)
	{
    	std::cout << i << '\n';
    	std::this_thread::sleep_for(std::chrono::seconds(1));
  	}
  	std::cout << "Lift off!\n";
}

// GUI线程
void gui_thread()
{
	// 循环直到收到结束消息 
    while(!gui_shutdown_message_received())
    {
    	std::cout << "gui_thread" << std::endl;
    	// 获取GUI消息并处理 
        get_and_process_gui_message();
        std::packaged_task<void()> task;
        {
            std::lock_guard<std::mutex> lk(m);
            // 如果队列中没有任务，执行下一次循环 
            if(tasks.empty())
                continue;
            // 从队列中获取任务 
            task=std::move(tasks.front());
            tasks.pop_front();
        }
        // 释放mutex，执行任务 
        task();
    }
}

std::thread gui_bg_thread(gui_thread);

template<typename Func>
std::future<void> post_task_for_gui_thread(Func f)
{
	std::cout << "post_task_for_gui_thread" << std::endl;
	// 创建packaged_task，任务函数为countdown 
    std::packaged_task<void()> task(f);
    // 从任务获取期望 
    std::future<void> res=task.get_future();
    std::lock_guard<std::mutex> lk(m);
    // 将任务加入到队列中 
    tasks.push_back(std::move(task));
    return res;
}

int main()
{
	std::cout << "main" << std::endl;
	
	std::future<void> f = post_task_for_gui_thread(countdown);
	std::cout << "posted 1" << std::endl;
	
	post_task_for_gui_thread(countdown);
	std::cout << "posted 2" << std::endl;

	// 等待第一个GUI线程的任务执行完成 
	f.get();
	std::cout << "got" << std::endl;
	
    gui_bg_thread.join();

}
