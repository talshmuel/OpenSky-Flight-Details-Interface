# OpenSky Network Flight Details Printer

This project provides an interface for printing flight details from the OpenSky Network. The project is designed to run on a Linux environment.

This project leverages processes and communication between them to enhance its functionality. By utilizing this approach, the program is able to effectively handle multiple tasks simultaneously, resulting in improved performance and efficient execution.

## Running Instructions

To run this project, follow these steps:

1. **Clone repository**: 
    ```
    git clone <repository_url>
    ```

2. **Create the Database**: 
    Run the provided bash script by typing the following command in the terminal:
    ```
    bash flightScanner.sh <arg1> <arg2> <...>
    ```
    Replace `<arg1>`, `<arg2>`, etc. with the ICAO codes of the airports you want to include in the database.

3. **Build the project**:
    Run the following commands in the terminal in the given order:
    a. `sudo apt-get install libzip-dev`
    b. `sudo apt install cmake`
    c. `cmake .`
    d. `make`

5. **Run the project**:
    Execute the following command in the terminal:
    ```
    ./runMe
    ```

6. **Menu Options**:
    After running the project, the following menu will appear:
    ```
    1 - Fetch airports incoming flights.
    2 - Fetch airports full flights schedule.
    3 - Fetch aircraft full flights schedule.
    4 - Update DB.
    5 - Zip DB files.
    6 - Get child process PID.
    7 - Exit.
    Please make your choice <1,2,3,4,5,6,7>:
    ```
    Choose an option by entering the corresponding number.

7. **Input Requirements**:
    - For option 1: Enter the ICAO codes of airports as arguments.
    - For options 2 and 3: Enter the ICAO code of the airport or the ICAO24 codes of aircraft, respectively.




## General Information

- **Operating System**: Linux.
- **Compiler**: GCC.
- To execute the program, you need permissions to read, write, and execute.
- If you exit the program using option 7 or CTRL+C, the DB will be compressed into a zip file, and the original DB will be deleted.
- If you open the program a second time, the compressed DB will be extracted to the program folder.



---

