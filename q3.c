#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
typedef long long int INT; // INT is used in place of long long int

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
    struct stat fs;
    INT A = argc;
    if (A != 4)
    {
        perror("Incorrect number of arguments supplied: Argument form ---> ./a.out <new file> <old file> <directory>"); // unsuccessful call
        exit(1);                                                                                                        // exit from program
    }
    int F1 = open(argv[1], O_RDONLY); // open is a system call that is used to open a the input file and obtain its file descriptor.
    if ((F1 < 0))
    {
        perror("The given path to the new_file doesnt direct to a regular file:"); // unsuccessful call
        exit(1);                                                                   // exit from program
    }
    int F2 = open(argv[2], O_RDONLY); // open is a system call that is used to open a the input file and obtain its file descriptor.
    if ((F2 < 0))
    {
        perror("The given path to the old_file doesnt direct to a regular file:"); // unsuccessful call
        exit(1);                                                                   // exit from program
    }
    INT decr = 1000000;                                 // chunk-size=decr
    char *BUFFER1 = (char *)calloc(decr, sizeof(char)); // stores the string read using read() function
    char *BUFFER2 = (char *)calloc(decr, sizeof(char)); // stores the string read using read() function
    char *s = (char *)calloc(decr, sizeof(char));       // used in some-parts of the code where size of string read < decr
    char string[100] = {'\0'};
    char *dirname = argv[3];
    INT size1 = lseek(F1, 0, SEEK_END); // The file offset is set to the size of the file bytes.
    INT size2 = lseek(F2, 0, SEEK_END); // The file offset is set to the size of the file bytes.
    lseek(F1, 0, SEEK_SET);             // The file offset is set to 0 bytes
    INT index = 0;
    INT S1 = size1; // size of new file
    INT S2 = size2; // size of odl file
    int sz1;
    INT same_files = 1; // initialsising same_files to 1
    if (S1 != S2)
    {
        same_files = 0; // if new file and old file dont have the same size,then same_files=0
    }
    else
    {
        if ((size1 - (decr)) >= 0) // while (size of file that is to be read till the start_bit)>=(chunk_size)
        {
            while ((size1 - (decr)) >= 0)
            {
                lseek(F2, -decr, SEEK_CUR);
                read(F1, BUFFER1, decr);     // attempts to read up to chunk_size bytes from file descriptor fd into BUFFER starting at current cursor position.
                read(F2, BUFFER2, decr);     // attempts to read up to chunk_size bytes from file descriptor fd into BUFFER starting at current cursor position.
                size1 -= decr;               // decrementing current_size by chunk_size
                s = str_rev(BUFFER2);        // reversing BUFFER2 using str_rev() function
                if (strcmp(BUFFER1, s) != 0) // comparing both strings at each iteration
                {
                    same_files = 0; // if the two strings are not same,then same_files=0 and break from the loop
                    break;
                }
                lseek(F2, -(decr), SEEK_CUR); // The file offset is set to its current location minus chunk_size bytes.
            }
            if ((size1 > 0))
            {
                INT diff = size1;                               // initialising diff to curr_size
                lseek(F2, 0, SEEK_SET);                         // The file offset is set to 0 bytes.
                read(F1, BUFFER1, size1);                       // attempts to read up to chunk_size bytes from file descriptor fd into BUFFER starting at current cursor position.
                read(F2, BUFFER2, size1);                       // attempts to read up to chunk_size bytes from file descriptor fd into BUFFER starting at current cursor position.
                char *s1 = (char *)calloc(size1, sizeof(char)); // used in some-parts of the code where size of string read < decr
                char *s2 = (char *)calloc(size1, sizeof(char)); // used in some-parts of the code where size of string read < decr
                strncpy(s1, BUFFER1, size1);                    // copies up to diff+1 characters from the string pointed to, by BUFFER to s1.
                strncpy(s2, BUFFER2, size1);                    // copies up to diff+1 characters from the string pointed to, by BUFFER to s1.
                s = str_rev(s2);                                // reversing s2 using str_rev() function
                size1 -= diff;                                  // decrementing current_size by diff bytes
                if (strcmp(s1, s) != 0)                         // comparing both strings
                {
                    same_files = 0; // if the two strings are not same,then same_files=0
                }
            }
        }
        else
        {
            INT diff = size1;                               // initialising diff to curr_size
            lseek(F2, 0, SEEK_SET);                         // The file offset is set to 0 bytes.
            read(F1, BUFFER1, size1);                       // attempts to read up to chunk_size bytes from file descriptor fd into BUFFER starting at current cursor position.
            read(F2, BUFFER2, size1);                       // attempts to read up to chunk_size bytes from file descriptor fd into BUFFER starting at current cursor position.
            char *s1 = (char *)calloc(size1, sizeof(char)); // used in some-parts of the code where size of string read < decr
            char *s2 = (char *)calloc(size1, sizeof(char)); // used in some-parts of the code where size of string read < decr
            strncpy(s1, BUFFER1, size1);                    // copies up to diff+1 characters from the string pointed to, by BUFFER to s1.
            strncpy(s2, BUFFER2, size1);                    // copies up to diff+1 characters from the string pointed to, by BUFFER to s1.
            s = str_rev(s2);                                // reversing s2 using str_rev() function
            size1 -= diff;                                  // decrementing current_size by diff bytes
            if (strcmp(s1, s) != 0)                         // comparing both strings
            {
                same_files = 0; // if the two strings are not same,then same_files=0
            }
        }
    }
    close(F1); // close() closes a file descriptor, so that it no longer refers to any file and may be reused.
    close(F2); // close() closes a file descriptor, so that it no longer refers to any file and may be reused.

    /***************************/

    INT R;
    R = stat(argv[3], &fs);
    INT flag = S_ISDIR(fs.st_mode);
    if (S_ISDIR(fs.st_mode))
    {
        sprintf(string, "Directory is created: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "Directory is created: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }

    /**/

    R = stat(argv[1], &fs);
    if (R == -1)
    {
        perror("File error in new_file\n");
        exit(1);
    }

    /**/

    /**/
    if (same_files)
    {
        sprintf(string, "Whether file contents are reversed in newfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "Whether file contents are reversed in newfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    /**/
    if (fs.st_mode & S_IRUSR)
    {
        sprintf(string, "User has read permissions on newfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "User has read permissions on newfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    /**/
    if (fs.st_mode & S_IWUSR)
    {
        sprintf(string, "User has write permission on newfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "User has write permission on newfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    /**/
    if (fs.st_mode & S_IXUSR)
    {
        sprintf(string, "User has execute permission on newfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "User has execute permission on newfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    /**/
    if (fs.st_mode & S_IRGRP)
    {
        sprintf(string, "Group has read permissions on newfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "Group has read permissions on newfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    /**/
    if (fs.st_mode & S_IWGRP)
    {
        sprintf(string, "Group has write permission on newfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "Group has write permission on newfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    /**/
    if (fs.st_mode & S_IXGRP)
    {
        sprintf(string, "Group has execute permission on newfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "Group has execute permission on newfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    /**/
    if (fs.st_mode & S_IROTH)
    {
        sprintf(string, "Others has read permissions on newfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "Others has read permissions on newfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    /**/
    if (fs.st_mode & S_IWOTH)
    {
        sprintf(string, "Others has write permission on newfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "Others has write permission on newfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    /**/
    if (fs.st_mode & S_IXOTH)
    {
        sprintf(string, "Others has execute permission on newfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "Others has execute permission on newfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }

    /***************************************/
    R = stat(argv[2], &fs);
    if (R == -1)
    {
        perror("File error in old_file\n");
        exit(1);
    }
    /**/
    if (fs.st_mode & S_IRUSR)
    {
        sprintf(string, "User has read permissions on oldfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "User has read permissions on oldfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    /**/
    if (fs.st_mode & S_IWUSR)
    {
        sprintf(string, "User has write permission on oldfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "User has write permission on oldfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    /**/
    if (fs.st_mode & S_IXUSR)
    {
        sprintf(string, "User has execute permission on oldfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "User has execute permission on oldfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    /**/
    if (fs.st_mode & S_IRGRP)
    {
        sprintf(string, "Group has read permissions on oldfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "Group has read permissions on oldfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    /**/
    if (fs.st_mode & S_IWGRP)
    {
        sprintf(string, "Group has write permission on oldfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "Group has write permission on oldfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    /**/
    if (fs.st_mode & S_IXGRP)
    {
        sprintf(string, "Group has execute permission on oldfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "Group has execute permission on oldfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    /**/
    if (fs.st_mode & S_IROTH)
    {
        sprintf(string, "Others has read permissions on oldfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "Others has read permissions on oldfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    /**/
    if (fs.st_mode & S_IWOTH)
    {
        sprintf(string, "Others has write permission on oldfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "Others has write permission on oldfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    /**/
    if (fs.st_mode & S_IXOTH)
    {
        sprintf(string, "Others has execute permission on oldfile: Yes\n");
        write(STDOUT_FILENO, string, strlen(string));
    }
    else
    {
        sprintf(string, "Others has execute permission on oldfile: No\n");
        write(STDOUT_FILENO, string, strlen(string));
    }

    /**************************/

    /**/
    if (flag)
    {
        R = stat(argv[3], &fs);
        if (R == -1)
        {
            perror("Directory error in <directory>\n");
            exit(1);
        }
        if (fs.st_mode & S_IRUSR)
        {
            sprintf(string, "User has read permissions on directory: Yes\n");
            write(STDOUT_FILENO, string, strlen(string));
        }
        else
        {
            sprintf(string, "User has read permissidirectoryoldfile: No\n");
            write(STDOUT_FILENO, string, strlen(string));
        }
        /**/
        if (fs.st_mode & S_IWUSR)
        {
            sprintf(string, "User has write permission on directory: Yes\n");
            write(STDOUT_FILENO, string, strlen(string));
        }
        else
        {
            sprintf(string, "User has write permission on directory: No\n");
            write(STDOUT_FILENO, string, strlen(string));
        }
        /**/
        if (fs.st_mode & S_IXUSR)
        {
            sprintf(string, "User has execute permission on directory: Yes\n");
            write(STDOUT_FILENO, string, strlen(string));
        }
        else
        {
            sprintf(string, "User has execute permission on directory: No\n");
            write(STDOUT_FILENO, string, strlen(string));
        }
        /**/
        if (fs.st_mode & S_IRGRP)
        {
            sprintf(string, "Group has read permissions on directory: Yes\n");
            write(STDOUT_FILENO, string, strlen(string));
        }
        else
        {
            sprintf(string, "Group has read permissions on directory: No\n");
            write(STDOUT_FILENO, string, strlen(string));
        }
        /**/
        if (fs.st_mode & S_IWGRP)
        {
            sprintf(string, "Group has write permission on directory: Yes\n");
            write(STDOUT_FILENO, string, strlen(string));
        }
        else
        {
            sprintf(string, "Group has write permission on directory: No\n");
            write(STDOUT_FILENO, string, strlen(string));
        }
        /**/
        if (fs.st_mode & S_IXGRP)
        {
            sprintf(string, "Group has execute permission on directory: Yes\n");
            write(STDOUT_FILENO, string, strlen(string));
        }
        else
        {
            sprintf(string, "Group has execute permission on directory: No\n");
            write(STDOUT_FILENO, string, strlen(string));
        }
        /**/
        if (fs.st_mode & S_IROTH)
        {
            sprintf(string, "Others has read permissions on directory: Yes\n");
            write(STDOUT_FILENO, string, strlen(string));
        }
        else
        {
            sprintf(string, "Others has read permissions on directory: No\n");
            write(STDOUT_FILENO, string, strlen(string));
        }
        /**/
        if (fs.st_mode & S_IWOTH)
        {
            sprintf(string, "Others has write permission on directory: Yes\n");
            write(STDOUT_FILENO, string, strlen(string));
        }
        else
        {
            sprintf(string, "Others has write permission on directory: No\n");
            write(STDOUT_FILENO, string, strlen(string));
        }
        /**/
        if (fs.st_mode & S_IXOTH)
        {
            sprintf(string, "Others has execute permission on directory: Yes\n");
            write(STDOUT_FILENO, string, strlen(string));
        }
        else
        {
            sprintf(string, "Others has execute permission on directory: No\n");
            write(STDOUT_FILENO, string, strlen(string));
        }
    }
}
