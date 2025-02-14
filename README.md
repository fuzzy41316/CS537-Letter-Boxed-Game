# CS537 Fall 2024 - Project 1 - Letter Boxed

## 🔍 Problem Statement  
The goal of this project was to implement a **Letter Boxed solution validator** in C. Given a board configuration and a dictionary file, the program checks whether a sequence of words follows the game's rules, ensuring that:  
- All letters on the board are used at least once.  
- Consecutive words connect by first and last letter.  
- Letters on the same side are not used consecutively.  
- Words appear in the provided dictionary.

**These are the same rules as found on the [New York Times](https://www.nytimes.com/puzzles/letter-boxed)**

---

## 🎯 What I (re)Learned  
This assignment helped me refine my **C programming** skills and reinforced several key concepts:  
✅ **File Handling in C** – Reading and parsing structured input files efficiently.  
✅ **String Processing** – Validating words and enforcing game rules using string manipulation.  
✅ **Dynamic Memory Management** – Handling unknown input sizes using `malloc()` and `free()`.  
✅ **Error Handling & Debugging** – Implementing robust error checks for edge cases.  
✅ **Algorithm Design** – Optimizing lookups for dictionary validation and board state tracking.  

---

## 🏆 Results  
I successfully implemented all required functionality and passed **all test cases**, achieving **100% on the assignment**. This project was a great refresher on lower-level programming and reinforced best practices for writing clean, efficient C code.  

---
## 🎮 How to Play
**This is an example of a valid board for the game:** 
  ```
       r  o  k
     +---------+
     |         |
    w|         |e
     |         |
    f|         |d
     |         |
    a|         |n
     |         |
     +---------+
       l  c  i
```
You can always check [today's](https://www.nytimes.com/puzzles/letter-boxed) letter boxed puzzle here and make your own boar. As I'm writing this, this was today's board:
```
       t  e  b
     +---------+
     |         |
    s|         |w
     |         |
    v|         |i
     |         |
    a|         |h
     |         |
     +---------+
       o  n  g
```
You can create your own board going into a shell (zsh, bash, my [wsh](https://github.com/fuzzy41316/CS537-Unix-Shell-WSH). The world is your oyster.), and typing this:
```bash
$ echo -e "teb\nwih\nong\nsva\n" > example.board
```
The newline carriages ensure that the program can accurately determine all sides of the board. 
If you were to check the file contents of example.board, then you should get this:
```
cat example.board
teb
wih
ong
sva
```
Once you have your board you can then begin the program. 
```bash
$ ./letter-boxed example.board dict.txt
```
dict.txt is important, as it contains the dictionary of all allowable words you can use. It has a lot, so don't worry.
Playing with the previous board, a valid solution out of many could be:
```
givens
showboat
```
And this is what you type into the console. Words that aren't in the dictionary cannot be used - so words that aren't words. "givens" would provide an error here. If you provide the incorrect input in the form of violating the dictionary or the board, the program exits, and you need to start it up again. So make sure you follow the rules! Have fun!
