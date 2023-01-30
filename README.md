# StopTheAnts
Simple DirectX2D Project for learning AI

- How to clone my project
  - 
    - git clone
  ```shell
  git clone --recursive https://github.com/Naezan/StopTheAnts.git
  ```

- Structure
  - 
    - main
	    - App
        - InputSystem
        - FSM
		    - Game
          - Node(개미 경로를 저장)
			    - Object
            - dynamic
              - Ant
              - Cursor
            - static
              - Wall
              - Leaf
    - Math
      - PathFinding
      - 오일러 적분
      - 쿼드 트리
      - 브레젠험

- Feature
  - 
    - A* PathFinding
    - FSM(마우스커서로 특정 행동 시도)
    - 오일러 적분(개미 움직임)
    - 쿼드 트리(가까운 나뭇잎 찾기)
    - 브레젠험

- Ingame Image
  - 
    - TO DO(gif)

- TO DO List
  - 
    - [ ] FSM
    - [x] grid system
      * ~~좌표평면은 왼쪽위가 (0, 0)에서 시작하고 -가 왼쪽과 아래쪽, +가 오른쪽과 위쪽이다~~
      * ~~좌표평면은 (0, 0)과 (800, 800)이 최대다~~
    - [x] InputSystem
      * ~~MouseButton 클릭시 블럭 생성~~
      * ~~숫자키로 블럭 타입 변경~~
      * ~~블럭 타입에 맞게 블럭이 커서를 따라 다님~~
      * ~~마우스 휠로 블럭 크기 변경~~
    - [ ] Math
      * PathFinding(A*) or 브레젠험
      * 오일러 적분
      * 쿼드 트리

- Reference
  - 
    * TO DO