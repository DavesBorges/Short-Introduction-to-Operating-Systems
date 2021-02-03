## Project Kernel Simulator

The aim of the project is to create a simple Kernel Simulator with a time-sharing system.\
The kernel should execute processes which instructions come from files (pseudo programs) and share the time executing
each process accordingly to  the  Priority of each process. 

## Usage

#### **Note: if you generated visual studio solutions using CMake then you can skip the following step since the command line arguments are added to the Visual Studio Debug CommandLine arguments properties. In that case you will only have to run the DEBUG configuration**

The program gets the name of the files and priority which can be 1(LOW) 2(MEDIUM) or 3(HIGH)
There are pseudo programs example at [this folder](./pseudo_programs). You can copy them into the executable directory or use a relative path.

Windows cmd
```bash
Kernel-Simulator "Program A" 1 "Program B" 3 "Program C" 2
```
Bash
```bash
./Kernel-Simulator "Program A" 1 "Program B" 3 "Program C" 2
```
The pseudo_program files are created by the subproject of this repository [ProgramsGenerator](../ProgramsGenerator)
___



## Project Specification
### Commands
Commands are read from a file and composed of an operator and an operand.
The **operator** can be any of the following **[jsr, load, store, fork, wait]**. Additionally **sleep** and **ret** operands were added.
The **operands are integers**.

#### Examples of a process commands

 ``` assembly
 load  25
 store 10
 fork  1
 wait  2
 ret   0
```

#### Commands meaning

The operators **fork**, **wait**, **sleep** and **ret** are treated differently:

| Command          | Effect                                                                            |
| -----------------| ----------------------------------------------------------------------------------|
| fork \<operand\> | Generate a child process with a different id                                      |
| wait \<operand\> | The process will wait for the process of pid == operand to finish before continue |
| sleep \<operand\>| Sleep for \<operand\> seconds. Equivalent to this_thread::sleep_for() c++ function|
| ret  \<operand\> | Terminate the current process                                                     |

The remaining commands dont affect the execution of a process

