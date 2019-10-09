# DM2019_homework1

>* Q: How to build and run the programs?
>
> - A: Follow this step
> - Step 1. Go to the directory with the puzzle name you want to solve. (There are 'NumberCross')
> - Step 2. Type "make clean" and then type "make" to build a program.
> - Step 3. The programs names are united as "quiz". So type like "./quiz <puzzle_file_name>".
> - Then, the program will show the solution for <puzzle_file_name>'s puzzle.

- Example: As an example, I want to get a solution for numbrix puzzle file named "test".

input:    

    ? ? ? ? ? ?
    ? ? 20 13 ? ?
    ? 26 ? ? 9 ?
    ? 25 ? ? 10 ?
    ? ? 23 36 ? ?
    ? ? ? ? ? ?

Step 1:

    s21800180@peace:/DiscreteMath/Math$ ls
    fillapix numbrix README.md sudoku

    s21800180@peace:/DiscreteMath/Math$ cd numbrix

    s21800180@peace:~/DiscreteMath/Math/numbrix$



Step 2:

    s21800180@peace:/DiscreteMath/Math/numbrix$ make clean
    rm *.o quiz

    s21800180@peace:/DiscreteMath/Math/numbrix$ make
    gcc -c -o numbrix.o numbrix.c gcc -o quiz numbrix.o

Step 3:

    s21800180@peace:~/DiscreteMath/Math/numbrix$ ./quiz test
output:    

    17 16 15 14 7 6
    18 19 20 13 8 5
    27 26 21 12 9 4
    28 25 22 11 10 3
    29 24 23 36 35 2
    30 31 32 33 34 1
