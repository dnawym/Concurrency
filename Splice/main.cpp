#include <iostream>
#include <list>

using namespace std;

int main(int argc, char** argv) 
{
	list<int> mylist1, mylist2;
	list<int>::iterator it;
	
	for (int i = 1; i <= 4; ++i)
	{
		mylist1.push_back(i);
	}
	
	for (int i = 1; i <= 3; ++i)
	{
		mylist2.push_back(i * 10);
	}
	
	it = mylist1.begin();
	++it;
	
	// 将mylist2的数据转移到mylist1中的第一个元素之后
	// 执行之后，mylist1: 1 10 20 30 2 3 4
	// mylist2为空
	// it仍然指向2（第五个元素） 
	mylist1.splice(it, mylist2);
	
	for (auto it = mylist1.begin(); it != mylist1.end(); ++it)
	{
		cout << ' ' << *it;
	}
	cout << endl;
	
	// 将mylist1中it指向的元素（2）转移到mylist2中
	// 执行之后，mylist1: 1 10 20 30 3 4
	// mylist2: 2
	// it非法 
	mylist2.splice(mylist2.begin(), mylist1, it);
	
	for (auto it = mylist2.begin(); it != mylist2.end(); ++it)
	{
		cout << ' ' << *it;
	}
	cout << endl;	
	
	it = mylist1.begin();
	advance(it, 3);			// it指向30
	
	// 将mylist1的30 3 4移到mylist1的头部
	// 执行之后，mylist1：30 3 4 1 10 20 
	mylist1.splice(mylist1.begin(), mylist1, it, mylist1.end());
	
	for (auto it = mylist1.begin(); it != mylist1.end(); ++it)
	{
		cout << ' ' << *it;
	}
	cout << endl;	
	
	return 0;
}
