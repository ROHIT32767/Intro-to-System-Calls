#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>

typedef long long int INT; // INT is used in place of long long int

void slice(const char *str, char *result, size_t start, size_t end) // slice function is used to extract file name from path
{
    strncpy(result, str + start, end - start);
}

char *str_rev(char *str) // str_rev function is used to reverse a string
{
    char *p1, *p2;

    if (!str || !*str)
        return str;
    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return str;
}
int main(int argc, char *argv[]) // argc and argv are argument parameters
{
    INT A = argc;
    if (A != 2)
    {
        perror("Incorrect number of arguments supplied: Argument form --> ./a.out <input_file_path>"); // unsuccessful call
        exit(1);                                                                                       // exit from program
    }
    int fd = open(argv[1], O_RDONLY); // open is a system call that is used to open a the input file and obtain its file descriptor.
    if (fd < 0)
    {
        perror("The given path doesnt direct to a regular file:"); // unsuccessful call
        exit(1);                                                   // exit from program
    }
    INT decr = 1000000;                                // chunk-size=decr
    char *BUFFER = (char *)calloc(decr, sizeof(char)); // stores the string read using read() function
    char *s = (char *)calloc(decr, sizeof(char));      // used in some-parts of the code where size of string read < decr
    char string[100] = {'\0'};
    char address[113] = {'\0'};
    char *path = (char *)calloc(200, sizeof(char)); // path is used to store the path-input given as argument
    char *name = (char *)calloc(100, sizeof(char)); // name stores the file_name
    path = argv[1];                                 // storing input-path in string path
    INT length = strlen(path);                      // length is the size of the string path
    INT index = -1;                                 // index is initialised to -1
    for (INT i = length - 1; i >= 0; i--)           // finding the last occurence of the character '/' in path
    {
        if (path[i] == '/')
        {
            index = i;
            break;
        }
    }
    slice(path, name, index + 1, length); // retrieving file_name from the slice function

    INT size1 = lseek(fd, 0, SEEK_END);        // The file offset is set to the size of the file bytes.
    index = 0;                                 // Initialising index to 0
    INT size = size1;                          // initialising size to size1
    sprintf(address, "Assignment/1_%s", name); // sends formatted output to string pointed by address
    int check;
    char *dirname = "Assignment";
    check = mkdir(dirname, 0700);                                // creating a directory named "Assignment"
    int fd1 = open(address, O_WRONLY | O_CREAT | O_TRUNC, 0600); // open is a system call that is used to open the output file and obtain its file descriptor.
    if (fd1 < 0)
    {
        perror("The output file couldnt be opened:"); // unsuccessful call
        exit(1);                                      // exit from program
    }
    int sz1;
    if ((size1 - (2 * decr)) >= 0) // while (size of file that is to be read)>=(2*chunk_size)
    {
        lseek(fd, -decr, SEEK_CUR); // The file offset is set to its current location minus chunk_size bytes.
        while ((size1 - (2 * decr)) >= 0)
        {
            read(fd, BUFFER, decr);                                                               // attempts to read up to chunk_size bytes from file descriptor fd into BUFFER starting at current cursor position.
            BUFFER[decr] = '\0';                                                                  // sets (chunk_size+1)th bit (1_base-indexing) to null character
            lseek(fd, -(2 * decr), SEEK_CUR);                                                     // As reading chunk_size bytes from (filesize-chunk_size)th byte moves the cursor to (file-size)th byte,we are decrementing cursor by (2*chunk_size)bytes
            sprintf(string, "Progress:%.2lf%%\r", ((double)(size - size1) / (double)size) * 100); // sends formatted output to "string"
            write(STDOUT_FILENO, string, strlen(string));                                         // writing string to standard output
            size1 -= decr;                                                                        // decrementing current_size by chunk_size
            s = str_rev(BUFFER);                                                                  // reversing BUFFER using str_rev() function
            sz1 = write(fd1, s, strlen(s));                                                       // writes up to strlen(s) bytes from the buffer starting at s to the file referred to by the file descriptor fd1.
        }
        if ((size1 > 0)) // while current_size to be read > 0
        {
            if (size1 > (decr)) // if current_size to be read > chunk_size
            {
                INT diff = size1 - decr;                                                              // difference between curr_size and chunk_size
              //  lseek(fd, -decr, SEEK_CUR);                                                           // The file offset is set to its current location minus chunk_size bytes.
                read(fd, BUFFER, decr);                                                               // attempts to read up to chunk_size bytes from file descriptor fd into BUFFER starting at current cursor position.
                BUFFER[decr] = '\0';                                                                  // sets (chunk_size+1)th bit (1_base-indexing) to null character
                BUFFER = str_rev(BUFFER);                                                             // reversing BUFFER using str_rev() function
                int sz1 = write(fd1, BUFFER, strlen(BUFFER));                                         // writes up to strlen(BUFFER) bytes from the buffer starting at BUFFER to the file referred to by the file descriptor fd1.
                size1 -= decr;                                                                        // decrementing current_size by chunk_size
                sprintf(string, "Progress:%.2lf%%\r", ((double)(size - size1) / (double)size) * 100); // sends formatted output to "string"
                write(STDOUT_FILENO, string, strlen(string));                                         // writing string to standard output
                lseek(fd, 0, SEEK_SET);                                                               // The file offset is set to 0 bytes.
                INT count = read(fd, BUFFER, diff);                                                   // attempts to read up to diff bytes from file descriptor fd into BUFFER starting at current cursor position.
                BUFFER[diff] = '\0';                                                                  // sets (diff+1)th bit (1_base-indexing) to null character
                char *s1 = (char *)calloc(diff + 1, sizeof(char));                                    // s1 stores part of BUFFER
                strncpy(s1, BUFFER, diff + 1);                                                        // copies up to diff+1 characters from the string pointed to, by BUFFER to s1.
                s = str_rev(s1);                                                                      // reversing s1 using str_rev() function
                sz1 = write(fd1, s, strlen(s));                                                       // writes up to strlen(s) bytes from the buffer starting at s to the file referred to by the file descriptor fd1.
                size1 -= diff;                                                                        // decrementing current_size by diff bytes
                sprintf(string, "Progress:%.2lf%%\r", ((double)(size - size1) / (double)size) * 100); // sends formatted output to "string"
                write(STDOUT_FILENO, string, strlen(string));                                         // writing string to standard output
            }
            else
            {
                INT diff = size1;                                                                     // initialising diff to curr_size
                lseek(fd, 0, SEEK_SET);                                                               // The file offset is set to 0 bytes.
                read(fd, BUFFER, size1);                                                              // attempts to read up to diff bytes from file descriptor fd into BUFFER starting at current cursor position.
                BUFFER[diff] = '\0';                                                                  // sets (diff+1)th bit (1_base-indexing) to null character
                char *s1 = (char *)calloc(size1 + 1, sizeof(char));                                   // s1 stores part of BUFFER
                strncpy(s1, BUFFER, size1 + 1);                                                       // copies up to diff+1 characters from the string pointed to, by BUFFER to s1.
                s = str_rev(s1);                                                                      // reversing s1 using str_rev() function
                sz1 = write(fd1, s, strlen(s));                                                       // writes up to strlen(s) bytes from the buffer starting at s to the file referred to by the file descriptor fd1.
                size1 -= diff;                                                                        // decrementing current_size by diff bytes
                sprintf(string, "Progress:%.2lf%%\r", ((double)(size - size1) / (double)size) * 100); // sends formatted output to "string"
                write(STDOUT_FILENO, string, strlen(string));                                         // writing string to standard output
            }
        }
    }
    else
    {
        if (size1 > (decr))
        {
            INT diff = size1 - decr;                                                              // difference between curr_size and chunk_size
            lseek(fd, -decr, SEEK_CUR);                                                           // The file offset is set to its current location minus chunk_size bytes.
            read(fd, BUFFER, decr);                                                               // attempts to read up to chunk_size bytes from file descriptor fd into BUFFER starting at current cursor position.
            BUFFER[decr] = '\0';                                                                  // sets (chunk_size+1)th bit (1_base-indexing) to null character
            BUFFER = str_rev(BUFFER);                                                             // reversing BUFFER using str_rev() function
            int sz1 = write(fd1, BUFFER, strlen(BUFFER));                                         // writes up to strlen(BUFFER) bytes from the buffer starting at BUFFER to the file referred to by the file descriptor fd1.
            size1 -= decr;                                                                        // decrementing current_size by chunk_size
            sprintf(string, "Progress:%.2lf%%\r", ((double)(size - size1) / (double)size) * 100); // sends formatted output to "string"
            write(STDOUT_FILENO, string, strlen(string));                                         // writing string to standard output
            lseek(fd, 0, SEEK_SET);                                                               // The file offset is set to 0 bytes.
            INT count = read(fd, BUFFER, diff);                                                   // attempts to read up to diff bytes from file descriptor fd into BUFFER starting at current cursor position.
            BUFFER[diff] = '\0';                                                                  // sets (diff+1)th bit (1_base-indexing) to null character
            char *s1 = (char *)calloc(diff + 1, sizeof(char));                                    // s1 stores part of BUFFER
            strncpy(s1, BUFFER, diff + 1);                                                        // copies up to diff+1 characters from the string pointed to, by BUFFER to s1.
            s = str_rev(s1);                                                                      // reversing s1 using str_rev() function
            sz1 = write(fd1, s, strlen(s));                                                       // writes up to strlen(s) bytes from the buffer starting at s to the file referred to by the file descriptor fd1.
            size1 -= diff;                                                                        // decrementing current_size by diff bytes
            sprintf(string, "Progress:%.2lf%%\r", ((double)(size - size1) / (double)size) * 100); // sends formatted output to "string"
            write(STDOUT_FILENO, string, strlen(string));                                         // writing string to standard output
        }
        else
        {
            INT diff = size1;                                                                     // initialising diff to curr_size
            lseek(fd, 0, SEEK_SET);                                                               // The file offset is set to 0 bytes.
            read(fd, BUFFER, size1);                                                              // attempts to read up to diff bytes from file descriptor fd into BUFFER starting at current cursor position.
            BUFFER[diff] = '\0';                                                                  // sets (diff+1)th bit (1_base-indexing) to null character
            char *s1 = (char *)calloc(size1 + 1, sizeof(char));                                   // s1 stores part of BUFFER
            strncpy(s1, BUFFER, size1 + 1);                                                       // copies up to diff+1 characters from the string pointed to, by BUFFER to s1.
            s = str_rev(s1);                                                                      // reversing s1 using str_rev() function
            sz1 = write(fd1, s, strlen(s));                                                       // writes up to strlen(s) bytes from the buffer starting at s to the file referred to by the file descriptor fd1.
            size1 -= diff;                                                                        // decrementing current_size by diff bytes
            sprintf(string, "Progress:%.2lf%%\r", ((double)(size - size1) / (double)size) * 100); // sends formatted output to "string"
            write(STDOUT_FILENO, string, strlen(string));                                         // writing string to standard output
        }
    }
    close(fd);  // close() closes a file descriptor, so that it no longer refers to any file and may be reused.
    close(fd1); // close() closes a file descriptor, so that it no longer refers to any file and may be reused.
}