# AWS IoT Core와 아두이노 (MKR WiFi 1010) 연결하기 실습

##  준비사항
- [LED와 DHT-11 센서를 포함한 MKR WiFi 1010 보드 설정하기](arduino-setting.md)
- 유효한 AWS 계정

<a name="1"></a>
## 1 관련 라이브러리 설치
1. **Arduino IDE**의 **스케치-라이브러리 포함하기-라이브러리 관리** 메뉴 클릭
2. 검색 창에서 다음 5가지 라이브러리를 검색하여 설치
	- WiFiNINA (or WiFi101 for the MKR1000)
	- ArduinoBearSSL
	- ArduinoECCX08
	- ArduinoMqttClient
	- Arduino Cloud Provider Examples

<a name="2"></a>
## 2 AWS IoT 정책 생성
- **AWS IoT 정책**은 디바이스가 **MQTT 주제 구독 또는 게시와 같은 AWS IoT 작업을 수행할 수 있는 권한을 부여**하는 데 사용합니다.
- **디바이스가 AWS IoT 작업을 수행하도록 허용하려면 AWS IoT 정책을 생성하여 디바이스 인증서에 연결**해야 합니다.
- **AWS IoT 정책을 생성하려면**
	1. 왼쪽 탐색 창에서 **보안**을 선택하고 **정책**을 선택합니다.
	2. **정첵** 페이지에서 [**생성**]를 선택합니다.
	3. **정책 생성** 페이지의 **이름** 필드에 정책 이름(예: *AllowEverything*)을 입력합니다.
		- **작업** 필드에 iot:*를 입력합니다.
		- **리소스 ARN 필드**에 *를 입력합니다.
		- [**허용**] 확인란을 선택합니다.
		- **생성**을 클릭합니다.

<a name="3"></a>
## 3 AWS IoT Core에 아두이노 보드를 연결하기 위한 설정
- AWS IoT Core는 X.509 인증서를 사용하여 디바이스를 인증함.
- X.509 인증서를 생성하기 위해서, 아두이노 보드에서 CSR(Certificate Signing Request)를 생성한후, 이를 AWS 콘솔에서 업로드할 것이다.
	- CSR 생성 절차
		1. MKR WiFi 1010 보드를 USB 케이블로 컴퓨터와 연결
		2. **Arduino IDE**의 **파일-예제-ArduinoECCX08-Tools-ECCX08CSR** 메뉴 선텍
		3. 프로그램을 보드에 업로드 시킨다.
		3. 시리얼 모니터 창을 연다. (줄 마침 설정은 "**Both NL & CR**"로 되어 있어야 함)
		4. 시리얼 모니터 창에서 CSR에 포함될 정보를 입력하는데, 아래 그림과 같이 **Common Name** 을 제외하곤 모두 빈칸을 입력 (아무 입력없이 **전송** 버튼을 누름)
			- **Common Name**: *MyMKRWiFi1010* 입력
			- **Would you like to generate...?"** 질문에 *Y*를 입력한다.

			<img src="https://hackster.imgix.net/uploads/attachments/721577/screen_shot_2019-01-14_at_2_30_16_pm_Ljc1LdEu31.png?auto=compress%2Cformat&w=680&h=510&fit=max" width=500>
		5. "-----BEGIN CERTIFICATE REQUEST-----" 과 "-----END CERTIFICATE REQUEST-----" 사이에 생성된 CSR 문자열을 복사하여 텍스트 에디터에 붙여넣기 한 후, **csr.txt** 파일로 저장한다.
			- 이 파일은 후에 AWS 콘솔에서 X.509 인증서 생성을 위해 업로드 됨

			- csr.txt 예시

				<img src="figures/csr_txt.png" width=400>

<a name="4"></a>
## 4 레지스트리에 디바이스 등록
1. AWS Management Console에 로그인하고 [AWS IoT 콘솔](https://console.aws.amazon.com/iot/home)을 엽니다.
2. 왼쪽 탐색 창에서 **관리 > 사물**을 선택합니다.
3. **사물** 페이지에서 **사물 생성**을 클릭합니다.
4. **사물 생성** 페이지에서 **단일 사물 생성**을 선택하고, **다음**을 클릭합니다.
5. **사물 속성 지정** 페이지의 **사물 이름** 필드에 *MyMKRWiFi1010*과 같은 사물의 이름을 입력합니다.
6. **디바이스 섀도우** 에서 **이름 없는 섀도우(클래식)**을 선택한 후에 **다음**을  클릭합니다.
6. **디바이스 인증서 구성** 페이지에서 **CSR 업로드**를 선택합니다.
	- **파일 선택**를 클릭한 후 앞서 생성한 CSR 파일 (csr.txt)을 열고, **다음**을 클릭합니다.
7. 다음 화면에서 앞서 생성한 정책(*AllowEverything*)을 선택하고, **사물 생성**을 클릭합니다.
8. 이제 여러분은 **사물** 페이지에서 새롭게 등록된 사물을 확인할 수 있습니다.


<a name="5"></a>
## 5 디바이스 구성
- 모든 디바이스에는 AWS IoT와 통신할 수 있도록 디바이스 인증서가 설치되어 있어야 합니다.
	1. 왼쪽 탐색 창에서 **보안**을 선택하고 **인증서**를 선택합니다.
	2. 앞서 생성한 인증서의 상자에서 ...를 선택해 드롭다운 메뉴를 연 다음 **활성화**를 선택한 후에, [**다운로드**]를 선택합니다.
	3. 다운로드된 인증서(*xxxxxxxxxx-certificate.perm.crt*)를 확인합니다.
	4. 왼쪽 탐색 창에서 **설정**을 선택하고, **엔드포인트**를 기억해 둡니다.
	4. **Arduino IDE**의 **파일-예제-Arduino Cloud Provider Examples-AWSIoT-AWS\_IoT\_WiFi** 메뉴 선텍
		- arduino_secrets.h 탭에서, 사용가능한 WiFi의 SSID와 Password를 설정

			```c
			// Fill in  your WiFi networks SSID and password
			#define SECRET_SSID ""
			#define SECRET_PASS ""
			```
		- 앞에서 확인한 **엔드포인트**를 **SECRET\_BROKER** 값으로 설정

			```c
			// Fill in the hostname of your AWS IoT broker
			#define SECRET_BROKER "xxxxxxxxxxxxxx.iot.xx-xxxx-x.amazonaws.com"
			```
		- 앞에서 다운로드한 인증서 파일을 텍스트 에디터로 열고 값을 복사하여 다음 영역에 붙여넣기 한다.

			```c
			// Fill in the boards public certificate
			const char SECRET_CERTIFICATE[] = R"(
			-----BEGIN CERTIFICATE-----
			-----END CERTIFICATE-----
			)";
			```

<a name="6"></a>
## 6 테스트
1. 스케치 프로그램을 보드에 업로드하고, 시리얼 모니터 창을 연다.
	- 보드는 WiFi 네트워크와 연결을 시도하고, 성공하면 AWS IoT에 MQTT를 통해 연결을 시도한다.

		![](figures/aws-iot-connect.png)

2. MQTT Client를 통해 AWS IoT Core와 디바이스 간의 상호작용을 모니터링 한다.
	- 디바이스의 스케치 프로그램은 **arduino/outgoing** 토픽으로 매 5초마다 메시지를 AWS IoT Core로 보내고, **arduino/incoming** 토픽에 대한 메시지를 기다린다.
	- **디바이스에서 AWS IoT로의 상호작용 모니터링**
		1. **AWS IoT Core 콘솔**의 **테스트** 메뉴를 클릭
		2. **구독 주제 (Subscribe topic)** 박스에 *arduino/outgoing*을 입력하고 **구독** 버튼을 클릭한다.

			![](figures/subscribe-topic.png)
			- 매 5초마다 보드에서 보낸 *hello* 메시지가 현재 시간정보와 함께 표시된다.
			![](figures/arduino_outgoing.png)

	- **AWS IoT에서 디바이스로의 상호작용 모니터링**
		1. **주제 게시**메뉴의 **게시**섹션에서, 게시할 주제로 *arduino/incoming*을 입력하고, **게시** 버튼을 클릭한다.

			![](figures/publish-topic.png)

		2. 시리얼 모니터 창에서 메시지 수신을 확인한다.

			![](figures/receive-msg.png)
