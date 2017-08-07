#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define SERIAL_PORT "/dev/cu.usbmodem1421"

int main(int argc, char const *argv[]) {

  char buf[255];                    /* バッファ */
  int fd;                           /* ファイルディスクリプタ */
  struct termios oldtio, newtio;    /* シリアル通信設定 */
  int count;

  fd = open(SERIAL_PORT, O_RDWR);   /* デバイスをオープンする */

  // ioctl(fd, TCGETS, &oldtio);       /* 現在のシリアルポートの設定を待避させる */
  tcgetattr(fd, &oldtio);
  newtio = oldtio;                  /* ポートの設定をコピー */

  cfsetospeed(&newtio, (speed_t)B9600);
  cfsetispeed(&newtio, (speed_t)B9600);

  /* Setting other Port Stuff */
  newtio.c_cflag     &=  ~PARENB;            // Make 8n1
  newtio.c_cflag     &=  ~CSTOPB;
  newtio.c_cflag     &=  ~CSIZE;
  newtio.c_cflag     |=  CS8;

  newtio.c_cflag     &=  ~CRTSCTS;           // no flow control
  newtio.c_cc[VMIN]   =  1;                  // read doesn't block
  newtio.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
  newtio.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

  /* Make raw */
  cfmakeraw(&newtio);

  /* Flush Port, then applies attributes */
  tcflush(fd, TCIFLUSH);

  tcsetattr(fd, TCSANOW, &newtio);       /* ポートの設定を有効にする */




  while (1) {
    ioctl(fd, FIONREAD, &count);
    printf("count: %d\n", count);
    if (count >= 3) {
      memset(buf, '\0', sizeof(buf));
      read(fd, buf, sizeof(buf));
      printf("%s\n", buf);
    }
    usleep(800000);
  }

  // read(fd, buf, sizeof(buf));       /* デバイスから255バイト読み込み */
  // write(fd, buf, sizeof(buf));      /* デバイスへ255バイト書き込み */

  // printf("%s\n", buf);

  // tcsetattr(fd, TCSANOW, &oldtio);       /* ポートの設定を元に戻す */
  close(fd);                        /* デバイスのクローズ */

  return 0;
}
