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


template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input)
{
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
    std::future<std::list<T> > new_lower(
        std::async(&parallel_quick_sort<T>,std::move(lower_part)));
    auto new_higher(
        parallel_quick_sort(std::move(input)));
    result.splice(result.end(),new_higher);
    result.splice(result.begin(),new_lower.get());
    return result;
}



int main(int argc, char** argv) 
{
	std::list<int> a_list;
	std::srand(std::time(0)); // use current time as seed for random generator
    	
	for (int i = 0; i < 5000; ++i)
	{
		a_list.push_back(std::rand());
	}
	
	std::list<int> b_list(a_list);
	std::cout << "b_list.size() = " << b_list.size() << std::endl;
	
    // record start time
    auto start = std::chrono::system_clock::now();
    // do some work
    parallel_quick_sort(a_list);
    
    // record end time
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end-start;
    std::cout << "Time to sort a list of " 
              << a_list.size() << " ints : " << diff.count() << " s\n";
	
		
    // record start time
    start = std::chrono::system_clock::now();
    std::cout << start << std::endl;
    // do some work
    sequential_quick_sort(b_list);
    
    // record end time
    end = std::chrono::system_clock::now();
    diff = end-start;
    std::cout << "Time to sort a list of " 
              << a_list.size() << " ints : " << diff.count() << " s\n";
	
	return 0;
}
