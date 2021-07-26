# active_team

### 팀 노션
https://www.notion.so/webserv-c80f39f04306484b858973d39af3129f

### 문서 공유
- notion 사용


### 작업 규칙

- 작업 브랜치 생성전에 작업 목표 issue 생성 후 issue NO. 와 동일한 NO.의 feature 브랜치 생성
- Merge 완료된 feature branch 는 바로 삭제하기
- 완료된 이슈 남겨두기
- Pull Request는 팀원의 Code review 후 직접  Merge 시키기
- 각 팀은 각 팀의 디렉토리 내에서 작업
- 공유 문서는 Master에서 feature branch 생성 후 루트 디렉토리에 추가. (추가 후 현재 문서 하단의 공유문서 리스트에 링크 업데이트)


#### 코드 컨벤션

- 들여쓰기 : \t (tap size: 4)

#### 주석 
- "//"
- 클래서 생성 시, 간단한 설명 주석 작성
- 
#### 클래스
- canonical form 규칙 준수
- getter, setter : get_m_variable, set_m_variable과 같이 사용한다.
- constructor, setter의 인자명은 멤버변수명에서 m_ 접두사만 제거하고 그대로 사용한다.

- 생성자 호출 시, Member initializer > 코드를 통한 초기화
 
#### 파일명
- class.cpp, class.hpp
- 각 구역에서 주석 구분 
  ```
  "Title comment": {
		"prefix": "bigcomment",
		"body": [
			"/* ---------------------------------------- */",
			"/* $2                    $1$2$3                     */",
			"/* ---------------------------------------- */",
			"",
		]
	},
  ```

#### 변수 및 함수 정의
- 클래스 : 대문자 구분
   - 오브젝트 : _ 구분 & 소문자 
   - 멤버함수 : 대문자 구분    ex) makeRequest
- 명명 순서 : 동사_명사
- 클래스의 멤버변수 : start with "m_"
- 함수 정의 시, "\n\t"
- 헤더 파일 만들 때, #ifndef

#### 헤더파일 내 나열 순서
- enum, static 변수, 멤버 변수, private 멤버 메소드, 캐노니컬 폼, getter, setter, public 멤버 메소드, 중첩 예외 클래스 순서로 작성한다.

#### 예외 처리
- throw ();

#### 기타
- char* s 보다는 char *s
