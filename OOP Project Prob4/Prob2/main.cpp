#include "inf_int.h"
#include<iostream>
using namespace std;

int SplitInput(char *, char**, int*);  //���ڿ��� �ɰ��� ���� �Լ�.

int main(){

	char *input;    // �Է��� ���� char �迭
	char *oper;     // ��ȣ�� ������ �迭.
	char **st = new char*[100]; // �Է¹��� �迭�� �ɰ��� �������ִ� 2�����迭
	char **inf= new char*[100]; // �Է¹��� �� �� ���ڸ� �������ִ� 2���� �迭
	int count = 0, countInt = 0, countOper = 0, countSpace = 0;
	int niNumArgs = 0;
	int i, j;

	inf_int a[100];  //��ü 100���� �迭�� �����

	input = new char[500];
	oper = new char[100];

	for (i = 0; i < 100; i++){ //2���� �迭���� �����Ҵ�.
		st[i] = new char[100];
		inf[i] = new char[100];
	}
	while (1){
		cout << "Input : ";
		cin.getline(input, 100);

		if (input[0] == '0'){ //�Է°��� 0�̸� ���α׷��� �����Ѵ�
			break;
		}

		count = SplitInput(input, st, &niNumArgs); // input�� ���� �������� ������ st[]�� �����Ѵ�. ����� string�� ���� ��ȯ.

		for (i = 0; i < count; i++){
			if (i % 2 == 1){ // st�� ����� ���� ¦����° (�迭������ [1]�� �ι�° �����̹Ƿ�) ���� ��ȣ �迭�� �����Ѵ�.
				oper[countOper] = st[i][0];
				countOper++;
			}
			else{
				inf[countInt] = st[i];    //st�� ����� ���� Ȧ����° ���� inf[]�� �����Ѵ�.
				inf_int b(inf[countInt]); //inf[]�� ����� ���ڿ��� �̿��� ��ü�� �����.
				a[countInt] = b;
				countInt++;
			}
		}
		oper[countOper] = '\0';

		/**********������ ���� �����ϱ� ���� ����**************/
		for (i = 0; i < countOper; i++){  // ��ȣ�� �迭�� ���� ���� ������ �ִٸ� �� �ڸ��� ���ڸ� ���� �����ش��� ���� ���ڵ�� ���� ��ȣ���� ��ĭ ��ܿ´�.
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

		/***���������� �����ٸ� +�� -�� �켱������ ���� ������ ������� �������ش�***/
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

		for (i = 0; i < 100; i++){  // �迭���� �ٽ� �̿��ϱ� ���� ����ش�,
			a[i] = b;
			*st[i] = NULL;
			*inf[i] = NULL;
		}
		oper[0] = '\0';
		countInt = 0, countOper = 0, count = 0;
	}

	delete st;
	delete inf; // ���α׷��� ���̳��ٸ� ���� �Ҵ��� ���� ���ش�.

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