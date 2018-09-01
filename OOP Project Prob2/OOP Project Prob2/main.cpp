#include<iostream>
#include<fstream>
#include<iomanip>
#include"SIMS.h"
using namespace std;

int compare(const void *a, const void *b);


int main(){

	int i=0;
	int searchType;//search ���.
	int t; // � �Լ��� �����ų���� ���� ����.
	int n=0; // ���° �л����� �˷��ִ� ����.
	int errorFlag = 0;
	char temp[20];
	int temp2;
	int count = 0; // ����, ���̷� search �Ҷ� ���ο� �迭�� �־����� �л� ��.(search ���� �л��� ��.)
	int j = 0;

	Student stud[100], stud2[100],stud3[100];


	fstream file,file2;
	file2.open("file1.txt", ios::app);             //�ؽ�Ʈ ������ �����ݴϴ�. ������ �����մϴ�.
	file.open("file1.txt", ios::in);               //������ ������ �Է¹ޱ� ���� �Է¸��� �ѹ� �� ������ ������.


	while (file.eof() == false){                   //���Ͽ� ������ ������ �� ������ student�迭�� �Է� ����
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
			for (i = 0; i < n; i++){                                            //ù��° �л����� �Է��ϱ� ���� �л����� �߿� �й��� ���� �л��� ������ �����޼����� ���
				if (stud[n].student_id==stud[i].student_id){
					cout << "Error : Already inserted\n";
					errorFlag = 1;
				}
			}
			if (errorFlag == 0){
				if (stud[n].student_id < 1000000000 || stud[n].student_id>9999999999){ //�й��� 10�ڸ��� �ƴ϶�� �����޼��� ���.
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
			case 1:                                        // �̸����� search ù��° �л����� �Էµ� �л����� �̸��� �Է¹��� �̸��� �������� ������ ȭ�鿡 ���.
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
			case 2:                                       // �й����� serach �̸����� �ϴ� ����� ����.
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
			case 3:                                      // �������� search / ����� 1,2 �� search�� ����ϴ� ������ �������� �ִ� ��츦 ����� ã�� �л��� ���ο� �迭�� �ְ� sort�Ͽ� ȭ�鿡 ���.
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
				for (i = 0; i < count; i++){       //���� search���� stud2������� ã�� ���� �迭�� �ʱ�ȭ ����.
					stud2[i] = stud3[i];  
				}
				break;
			case 4:                                // ���̷� search , 3�� �������� �ϴ¹��� ����.
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
			case 5: // �Էµ� �л��� ��� ������.
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
			for (i = 0; i < n; i++){						//�Է¹��� �й��� ���� �л��� ã�� �� �ڿ��ִ� �л����� �迭�� ��ĭ ������ ���.
				if (temp2==stud[i].student_id){
					for (; i < n; i++){
						stud[i] = stud[i + 1];
					}
				}
			}
			n--;
			file2.close();
			file2.open("file1.txt", ios::out);// ������ �ݰ� �ƹ��͵� ���������� ���� ������ ���� ����.
			for (i = 0; i < n; i++){         // delete�� �����ϰ� ���� �迭�� �ٽ� ���Ͽ� ��.

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