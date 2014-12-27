
typedef long long LL;

__device__ 
int cuda_field_modulus;

__device__
int inverse(int a, int p){
	return a == 1 ? 1 : ((LL)(a-inverse(p%a, a))*p+1)/a;
}

__device__
void cuda_field_init(int m){
	cuda_field_modulus = m;
}

struct cuda_field_element {
	__device__
	cuda_field_element() : value(0) {}
	
	__device__
	cuda_field_element(const cuda_field_element &v) : value(v.value) {}

	__device__
	cuda_field_element(int v) : value(v) { value %= cuda_field_modulus; }


	__device__
	cuda_field_element& operator=(const cuda_field_element &v){ value = v.value; return *this; }

	__device__
	cuda_field_element& operator+=(const cuda_field_element &v){
		value += v.value; if(value >= cuda_field_modulus) value -= cuda_field_modulus; return *this;
	}

	__device__
	cuda_field_element& operator-=(const cuda_field_element &v){
		value -= v.value; if(value < 0) value += cuda_field_modulus; return *this;
	}

	__device__
	cuda_field_element& operator*=(const cuda_field_element &v){ value *= v.value; value %= cuda_field_modulus; return *this; }
	

	__device__
	cuda_field_element operator+(const cuda_field_element &v) const { return cuda_field_element(*this) += v; }

	__device__
	cuda_field_element operator-(const cuda_field_element &v) const { return cuda_field_element(*this) -= v; }

	__device__
	cuda_field_element operator*(const cuda_field_element &v) const { return cuda_field_element(*this) *= v; }


	__device__
	cuda_field_element operator-() const { return -value; }

	__device__
	cuda_field_element operator~() const { return inverse(value, cuda_field_modulus); }


	__device__
	bool operator==(const cuda_field_element &v) const { return value == v.value; }

	__device__
	bool operator!=(const cuda_field_element &v) const { return value != v.value; }


	__device__
	int get_value() const { return value; }

private:
	int value;
};
