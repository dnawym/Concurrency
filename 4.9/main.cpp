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
	// ִ��3�������GUI�߳� 
	return ((shutdown--) < 0);
}
void get_and_process_gui_message()
{
	
}

// ����ʱ3�� 
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

// GUI�߳�
void gui_thread()
{
	// ѭ��ֱ���յ�������Ϣ 
    while(!gui_shutdown_message_received())
    {
    	std::cout << "gui_thread" << std::endl;
    	// ��ȡGUI��Ϣ������ 
        get_and_process_gui_message();
        std::packaged_task<void()> task;
        {
            std::lock_guard<std::mutex> lk(m);
            // ���������û������ִ����һ��ѭ�� 
            if(tasks.empty())
                continue;
            // �Ӷ����л�ȡ���� 
            task=std::move(tasks.front());
            tasks.pop_front();
        }
        // �ͷ�mutex��ִ������ 
        task();
    }
}

std::thread gui_bg_thread(gui_thread);

template<typename Func>
std::future<void> post_task_for_gui_thread(Func f)
{
	std::cout << "post_task_for_gui_thread" << std::endl;
	// ����packaged_task��������Ϊcountdown 
    std::packaged_task<void()> task(f);
    // �������ȡ���� 
    std::future<void> res=task.get_future();
    std::lock_guard<std::mutex> lk(m);
    // ��������뵽������ 
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

	// �ȴ���һ��GUI�̵߳�����ִ����� 
	f.get();
	std::cout << "got" << std::endl;
	
    gui_bg_thread.join();

}
