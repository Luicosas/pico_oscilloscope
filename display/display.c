// C library headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

int main() {
    // serial setup from https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
    // we pick the most common settings and hope its correct for the pi pico
    
    char * port_filename0 = "/dev/ttyACM0";
    char * port_filename1 = "/dev/ttyACM1";
    int serial_port = open(port_filename0, O_RDWR);

    // Check for errors
    if (serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
//        exit(1);
    }
    if(serial_port < 0) {
        serial_port = open(port_filename1, O_RDWR);
    }
    if (serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        exit(1);
    }
#ifdef WITH_STATUS
    printf("Opened \"%s\" port, serial_port: %d\n", port_filename0, serial_port);
#endif
    
    struct termios tty;
    // get tty settings struct
    if(tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        exit(1);
    }
    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all the size bits, then use one of the statements below
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    tty.c_cc[VTIME] = 100;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 128;
    cfsetspeed(&tty, B115200); // set baud rate
    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        exit(1);
    }
#ifdef WITH_STATUS
    printf("Set settings for serial port: %d\n", serial_port);
#endif
    
    unsigned char prev_byte_read = 0;
    bool is_debug_message = false;
    while(true) {
        struct timeval start_time;
        gettimeofday(&start_time, NULL);
        
        size_t total_bytes_read = 0;
        int64_t messages_printed = 0;
        const int num_chars = 'z' - 'a' + 1;
        while(true) {
            struct timeval cur_time;
            gettimeofday(&cur_time, NULL);
            int64_t usec_since_start = (cur_time.tv_sec * 1000 + cur_time.tv_usec / 1000) - (start_time.tv_sec * 1000 + start_time.tv_usec / 1000);
            if(usec_since_start > 1000) {
                break;
            }
            
            unsigned char read_buf[1024];
            size_t bytes_read = read(serial_port, &read_buf, sizeof(read_buf));
            total_bytes_read += bytes_read;
            
            for(int i = 0; i < bytes_read; i++) {
                if(read_buf[i] == '\0') {
                    is_debug_message ^= 1;
                } else if(is_debug_message) {
                    char c = ((char *)read_buf)[i];
//                    printf("%d(%c) ", read_buf[i], c);
                    printf("%c", c);
                } else {
                    unsigned char current_read_byte = read_buf[i] - 'a';
                    if((current_read_byte - prev_byte_read + num_chars) % (num_chars) != 1) {
                        printf("Host: Break occurred with previous char %d, current char %d\n", prev_byte_read, current_read_byte);
                    }
                    prev_byte_read = current_read_byte;
                }
            }
        }
        
        struct timeval cur_time;
        gettimeofday(&cur_time, NULL);
        int64_t usec_since_start = (cur_time.tv_sec * 1000 + cur_time.tv_usec / 1000) - (start_time.tv_sec * 1000 + start_time.tv_usec / 1000);

        int64_t average_byte_per_second = total_bytes_read * 1000 / usec_since_start;
        printf("Host: %ld milli seconds has passed, %ld bytes read, averaging %ld per second\n", usec_since_start, total_bytes_read, average_byte_per_second);
    }
    

    
    close(serial_port);
}
