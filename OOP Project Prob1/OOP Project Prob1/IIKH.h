/*프로그램이 작동하기 위해선 기본 두개의 텍스트 파일이 필요함. 하나는 recipe를 저장해두는 텍스트와
하나는 식단 계획을 저장해 놓는 텍스트. 레시피를 저장해놓는 텍스트에는 그 요리를 하기 위해 필요한 성분과
요리법이 적혀있고, 식단 계획이 들어있는 텍스트에는 하루 세개의 레시피가 저장되어 있다.*/

class Greeter{

	DataBase datebase;
private:
	int menu;//Greeter 에서 어떤 기능을 수행할지에 대한변수.

public:
	void browse_recipes();    //존재하는 레시피를 레시피텍스트에서 찾아오는 함수.
	void review_plan();		  //텍스트에 존재하는 plan을 화면에 display.
	void create_plan();		  //새로운 plan을 추가함.(텍스트 파일에 입력).

};

class DataBase :public Greeter{

	Recipe *recipe;           //레시피들을 저장할 레시피들의 배열.
	Plan_Manager manager;
	Date date;
	Meal meal;


};

class Recipe :public DataBase{
private:
	char **ingredients;       //요리를 만들때 들어가는 성분들을 2차원 배열에 저장함 row[0]엔 첫번째성분, row[1]엔 두번째성분 과 같은 방식.

public :
	void search_recipe();     //레시피를 검색해서 찾을 수 있음.
	void load_recipe();       //파일 읽기 모드로 텍스트를 열어 텍스트에 존재하는 레시피를 불러옴.
	void add_recipe();		  //파일 추가 쓰기 모드로 레시피텍스트를 열어 새로운 레시피를 추가해줌.
	void edit_recipe();		  //레시피에 내용을 추가하거나 편집을 하기 위해 텍스트를 새로 열어 내용을 추가한 레시피를 덮어씌움.
	void write_recipe();	  //프로그램을 종료하기 전에 배열에 저장되어있는 레시피들을 텍스트에 입력해줌.
	void show_recipe();       //저장되어있는 요리의 조리법에 대한 글을 나타내 준다.
};

class Plan_Manager :public DataBase {
private:
	int days[7];              //1주일간의 식단계획을 짜기 위해 7일의 날짜를 선택해줄수있는 배열.

public:
	void select_dates();      //날짜를 지정해 그 기간의 식단을 선택할수 있게 해준다.(IIKH에는 1주일간의 식단이 저장되어있는데 예륻들어 2,3일차 저녁을 식단을 찾아볼수있다.)
	void edit_plan();		  //식단 계획을 변경할수 있게 해줌.
};

class Date :public DataBase {
private:
	int days2[31];             //한달 31일을 기준으로 1일부터 31일까지를 나타내준다.

public:
	void edit_meals();        //식단을 수정할수 있는 함수.(편집과 추가 둘다 가능하다.)
	void print_grocery();	  //식단을 구성하고있는 구성원들을 display해준다.
};

class Meal :public DataBase {
private:
	int numOfPeople;          //식사를 할 인원수. 인원수에 맞게 (레시피에 필요한 재료)*(인원수) 를 해서 그 식사에 필요한 재료를 확정할수 있음.


public :
	void select_recipe();     //식단의 레시피를 선택한다. (데이터 베이스의 레시피에서 선택 가능.)
	void grocery_list();	  //재료들의 리스트를 display.

};