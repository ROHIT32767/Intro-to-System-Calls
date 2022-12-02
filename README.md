## Assignment1 - Intro to System Calls
## 2021101113
## Operating Systems and Networks
## Gowlapalli Rohit
>#### All these commands are tested on Ubuntu Version 20.04.3 LTS (Focal Fossa) 
----------------------------------------------------------------------------------------
```
2021101113
├── q1.c
├── q2.c
├── q3.c
└── README.md
```
```
=> As it's not possible to read the input file in 1 iteration for all possible input files,I have chosen to read the file in smaller chunks of 1000000 bytes
=> It is assumed that the input file path directs to a valid file,else a perror is displayed for error handling
=> For q1.c,q2.c it is assumed that if the directory "Assignment" already exists,its just overwritten by adding new files into the directory
=> All programs use system calls only
=> sprintf is only used for formatting strings and printing progress
=> C-libraries like fcntl.h , errno.h ,string.h , limits.h , unistd.h & sys/stat.h are used in the following programs
=> Since strrev function in string.h is not working in my system,I have used my own str_rev function
=> I have used my own slice function to extract file-name from path of the file
=> It is assumed that the string-length of path of a file doesnt exceed 200
=> RAM_SIZE is assumed to be greater than 1000000 bytes
```

----------------------------------------------------------------------------------------

### Command to be executed to run q1.c 
  >*   `gcc q1.c`
  >*  `./a.out < input_file_path >`
```
=> progress-bar is of the form "Progress:<xyz.wv>%",(xyz.vw starts from 0.00 and progresses to 100.00)
=> If the path of a file is of the form /home/rohit/Desktop/IIIT-H/SEM-3/OSN/OSN22A/A1/q23.txt (Example),then a file with name 1_q23.txt is created in the directory "Assignment"
```
---------------------------------------------------------------------------------------

### Command to be executed to run q2.c
  >*   `gcc q2.c`
  >*  `./a.out < input_file_path > start_char end_char`
```
 => It is assumed that file_size > end_char > start_char >= 0
 => progress-bar is of the form "Progress:<xyz.wv>%",(xyz.vw starts from 0.00 and progresses to 100.00)
 => If the path of a file is of the form /home/rohit/Desktop/IIIT-H/SEM-3/OSN/OSN22A/A1/q23.txt (Example),then a file with name 1_q23.txt is created in the directory "Assignment"
```
----------------------------------------------------------------------------------------
### Command to be executed to run q3.c
  >*   `gcc q3.c`
  >*  `./a.out < newfile >< oldfile >< directory >`

```
The output to this file when given appropriate inputs is similar to:
Directory is created: Yes/No
Whether file contents are reversed in newfile: Yes/No
27 lines regarding the Read/write/execute permissions of newfile , oldfile & directory correspindingly

```
```
If the directory mentioned in the argument already exists ,then "Directory is created:No" is displayed on the screen
If the paths for the newfile/oldfile dont direct to a regular file,then the corresponding outputs regarding the permissions of those files are displayed as "No"
```







