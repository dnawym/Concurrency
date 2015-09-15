#include <iostream>
#include <list>
#include <algorithm>
#include <future>
#include <numeric>
#include <chrono>
#include <cstdlib>
#include <ctime>

template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
    if(input.empty())
    {
        return input;
    }
    std::list<T> result;
	// ʹ��splice������input��ȡ����һ��Ԫ�أ�����result�����У���Ϊ��λ��
    result.splice(result.begin(),input,input.begin());
	
	// Ϊ�˼��ٲ���Ҫ�Ŀ�������ȡ��λ��������
    T const& pivot=*result.begin();
	
	// ʹ��partition��������������Ԫ�ؽ��л��֣�������λ���ĵ�����λ��
	// С����λ����Ԫ���ڵ�����λ�õ���ߣ����ڵ�����λ����Ԫ���ڵ������ұߣ�����������λ�ñ���
    auto divide_point=std::partition(input.begin(),input.end(),
        [&](T const& t){return t<pivot;});
    
	std::list<T> lower_part;
	// �ݹ����sequential_quick_sort���������������
	// ��С����λ����Ԫ�ط���lower_part�����У����ڵ�����λ����Ԫ������input������
    lower_part.splice(lower_part.end(),input,input.begin(),divide_point);
	
	// ʹ��move�������Ա��⿽��
    auto new_lower(
        sequential_quick_sort(std::move(lower_part)));
    auto new_higher(
        sequential_quick_sort(std::move(input)));
		
	// ʹ��splice������װ������new_higher�ŵ�result��β������λ��֮��
    result.splice(result.end(),new_higher);
	// ��new_lower�ŵ�result��ͷ������λ��֮ǰ��
    result.splice(result.begin(),new_lower);
    return result;
}

static int thread_counter = 1;

template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input)
{
	//std::cout << "parallel_quick_sort list size = " << input.size() << std::endl;
	thread_counter++;
    if(input.empty())
    {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(),input,input.begin());
    T const& pivot=*result.begin();
    auto divide_point=std::partition(input.begin(),input.end(),
        [&](T const& t){return t<pivot;});
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(),input,input.begin(),
        divide_point);
    
    if (lower_part.size() <= 10000)
    {
    	auto new_lower(
        	sequential_quick_sort(std::move(lower_part)));
        	
        if (input.size() > 1)
        {
        	auto new_higher(
        	parallel_quick_sort(std::move(input)));
        	
        	result.splice(result.end(),new_higher);
		}
		else
		{
			result.splice(result.end(), input);
		}

        result.splice(result.begin(),new_lower);
	}
	else
	{
		std::future<std::list<T> > new_lower(
			std::async(&parallel_quick_sort<T>,std::move(lower_part)));
        if (input.size() > 1)
        {
        	auto new_higher(
        	parallel_quick_sort(std::move(input)));
        	
        	result.splice(result.end(),new_higher);
		}
		else
		{
			result.splice(result.end(), input);
		}
    	result.splice(result.begin(),new_lower.get());
	}
    
    return result;
}



int main(int argc, char** argv) 
{
	system("PAUSE");
	
	std::list<int> a_list, b_list;
	std::srand(std::time(0)); // use current time as seed for random generator
    	
	for (int i = 0; i < 5000000; ++i)
	{
		int value = std::rand();
		a_list.push_back(value);
		b_list.push_back(value);
	}
	
	//std::list<int> b_list(a_list);
	std::cout << "b_list.size() = " << b_list.size() << std::endl;
	
    // record start time
    auto start = std::chrono::system_clock::now();
    // do some work
    a_list = parallel_quick_sort(a_list);
    
    // record end time
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end-start;
    std::cout << "Time to sort a list of " 
              << a_list.size() << " ints : " << diff.count() << " s\n";
              
    std::cout << "Thread counter is " << thread_counter << std::endl;
	
		
    // record start time
    auto startb = std::chrono::system_clock::now();

    // do some work
    b_list = sequential_quick_sort(b_list);
    
    // record end time
    auto endb = std::chrono::system_clock::now();
    std::chrono::duration<double> diffb = endb-startb;
    std::cout << "Time to sort b list of " 
              << b_list.size() << " ints : " << diffb.count() << " s\n";
              
    auto itrA = a_list.begin();
    auto itrB = b_list.begin();
    for (int i = 0; i < 10; i++)
    {
    	std::cout << *(itrA++) << " ";
	}
	std:: cout << std::endl;

    for (int i = 0; i < 10; i++)
    {
    	std::cout << *(itrB++) << " ";
	}
	
	return 0;
}
