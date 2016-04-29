#include<iostream>
#include<string>
#include<cassert>
using namespace std;

//enum Status
//{
//	EMPTY,
//	DELETE,
//	EXITS,
//};
//
//template<class K>
//class HashTable
//{
//public:
//	HashTable()
//		:_table(NULL)
//		, _size(0)
//		, _capacity(0)
//		, _status(0)
//	{}
//	HashTable(size_t capacity)
//		: _size(0)
//		, _capacity(capacity)
//		, _table(new K[capacity])
//		, _status(new K[capacity])
//	{
//		for (size_t i = 0; i < _capacity; ++i)
//		{
//			_table[i] = 0;
//			_status[i] = EMPTY;
//		}
//	}
//	~HashTable()
//	{
//		if (_table)
//		{
//			delete[] _table;
//			delete[] _status;
//		}
//	}
//public:
//	bool Insert(const K& key)   //线性检索
//	{
//		size_t index = HashValue(key);
//		//检查负载因子，是否增容
//		if (_size * 10 / _capacity >= 8)
//		{
//			HashTable<K> newTable(2 * _capacity);
//			for(size_t i = 0; i < _capacity; ++i)
//			{
//				if(_table[i] == EXITS)
//				{
//					newTable.Insert(_table[i]);
//				}
//			}
//			this->Swap(newTable);
//		}
//		while (_status[index] == EXITS)
//		{
//			++index;
//			if (index == _capacity)
//			{
//				index = 0;
//			}
//		}
//		_table[index] = key;
//		_status[index] = EXITS;
//		++_size;
//		return true;
//	}
//
//	bool Remove(const K& key)
//	{
//		size_t index = HashValue(key);
//		while (_status[index] != EMPTY)
//		{
//			if (_table[index] == key)
//			{
//				if (_status[index] == EXITS)
//				{
//					_status[index] = DELETE;
//					--_size;
//					return true;
//				}
//				return false;
//			}
//			++index;
//			if (index == _capacity)
//			{
//				index = 0;
//			}
//		}
//		return false;
//	}
//
//	size_t HashValue(const K& key)
//	{
//		return (key%_capacity);
//	}
//	
//	void Print()
//	{
//		size_t index = 0;
//		while (index < _capacity)
//		{
//			cout << _status[index] << ':' << _table[index] << " ";
//			++index;
//		}
//		cout << endl;
//	}
//protected:
//	void Swap(HashTable<K> ht1)
//	{
//		swap(_table, ht1._table);
//		swap(_status, ht1._status);
//		swap(_size, ht1._size);
//		swap(_cpacity, ht1._capacity);
//	}
//private:
//	K* _table;
//	size_t _size;
//	size_t _capacity;
//	K* _status;
//
//};


enum Status
{
	EMPTY,
	DELETE,
	EXITS,
};

template<class K,class V>
class KeyValueNodes
{
public:
	KeyValueNodes()
		:_key(0)
		, _value(0)
	{}
	KeyValueNodes(const K& key, const V& value)
		:_key(key)
		, _value(value)
	{}
	KeyValueNodes operator=(KeyValueNodes<K, V> kv)
	{
		_key = kv._key;
		_value = kv._value;
	}
	K _key;
	V _value;
};

size_t BKDRHash(const char* str)
{
	assert(str);
	size_t seek = 131;
	size_t hash = 0;
	while (*str)
	{
		hash = hash*seek + abs(*str++);
	}
	return hash;
}

template<class K>
struct HashFuncer
{
	size_t operator()(const K& key)
	{
		return key;
	}
};

template<>
struct HashFuncer<string>
{
	size_t operator()(const string& key)
	{
		return BKDRHash(key.c_str());
	}
};

template < class K, class V,class HashFun=HashFuncer<K>>
class HashTables
{
	typedef KeyValueNodes<K,V> KVNode;
public:
	HashTables()
		:_table(NULL)
		, _size(0)
		, _capacity(0)
		, _status(0)
	{}
	HashTables(size_t capacity)
		:_capacity(capacity)
		,_table(new KVNode[capacity])
		, _size(0)
		, _status(new Status[capacity])
	{
		for (size_t i = 0; i < _capacity; ++i)
		{
			_status[i] = EMPTY;
		}
	}
	~HashTables()
	{
		if (_table)
		{
			delete[] _table;
			delete[] _status;
		}
	}
public:

	bool Insert(const K& key, const V& value)   //二次探测
	{
		size_t index = HashFunc(key);
		//检查负载因子，是否增容
		if (_size * 10 / _capacity >= 8)
		{
			HashTables newTable(2 * _capacity);
			for (size_t i = 0; i < _capacity; ++i)
			{
				if (_status[i] == EXITS)
				{
					newTable.Insert(_table[i]._key,_table[i]._value);
				}
			}
			this->Swap(newTable);
		}
		size_t times = 1;
		while (_status[index] == EXITS)   //探测
		{
			index = HashFunc2(index, times);
			++times;
		}
		_table[index]._key = key;
		_table[index]._value = value;
		_status[index] = EXITS;
		++_size;
		return true;
	}
	KVNode* Find(const K& key)
	{
		size_t index = HashFunc(key);
		size_t times = 1;
		while (_status[index] != EMPTY)
		{
			if (_table[index]._key == key)
			{
				if (_status[index] == EXITS)
				{
					return &_table[index];
				}
				return NULL;
			}
			index = HashFunc2(index, times);
			++times;
		}
		return NULL;
	}

	/*bool Remove(const K& key)
	{
	size_t index = HashFunc(key);
	while (_status[index] != EMPTY)
	{
	if (_table[index] == key)
	{
	if (_status[index] == EXITS)
	{
	_status[index] = DELETE;
	return true;
	}
	return false;
	}
	++index;
	if (index == _capacity)
	{
	index = 0;
	}
	}
	return false;
	}*/


	void Print()
	{
		size_t index = 0;
		while (index < _capacity)
		{
			cout << _status[index] << ':' << _table[index]._key << " " << _table[i]._value << " ";
			++index;
		}
		cout << endl;
	}
protected:
	void Swap(HashTables<K, V> ht1)
	{
		swap(_table, ht1._table);
		swap(_status, ht1._status);
		swap(_size, ht1._size);
		swap(_capacity, ht1._capacity);
	}

	size_t HashFunc(const K& key)
	{
		HashFun hf;
		return hf(key) % _capacity;
	}

	size_t HashFunc2(size_t prevHash, size_t times)
	{
		return (prevHash + 2 * times - 1) % _capacity;
	}

private:
	size_t _capacity;
	KVNode* _table;
	size_t _size;
	Status* _status;

};