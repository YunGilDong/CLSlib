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
	struct pollfd     poll_events;      // üũ�� event ������ ���� struct
	int    poll_state;
	char   buf[1024];
	int fd;
	int cnt;
	// ��� ��Ʈ�� ���� �������� ����ϱ� ���� ��ũ���� �Դϴ�.
	// �� ���� ��ũ���͸� �̿��Ͽ� ǥ�� ����� �Լ��� �̿��� �� �ֽ��ϴ�. 

	struct termios newtio;
	// �������̳� stop bit ũ�� ���� �ø��� ��� ȯ���� �����ϱ� ����
	// termios ����ü�� �����߽��ϴ�. 

	fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NONBLOCK);

	// /dev/ttyS0�� ����ϱ� ���� open()�Լ��� ����մϴ�.
	// O_RDWR�� ���� ��ũ������ fd�� �б�� ���� ���� ���� ���� �����̸�
	// O_NOCCTY�� O_NONBLOCK�� �ø��� ��� ��ġ�� ���߾� �߰��߽��ϴ�.

	memset(&newtio, 0, sizeof(newtio));

	// �ø��� ���ȯ�� ������ ���� ����ü ���� newtio ���� 0 ����Ʈ�� ������ ä��ϴ�. 

	newtio.c_cflag = B115200;   // ��� �ӵ� 115200 
	newtio.c_cflag |= CS8;      // ������ ��Ʈ�� 8bit 
	newtio.c_cflag |= CLOCAL;   // �ܺ� ���� ������� �ʰ� ���� ��� ��Ʈ ��� 
	newtio.c_cflag |= CREAD;    // ����� �⺻, �б⵵ �����ϰ� 
	newtio.c_iflag = 0;         // parity ��Ʈ�� ����
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 1;

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);   // ��Ʈ�� ���� ��� ȯ���� �����մϴ�. 

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
	
	// poll ����� ���� �غ�
	poll_events.fd = fd;
	poll_events.events = POLLIN | POLLERR;          // ���ŵ� �ڷᰡ �ִ���, ������ �ִ���
	poll_events.revents = 0;

	while (1)
	{
		poll_state = poll(                               // poll()�� ȣ���Ͽ� event �߻� ���� Ȯ��     
			(struct pollfd*)&poll_events, // event ��� ����
			1,  // üũ�� pollfd ����
			1000   // time out �ð�
		);

		if (0 < poll_state)                             // �߻��� event �� ����
		{
			if (poll_events.revents & POLLIN)            // event �� �ڷ� ����?
			{
				cnt = read(fd, buf, 1024);				
				printf("data received - %d %s\n", cnt, buf);
			}
			if (poll_events.revents & POLLERR)      // event �� ����?
			{
				printf("��� ���ο� ������ �߻�, ���α׷� ����");
				break;
			}
		}
	}


	close(fd);
	// ��� ��Ʈ�� �ݾ� ����� �����մϴ�. 

	return 0;
}