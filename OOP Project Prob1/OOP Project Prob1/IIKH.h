/*���α׷��� �۵��ϱ� ���ؼ� �⺻ �ΰ��� �ؽ�Ʈ ������ �ʿ���. �ϳ��� recipe�� �����صδ� �ؽ�Ʈ��
�ϳ��� �Ĵ� ��ȹ�� ������ ���� �ؽ�Ʈ. �����Ǹ� �����س��� �ؽ�Ʈ���� �� �丮�� �ϱ� ���� �ʿ��� ���а�
�丮���� �����ְ�, �Ĵ� ��ȹ�� ����ִ� �ؽ�Ʈ���� �Ϸ� ������ �����ǰ� ����Ǿ� �ִ�.*/

class Greeter{

	DataBase datebase;
private:
	int menu;//Greeter ���� � ����� ���������� ���Ѻ���.

public:
	void browse_recipes();    //�����ϴ� �����Ǹ� �������ؽ�Ʈ���� ã�ƿ��� �Լ�.
	void review_plan();		  //�ؽ�Ʈ�� �����ϴ� plan�� ȭ�鿡 display.
	void create_plan();		  //���ο� plan�� �߰���.(�ؽ�Ʈ ���Ͽ� �Է�).

};

class DataBase :public Greeter{

	Recipe *recipe;           //�����ǵ��� ������ �����ǵ��� �迭.
	Plan_Manager manager;
	Date date;
	Meal meal;


};

class Recipe :public DataBase{
private:
	char **ingredients;       //�丮�� ���鶧 ���� ���е��� 2���� �迭�� ������ row[0]�� ù��°����, row[1]�� �ι�°���� �� ���� ���.

public :
	void search_recipe();     //�����Ǹ� �˻��ؼ� ã�� �� ����.
	void load_recipe();       //���� �б� ���� �ؽ�Ʈ�� ���� �ؽ�Ʈ�� �����ϴ� �����Ǹ� �ҷ���.
	void add_recipe();		  //���� �߰� ���� ���� �������ؽ�Ʈ�� ���� ���ο� �����Ǹ� �߰�����.
	void edit_recipe();		  //�����ǿ� ������ �߰��ϰų� ������ �ϱ� ���� �ؽ�Ʈ�� ���� ���� ������ �߰��� �����Ǹ� �����.
	void write_recipe();	  //���α׷��� �����ϱ� ���� �迭�� ����Ǿ��ִ� �����ǵ��� �ؽ�Ʈ�� �Է�����.
	void show_recipe();       //����Ǿ��ִ� �丮�� �������� ���� ���� ��Ÿ�� �ش�.
};

class Plan_Manager :public DataBase {
private:
	int days[7];              //1���ϰ��� �Ĵܰ�ȹ�� ¥�� ���� 7���� ��¥�� �������ټ��ִ� �迭.

public:
	void select_dates();      //��¥�� ������ �� �Ⱓ�� �Ĵ��� �����Ҽ� �ְ� ���ش�.(IIKH���� 1���ϰ��� �Ĵ��� ����Ǿ��ִµ� ���Y��� 2,3���� ������ �Ĵ��� ã�ƺ����ִ�.)
	void edit_plan();		  //�Ĵ� ��ȹ�� �����Ҽ� �ְ� ����.
};

class Date :public DataBase {
private:
	int days2[31];             //�Ѵ� 31���� �������� 1�Ϻ��� 31�ϱ����� ��Ÿ���ش�.

public:
	void edit_meals();        //�Ĵ��� �����Ҽ� �ִ� �Լ�.(������ �߰� �Ѵ� �����ϴ�.)
	void print_grocery();	  //�Ĵ��� �����ϰ��ִ� ���������� display���ش�.
};

class Meal :public DataBase {
private:
	int numOfPeople;          //�Ļ縦 �� �ο���. �ο����� �°� (�����ǿ� �ʿ��� ���)*(�ο���) �� �ؼ� �� �Ļ翡 �ʿ��� ��Ḧ Ȯ���Ҽ� ����.


public :
	void select_recipe();     //�Ĵ��� �����Ǹ� �����Ѵ�. (������ ���̽��� �����ǿ��� ���� ����.)
	void grocery_list();	  //������ ����Ʈ�� display.

};