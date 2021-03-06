문자열을 받아 객체를 생성하는 방법은 우선 받아온 문자열의 첫자리를 살펴보고 '-'문자가 들어있다면 음수처리 아니라면 양수로 했습니다. 만약 양수라면 객체를 만들어주는데 digits배열은 들어온 문자열의 길이보다 1 크게('\0'을 삽입하기 위해.) 객체의 길이는 문자열의 길이와 같게 만들어 주고 맨 뒤의 문자부터 차례차례 digits[]에 넣어줍니다.(buf에 넣어 digits로 옮긺.) 음수라면 str의 마지막부터 str[1]까지 넣어줍니다.(부호는 넣지 않아도 되기 때문에.)

operator> 연산은 우선 두 수의 절댓값을 비교하고 부호를 비교하는 방법을 이용했습니다. 절댓값을 비교하는 방법은 우선 길이를 비교해서 같다면 가장 큰 자리수부터 차례대로 비교하는 방법을 이용했습니다. 이때 루프 카운트를 잘못 이용했는지 길이가 1일때는 비교가 반대로 되는 현상이 발생하여 길이가 1인경우만 따로 처리해주었습니다.

operator< 연산은 >연산과 반대의 임무를 수행시켰습니다.

operator- 연산은 다음과 같이 이루어집니다. 두 수의 부호 비교 ->  두 수 모두 양수라면 큰수에서 작은수 빼기/ 두 수 모두 음수라면 작은수에서 큰수를 뺀 후 부호 반전을 시켜줍니다. 두 수의 부호가 다르다면 덧셈연산을 하여 부호만 설정해 주면 됩니다. 음수 - 양수 = 음수 / 양수 - 음수 = 양수.

operator-를 위해 Sub라는 함수를 만들었습니다. operator- 에서 sub를 하기 위해 몇번의 판단을 거치는데 결국 Sub의 매개변수는 절대값이 큰것이 앞으로, 작은것이 뒤로 가게 됩니다. 따라서 이 함수에서는 절댓값이 큰 수에서 작은 수의 뺄셈만 하면 됩니다. 우선 한자리씩 비교하여 큰 수에서 작은수를 빼는 것이면 그냥 빼줍니다. 하지만 반대라면 앞의 자리에서 1을 빌려와 빼면 됩니다. 그 방법을 이렇게 했습니다. 우선 작은수의 숫자를 반전시킵니다.(예를들어 2였다면 8로, 4였다면 6으로.) 그리고 더해줍니다. 더한 값이 10보다 작다면 그대로 써주고 10보다 크다면 10을 빼줍니다. 그리고 앞의 자리에서 빌려왔을때는 앞의 자리에 -1을 해줍니다.(그러나 앞의 자리가 0인 경우는 9로 만들고 그 앞의 자리에 -1을 해준다.)

operator* 연산은 부호를 비교해 같은 부호의 곱셉을 하면 양수로, 아니라면 음수로 설정하였습니다.

operator*를 위해 Mult라는 함수를 만들었습니다. Mult에서는 두 수의 곱셈을 합니다. 우선 결과를 저장할 객체를 만들어 x의 길이보다 2만큼 더 큰 문자 배열을 만들어 줍니다.(첫 연산의 최대 길이가 x의 길이보다 1 큰 값이기 때문에. 마지막 자리에는 '\0'이 들어감.) 그리고 결과의 digits에 x를 y의 첫째 자리만큼 더한 값을 저장 해 줍니다. 다음 연산은 x를 y의 둘째 자리만큼 더한 값에 뒤에 '0'을 붙여 결과에 더해줍니다.(이 과정에서 늘어난 0만큼 realooc을 통해 digits의 길이를 다시 설정해줍니다.) 이렇게 y의 끝자리까지 다 곱해서 더해주면 결과를 리턴해주면 됩니다.
