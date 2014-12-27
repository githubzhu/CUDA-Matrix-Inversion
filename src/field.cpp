#include "field.hpp"

int field_element::modulus(2);

int inverse(int a, int p){
	// inverse(p%a, a) * p = ka + 1
	return a == 1 ? 1 : ((LL)(a-inverse(p%a, a))*p+1)/a;
}

void field_init(int m){
	field_element::init(m);
}

int field_modulus(){
	return field_element::get_modulus();
}
