#pragma once

template <typename T>
class DataCarrier
{
public:
	int sort_value;
	T	data;

	DataCarrier(int sort_value, T data);
	DataCarrier(int sort_value);
	~DataCarrier(){};
};

template <typename T>
DataCarrier<T>::DataCarrier(int sort_value, T data) : sort_value(sort_value), data(data)
{
}

template <typename T>
DataCarrier<T>::DataCarrier(int sort_value) : sort_value(sort_value)
{
}

