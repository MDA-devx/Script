#include "uart_utils.h"
#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

int setup_uart(const char *device, int baud_rate) {
  int uart_fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
  if (uart_fd == -1) {
    std::cerr << "Failed to open UART device: " << device << std::endl;
    return -1;
  }

  struct termios options;
  tcgetattr(uart_fd, &options);
  cfsetispeed(&options, baud_rate);
  cfsetospeed(&options, baud_rate);
  options.c_cflag |= (CLOCAL | CREAD);
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;
  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CRTSCTS;
  tcsetattr(uart_fd, TCSANOW, &options);

  return uart_fd;
}