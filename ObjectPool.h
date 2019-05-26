#pragma once
//#define__OBJECT_POOL_H__


//T的类型会改变，平台不同，要考虑内存对齐的问题
template<class T>

//定长内存池
class ObjectPool{
protected:
	struct Block{
		//内存块管理，用字节数统计管理
		char* _start = nullptr;
		//char* _finish;
		size_t _bytesize = 0;
		//剩余字节数
		size_t _byteleft = 0;
		Block* _next = nullptr;

		Block(size_t bytesize){
			_start = (char*)malloc(bytesize);
			_bytesize = bytesize;
			_next = nullptr;
		}
		~Block(){
			free(_start);
			_bytesize = _byteleft = 0;
			_next = nullptr;

		}
	};

	static size_t GetItemSize(){
		if (sizeof(T) > sizeof(*T)){
			return sizeof(T);
		}
		else{
			return sizeof(*T);
		}
	}

public:
	ObjectPool(size_t initnum = 8){
		_head = _tail = new Block(initnum * sizeof(T);
	}

	~Block(){
		Destory();
	}
	void Destory(){
		Block* cur = _head;
		while (cur){
			Block* next = cur->_next;
			delete cur;
			cur = next;
		}
		_head = _tail = nullptr;
		_freelist = nullptr;
	}
	
	T*& OBJ_NEXT(T* obj){
		return *(T**)obj;
	}

	//申请对象
	T* New(){
		T* obj = nullptr;

		if(_freelist != nullptr){
			obj = _freelsit;
			_freelist = OBJ_NEXT(_freelist);
		}else{
			//Block tail = _tail;
			if(tail->byteleft == 0){
				Block newblock = new Block(tail->_bytesize * 2);
				_tail->_next = newblock;
				_tail = newblock;
			}
			obj = (T*)(_tail->_start + (_tail->bytesize - _tail->_byteleft));
			_tail->byteleft -= sizeof(T);
		}
		new(obj)T();
		return obj;

	}

	void Delete(T* ptr){
		ptr->~T();

		if (_freelist == nullptr){
			_freelist = ptr;
			//(*(T**)ptr) == nullptr;
			OBJ_NEXT(ptr) = nullptr;
		}
		else{
			//(*(T**)ptr) = _freelist;
			OBJ_NEXT(ptr) = _freelist;
			_freelist = ptr;
		}
	}
protected:


	//自由链表
	T* _freelist = nullptr;

	//块管理
	Block* _head = nullptr;
	Block* _tail = nullptr;
};

void TestObjectPool(){
	ObjectPool<int> pool;
	int *p1 = pool.New();
	int *p2 = pool.New();
	
	cout << p1 << endl;
	cout << p2 << endl;

	pool.Delete(p1);
	pool.Delete(p2);

	cout << pool.New() << endl;
	cout << pool.New() << endl;

}

