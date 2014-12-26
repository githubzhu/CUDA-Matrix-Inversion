#ifndef FIELD_HPP
#define FIELD_HPP

typedef long long LL;

int inverse(int a, int p){
	// inverse(p%a, a) * p = ka + 1
	return a == 1 ? 1 : ((LL)(a-inverse(p%a, a))*p+1)/a;
}

struct Value {
	Value() = default;
	Value(const Value &v) = default;
	Value(int v) : value(v) { value %= modulus; }

	~Value() noexcept = default;

	Value& operator=(const Value &v) = default;
	Value& operator+=(const Value &v){ value += v.value; if(value >= modulus) value -= modulus; }
	Value& operator-=(const Value &v){ value -= v.value; if(value < 0) value += modulus; }
	Value& operator*=(const Value &v){ value *= v.value; value %= modulus; }
	
	Value operator+(const Value &v) const { return Value(*this) += a; }
	Value operator-(const Value &v) const { return Value(*this) -= a; }
	Value operator*(const Value &v) const { return Value(*this) *= a; }

	Value operator-() const { return -value; }
	Value operator~() const { return inverse(value, modulus); }

	bool operator==(const Value &v) const { return value == v.value; }
	bool operator!=(const Value &v) const { return value != v.value; }

	int get_value() const { return value; }

	static void init(int m) { modulus = m; }
	static int get_modulus() { return modulus; }

private:
	int value;
	static int modulus;
};

int Value::modulus(2);

#endif FIELD_HPP
