# Cannibals and Missionary Simulation

This program simulates a scenario involving cannibals and a missionary using processes, shared memory, and semaphores.

## Overview

The simulation involves cannibals and a missionary who share a common resource (a pot of portions) and a cook. The cook refills the pot whenever it's empty, and cannibals consume portions from the pot.

## Prerequisites

- C Compiler (e.g., GCC)
- Cygwin (for Windows users) or a Unix-like environment

## Getting Started

**My enviroment:**
    Install
  ```bash
    https://cygwin.com/install.html
  ```
  Adds
    
  ```bash
    mc
    make
    gcc-core
    cygrunsrv
  ```
  Start cygwin as admin
  ```bash
    cygserver-config
    cygrunsrv -S cygserver
  ```

1. **Clone the Repository:**
    
    ```bash
    git clone git@github.com:prozyr/LAB_task_Cannibals_and_Cooker.git
    cd LAB_task_Cannibals_and_Cooker
    ```

2. **Compile the Program:**

    ```bash
    make
    ```

3. **Run the Program:**

    ```bash
    ./cannibals_simulation
    ```

## Parameters in code

- `no_cannibals`: Number of cannibals in the simulation.
- `no_refill`: Number of times the cook refills the pot.
- `no_portion`: Number of portions added to the pot each time it's refilled.

## Sample Output

Include a sample output from running the program, showcasing the interactions between cannibals, the cook, and the missionary.

## License

This project is licensed under the [MIT License](LICENSE).

Feel free to contribute, report issues, or suggest improvements!
