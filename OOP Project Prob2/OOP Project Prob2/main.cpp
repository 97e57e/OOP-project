#include<iostream>
#include<fstream>
#include<iomanip>
#include"SIMS.h"
using namespace std;

int compare(const void *a, const void *b);


int main(){

	int i=0;
	int searchType;//search 방법.
	int t; // 어떤 함수를 실행시킬지에 대한 변수.
	int n=0; // 몇번째 학생인지 알려주는 변수.
	int errorFlag = 0;
	char temp[20];
	int temp2;
	int count = 0; // 전공, 나이로 search 할때 새로운 배열에 넣어지는 학생 수.(search 당한 학생의 수.)
	int j = 0;

	Student stud[100], stud2[100],stud3[100];


	fstream file,file2;
	file2.open("file1.txt", ios::app);             //텍스트 파일을 열어줍니다. 없으면 생성합니다.
	file.open("file1.txt", ios::in);               //파일의 내용을 입력받기 위해 입력모드로 한번 더 파일을 열어줌.


	while (file.eof() == false){                   //파일에 내용이 있으면 그 내용을 student배열에 입력 받음
		file >> stud[i].student_name;
		file >> stud[i].student_id;
		file >> stud[i].student_department;
		file >> stud[i].student_age;
		file >> stud[i].student_tel;
		i++;
	}
	i--;

	n = i;
	while (1){

		cout << "1. Insertion" << "\n";
		cout << "2. Search" << "\n";
		cout << "3. Deletion" << "\n";
		cout << "4. Exit" << "\n";
		cout << ">";

		cin >> t;
		/* - Insertion - */
		if (t == 1){          
			cout << "- Insertion -\n";
			cout << "Name ";
			cin >> stud[n].student_name;
			cout << "Student ID ";
			cin >> stud[n].student_id;
			for (i = 0; i < n; i++){                                            //첫번째 학생부터 입력하기 전의 학생까지 중에 학번이 같은 학생이 있으면 에러메세지를 출력
				if (stud[n].student_id==stud[i].student_id){
					cout << "Error : Already inserted\n";
					errorFlag = 1;
				}
			}
			if (errorFlag == 0){
				if (stud[n].student_id < 1000000000 || stud[n].student_id>9999999999){ //학번이 10자리가 아니라면 에러메세지 출력.
					cout << "Error : Student ID should be exactly 10digits\n";
					errorFlag = 1;
				}
				if (errorFlag == 0){
					cout << "Age ";
					cin >> stud[n].student_age;
					cout << "Department ";
					cin >> stud[n].student_department;
					cout << "Tel ";
					cin >> stud[n].student_tel;

					file2 << stud[i].student_name << endl;
					file2 << stud[i].student_id << endl;
					file2 << stud[i].student_department << endl;
					file2 << stud[i].student_age << endl;
					file2 << stud[i].student_tel << endl;

					n++;
				}
			}
			errorFlag = 0;
		}
		/*- Search -*/
		if (t == 2){
			cout << "- Search - \n";
			cout << "1. Search by name \n";
			cout << "2. Search by student ID (10 numbers)\n";
			cout << "3. Search by department name\n";
			cout << "4. Search by Age\n";
			cout << "5. List All\n";
			cin >> searchType;
			switch (searchType)
			{
			case 1:                                        // 이름으로 search 첫번째 학생부터 입력된 학생까지 이름중 입력받은 이름과 같은것이 있으면 화면에 출력.
				cout << "Name keyword? ";
				cin >> temp;
				for (i = 0; i < 100; i++){
					if (strcmp(temp, stud[i].student_name)==0){
						cout.setf(ios::left);
						cout << setw(16) << "Name" << setw(11) << "Student ID" << setw(21) << "Dept" << setw(4) << "Age" << "Tel\n" << endl;
						cout << setw(16) << stud[i].student_name << setw(11) << stud[i].student_id << setw(21) << stud[i].student_department << setw(4) << stud[i].student_age << setw(3) << stud[i].student_tel << endl;
					}
				}
				break;
			case 2:                                       // 학번으로 serach 이름으로 하는 방법과 동일.
				cout << "Student ID ? ";
				cin >> temp2;
				for (i = 0; i < 100; i++){
					if (temp2==stud[i].student_id){
						cout.setf(ios::left);
						cout << setw(16) << "Name" << setw(11) << "Student ID" << setw(21) << "Dept" << setw(4) << "Age" << "Tel\n" << endl;
						cout << setw(16) << stud[i].student_name << setw(11) << stud[i].student_id << setw(21) << stud[i].student_department << setw(4) << stud[i].student_age << setw(3) << stud[i].student_tel << endl;
					}
				}
				break;
			case 3:                                      // 전공으로 search / 방법은 1,2 번 search와 비슷하다 하지만 여러명이 있는 경우를 대비해 찾은 학생을 새로운 배열에 넣고 sort하여 화면에 출력.
				cout << "Department name keyword? ";
				cin >> temp;
				count = 0;
				for (i = 0; i < 100; i++){
					if (strcmp(temp, stud[i].student_department) == 0){
						stud2[count] = stud[i];
						count++;
					}
				}
				qsort(stud2, count, sizeof(Student), compare);
				cout.setf(ios::left);
				cout << setw(16) << "Name" << setw(11) << "Student ID" << setw(21) << "Dept" << setw(4) << "Age" << "Tel\n" << endl;
				for (i = 0; i < count; i++){
					cout.setf(ios::left);
					cout << setw(16) << stud2[i].student_name << setw(11) << stud2[i].student_id << setw(21) << stud2[i].student_department << setw(4) << stud2[i].student_age << setw(3) << stud2[i].student_tel << endl;
				}
				for (i = 0; i < count; i++){       //다음 search때도 stud2를사용해 찾기 위해 배열을 초기화 해줌.
					stud2[i] = stud3[i];  
				}
				break;
			case 4:                                // 나이로 search , 3번 전공으로 하는법과 동일.
				cout << "enter the age ";
				cin >> temp2;
				count = 0;
				for (i = 0; i < 100; i++){
					if (temp2==stud[i].student_age){
						stud2[count] = stud[i];
						count++;
					}
				}
				qsort(stud2, count, sizeof(Student), compare);
				cout.setf(ios::left);
				cout << setw(16) << "Name" << setw(11) << "Student ID" << setw(21) << "Dept" << setw(4) << "Age" << "Tel\n" << endl;
				for (i = 0; i < count; i++){
					cout.setf(ios::left);
					cout << setw(16) << stud2[i].student_name << setw(11) << stud2[i].student_id << setw(21) << stud2[i].student_department << setw(4) << stud2[i].student_age << setw(3) << stud2[i].student_tel << endl;
				}
				for (i = 0; i < count; i++){
					stud2[i] = stud3[i];
				}
				break;
			case 5: // 입력된 학생을 모두 보여줌.
				qsort(stud, n, sizeof(Student), compare);
				cout.setf(ios::left);
				cout << setw(16) << "Name" << setw(11) << "Student ID" << setw(21) << "Dept" << setw(4) << "Age" << "Tel\n" << endl;
				for (i = 0; i < n; i++){
					cout.setf(ios::left);
					cout << setw(16) << stud[i].student_name << setw(11) << stud[i].student_id << setw(21) << stud[i].student_department << setw(4) << stud[i].student_age << setw(3) << stud[i].student_tel << endl;
				}
				break;
			default:
				break;
			}
		}

		/*- Deletion -*/

		if (t == 3){
			cout << "- Deletion -\n" << "Student ID to delete : ";
			cin >> temp2;
			for (i = 0; i < n; i++){						//입력받은 학번을 가진 학생을 찾아 그 뒤에있는 학생부터 배열의 한칸 앞으로 당김.
				if (temp2==stud[i].student_id){
					for (; i < n; i++){
						stud[i] = stud[i + 1];
					}
				}
			}
			n--;
			file2.close();
			file2.open("file1.txt", ios::out);// 파일을 닫고 아무것도 쓰여져있지 않은 파일을 새로 열음.
			for (i = 0; i < n; i++){         // delete를 수행하고난 후의 배열을 다시 파일에 씀.

				file2 << stud[i].student_name << endl;
				file2 << stud[i].student_id << endl;
				file2 << stud[i].student_department << endl;
				file2 << stud[i].student_age << endl;
				file2 << stud[i].student_tel << endl;			
			}
		}
		if (t == 4)
			break;
	}
	
	file2.close();
	file.close();
	return 0;
}

int compare(const void *a, const void *b) {
	return(strcmp((char *)a, (char *)b));
}