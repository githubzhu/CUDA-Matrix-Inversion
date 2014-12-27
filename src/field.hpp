#ifndef FIELD_HPP
#define FIELD_HPP

typedef long long LL;

int inverse(int, int);
void field_init(int);

struct field_element {
	field_element() : value(0) {}
	field_element(const field_element &v) : value(v.value) {}
	field_element(int v) : value(v) { value %= modulus; }

	field_element& operator=(const field_element &v){ value = v.value; return *this; }
	field_element& operator+=(const field_element &v){
		value += v.value; if(value >= modulus) value -= modulus; return *this;
	}
	field_element& operator-=(const field_element &v){
		value -= v.value; if(value < 0) value += modulus; return *this;
	}
	field_element& operator*=(const field_element &v){ value *= v.value; value %= modulus; return *this; }
	
	field_element operator+(const field_element &v) const { return field_element(*this) += v; }
	field_element operator-(const field_element &v) const { return field_element(*this) -= v; }
	field_element operator*(const field_element &v) const { return field_element(*this) *= v; }

	field_element operator-() const { return -value; }
	field_element operator~() const { return inverse(value, modulus); }

	bool operator==(const field_element &v) const { return value == v.value; }
	bool operator!=(const field_element &v) const { return value != v.value; }

	int get_value() const { return value; }

	static void init(int m) { modulus = m; }

	static int get_modulus() { return modulus; }

private:
	int value;
	static int modulus;
};

#endif // FIELD_HPP
