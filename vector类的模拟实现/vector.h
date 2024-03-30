#pragma once
#include <iostream>
#include <assert.h>
#include<algorithm>
#include<vector>
using namespace std;



namespace bit
{
	template<class T>
	class vector
	{
	public:
		typedef T* iterator;
		typedef const T* const_iterator;
		//无参默认构造函数
		vector()
		{}

		//第一种vector类的构造函数：迭代器区间构造函数（类模板的成员函数可以是函数模板）
		template <class InputIterator>//运用模板，该迭代器可以是其它类的迭代器
		vector(InputIterator first, InputIterator last)
		{
			while (first != last)//遍历迭代器区间
			{
				push_back(*first);
				++first;
			}
		}

		//第二种vector的构造函数
		vector(size_t n, const T& val = T())//仍然是匿名对象
		{
			reserve(n);
			for (size_t i = 0; i < n; i++)
			{
				push_back(val);
			}
		}

		//第三种vector的构造函数（C++11）：对于initializer_list类型的数据
		// vector<int> v1 = { 1,2,3,4,5,6,7,8,9,10 };
		vector(initializer_list<T> il)
		{
			reserve(il.size());//开空间
			for (auto& e : il)//用引用是为了避免不必要的拷贝，能用范围for是因为initializer_list类模板支持迭代器
			{
				push_back(e);
			}
		}


		//为了解决图④问题而写的函数重载
		vector(int n, const T& val = T())//仍然是匿名对象
		{
			reserve(n);
			for (int i = 0; i < n; i++)
			{
				push_back(val);
			}
		}


		//析构函数
		~vector()
		{
			delete[] _start;//对于new时是[]的delete时也记得用[]
			_start = _finish = _endofstorage = nullptr;
		}

		//拷贝构造
		vector(const vector<T>& v)
		{
			reserve(v.capacity());//后面的push_back还要扩容，提前为v2开辟好v1一样大小的空间就可以避免可能的多次开辟空间
			for (auto& e : v)//T为string时，基于迭代器的范围for，*it被给予e，迭代器获取的数据给了e，这时是一个深拷贝
			{
				push_back(e);
			}
		}

		//交换函数
		void swap(vector<T>& v)//有隐藏的this指针（vector<T>this,vector<T>& v）
		{
			std::swap(_start, v._start);//调用std库中的swap函数，不加std会调用自定义实现的swap，就会出现参数不匹配问题
			std::swap(_finish, v._finish);//(this->_finish,v.)
			std::swap(_endofstorage, v._endofstorage);//对于赋值而言这里换的是v3的拷贝与v1之间的内容，v3的拷贝在=运算符重载函数结束时生命周期销毁
		}

		//赋值函数,=运算符重载（v1 = v3）
		vector<T>& operator=(vector<T> v)//v是v3的拷贝，如果v是v3的引用的话赋值v1的同时也会改变v3
		{
			swap(v);
			return *this;
		}


		//迭代器
		iterator begin()
		{
			return _start;
		}

		//迭代器
		iterator end()
		{
			return _finish;
		}

		//const版本迭代器
		const_iterator begin()const
		{
			return _start;
		}

		//const版本迭代器
		const_iterator end()const
		{
			return _finish;
		}

		//[]运算符重载
		T& operator[](size_t pos)
		{
			assert(pos < size());//越界判断
			return _start[pos];//返回pos位置的值
		}

		//const版本的[]运算符重载
		const T& operator[](size_t pos) const
		{
			assert(pos < size());//越界判断
			return _start[pos];//返回pos位置的值
		}

		//计算有效字符个数
		size_t size()const
		{
			return _finish - _start;//两指针相减返回的是两指针之间的元素数量
		}

		//计算空间大小
		size_t capacity()const
		{
			return _endofstorage - _start;
		}

		//扩容函数reserve
		void reserve(size_t n)
		{
			if (n > capacity())//当n大于capacity时才会进行扩容
			{
				T* tmp = new T[n];//临时指针tmp
				size_t old_size = size();//提前保存size()
				//memcpy(tmp, _start, size() * sizeof(T));//新旧空间中内容的拷贝
				for (size_t i = 0; i < old_size; i++)
				{
					tmp[i] = _start[i];
				}

				delete[] _start;//删除_start指向的旧空间

				_start = tmp;//_start指向新空间
				_finish = tmp + old_size;//更新此时_finish的指向,这里的不能写tmp + size因为size的计算方式是_finish - _start，如果是+size就会变成：_finish = tmp + _finish - _start，tmp又等于tmp所以最后式子就会变成_finish = _finish
				_endofstorage = tmp + n;//更新此时_endofstorage的指向
			}
		}

		//将vector数组扩大到指定大小n
		void resize(size_t n, const T& val = T())//需要为val提供一个缺省值，该缺省值是一个匿名对象，T可能是内置类型也可能是自定义类型，对于内置类型int()的情况，int i = int() = 0、double i = double() = 0.0、char i = char() = \0、某指针类型* = 某指针类型() = 空指针
		{
			if (n > size())//如果n大于数组有效数据个数就进行扩容判断
			{
				reserve(n);//判断到底要不要扩容
				// 插入
				while (_finish < _start + n)//循环插入（即使不需要扩容也要循环插入）
				{
					*_finish = val;//插入数据
					++_finish;
				}
			}
			else//否则就缩容
			{
				_finish = _start + n;//仅改变指针指向不删除有效数据
			}
		}

		//尾插，采用传引用传参，避免如果val的类型是string等类时还要去拷贝构造浪费资源
		void push_back(const T& val)
		{
			//写法一：
			//if (_finish == _endofstorage)
			//{
			//	reserve(capacity() == 0 ? 4 : capacity() * 2);
			//}
			//*_finish = val;
			//++_finish;

			//写法二：
			insert(end(), val);
		}

		//尾删
		void pop_back()
		{
			//写法一：
			//assert(empty());//尾删时要判断是否为空
			//--_finish;

			//写法二：
			erase(--end());
		}

		//是否为空的判断
		void empty()
		{
			return _start == _finish;
		}

		//在指定位置插入
		void insert(iterator pos, const T& val)//传入的pos是一个指针
		{
			assert(pos >= _start);
			assert(pos <= _finish);//越界判断

			//if (_finish == _endofstorage)//插入前先看当前空间够不够不够去扩容//这种扩容方式会导致迭代器失效问题（开辟空间后pos仍指向原来的就空间），具体情况请看，图②
			//{
			//	reserve(capacity() == 0 ? 4 : capacity() * 2);
			//}

			if (_finish == _endofstorage)//扩容前要保存一下pos的相对位置，开辟空间后令pos指向新空间的相对位置
			{
				size_t len = pos - _start;//存放相对位置
				reserve(capacity() == 0 ? 4 : capacity() * 2);//到这里是必定扩容的，只是看要扩多少

				//更新pos
				pos = _start + len;
			}

			iterator it = _finish - 1;//it是一个指向尾有效字符的指针
			while (it >= pos)//图①
			{
				*(it + 1) = *it;//向后移动数值并进行赋值
				--it;//指针向前走
			}
			*pos = val;//更新指定位置存放的值
			++_finish;//由于只是插入一个元素所以将_finish向后移动一个即可
		}

		//在指定位置删除
		iterator erase(iterator pos)
		{
			assert(pos >= _start);
			assert(pos < _finish);//越界判断
			iterator it = pos + 1;
			while (it < _finish)//图③
			{
				*(it - 1) = *it;
				++it;
			}
			--_finish;
			return pos;
		}

	private:
		iterator _start = nullptr;//指向当前数组有效字符中的首字符的指针
		iterator _finish = nullptr;//指向当前数组有效字符中的尾字符下一个位置的指针
		iterator _endofstorage = nullptr;//指向数组空间末尾的指针
	};

	//打印函数+函数模板
	template<class T>
	void print_vector(const vector<T>& v)
	{
		for (size_t i = 0; i < v.size(); i++)//简单的for循环遍历
		{
			cout << v[i] << " ";
		}
		cout << endl;

		////vector<T>::const_iterator it = v.begin();//使用迭代器
		//auto it = v.begin();//等价于上面的写法，但是实际在写上面类型时要在vector之前加上一个typename，否则编译器会报一个无法识别的标识符错误，这是因为编译器无法区分it前面的内容到底是变量还是烈性，使用typename是为了告诉编译器那一长串是一个类型
		//while (it != v.end())
		//{
		//	cout << *it << " ";
		//	++it;
		//}
		//cout << endl;

		//for (auto c : v)//范围for（使用范围for是会转换为begin和end而不是cbegin和cend等其它迭代器）
		//{
		//	cout << c << " ";
		//}
		//cout << endl;
	}


	//vector功能测试函数1：尾插 + 指定位置插入 + 删除指定位置 + 扩容/缩容
	void test_vector1()
	{
		vector<int> v1;//实例化整型
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);
		print_vector(v1);

		vector<double> v2;//实例化浮点型
		v2.push_back(1.1);
		v2.push_back(2.2);
		v2.push_back(3.3);
		v2.push_back(4.4);
		v2.push_back(5.5);
		print_vector(v2);

		v2.insert(v2.begin(), 11.11);//头插
		print_vector(v2);

		v2.insert(v2.begin() + 3, 22.22);//在第三个位置插入
		print_vector(v2);

		v2.erase(v2.begin());//头删
		print_vector(v2);

		v2.erase(v2.begin() + 4);//删除第三个位置
		print_vector(v2);

		v1.resize(10);
		print_vector(v1);

		v1.resize(3);
		print_vector(v1);
	}

	//vector功能测试函数2：拷贝构造
	void test_vector2()
	{
		vector<int> v1;//实例化整型
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);
		vector<int> v2(v1);
		print_vector(v2);
	}

	//vector功能测试函数3：赋值
	void test_vector3() 
	{
		vector<int> v1;//实例化整型
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);


		vector<int> v3;//实例化整型
		v3.push_back(10);
		v3.push_back(20);
		v3.push_back(30);
		v1 = v3;
		print_vector(v1);
	}

	//vector功能测试函数4：迭起器区间模板
	void test_vector4()
	{
		vector<int> v1;//实例化整型
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);
		print_vector(v1);

		//vector<int> v2(v1.begin(), v1.end());//将v1的全部内容拷贝至v2
		//print_vector(v2);

		vector<int> v2(v1.begin() + 1, v1.end() - 1);//将v1的全部内容除了首尾数据外拷贝至v2
		print_vector(v2);

		string str("abcd");
		vector<int> v3(str.begin(), str.end());//将string类类型的数据以整型存放，发生隐式转换，存放的是字符串中每个字符的ASCII码值，打印出来的也是
		print_vector(v3);
	}

	//vector功能测试函数5：构造函数的选择
	void test_vector5()
	{
		vector<int> v1(10, 1);//预期调用第二种默认构造函数，实际调用第一种，报错，图④
		print_vector(v1);

		/*	vector<int> v2(10u, 1);//x86环境和x64环境运行结果不同的缘故这里不予以使用
			print_vector(v2);*/

		vector<char> v3(10, 'a');
		print_vector(v3);
	}

	//vector功能测试函数6：C++11支持的新特性
	void test_vector6()//图⑤ 
	{
	/*	initializer_list<int> x = { 1,2,3,4,5,6,7,8,9,10 };
		vector<int> v1 = { 1,2,3,4,5,6,7,8,9,10 };
		vector<int> v1 = x;
		cout << typeid(x).name() << endl;*/

		//隐式类型转换 + 优化（构造 + 拷贝构造 = 直接构造）：在只有{1,2,3,4,5,6,7,8,9,10}时它并没有明确指定类型所以是“无类型”，当它被赋值给vector<int> x，它会隐式类型转换为initializer_list<int>类型然后，调用initializer_list的构造函数，最后将构造出的结果赋值v1
		vector<int> v1 = { 1,2,3,4,5,6,7,8,9,10 };
		for (auto e : v1)
		{
			cout << e << " ";
		}
		cout << endl;

		
		//直接构造（第三种构造方式）
		vector<int> v2({10,20,30,40,50,60,70,80,90,100 });
		for (auto e : v2)
		{
			cout << e << " ";
		}
		cout << endl;

		//C++11还支持这样的写法：
		//vector<int> v1{ 1,2,3,4,5,6,7,8,9,10 };
		//int i = 1;
		//int j = { 1 };
		//int k{ 1 };
	}

	//vector功能测试函数7：memcpy拷贝的问题,图⑥
	void test_vector7()
	{
		vector<string> v;
		v.push_back("11111");
		v.push_back("22222");
		v.push_back("33333");
		v.push_back("44444");
		v.push_back("55555");

		for (auto& e : v)
		{
			cout << e << " ";
		}
		cout << endl;
	} 

	//vector功能测试函数8：insert后导致迭代器失效（），图⑦
	void test_vector8()
	{
		vector<int> v1;
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);
		v1.push_back(5);
		v1.push_back(6);
		v1.push_back(7);
		v1.push_back(8);
		print_vector(v1);

		//insert后，it就失效了
		vector<int>::iterator it = v1.begin() + 3;

		v1.insert(it, 40);
		print_vector(v1);

		//it = v1.begin()+3//更新
		cout << *it << endl;//打印随机值

		//解决方案，不再使用it，将它置空或者将他更新，it = v1.begin()+3
	}

	//vector功能测试函数9：删除偶数，图⑧
	void test_vector9()
	{
		////情况一：只有一个偶数
		//vector<int> v1;
		//v1.push_back(1);
		//v1.push_back(2);
		//v1.push_back(3);
		//v1.push_back(4);
		//v1.push_back(5);
		//
		////删除偶数
		//vector<int>::iterator it1 = v1.begin();
		//while (it1 != v1.end())
		//{
		//	if (*it1 % 2 == 0)
		//	{
		//		v1.erase(it1);
		//	}
		//	++it1;
		//}
		//print_vector(v1);

		////情况二：有两个相同偶数
		//vector<int> v2;
		//v2.push_back(1);
		//v2.push_back(2);
		//v2.push_back(3);
		//v2.push_back(4);
		//v2.push_back(4);
		//v2.push_back(5);

		////删除偶数
		//vector<int>::iterator it2 = v2.begin();
		//while (it2 != v2.end())
		//{
		//	if (*it2 % 2 == 0)
		//	{
		//		v2.erase(it2);
		//	}
		//	++it2;
		//}
		//print_vector(v2);

		////情况三：有两个不同偶数
		//vector<int> v3;
		//v3.push_back(1);
		//v3.push_back(2);
		//v3.push_back(3);
		//v3.push_back(4);
		//v3.push_back(4);
		//v3.push_back(5);

		////删除偶数
		//vector<int>::iterator it3 = v3.begin();
		//while (it2 != v3.end())
		//{
		//	if (*it3 % 2 == 0)
		//	{
		//		v3.erase(it3);
		//	}
		//	++it3;
		//}
		//print_vector(v3);

		//情况一：只有一个偶数
		std::vector<int> v1;//没实现vector库中提供的erase功能所以后续举例子直接用std提供的vector即可
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);
		v1.push_back(5);

		//删除偶数
		std::vector<int>::iterator it1 = v1.begin();
		while (it1 != v1.end())
		{
			if (*it1 % 2 == 0)
			{
				it1 = v1.erase(it1);
			}
			else
			{
				++it1;
			}
		}

		for (auto e : v1)
		{
			cout << e << " ";
		}
		cout << endl;

		//情况二：有两个相同偶数
		std::vector<int> v2;
		v2.push_back(1);
		v2.push_back(2);
		v2.push_back(3);
		v2.push_back(4);
		v2.push_back(4);
		v2.push_back(5);

		//删除偶数
		std::vector<int>::iterator it2 = v2.begin();
		while (it2 != v2.end())
		{
			if (*it2 % 2 == 0)
			{
				it2 = v2.erase(it2);
			}
			else
			{
				++it2;
			}
		}

		for (auto e : v2)
		{
			cout << e << " ";
		}
		cout << endl;

		//情况三：有两个不同偶数
		std::vector<int> v3;
		v3.push_back(1);
		v3.push_back(2);
		v3.push_back(3);
		v3.push_back(4);
		v3.push_back(4);
		v3.push_back(5);

		//删除偶数
		std::vector<int>::iterator it3 = v3.begin();
		while (it3 != v3.end())
		{
			if (*it3 % 2 == 0)
			{
				it3 = v3.erase(it3);
			}
			else
			{
				++it3;
			}
		}

		for (auto e : v3)
		{
			cout << e << " ";
		}
		cout << endl;

	
	}
}
