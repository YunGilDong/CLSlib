#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>
#include <fcntl.h>

int main(void)
{
	int fd;
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
	while (1)
	{
		/*if (write(fd, "forum.falinux.com", 17))
			printf("forum.falinux.com\n");*/
		if (write(fd, "1", 1))
			printf("1\n");
		// ǥ�� ����� �Լ��� �̿��Ͽ� �ø��� ��Ʈ�� 17���� ���ڿ��� �����մϴ�.
		usleep(1000000); //1000 msec
	}

	close(fd);
	// ��� ��Ʈ�� �ݾ� ����� �����մϴ�. 

	return 0;
}