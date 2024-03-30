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
		//�޲�Ĭ�Ϲ��캯��
		vector()
		{}

		//��һ��vector��Ĺ��캯�������������乹�캯������ģ��ĳ�Ա���������Ǻ���ģ�壩
		template <class InputIterator>//����ģ�壬�õ�����������������ĵ�����
		vector(InputIterator first, InputIterator last)
		{
			while (first != last)//��������������
			{
				push_back(*first);
				++first;
			}
		}

		//�ڶ���vector�Ĺ��캯��
		vector(size_t n, const T& val = T())//��Ȼ����������
		{
			reserve(n);
			for (size_t i = 0; i < n; i++)
			{
				push_back(val);
			}
		}

		//������vector�Ĺ��캯����C++11��������initializer_list���͵�����
		// vector<int> v1 = { 1,2,3,4,5,6,7,8,9,10 };
		vector(initializer_list<T> il)
		{
			reserve(il.size());//���ռ�
			for (auto& e : il)//��������Ϊ�˱��ⲻ��Ҫ�Ŀ��������÷�Χfor����Ϊinitializer_list��ģ��֧�ֵ�����
			{
				push_back(e);
			}
		}


		//Ϊ�˽��ͼ�������д�ĺ�������
		vector(int n, const T& val = T())//��Ȼ����������
		{
			reserve(n);
			for (int i = 0; i < n; i++)
			{
				push_back(val);
			}
		}


		//��������
		~vector()
		{
			delete[] _start;//����newʱ��[]��deleteʱҲ�ǵ���[]
			_start = _finish = _endofstorage = nullptr;
		}

		//��������
		vector(const vector<T>& v)
		{
			reserve(v.capacity());//�����push_back��Ҫ���ݣ���ǰΪv2���ٺ�v1һ����С�Ŀռ�Ϳ��Ա�����ܵĶ�ο��ٿռ�
			for (auto& e : v)//TΪstringʱ�����ڵ������ķ�Χfor��*it������e����������ȡ�����ݸ���e����ʱ��һ�����
			{
				push_back(e);
			}
		}

		//��������
		void swap(vector<T>& v)//�����ص�thisָ�루vector<T>this,vector<T>& v��
		{
			std::swap(_start, v._start);//����std���е�swap����������std������Զ���ʵ�ֵ�swap���ͻ���ֲ�����ƥ������
			std::swap(_finish, v._finish);//(this->_finish,v.)
			std::swap(_endofstorage, v._endofstorage);//���ڸ�ֵ�������ﻻ����v3�Ŀ�����v1֮������ݣ�v3�Ŀ�����=��������غ�������ʱ������������
		}

		//��ֵ����,=��������أ�v1 = v3��
		vector<T>& operator=(vector<T> v)//v��v3�Ŀ��������v��v3�����õĻ���ֵv1��ͬʱҲ��ı�v3
		{
			swap(v);
			return *this;
		}


		//������
		iterator begin()
		{
			return _start;
		}

		//������
		iterator end()
		{
			return _finish;
		}

		//const�汾������
		const_iterator begin()const
		{
			return _start;
		}

		//const�汾������
		const_iterator end()const
		{
			return _finish;
		}

		//[]���������
		T& operator[](size_t pos)
		{
			assert(pos < size());//Խ���ж�
			return _start[pos];//����posλ�õ�ֵ
		}

		//const�汾��[]���������
		const T& operator[](size_t pos) const
		{
			assert(pos < size());//Խ���ж�
			return _start[pos];//����posλ�õ�ֵ
		}

		//������Ч�ַ�����
		size_t size()const
		{
			return _finish - _start;//��ָ��������ص�����ָ��֮���Ԫ������
		}

		//����ռ��С
		size_t capacity()const
		{
			return _endofstorage - _start;
		}

		//���ݺ���reserve
		void reserve(size_t n)
		{
			if (n > capacity())//��n����capacityʱ�Ż��������
			{
				T* tmp = new T[n];//��ʱָ��tmp
				size_t old_size = size();//��ǰ����size()
				//memcpy(tmp, _start, size() * sizeof(T));//�¾ɿռ������ݵĿ���
				for (size_t i = 0; i < old_size; i++)
				{
					tmp[i] = _start[i];
				}

				delete[] _start;//ɾ��_startָ��ľɿռ�

				_start = tmp;//_startָ���¿ռ�
				_finish = tmp + old_size;//���´�ʱ_finish��ָ��,����Ĳ���дtmp + size��Ϊsize�ļ��㷽ʽ��_finish - _start�������+size�ͻ��ɣ�_finish = tmp + _finish - _start��tmp�ֵ���tmp�������ʽ�Ӿͻ���_finish = _finish
				_endofstorage = tmp + n;//���´�ʱ_endofstorage��ָ��
			}
		}

		//��vector��������ָ����Сn
		void resize(size_t n, const T& val = T())//��ҪΪval�ṩһ��ȱʡֵ����ȱʡֵ��һ����������T��������������Ҳ�������Զ������ͣ�������������int()�������int i = int() = 0��double i = double() = 0.0��char i = char() = \0��ĳָ������* = ĳָ������() = ��ָ��
		{
			if (n > size())//���n����������Ч���ݸ����ͽ��������ж�
			{
				reserve(n);//�жϵ���Ҫ��Ҫ����
				// ����
				while (_finish < _start + n)//ѭ�����루��ʹ����Ҫ����ҲҪѭ�����룩
				{
					*_finish = val;//��������
					++_finish;
				}
			}
			else//���������
			{
				_finish = _start + n;//���ı�ָ��ָ��ɾ����Ч����
			}
		}

		//β�壬���ô����ô��Σ��������val��������string����ʱ��Ҫȥ���������˷���Դ
		void push_back(const T& val)
		{
			//д��һ��
			//if (_finish == _endofstorage)
			//{
			//	reserve(capacity() == 0 ? 4 : capacity() * 2);
			//}
			//*_finish = val;
			//++_finish;

			//д������
			insert(end(), val);
		}

		//βɾ
		void pop_back()
		{
			//д��һ��
			//assert(empty());//βɾʱҪ�ж��Ƿ�Ϊ��
			//--_finish;

			//д������
			erase(--end());
		}

		//�Ƿ�Ϊ�յ��ж�
		void empty()
		{
			return _start == _finish;
		}

		//��ָ��λ�ò���
		void insert(iterator pos, const T& val)//�����pos��һ��ָ��
		{
			assert(pos >= _start);
			assert(pos <= _finish);//Խ���ж�

			//if (_finish == _endofstorage)//����ǰ�ȿ���ǰ�ռ乻��������ȥ����//�������ݷ�ʽ�ᵼ�µ�����ʧЧ���⣨���ٿռ��pos��ָ��ԭ���ľͿռ䣩����������뿴��ͼ��
			//{
			//	reserve(capacity() == 0 ? 4 : capacity() * 2);
			//}

			if (_finish == _endofstorage)//����ǰҪ����һ��pos�����λ�ã����ٿռ����posָ���¿ռ�����λ��
			{
				size_t len = pos - _start;//������λ��
				reserve(capacity() == 0 ? 4 : capacity() * 2);//�������Ǳض����ݵģ�ֻ�ǿ�Ҫ������

				//����pos
				pos = _start + len;
			}

			iterator it = _finish - 1;//it��һ��ָ��β��Ч�ַ���ָ��
			while (it >= pos)//ͼ��
			{
				*(it + 1) = *it;//����ƶ���ֵ�����и�ֵ
				--it;//ָ����ǰ��
			}
			*pos = val;//����ָ��λ�ô�ŵ�ֵ
			++_finish;//����ֻ�ǲ���һ��Ԫ�����Խ�_finish����ƶ�һ������
		}

		//��ָ��λ��ɾ��
		iterator erase(iterator pos)
		{
			assert(pos >= _start);
			assert(pos < _finish);//Խ���ж�
			iterator it = pos + 1;
			while (it < _finish)//ͼ��
			{
				*(it - 1) = *it;
				++it;
			}
			--_finish;
			return pos;
		}

	private:
		iterator _start = nullptr;//ָ��ǰ������Ч�ַ��е����ַ���ָ��
		iterator _finish = nullptr;//ָ��ǰ������Ч�ַ��е�β�ַ���һ��λ�õ�ָ��
		iterator _endofstorage = nullptr;//ָ������ռ�ĩβ��ָ��
	};

	//��ӡ����+����ģ��
	template<class T>
	void print_vector(const vector<T>& v)
	{
		for (size_t i = 0; i < v.size(); i++)//�򵥵�forѭ������
		{
			cout << v[i] << " ";
		}
		cout << endl;

		////vector<T>::const_iterator it = v.begin();//ʹ�õ�����
		//auto it = v.begin();//�ȼ��������д��������ʵ����д��������ʱҪ��vector֮ǰ����һ��typename������������ᱨһ���޷�ʶ��ı�ʶ������������Ϊ�������޷�����itǰ������ݵ����Ǳ����������ԣ�ʹ��typename��Ϊ�˸��߱�������һ������һ������
		//while (it != v.end())
		//{
		//	cout << *it << " ";
		//	++it;
		//}
		//cout << endl;

		//for (auto c : v)//��Χfor��ʹ�÷�Χfor�ǻ�ת��Ϊbegin��end������cbegin��cend��������������
		//{
		//	cout << c << " ";
		//}
		//cout << endl;
	}


	//vector���ܲ��Ժ���1��β�� + ָ��λ�ò��� + ɾ��ָ��λ�� + ����/����
	void test_vector1()
	{
		vector<int> v1;//ʵ��������
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);
		print_vector(v1);

		vector<double> v2;//ʵ����������
		v2.push_back(1.1);
		v2.push_back(2.2);
		v2.push_back(3.3);
		v2.push_back(4.4);
		v2.push_back(5.5);
		print_vector(v2);

		v2.insert(v2.begin(), 11.11);//ͷ��
		print_vector(v2);

		v2.insert(v2.begin() + 3, 22.22);//�ڵ�����λ�ò���
		print_vector(v2);

		v2.erase(v2.begin());//ͷɾ
		print_vector(v2);

		v2.erase(v2.begin() + 4);//ɾ��������λ��
		print_vector(v2);

		v1.resize(10);
		print_vector(v1);

		v1.resize(3);
		print_vector(v1);
	}

	//vector���ܲ��Ժ���2����������
	void test_vector2()
	{
		vector<int> v1;//ʵ��������
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);
		vector<int> v2(v1);
		print_vector(v2);
	}

	//vector���ܲ��Ժ���3����ֵ
	void test_vector3() 
	{
		vector<int> v1;//ʵ��������
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);


		vector<int> v3;//ʵ��������
		v3.push_back(10);
		v3.push_back(20);
		v3.push_back(30);
		v1 = v3;
		print_vector(v1);
	}

	//vector���ܲ��Ժ���4������������ģ��
	void test_vector4()
	{
		vector<int> v1;//ʵ��������
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);
		print_vector(v1);

		//vector<int> v2(v1.begin(), v1.end());//��v1��ȫ�����ݿ�����v2
		//print_vector(v2);

		vector<int> v2(v1.begin() + 1, v1.end() - 1);//��v1��ȫ�����ݳ�����β�����⿽����v2
		print_vector(v2);

		string str("abcd");
		vector<int> v3(str.begin(), str.end());//��string�����͵����������ʹ�ţ�������ʽת������ŵ����ַ�����ÿ���ַ���ASCII��ֵ����ӡ������Ҳ��
		print_vector(v3);
	}

	//vector���ܲ��Ժ���5�����캯����ѡ��
	void test_vector5()
	{
		vector<int> v1(10, 1);//Ԥ�ڵ��õڶ���Ĭ�Ϲ��캯����ʵ�ʵ��õ�һ�֣�����ͼ��
		print_vector(v1);

		/*	vector<int> v2(10u, 1);//x86������x64�������н����ͬ��Ե�����ﲻ����ʹ��
			print_vector(v2);*/

		vector<char> v3(10, 'a');
		print_vector(v3);
	}

	//vector���ܲ��Ժ���6��C++11֧�ֵ�������
	void test_vector6()//ͼ�� 
	{
	/*	initializer_list<int> x = { 1,2,3,4,5,6,7,8,9,10 };
		vector<int> v1 = { 1,2,3,4,5,6,7,8,9,10 };
		vector<int> v1 = x;
		cout << typeid(x).name() << endl;*/

		//��ʽ����ת�� + �Ż������� + �������� = ֱ�ӹ��죩����ֻ��{1,2,3,4,5,6,7,8,9,10}ʱ����û����ȷָ�����������ǡ������͡�����������ֵ��vector<int> x��������ʽ����ת��Ϊinitializer_list<int>����Ȼ�󣬵���initializer_list�Ĺ��캯������󽫹�����Ľ����ֵv1
		vector<int> v1 = { 1,2,3,4,5,6,7,8,9,10 };
		for (auto e : v1)
		{
			cout << e << " ";
		}
		cout << endl;

		
		//ֱ�ӹ��죨�����ֹ��췽ʽ��
		vector<int> v2({10,20,30,40,50,60,70,80,90,100 });
		for (auto e : v2)
		{
			cout << e << " ";
		}
		cout << endl;

		//C++11��֧��������д����
		//vector<int> v1{ 1,2,3,4,5,6,7,8,9,10 };
		//int i = 1;
		//int j = { 1 };
		//int k{ 1 };
	}

	//vector���ܲ��Ժ���7��memcpy����������,ͼ��
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

	//vector���ܲ��Ժ���8��insert���µ�����ʧЧ������ͼ��
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

		//insert��it��ʧЧ��
		vector<int>::iterator it = v1.begin() + 3;

		v1.insert(it, 40);
		print_vector(v1);

		//it = v1.begin()+3//����
		cout << *it << endl;//��ӡ���ֵ

		//�������������ʹ��it�������ÿջ��߽������£�it = v1.begin()+3
	}

	//vector���ܲ��Ժ���9��ɾ��ż����ͼ��
	void test_vector9()
	{
		////���һ��ֻ��һ��ż��
		//vector<int> v1;
		//v1.push_back(1);
		//v1.push_back(2);
		//v1.push_back(3);
		//v1.push_back(4);
		//v1.push_back(5);
		//
		////ɾ��ż��
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

		////���������������ͬż��
		//vector<int> v2;
		//v2.push_back(1);
		//v2.push_back(2);
		//v2.push_back(3);
		//v2.push_back(4);
		//v2.push_back(4);
		//v2.push_back(5);

		////ɾ��ż��
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

		////���������������ͬż��
		//vector<int> v3;
		//v3.push_back(1);
		//v3.push_back(2);
		//v3.push_back(3);
		//v3.push_back(4);
		//v3.push_back(4);
		//v3.push_back(5);

		////ɾ��ż��
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

		//���һ��ֻ��һ��ż��
		std::vector<int> v1;//ûʵ��vector�����ṩ��erase�������Ժ���������ֱ����std�ṩ��vector����
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);
		v1.push_back(5);

		//ɾ��ż��
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

		//���������������ͬż��
		std::vector<int> v2;
		v2.push_back(1);
		v2.push_back(2);
		v2.push_back(3);
		v2.push_back(4);
		v2.push_back(4);
		v2.push_back(5);

		//ɾ��ż��
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

		//���������������ͬż��
		std::vector<int> v3;
		v3.push_back(1);
		v3.push_back(2);
		v3.push_back(3);
		v3.push_back(4);
		v3.push_back(4);
		v3.push_back(5);

		//ɾ��ż��
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
