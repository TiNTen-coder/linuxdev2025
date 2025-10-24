#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#define BUFFER_SIZE 8192

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <infile> <outfile>\n", argv[0]);
        return 1;
    }
    
    const char *infile = argv[1];
    const char *outfile = argv[2];
    
    int fd_in, fd_out;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;
    struct stat st;
    
    fd_in = open(infile, O_RDONLY);
    if (fd_in == -1) {
        perror(infile);
        return 2;
    }
    
    if (fstat(fd_in, &st) == -1) {
        perror("fstat");
        close(fd_in);
        return 3;
    }
    
    fd_out = open(outfile, O_WRONLY | O_CREAT | O_EXCL, st.st_mode);
    if (fd_out == -1) {
        perror(outfile);
        close(fd_in);
        return 4;
    }
    
    while ((bytes_read = read(fd_in, buffer, BUFFER_SIZE)) > 0) {
        char *ptr = buffer;
        ssize_t remaining = bytes_read;
        
        while (remaining > 0) {
            bytes_written = write(fd_out, ptr, remaining);
            if (bytes_written == -1) {
                perror("write");
                close(fd_in);
                close(fd_out);
                unlink(outfile);
                return 5;
            }
            remaining -= bytes_written;
            ptr += bytes_written;
        }
    }
    
    if (bytes_read == -1) {
        perror("read");
        close(fd_in);
        close(fd_out);
        unlink(outfile);
        return 6;
    }
    
    if (fsync(fd_out) == -1) {
        perror("fsync");
        close(fd_in);
        close(fd_out);
        unlink(outfile);
        return 7;
    }
    
    close(fd_in);
    close(fd_out);
    
    if (unlink(infile) == -1) {
        perror("unlink");
        return 8;
    }
    
    return 0;
}
