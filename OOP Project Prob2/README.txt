
- 프로그램 작동 순서 -
1.프로그램을 실행 시키면 우선 프로그램이 작동하기 위한 각 변수들을 선언합니다. 
2.그 후 파일을 열어주는데 app모드로 한번 in모드로 한번 열어줍니다. (원래는 app모드로만 열었었는데 그렇게하면 파일의 내용을 배열에 입력하는데 오류가 나서 in모드로 한번 더 열어주었더니 제대로 작동하기에 그렇게 했습니다. app모드가 입출력 모두 되는 모드로 알고 있는데 왜 오류가 났는지는 잘 모르겠습니다.)
3.파일을 열면 우선 그 파일의 내용을 student배열에 넣습니다.(이전까지 입력된 자료를 모두 유지하는 과정)
4.그 후에 4가지 모드의 선택창이 나와 실행하고자 하는 번호를 입력하면 됩니다.

4.1) Insertion : 학생의 이름, 학번, 나이, 전공, 전화번호 순서대로 입력을 받습니다. 입력받은 정보는 지금까지 입력된 학생의 다음 학생객체에 넣습니다. 정보입력중 두가지 오류케이스가 있습니다. 첫번째는 학번이 중복되서 입력될 경우 Already inserted라는 에러 메세지를 출력합니다. 두번째로 학번이 10자리가 아닐 경우에도 에러 메세지를 출력합니다.

4.2) Search : 서치에는 5가지 모드가 있습니다. 전화번호를 제외한 다른 정보로 학생을 찾는것과 저장된 학생을 모두 보여주는 것입니다. 마지막것을 제외한 네가지의 서치 방법은 비슷 합니다. 정보를 하나 입력 받고 첫번째 학생부터 마지막 학생까지 한명한명 관찰해가며 같은 정보가 있는지 찾아내는 것입니다. 하지만 나이와 전공에서는 같은 정보를 가진 학생이 여러명 나올수 있기 때문에 새로운 학생 배열을 만들어 찾아낸 학생을 순서대로 배열에 집어 넣습니다. 그 후에 qsort를 이용하여 이름순서대로 정렬을 한 뒤 출력해줍니다. 학생을 모두 보여주는것은 처음부터 qsort를 한뒤 바로 출력해주면 됩니다.

4.3) Delete : 삭제는 학생의 학번을 가지고 진행됩니다. 중복되는 학번이 있으면 안된다는 사실로 인해 학번으로 진행되는듯 합니다. 학번을 입력받고 첫번째 학생부터 마지막 학생까지 같은 학번을 가진 학생을 찾습니다. 학생을 찾았다면 그 뒤에있는 학생부터는 배열의 한칸씩 앞으로 당깁니다. 단순히 당기기만 하는것이기때문에 그냥 바로 stud[i]=stud[i+1]과 같이 덮어 씌웠습니다. 그 후에 당겨진 배열이 완성되면 텍스트 파일에 저장을 해야하는데 여기서 어떻게 할지 많이 고민 했습니다. 고민 끝에 나온 방법은 원래 텍스트에 있던 내용을 다 지우고 새로운 배열에 들어온 학생의 정보를 입력하는 것입니다. 이렇게 하니 중간에 있는 학생이 텍스트에서 삭제되기는 했지만 한가지 문제점이 발생 했습니다. 중간에 qsort를 한번이라도 수행 한다면 그 정렬된 학생의 정보가 텍스트에 남아있다는 것입니다. 하지만 이렇게 하지 않고는 중간에있는 학생의 정보를 빼내지 못해서 수정하지는 못했습니다.