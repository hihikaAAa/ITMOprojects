#ifndef MY_VECTOR_HPP
#define MY_VECTOR_HPP
#include <cstddef>
#include <stdexcept>

template< typename T >
class Vector
{
  public:
	Vector();

	~Vector();

	void push_back(const T& value);

	T& operator[](size_t index);
	const T& operator[](size_t index) const;

	[[nodiscard]] size_t size() const;

	void clear();

	T* begin();
	T* end();
	const T* begin() const;
	const T* end() const;

  private:
	T* data;
	size_t sz;
	size_t capacity;

	void reallocate(size_t new_capacity);
};

template< typename T >
Vector< T >::Vector()
{
	data = nullptr;
	sz = 0;
	capacity = 0;
}

template< typename T >
Vector< T >::~Vector()
{
	delete[] data;
}

template< typename T >
void Vector< T >::push_back(const T& value)
{
	if (sz == capacity)
	{
		const size_t new_capacity = (capacity == 0) ? 1 : capacity * 2;
		reallocate(new_capacity);
	}
	data[sz++] = value;
}

template< typename T >
T& Vector< T >::operator[](size_t index)
{
	if (index >= sz)
	{
		throw std::out_of_range("Vector index out of range");
	}
	return data[index];
}

template< typename T >
const T& Vector< T >::operator[](size_t index) const
{
	if (index >= sz)
	{
		throw std::out_of_range("Vector index out of range");
	}
	return data[index];
}

template< typename T >
size_t Vector< T >::size() const
{
	return sz;
}

template< typename T >
void Vector< T >::clear()
{
	sz = 0;
}

template< typename T >
T* Vector< T >::begin()
{
	return data;
}

template< typename T >
T* Vector< T >::end()
{
	return data + sz;
}

template< typename T >
const T* Vector< T >::begin() const
{
	return data;
}

template< typename T >
const T* Vector< T >::end() const
{
	return data + sz;
}

template< typename T >
void Vector< T >::reallocate(const size_t new_capacity)
{
	T* new_data = new T[new_capacity];
	for (size_t i = 0; i < sz; i++)
	{
		new_data[i] = data[i];
	}
	delete[] data;
	data = new_data;
	capacity = new_capacity;
}

#endif	  // MY_VECTOR_HPP
