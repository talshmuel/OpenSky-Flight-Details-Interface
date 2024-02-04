# OpenSky Network Flight Details Printer

This project provides an interface for printing flight details from the OpenSky Network. It is specifically designed to run on a Linux environment.

The project leverages processes and communication between them to enhance its functionality. By utilizing this approach, the program is able to effectively handle multiple tasks simultaneously, resulting in improved performance and efficient execution.

## Steps for Usage

To use this project, follow the steps outlined below:

1. **Clone repository**: 
    ```
    git clone <repository_url>
    ```
2. **Build the project**:
    ```
    cd <repository_directory>
    mkdir build
    cd build
    cmake ..
    make
    cd ..
    ```
3. **Run the program**:
    ```
    ./runMe.out
    ```
4. **Press "UpdateDB"**: 
    This option is number 4 in the menu. It is essential to create the database, and it should be the first action you take.

5. **Choose different actions from the menu**:
    Once the database is updated, you can select various actions from the menu.

## General Notes

Please take note of the following general information:

- **Operating System**: Linux.
- **Compiler**: GCC.
- The directory "flightsDB" should be in the root directory.
- To execute the program, you need permissions to read, write, and execute.

## Program Behavior Notes

Consider the following notes about the behavior of the program:

- If there aren't any arrivals or departures from a certain airport, the program will delete the files `.arv` or `.dst`, and if both are missing, it'll delete the airport directory.
- The program's arguments are case-sensitive. You should type Airports ICAO in uppercase and Airplane ICAO24 in lowercase.

---
