2018008531 송연주
**Cow Roller Coaster**

- 39~44 : 변수의 추가
  - firstPos : cow의 첫번째 위치
  - cowCount : 몇 번째 cow를 그렸는지 나타냄
  - cowPos : cow를 순서대로 선택하기 위한 배열
  - isNotInitialClick : 첫 번째 click인지 아닌지 나타냄
  - returnCow : cow roller coaster를 끝내고 초기 cow의 위치로 돌아오기 위한 변수
  - timeStart : animation을 시작할 때 이 변수를 1로 만들어서 표시함

- 227~230 : catmullRomSpline(P0, P1, P2, P3, t)
  함수의 추가, spline 곡선을 아용하기 위한 함수

- 233~259 : getHeadDir(p0, p1, p2, p3, t)
  함수의 추가, world coordinate에 상대적인 cow의 coordinate를 계산하고 cow의 방향을 계산함.

- 261~307 : display()
  함수의 수정
  - 262 : 위에서 새로 만든 global 변수를 사용함.
  - 276~306 : cowCount의 값에 따라 cow를 그릴 것인지 cow가 움직이는 애니매이션을 보여줄 것인지 결정한다. cowCount가 6이 되기 전까지는 커서를 놓은 위치에 cow를 그리면 되고 cowCount가 6이 되면 애니매이션을 출력한다.

- 289~416 : onMouseButton()
  함수의 수정
  - 390 : 위에서 새로 만든 global 변수를 사용함.
  - 391~392 : cowCount가 6이 되어 cow가 움직이고 있을 때에 마우스를 click하는 것을 방지함.
  - 397~412 : 마우스의 왼쪽 버튼이 눌리고 마우스 커서가 cow의 bounding box의 범위에 있을 때 마우스가 클릭되어있는 상태이면(GLFW_DOWN) isDrag의 값을 V_Drag로 바꾼다. 마우스가 떼져있고(GLFW_UP) drag중이라면 isNotInitialClick의 값을 1 증가시킨다. 가장 처음 1을 증가시켰을 때는 최초 click이기 때문에 아래의 if문을 실행하지 않지만 그 다음부터는 if문을 실행시킨다.