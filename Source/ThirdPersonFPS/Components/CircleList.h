#pragma once

template<typename T>
class CircleList
{

public:
	CircleList<T>* Previous;
	CircleList<T>* Next;
	T Data;
	
	CircleList(): Previous(this), Next(this) {}
	CircleList(const T& obj): Previous(this), Next(this), Data(obj) {}
	
	CircleList<T>* insert(const T& obj)
	{
		CircleList<T>* t = new CircleList<T>(obj);
		CircleList<T>* prev = this->Previous;
		prev->Next = t;
		t->Previous = prev;

		t->Next = this;
		this->Previous = t;
		return t;
	}

	CircleList<T>* erase()
	{
		if (this->Previous == this)
		{
			delete this;
			return nullptr;
		}
		
		CircleList<T>* prev = this->Previous;
		CircleList<T>* next = this->Next;
		prev->Next = next;
		next->Previous = prev;
		
		delete this;
		return next;
	}
	

	bool OneElement()
	{
		return this->Previous == this && this->Next == this;
	}
};
