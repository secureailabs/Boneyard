#pragma once
#include <string>

template<typename T>
class Array {
public:
	Array(int len = 10) : len_(len), data_(new T[len]) { }
	~Array() { delete[] data_; }
	int len() const { return len_; }
	const T& operator[](int i) const { return data_[check(i)]; }
	T&       operator[](int i) { return data_[check(i)]; }
	Array<T>& operator= (const Array<T>& x) {
		for (int i = 0; i < len_; i++)
			data_[i] = x[i];
	};
	//Array<T>& operator= (Array<T>&&);
private:
	int len_;
	T*  data_;
	int check(int i) const {
		if (i >= 0 && i < len_)
			return i;
		else
			return 0;
	}
};

double ai_ad(std::string ad_json, char *patient_json);

