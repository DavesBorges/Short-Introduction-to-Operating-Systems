## Project Kernel Simulator

The aim of the project is to create a simple Kernel Simulator with a time-sharing system.\
The kernel should execute processes which instructions come from files (pseudo programs) and share the time executing
each process accordingly to  the  Priority of each process.

## Project Specification
### Commands

Commands are read from a file and composed of an operator and an operand.
The **operator** can be any of the following **[jsr, load, store, fork, wait]**. Additionally **sleep** and **ret** operands were added.\
The **operands are integers**.

#### Examples of a process commands

 ``` assembly
 load 25
 store 10
 fork  1
 wait 2
 ret 0
```


The operators **fork**, **wait**, **sleep** and **ret** are threaded differently:

| Command          | Effect                                                                            |
| -----------------| ----------------------------------------------------------------------------------|
| fork \<operand\> | Generate a child process with a different id                                      |
| wait \<operand\> | The process will wait for the process of pid == operand to finish before continue |
| sleep \<operand\>| Sleep for \<operand\> seconds. Equivalent to this_thread::sleep_for() c++ function|
| ret  \<operand\> | Terminate the current process                                                     |

The pseudo_program files are created by the subproject of this repository [ProgramsGenerator](../ProgramsGenerator)
