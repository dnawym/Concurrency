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
	
	// ��mylist2������ת�Ƶ�mylist1�еĵ�һ��Ԫ��֮��
	// ִ��֮��mylist1: 1 10 20 30 2 3 4
	// mylist2Ϊ��
	// it��Ȼָ��2�������Ԫ�أ� 
	mylist1.splice(it, mylist2);
	
	for (auto it = mylist1.begin(); it != mylist1.end(); ++it)
	{
		cout << ' ' << *it;
	}
	cout << endl;
	
	// ��mylist1��itָ���Ԫ�أ�2��ת�Ƶ�mylist2��
	// ִ��֮��mylist1: 1 10 20 30 3 4
	// mylist2: 2
	// it�Ƿ� 
	mylist2.splice(mylist2.begin(), mylist1, it);
	
	for (auto it = mylist2.begin(); it != mylist2.end(); ++it)
	{
		cout << ' ' << *it;
	}
	cout << endl;	
	
	it = mylist1.begin();
	advance(it, 3);			// itָ��30
	
	// ��mylist1��30 3 4�Ƶ�mylist1��ͷ��
	// ִ��֮��mylist1��30 3 4 1 10 20 
	mylist1.splice(mylist1.begin(), mylist1, it, mylist1.end());
	
	for (auto it = mylist1.begin(); it != mylist1.end(); ++it)
	{
		cout << ' ' << *it;
	}
	cout << endl;	
	
	return 0;
}
