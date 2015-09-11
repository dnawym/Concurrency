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
	// 使用splice方法从input中取出第一个元素，放入result链表中，作为中位数
    result.splice(result.begin(),input,input.begin());
	
	// 为了减少不必要的拷贝，获取中位数的引用
    T const& pivot=*result.begin();
	
	// 使用partition方法，将链表中元素进行划分，返回中位数的迭代器位置
	// 小于中位数的元素在迭代器位置的左边，大于等于中位数的元素在迭代器右边（包括迭代器位置本身）
    auto divide_point=std::partition(input.begin(),input.end(),
        [&](T const& t){return t<pivot;});
    
	std::list<T> lower_part;
	// 递归调用sequential_quick_sort来对链表进行排序
	// 将小于中位数的元素放入lower_part链表中，大于等于中位数的元素留在input链表中
    lower_part.splice(lower_part.end(),input,input.begin(),divide_point);
	
	// 使用move操作可以避免拷贝
    auto new_lower(
        sequential_quick_sort(std::move(lower_part)));
    auto new_higher(
        sequential_quick_sort(std::move(input)));
		
	// 使用splice重新组装链表，将new_higher放到result的尾部（中位数之后）
    result.splice(result.end(),new_higher);
	// 将new_lower放到result的头部（中位数之前）
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
