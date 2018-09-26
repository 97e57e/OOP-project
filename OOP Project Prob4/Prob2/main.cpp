#include "inf_int.h"
#include<iostream>
using namespace std;

int SplitInput(char *, char**, int*);  //문자열을 쪼개기 위한 함수.

int main(){

	char *input;    // 입력을 받을 char 배열
	char *oper;     // 부호를 저장할 배열.
	char **st = new char*[100]; // 입력받은 배열을 쪼개서 저장해주는 2차원배열
	char **inf= new char*[100]; // 입력받은 것 중 숫자를 저장해주는 2차원 배열
	int count = 0, countInt = 0, countOper = 0, countSpace = 0;
	int niNumArgs = 0;
	int i, j;

	inf_int a[100];  //객체 100개의 배열을 만든다

	input = new char[500];
	oper = new char[100];

	for (i = 0; i < 100; i++){ //2차원 배열들의 동적할당.
		st[i] = new char[100];
		inf[i] = new char[100];
	}
	while (1){
		cout << "Input : ";
		cin.getline(input, 100);

		if (input[0] == '0'){ //입력값이 0이면 프로그램을 종료한다
			break;
		}

		count = SplitInput(input, st, &niNumArgs); // input을 띄어쓰기 기준으로 나누어 st[]에 저장한다. 저장된 string의 수를 반환.

		for (i = 0; i < count; i++){
			if (i % 2 == 1){ // st에 저장된 것중 짝수번째 (배열에서는 [1]이 두번째 원소이므로) 것을 부호 배열에 저장한다.
				oper[countOper] = st[i][0];
				countOper++;
			}
			else{
				inf[countInt] = st[i];    //st에 저장된 것중 홀수번째 것을 inf[]에 저장한다.
				inf_int b(inf[countInt]); //inf[]에 저장된 문자열을 이용해 객체를 만든다.
				a[countInt] = b;
				countInt++;
			}
		}
		oper[countOper] = '\0';

		/**********곱셈을 먼저 수행하기 위한 과정**************/
		for (i = 0; i < countOper; i++){  // 부호의 배열을 보며 만약 곱셈이 있다면 그 자리의 숫자를 먼저 곱해준다음 다음 숫자들과 다음 부호들을 한칸 당겨온다.
			if (oper[i] == '*'){
				a[i] = a[i] * a[i + 1];
				for (j = i + 1; j < countInt; j++){
					a[j] = a[j + 1];
				}
				countInt--;
				for (j = i; j < countOper; j++){
					oper[j] = oper[j + 1];
				}
				i--;
			}
		}

		/***곱셈연산이 끝났다면 +와 -는 우선순위가 같기 때문에 순서대로 연산해준다***/
		inf_int result = a[0];
		
		for (i = 0; i < countOper; i++){
			if (oper[i] == '+'){
				result = result + a[i + 1];
			}
			else if (oper[i] == '-'){
				result = result - a[i + 1];
			}
		}

		cout << "Output : ";
		cout << result << endl;
		inf_int b;
		result = b;

		for (i = 0; i < 100; i++){  // 배열들을 다시 이용하기 위해 비워준다,
			a[i] = b;
			*st[i] = NULL;
			*inf[i] = NULL;
		}
		oper[0] = '\0';
		countInt = 0, countOper = 0, count = 0;
	}

	delete st;
	delete inf; // 프로그램이 끝이났다면 동적 할당을 해제 해준다.

}

int SplitInput(char *put, char **splited, int  *n)
{
	int  i;
	int  stringNum = 0;
	int  len = 0;
	char checkSpace;

	if ((len = strlen(put)) <= 0) {
		*n = 0;
		return(0);
	}

	splited[0] = put;
	stringNum = 1;

	for (i = 0; i < len; i++) {
		checkSpace = put[i];
		if (checkSpace == ' ') {
			put[i] = 0;
			splited[stringNum++] = put + i + 1;
		}
		else if (checkSpace == '\n') {
			put[i] = 0;
		}
	}
	return(stringNum);

}