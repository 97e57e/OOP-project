#include "inf_int.h"
#include<iostream>
#pragma warning (disable : 4996)
using namespace std;

inf_int::inf_int()
{
	this->digits = new char[2];   // 동적할당

	this->digits[0] = '0';      // default 값 0 설정
	this->digits[1] = '\0';
	this->length = 1;
	this->thesign = true;
}
inf_int::inf_int(int n){
	char buf[100];

	if (n<0){      // 음수 처리
		this->thesign = false;
		n = -n;
	}
	else{
		this->thesign = true;
	}

	int i = 0;
	while (n>0){         // 숫자를 문자열로 변환하는 과정
		buf[i] = n % 10 + '0';

		n /= 10;
		i++;
	}

	if (i == 0){   // 숫자의 절댓값이 0일 경우
		new (this) inf_int();   // 생성자 재호출...gcc에서 컴파일에러가 있다고 함. inf_int()의 경우 별개의 인스턴스가 생성됨. 
	}
	else{
		buf[i] = '\0';

		this->digits = new char[i + 1];
		this->length = i;
		strcpy(this->digits, buf);
	}
}
inf_int::inf_int(const char* str)
{
	char buf[100];
	unsigned int len = strlen(str);
	unsigned int temp;
	int i;
	/**************** 부호처리**********************/
	if (str[0] == '-'){     //음수 처리
		this->thesign = false;
	}
	else
		this->thesign = true;

	/******************내부표현에 맞게 변환*************************/
	if (this->thesign == true){    //0이아닌 양수인 경우.
		this->digits = new char[len + 1];
		this->length = len;
		temp = len;
		for (i = 0; i < temp; i++){
			buf[i] = str[len - 1];
			len--;
		}
		buf[temp] = '\0';
	}
	else{
		this->digits = new char[len]; //음수인경우 str에 부호가 들어있는데 char배열엔 숫자만 들어가기때문에 공간이 하나 비게 되어서 +1을 안해줘도됨.
		this->length = len - 1;
		temp = len;
		for (i = 0; i < temp - 1; i++){
			buf[i] = str[len - 1];
			len--;
		}
		buf[temp - 1] = '\0';
	}

	if (str[0] == '0')      // 0이면 디폴트 생성자. 
		new (this) inf_int();
	else
		strcpy(this->digits, buf);

}
inf_int::inf_int(const inf_int& a){   // copy constructor
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;
}
inf_int::~inf_int(){
	delete digits;      // 메모리 할당 해제
}
inf_int& inf_int::operator=(const inf_int& a)
{
	if (this->digits) {
		delete this->digits;      // 이미 문자열이 있을 경우 제거.
	}
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;

	return *this;
}

bool operator==(const inf_int& a, const inf_int& b)   // 객체에 들어있는 문자열과 부호가 같은면 참을 리턴
{
	if ((strcmp(a.digits, b.digits) == 0) && (a.thesign == b.thesign))
		return true;
	else
		return false;
}
bool operator!=(const inf_int& a, const inf_int& b)
{
	if (operator==(a, b) == true)
		return false;
	else
		return true;
}
bool operator>(const inf_int& a, const inf_int& b)
{
	int i;
	inf_int max;
	max = a;
	if (a == b)
		return false;
	/************두 수의 절댓값 비교***************/
	if (a.length == b.length&&a.length != 1){
		i = a.length - 1;
		while ((a.digits[i] == b.digits[i]) && (a.digits[i])){
			i--;
		}
		if (a.digits[i] < b.digits[i])
			max = b;
	}
	else if (a.length == b.length&&a.length == 1){
		if (a.digits[0] < b.digits[0])
			max = b;
	}
	else{
		if (a.length < b.length)
			max = b;
	}

	/**************두 수의 부호 비교**********************/
	if (a.thesign == true && b.thesign == true){     //둘 다 양수일 경우
		if (max == a)
			return true;
		else
			return false;
	}
	else if (a.thesign == false && b.thesign == false){  // 둘 다 음수일 경우
		if (max == a)
			return false;
		else
			return true;
	}
	else{    //두 수의 부호가 다를 경우
		if (a.thesign == true)
			return true;
		else
			return false;
	}
}
bool operator<(const inf_int& a, const inf_int& b)
{
	if (operator>(a, b) == true || operator==(a, b) == true)
		return false;
	else
		return true;
}

inf_int operator+(const inf_int& a, const inf_int& b)
{
	inf_int c;
	unsigned int i;

	if (a.thesign == b.thesign){   // 이항의 부호가 같을 경우 + 연산자로 연산
		for (i = 0; i < a.length; i++){
			c.Add(a.digits[i], i + 1);
		}
		for (i = 0; i < b.length; i++){
			c.Add(b.digits[i], i + 1);
		}

		c.thesign = a.thesign;

		return c;
	}
	else{   // 이항의 부호가 다를 경우 - 연산자로 연산
		c = b;
		c.thesign = a.thesign;

		return a - c;
	}
}
inf_int operator-(const inf_int& a, const inf_int& b)
{
	inf_int c;
	inf_int m1(a), m2(b);
	if (m1.thesign == m2.thesign){
		if (m1.thesign == true){      // 두 수의 부호가 같을 경우 큰 수에서 작은수를 빼고 부호를 설정해준다.
			if (m1 > m2){
				c = Sub(m1, m2);
				return c;
			}
			else if (m2 == m1){
				return c;
			}
			else{
				c = Sub(m2, m1);
				c.thesign = false;
				return c;
			}

		}
		else{
			if (m1 > m2){
				c = Sub(m2, m1);
				c.thesign = true;
				return c;
			}
			else if (m1 == m2){
				return c;
			}
			else{
				c = Sub(m1, m2);
				c.thesign = false;
				return c;
			}

		}
	}
	else{                // 두 수의 부호가 다를 경우 +연산을 해준후 부호를 맞춰준다.
		if (m1.thesign == true){
			inf_int sub(m2);
			sub.thesign = true;
			return m1 + sub;
		}
		else{
			inf_int sub(m2);
			sub.thesign = false;
			return m1 + sub;
		}
	}
}
inf_int operator*(const inf_int& a, const inf_int& b)
{
	inf_int result;
	if (a.thesign == b.thesign){         // 같은 부호의 두 수의 곱셈은 항상 양수가 된다.
		if (a.thesign == true){
			if (a.length >= b.length){       //자리수를 맞추기 위해 큰수를 승수로 작은수를 피승수로 둔디.
				result = Mult(a, b);
			}
			else
				result = Mult(b, a);
		}
		else{
			if (a > b || a == b){
				result = Mult(b, a);
				result.thesign = true;
			}
			else{
				result = Mult(a, b);
				result.thesign = true;
			}
		}
	}
	else{                             // 다른 부호의 두 수를 곱하면 결과는 음수가 된다.
		if (a.length >= b.length){
			result = Mult(a, b);
			result.thesign = false;
		}
		else{
			result = Mult(b, a);
			result.thesign = false;
		}
	}
	return result;
}

ostream& operator<<(ostream& out, const inf_int& a){
	int i;

	if (a.thesign == false){
		out << '-';
	}
	for (i = a.length - 1; i >= 0; i--){
		out << a.digits[i];
	}
	return out;
}

void inf_int::Add(const char num, const unsigned int index){   //a의 index 자리수에 n을 더한다.0<=n<=9, ex) a가 391일때, Add(a,2,2)의 결과는 411
	if (this->length < index){
		this->digits = (char*)realloc(this->digits, index + 1);

		if (this->digits == NULL){   // 할당 실패 예외처리
			cout << "Memory reallocation failed, the program will terminate." << endl;
			exit(0);
		}
		this->length = index;              //길이 지정
		this->digits[this->length] = '\0'; //널문자 삽입
	}
	if (this->digits[index - 1] < '0')  // 연산 전에 '0'보다 작은 아스키값인 경우 0으로 채움. 쓰여지지 않았던 샐운 자리수일 경우 발생
	{
		this->digits[index - 1] = '0';
	}
	this->digits[index - 1] += num - '0'; // 값 연산
	if (this->digits[index - 1] > '9'){   // 자리 올림이 발생한 경우
		this->digits[index - 1] -= 10;    // 현재 자릿수에서(아스키값)10을 빼고
		Add('1', index + 1);              // 윗자리에 1을 더한다.
	}
}
inf_int Sub(const inf_int&x, const inf_int&y){
	inf_int result(x);
	int i, j = 0, h, l, carry = 0;
	int mCount = 0;
	for (i = 0; i < y.length; i++){
		if (result.digits[i] >= y.digits[i]){    // x의 자리수가 y의 자리수보다 크면 그냥 빼준다.
			result.digits[i] = result.digits[i] - y.digits[i] + '0' + carry;
			carry = 0;
		}
		else{    // y의 자리수가 더 크면 y의 숫자를 반전시켜 더한다.(그 합이 10이 넘을 경우 10을 빼준다.)
			y.digits[i] = '9' - y.digits[i] + 1 + '0';
			result.digits[i] = result.digits[i] + y.digits[i] - '0' + carry;
			if (result.digits[i] >(10 + '0')){
				result.digits[i] = result.digits[i] - 10;
			}
			carry = -1;
			if (result.digits[i + 1] == '0'){     //캐리가 발생 했을때 앞자리가 0이면 그 앞자리에서 받아온다.
				j = i + 1;
				while (result.digits[j] == '0'){
					j++;
					mCount++;
				}
				for (h = i + 1; h < (i + 1) + mCount + 1; h++){
					result.digits[h] = result.digits[h] - 1;
				}
				for (l = 0; l < result.length; l++){
					if (result.digits[l] == '/')
						result.digits[l] = '9';
				}
				carry = 0;
			}
		}
	}

	if (carry == -1){
		result.digits[y.length] -= 1;
	}
	for (i = 0; i < result.length; i++){
		if (result.digits[i] == '/')
			result.digits[i] = '9';
	}
	for (i = result.length; i > 0; i--){   // 문자열의 길이기 줄어들었다면 length를 줄여준다.
		if (result.digits[i - 1] == '0'){
			result.digits[i - 1] = '\0';
			result.length--;
		}
		else
			break;
	}
	return result;
}
inf_int Mult(const inf_int&x, const inf_int&y){
	inf_int result;

	int i, j, count = 0;

	result.digits = new char[x.length + 2];
	result.digits[0] = '0';
	result.digits[1] = '\0';
	while (count < y.length){      // 곱셈을 하는방법은 먼저 승수를 피승수의 각 자리수만큼 더한다.
		inf_int temp;              // 그리고 자릿수에 맞게 '0'을 더 넣어주어서 다시 더한다.
		temp.digits = new char[x.length + 2];
		temp.digits[0] = '0';
		temp.digits[1] = '\0';
		for (i = 0; i < (y.digits[count] - '0'); i++){
			temp = temp + x;
		}
		if (count > 0){
			temp.digits = (char*)realloc(temp.digits, temp.length + count + 1);
			result.digits = (char*)realloc(result.digits, result.length + count + 1);
			for (j = 0; j < count; j++){
				for (i = temp.length; i >= 0; i--){
					temp.digits[i + 1] = temp.digits[i];
				}
				temp.digits[0] = '0';
				temp.length++;
			}
			for (i = 0; i < temp.length; i++){
				if (temp.digits[i] < '0'){
					temp.digits[i] = '0';
				}
			}
		}
		result = result + temp;
		count++;
		for (i = 0; i < temp.length; i++){
			temp.digits[i] = '0';
		}
	}
	return result;
}