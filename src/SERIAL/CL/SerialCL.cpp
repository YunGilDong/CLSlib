#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/poll.h>

int main(void)
{
	struct pollfd     poll_events;      // 체크할 event 정보를 갖는 struct
	int    poll_state;
	char   buf[1024];
	int fd;
	int cnt;
	// 통신 포트를 파일 개념으로 사용하기 위한 디스크립터 입니다.
	// 이 파일 디스크립터를 이용하여 표준 입출력 함수를 이용할 수 있습니다. 

	struct termios newtio;
	// 보오율이나 stop bit 크기 등의 시리얼 통신 환경을 설정하기 위해
	// termios 구조체를 선언했습니다. 

	fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NONBLOCK);

	// /dev/ttyS0를 사용하기 위해 open()함수를 사용합니다.
	// O_RDWR은 파일 디스크립터인 fd를 읽기와 쓰기 모드로 열기 위한 지정이며
	// O_NOCCTY와 O_NONBLOCK는 시리얼 통신 장치에 맞추어 추가했습니다.

	memset(&newtio, 0, sizeof(newtio));

	// 시리얼 통신환경 설정을 위한 구조체 변수 newtio 값을 0 바이트로 깨끗이 채웁니다. 

	newtio.c_cflag = B115200;   // 통신 속도 115200 
	newtio.c_cflag |= CS8;      // 데이터 비트가 8bit 
	newtio.c_cflag |= CLOCAL;   // 외부 모뎀을 사용하지 않고 내부 통신 포트 사용 
	newtio.c_cflag |= CREAD;    // 쓰기는 기본, 읽기도 가능하게 
	newtio.c_iflag = 0;         // parity 비트는 없음
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 1;

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);   // 포트에 대한 통신 환경을 설정합니다. 

	//char ch='0';
	//int readLen = 0;
	//while (true)
	//{
	//	if ((readLen = read(fd, &ch, 1024)) > 0)
	//	{
	//		printf("ch:%c\n", ch);
	//	}
	//	printf("readLen:%d\n", readLen);
	//	usleep(1000000); //1000 msec
	//}
	
	// poll 사용을 위한 준비
	poll_events.fd = fd;
	poll_events.events = POLLIN | POLLERR;          // 수신된 자료가 있는지, 에러가 있는지
	poll_events.revents = 0;

	while (1)
	{
		poll_state = poll(                               // poll()을 호출하여 event 발생 여부 확인     
			(struct pollfd*)&poll_events, // event 등록 변수
			1,  // 체크할 pollfd 개수
			1000   // time out 시간
		);

		if (0 < poll_state)                             // 발생한 event 가 있음
		{
			if (poll_events.revents & POLLIN)            // event 가 자료 수신?
			{
				cnt = read(fd, buf, 1024);				
				printf("data received - %d %s\n", cnt, buf);
			}
			if (poll_events.revents & POLLERR)      // event 가 에러?
			{
				printf("통신 라인에 에러가 발생, 프로그램 종료");
				break;
			}
		}
	}


	close(fd);
	// 통신 포트를 닫아 사용을 중지합니다. 

	return 0;
}