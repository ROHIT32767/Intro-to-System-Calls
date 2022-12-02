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
    if (A != 4)
    {
        perror("Incorrect number of arguments supplied: Argument form ---> ./a.out <input_file_path> start_char end_char"); // unsuccessful call
        exit(1);                                           // exit from program
    }
    int fd = open(argv[1], O_RDONLY); // open is a system call that is used to open a the input file and obtain its file descriptor.
    if (fd < 0)
    {
        perror("The given path doesnt direct to a regular file:"); // unsuccessful call
        exit(1);                                                   // exit from program
    }
    INT decr = 1000000;                                // chunk-size=decr
    char *path = (char *)calloc(200, sizeof(char));    // path is used to store the path-input given as argument
    char *name = (char *)calloc(100, sizeof(char));    // name stores the file_name
    char *BUFFER = (char *)calloc(decr, sizeof(char)); // stores the string read using read() function
    char *s = (char *)calloc(decr, sizeof(char));      // used in some-parts of the code where size of string read < decr
    char string[100] = {'\0'};
    char address[113] = {'\0'};
    INT start_bit = atoll(argv[2]);       // start_bit stores start_char
    INT end_bit = atoll(argv[3]);         // end_bit stores end_char
    if(end_bit<start_bit)
    {
        perror("end_char < start_char,which is not in accordance with the question"); // unsuccessful call
        exit(1);                                                   // exit from program
    }
    path = argv[1];                       // path stores <input_file path>
    INT length = strlen(path);            // length stores the size of path
    INT index = -1;                       // index is initialised to -1
    for (INT i = length - 1; i >= 0; i--) // finding the last occurence of the character '/' in path
    {
        if (path[i] == '/')
        {
            index = i;
            break;
        }
    }
    slice(path, name, index + 1, length);      // retrieving file_name from the slice function
    sprintf(address, "Assignment/2_%s", name); // sends formatted output to string pointed by address

    INT size1 = lseek(fd, 0, SEEK_END);     // The file offset is set to the size of the file bytes.
    INT size = size1;                       // initialising size to size1
    INT SZ = 0;                             // SZ is used to print the formatted string so as to print the correct % of file that is read
    size1 = lseek(fd, start_bit, SEEK_SET); // The file offset is set to start_bit bytes.

    index = 0; // initialising index to 0

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
    INT flag = 1;
    if ((size1 - (2 * decr)) >= 0) // while (size of file that is to be read till the start_bit)>=(2*chunk_size)
    {
        lseek(fd, -decr, SEEK_CUR); // The file offset is set to its current location minus chunk_size bytes.
        while ((size1 - (2 * decr)) >= 0)
        {
            read(fd, BUFFER, decr);           // attempts to read up to chunk_size bytes from file descriptor fd into BUFFER starting at current cursor position.
            BUFFER[decr] = '\0';              // sets (chunk_size+1)th bit (1_base-indexing) to null character
            lseek(fd, -(2 * decr), SEEK_CUR); // As reading chunk_size bytes from (filesize till start_bit-chunk_size)th byte moves the cursor to (file-size till start_bit)th byte,we are decrementing cursor by (2*chunk_size)bytes

            sprintf(string, "Progress:%.2lf%%\r", ((double)(SZ) / (double)(size)) * 100); // sends formatted output to "string"
            write(STDOUT_FILENO, string, strlen(string));                                 // writing string to standard output
            size1 -= decr;                                                                // decrementing current_size by chunk_size
            SZ += decr;                                                                   // incrementing SZ by chunk_size
            s = str_rev(BUFFER);                                                          // reversing BUFFER using str_rev() function
            sz1 = write(fd1, s, strlen(s));                                               // writes up to strlen(s) bytes from the buffer starting at s to the file referred to by the file descriptor fd1.
        }
        if ((size1 > 0)) // while current_size to be read till start_bit > 0
        {
            if (size1 > (decr))
            {
                INT diff = size1 - decr;                      // difference between curr_size and chunk_size
              //  lseek(fd, -decr, SEEK_CUR);                   // The file offset is set to its current location minus chunk_size bytes.
                read(fd, BUFFER, decr);                       // attempts to read up to chunk_size bytes from file descriptor fd into BUFFER starting at current cursor position.
                BUFFER[decr] = '\0';                          // sets (chunk_size+1)th bit (1_base-indexing) to null character
                BUFFER = str_rev(BUFFER);                     // reversing BUFFER using str_rev() function
                int sz1 = write(fd1, BUFFER, strlen(BUFFER)); // writes up to strlen(BUFFER) bytes from the buffer starting at BUFFER to the file referred to by the file descriptor fd1.

                sprintf(string, "Progress:%.2lf%%\r", ((double)(SZ) / (double)(size)) * 100); // sends formatted output to "string"
                write(STDOUT_FILENO, string, strlen(string));                                 // writing string to standard output
                SZ += decr;                                                                   // incrementing SZ by chunk_size
                size1 -= decr;                                                                // decrementing current_size by chunk_size
                lseek(fd, 0, SEEK_SET);                                                       // The file offset is set to 0 bytes.
                INT count = read(fd, BUFFER, diff);                                           // attempts to read up to diff bytes from file descriptor fd into BUFFER starting at current cursor position.
                BUFFER[diff] = '\0';                                                          // sets (diff+1)th bit (1_base-indexing) to null character
                char *s1 = (char *)calloc(diff + 1, sizeof(char));                            // s1 stores part of BUFFER
                strncpy(s1, BUFFER, diff + 1);                                                // copies up to diff+1 characters from the string pointed to, by BUFFER to s1.
                s = str_rev(s1);                                                              // reversing s1 using str_rev() function
                sz1 = write(fd1, s, strlen(s));                                               // writes up to strlen(s) bytes from the buffer starting at s to the file referred to by the file descriptor fd1.

                sprintf(string, "Progress:%.2lf%%\r", ((double)(SZ) / (double)(size)) * 100); // sends formatted output to "string"
                write(STDOUT_FILENO, string, strlen(string));                                 // writing string to standard output
                size1 -= diff;                                                                // decrementing current_size by diff bytes
                SZ += diff;                                                                   // incrementing SZ by diff
            }
            else
            {
                INT diff = size1;                                   // initialising diff to curr_size
                lseek(fd, 0, SEEK_SET);                             // The file offset is set to 0 bytes.
                read(fd, BUFFER, size1);                            // attempts to read up to diff bytes from file descriptor fd into BUFFER starting at current cursor position.
                BUFFER[diff] = '\0';                                // sets (diff+1)th bit (1_base-indexing) to null character
                char *s1 = (char *)calloc(size1 + 1, sizeof(char)); // s1 stores part of BUFFER
                strncpy(s1, BUFFER, size1 + 1);                     // copies up to diff+1 characters from the string pointed to, by BUFFER to s1.
                s = str_rev(s1);                                    // reversing s1 using str_rev() function
                sz1 = write(fd1, s, strlen(s));                     // writes up to strlen(s) bytes from the buffer starting at s to the file referred to by the file descriptor fd1.

                sprintf(string, "Progress:%.2lf%%\r", ((double)(SZ) / (double)(size)) * 100); // sends formatted output to "string"
                write(STDOUT_FILENO, string, strlen(string));                                 // writing string to standard output
                size1 -= diff;                                                                // decrementing current_size by diff bytes
                SZ += diff;                                                                   // incrementing SZ by diff bytes
            }
        }
    }
    else
    {
        if (size1 > (decr))
        {
            INT diff = size1 - decr;                      // difference between curr_size and chunk_size
            lseek(fd, -decr, SEEK_CUR);                   // The file offset is set to its current location minus chunk_size bytes.
            read(fd, BUFFER, decr);                       // attempts to read up to chunk_size bytes from file descriptor fd into BUFFER starting at current cursor position.
            BUFFER[decr] = '\0';                          // sets (chunk_size+1)th bit (1_base-indexing) to null character
            BUFFER = str_rev(BUFFER);                     // reversing BUFFER using str_rev() function
            int sz1 = write(fd1, BUFFER, strlen(BUFFER)); // writes up to strlen(BUFFER) bytes from the buffer starting at BUFFER to the file referred to by the file descriptor fd1.

            sprintf(string, "Progress:%.2lf%%\r", ((double)(SZ) / (double)(size)) * 100); // sends formatted output to "string"
            write(STDOUT_FILENO, string, strlen(string));                                 // writing string to standard output
            SZ += decr;                                                                   // incrementing SZ by chunk_size
            size1 -= decr;                                                                // decrementing current_size by chunk_size
            lseek(fd, 0, SEEK_SET);                                                       // The file offset is set to 0 bytes.
            INT count = read(fd, BUFFER, diff);                                           // attempts to read up to diff bytes from file descriptor fd into BUFFER starting at current cursor position.
            BUFFER[diff] = '\0';                                                          // sets (diff+1)th bit (1_base-indexing) to null character
            char *s1 = (char *)calloc(diff + 1, sizeof(char));                            // s1 stores part of BUFFER
            strncpy(s1, BUFFER, diff + 1);                                                // copies up to diff+1 characters from the string pointed to, by BUFFER to s1.
            s = str_rev(s1);                                                              // reversing s1 using str_rev() function
            sz1 = write(fd1, s, strlen(s));                                               // writes up to strlen(s) bytes from the buffer starting at s to the file referred to by the file descriptor fd1.

            sprintf(string, "Progress:%.2lf%%\r", ((double)(SZ) / (double)(size)) * 100); // sends formatted output to "string"
            write(STDOUT_FILENO, string, strlen(string));                                 // writing string to standard output
            size1 -= diff;                                                                // decrementing current_size by diff bytes
            SZ += diff;                                                                   // incrementing SZ By diff bytes
        }
        else
        {
            INT diff = size1;                                   // initialising diff to curr_size
            lseek(fd, 0, SEEK_SET);                             // The file offset is set to 0 bytes.
            read(fd, BUFFER, size1);                            // attempts to read up to diff bytes from file descriptor fd into BUFFER starting at current cursor position.
            BUFFER[diff] = '\0';                                // sets (diff+1)th bit (1_base-indexing) to null character
            char *s1 = (char *)calloc(size1 + 1, sizeof(char)); // s1 stores part of BUFFER
            strncpy(s1, BUFFER, size1 + 1);                     // copies up to diff+1 characters from the string pointed to, by BUFFER to s1.
            s = str_rev(s1);                                    // reversing s1 using str_rev() function
            sz1 = write(fd1, s, strlen(s));                     // writes up to strlen(s) bytes from the buffer starting at s to the file referred to by the file descriptor fd1.

            sprintf(string, "Progress:%.2lf%%\r", ((double)(SZ) / (double)(size)) * 100); // sends formatted output to "string"
            write(STDOUT_FILENO, string, strlen(string));                                 // writing string to standard output
            size1 -= diff;                                                                // decrementing current_size by diff bytes
            SZ += diff;                                                                   // incrementing SZ by diff bytes
        }
    }
    /****REVERSING OF FILE ***********TILL start_char *******************COMPLETE****************************/
    size1 = lseek(fd, start_bit, SEEK_SET) - 1; // changing current_size to start_bit -1
    SZ = size - size1;

    if ((size1 + (decr)) <= end_bit) // while curr_size <= (end_bit-chunk_size)
    {
        while ((size1 + (decr)) <= end_bit)
        {
            read(fd, BUFFER, decr);                                                              // attempts to read up to chunk_size bytes from file descriptor fd into BUFFER starting at current cursor position.
            BUFFER[decr] = '\0';                                                                 // sets (diff+1)th bit (1_base-indexing) to null character
            sprintf(string, "Progress:%.2lf%%\r", ((double)(size - SZ) / (double)(size)) * 100); // sends formatted output to "string"
            write(STDOUT_FILENO, string, strlen(string));                                        // writing string to standard output
            size1 += decr;                                                                       // incrementing curr_size by decr
            SZ -= decr;                                                                          // decrementing SZ by decr
            sz1 = write(fd1, BUFFER, strlen(BUFFER));                                            // writes up to strlen(s) bytes from the buffer starting at s to the file referred to by the file descriptor fd1.
        }
        if ((size1 < end_bit))
        {
            INT diff = end_bit - size1;                                                          // initialising diff to end_bit-curr_size
            read(fd, BUFFER, diff);                                                              // attempts to read up to chunk_size bytes from file descriptor fd into BUFFER starting at current cursor position.
            BUFFER[diff] = '\0';                                                                 // sets (diff+1)th bit (1_base-indexing) to null character
            char *s1 = (char *)calloc(diff + 1, sizeof(char));                                   // s1 stores part of BUFFER
            strncpy(s1, BUFFER, diff + 1);                                                       // copies up to diff+1 characters from the string pointed to, by BUFFER to s1.
            sz1 = write(fd1, s1, strlen(s1));                                                    // writes up to strlen(s) bytes from the buffer starting at s to the file referred to by the file descriptor fd1.
            size1 += diff;                                                                       // incrementing curr_size by diff
            sprintf(string, "Progress:%.2lf%%\r", ((double)(size - SZ) / (double)(size)) * 100); // sends formatted output to "string"
            write(STDOUT_FILENO, string, strlen(string));                                        // writing string to standard output
            SZ -= diff;                                                                          // decrementing SZ by diff bytes
        }
    }
    else
    {
        INT diff = end_bit - size1;                                                          // initialising diff to end_bit-curr_size
        read(fd, BUFFER, diff);                                                              // attempts to read up to chunk_size bytes from file descriptor fd into BUFFER starting at current cursor position.
        BUFFER[diff] = '\0';                                                                 // sets (diff+1)th bit (1_base-indexing) to null character
        char *s1 = (char *)calloc(diff + 1, sizeof(char));                                   // s1 stores part of BUFFER
        strncpy(s1, BUFFER, diff + 1);                                                       // copies up to diff+1 characters from the string pointed to, by BUFFER to s1.
        sz1 = write(fd1, s1, strlen(s1));                                                    // writes up to strlen(s) bytes from the buffer starting at s to the file referred to by the file descriptor fd1.
        size1 += diff;                                                                       // incrementing curr_size by diff
        sprintf(string, "Progress:%.2lf%%\r", ((double)(size - SZ) / (double)(size)) * 100); // sends formatted output to "string"
        write(STDOUT_FILENO, string, strlen(string));                                        // writing string to standard output
        SZ -= diff;                                                                          // decrementing SZ by diff bytes
    }
    /****PRINTING THE FILE*********FROM start_char to end_char********** FINISHED******************/

    size1 = lseek(fd, 0, SEEK_END) - end_bit - 1; // changing curr_size to file_size-end_bit-1
    if ((size1 - (2 * decr)) >= 0)
    {
        lseek(fd, -decr, SEEK_CUR); // The file offset is set to its current location minus chunk_size bytes.
        while ((size1 - (2 * decr)) >= 0)
        {
            read(fd, BUFFER, decr);                                                              // attempts to read up to chunk_size bytes from file descriptor fd into BUFFER starting at current cursor position.
            BUFFER[decr] = '\0';                                                                 // sets (chunk_size+1)th bit (1_base-indexing) to null character
            lseek(fd, -(2 * decr), SEEK_CUR);                                                    // As reading chunk_size bytes from (filesize-chunk_size)th byte moves the cursor to (file-size)th byte,we are decrementing cursor by (2*chunk_size)bytes
            sprintf(string, "Progress:%.2lf%%\r", ((double)(size - SZ) / (double)(size)) * 100); // sends formatted output to "string"
            write(STDOUT_FILENO, string, strlen(string));                                        // writing string to standard output
            size1 -= decr;                                                                       // decrementing current_size by chunk_size
            SZ -= decr;                                                                          // decrementing SZ by decr bytes
            s = str_rev(BUFFER);                                                                 // reversing BUFFER using str_rev() function
            sz1 = write(fd1, s, strlen(s));                                                      // writes up to strlen(s) bytes from the buffer starting at s to the file referred to by the file descriptor fd1.
        }
        if ((size1 > 0)) // while current_size to be read > 0
        {
            if (size1 > (decr)) // if current_size to be read > chunk_size
            {
                INT diff = size1 - decr;                                                             // difference between curr_size and chunk_size
              //  lseek(fd, -decr, SEEK_CUR);                                                          // The file offset is set to its current location minus chunk_size bytes.
                read(fd, BUFFER, decr);                                                              // attempts to read up to chunk_size bytes from file descriptor fd into BUFFER starting at current cursor position.
                BUFFER[decr] = '\0';                                                                 // sets (chunk_size+1)th bit (1_base-indexing) to null character
                BUFFER = str_rev(BUFFER);                                                            // reversing BUFFER using str_rev() function
                int sz1 = write(fd1, BUFFER, strlen(BUFFER));                                        // writes up to strlen(BUFFER) bytes from the buffer starting at BUFFER to the file referred to by the file descriptor fd1.
                size1 -= decr;                                                                       // decrementing current_size by chunk_size
                sprintf(string, "Progress:%.2lf%%\r", ((double)(size - SZ) / (double)(size)) * 100); // sends formatted output to "string"
                write(STDOUT_FILENO, string, strlen(string));                                        // writing string to standard output
                SZ -= decr;                                                                          // decrementing SZ by decr bytes
                lseek(fd, end_bit + 1, SEEK_SET);                                                    // The file offset is set to end_bit+1 bytes.
                INT count = read(fd, BUFFER, diff);                                                  // attempts to read up to diff bytes from file descriptor fd into BUFFER starting at current cursor position.
                BUFFER[diff] = '\0';                                                                 // sets (diff+1)th bit (1_base-indexing) to null character
                char *s1 = (char *)calloc(diff + 1, sizeof(char));                                   // s1 stores part of BUFFER
                strncpy(s1, BUFFER, diff + 1);                                                       // copies up to diff+1 characters from the string pointed to, by BUFFER to s1.
                s = str_rev(s1);                                                                     // reversing s1 using str_rev() function
                sz1 = write(fd1, s, strlen(s));                                                      // writes up to strlen(s) bytes from the buffer starting at s to the file referred to by the file descriptor fd1.
                size1 -= diff;                                                                       // decrementing current_size by diff bytes
                SZ -= diff;                                                                          // decrementing SZ by diff bytes
                sprintf(string, "Progress:%.2lf%%\r", ((double)(size - SZ) / (double)(size)) * 100); // sends formatted output to "string"
                write(STDOUT_FILENO, string, strlen(string));                                        // writing string to standard output
            }
            else
            {
                INT diff = size1;                                   // initialising diff to curr_size
                lseek(fd, end_bit + 1, SEEK_SET);                   // The file offset is set to end_bit+1 bytes.
                read(fd, BUFFER, size1);                            // attempts to read up to diff bytes from file descriptor fd into BUFFER starting at current cursor position.
                BUFFER[diff] = '\0';                                // sets (diff+1)th bit (1_base-indexing) to null character
                char *s1 = (char *)calloc(size1 + 1, sizeof(char)); // s1 stores part of BUFFER
                strncpy(s1, BUFFER, size1 + 1);                     // copies up to diff+1 characters from the string pointed to, by BUFFER to s1.
                s = str_rev(s1);                                    // reversing s1 using str_rev() function
                sz1 = write(fd1, s, strlen(s));
                size1 -= diff;                                                                       // decrementing current_size by diff bytes
                SZ -= diff;                                                                          // decrementing SZ by diff bytes
                sprintf(string, "Progress:%.2lf%%\r", ((double)(size - SZ) / (double)(size)) * 100); // sends formatted output to "string"
                write(STDOUT_FILENO, string, strlen(string));                                        // writing string to standard output
            }
        }
    }
    else
    {
        if (size1 > (decr))
        {
            INT diff = size1 - decr;                                                             // difference between curr_size and chunk_size
            lseek(fd, -decr, SEEK_CUR);                                                          // The file offset is set to its current location minus chunk_size bytes.
            read(fd, BUFFER, decr);                                                              // attempts to read up to chunk_size bytes from file descriptor fd into BUFFER starting at current cursor position.
            BUFFER[decr] = '\0';                                                                 // sets (chunk_size+1)th bit (1_base-indexing) to null character
            BUFFER = str_rev(BUFFER);                                                            // reversing BUFFER using str_rev() function
            int sz1 = write(fd1, BUFFER, strlen(BUFFER));                                        // writes up to strlen(BUFFER) bytes from the buffer starting at BUFFER to the file referred to by the file descriptor fd1.
            size1 -= decr;                                                                       // decrementing current_size by chunk_size
            sprintf(string, "Progress:%.2lf%%\r", ((double)(size - SZ) / (double)(size)) * 100); // sends formatted output to "string"
            write(STDOUT_FILENO, string, strlen(string));                                        // writing string to standard output
            SZ -= decr;                                                                          // decrementing SZ by decr bytes
            lseek(fd, end_bit + 1, SEEK_SET);                                                    // The file offset is set to end_bit+1 bytes.
            INT count = read(fd, BUFFER, diff);                                                  // attempts to read up to diff bytes from file descriptor fd into BUFFER starting at current cursor position.
            BUFFER[diff] = '\0';                                                                 // sets (diff+1)th bit (1_base-indexing) to null character
            char *s1 = (char *)calloc(diff + 1, sizeof(char));                                   // s1 stores part of BUFFER
            strncpy(s1, BUFFER, diff + 1);                                                       // copies up to diff+1 characters from the string pointed to, by BUFFER to s1.
            s = str_rev(s1);                                                                     // reversing s1 using str_rev() function
            sz1 = write(fd1, s, strlen(s));                                                      // writes up to strlen(s) bytes from the buffer starting at s to the file referred to by the file descriptor fd1.
            size1 -= diff;                                                                       // decrementing current_size by diff bytes
            SZ -= diff;                                                                          // decrementing SZ by diff bytes
            sprintf(string, "Progress:%.2lf%%\r", ((double)(size - SZ) / (double)(size)) * 100); // sends formatted output to "string"
            write(STDOUT_FILENO, string, strlen(string));                                        // writing string to standard output
        }
        else
        {
            INT diff = size1;                                   // initialising diff to curr_size
            lseek(fd, end_bit + 1, SEEK_SET);                   // The file offset is set to end_bit+1 bytes.
            read(fd, BUFFER, size1);                            // attempts to read up to diff bytes from file descriptor fd into BUFFER starting at current cursor position.
            BUFFER[diff] = '\0';                                // sets (diff+1)th bit (1_base-indexing) to null character
            char *s1 = (char *)calloc(size1 + 1, sizeof(char)); // s1 stores part of BUFFER
            strncpy(s1, BUFFER, size1 + 1);                     // copies up to diff+1 characters from the string pointed to, by BUFFER to s1.
            s = str_rev(s1);                                    // reversing s1 using str_rev() function
            sz1 = write(fd1, s, strlen(s));
            size1 -= diff;                                                                       // decrementing current_size by diff bytes
            SZ -= diff;                                                                          // decrementing SZ by diff bytes
            sprintf(string, "Progress:%.2lf%%\r", ((double)(size - SZ) / (double)(size)) * 100); // sends formatted output to "string"
            write(STDOUT_FILENO, string, strlen(string));                                        // writing string to standard output
        }
    }
    close(fd);  // close() closes a file descriptor, so that it no longer refers to any file and may be reused.
    close(fd1); // close() closes a file descriptor, so that it no longer refers to any file and may be reused.
}