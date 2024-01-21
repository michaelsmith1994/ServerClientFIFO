#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

#define FIFO_NAME "client2Server"
#define MEMORY_SIZE 10

int memory[MEMORY_SIZE] = {0};

void print_system_call_details(int client_number, int system_call_number, int num_parameters)
{
    // Function to print system call details
    char print_buffer[256];
    int print_length;

    print_length = snprintf(print_buffer, sizeof(print_buffer), "SYSTEM CALL FROM CLIENT '%d'\n", client_number);
    write(STDOUT_FILENO, print_buffer, print_length);

    print_length = snprintf(print_buffer, sizeof(print_buffer), "System Call %d requested with %d parameter(s).\n", system_call_number, num_parameters);
    write(STDOUT_FILENO, print_buffer, print_length);
}

int main()
{
    setbuf(stdout, NULL);
    int server_fifo, client_fifo;
    int client_number, system_call_number, num_parameters;
    char client_fifo_name[20];

    mkfifo(FIFO_NAME, 0666);

    printf("Waiting for clients...\n");
    fflush(stdout);

    server_fifo = open(FIFO_NAME, O_RDWR);

    while (1)
    {
        // Read client request
        read(server_fifo, &client_number, sizeof(client_number));
        read(server_fifo, &system_call_number, sizeof(system_call_number));
        read(server_fifo, &num_parameters, sizeof(num_parameters));

        char print_buffer[256];
        int print_length;

        print_length = snprintf(print_buffer, sizeof(print_buffer), "SYSTEM CALL FROM CLIENT '%d'\n", client_number);
        write(STDOUT_FILENO, print_buffer, print_length);

        print_system_call_details(client_number, system_call_number, num_parameters);

        if (system_call_number == 1)
        {
            // CONNECT system call
            char client_reply_fifo[20];
            snprintf(client_reply_fifo, sizeof(client_reply_fifo), "client_fifo_%d", client_number);

            int reply_fifo = open(client_reply_fifo, O_WRONLY);

            int client_pid = getpid();

            write(reply_fifo, &client_pid, sizeof(client_pid));

            close(reply_fifo);
        }
        else if (system_call_number == 2)
        {
            // DOUBLE system call
            int number_to_double;
            read(server_fifo, &number_to_double, sizeof(number_to_double));

            int result = number_to_double * 2;

            // Send the result back to the client
            char client_reply_fifo[20];
            snprintf(client_reply_fifo, sizeof(client_reply_fifo), "client_fifo_%d", client_number);

            mkfifo(client_reply_fifo, 0666);

            int reply_fifo = open(client_reply_fifo, O_WRONLY);

            write(reply_fifo, &result, sizeof(result));

            close(reply_fifo);
            unlink(client_reply_fifo);
        }
        else if (system_call_number == 3)
        {
            // TRIPLE system call
            int number_to_triple;
            read(server_fifo, &number_to_triple, sizeof(number_to_triple));

            int result = number_to_triple * 3;

            // Send the result back to the client
            char client_reply_fifo[20];
            snprintf(client_reply_fifo, sizeof(client_reply_fifo), "client_fifo_%d", client_number);

            mkfifo(client_reply_fifo, 0666);

            int reply_fifo = open(client_reply_fifo, O_WRONLY);

            write(reply_fifo, &result, sizeof(result));

            close(reply_fifo);
            unlink(client_reply_fifo);
        }
        else if (system_call_number == 4)
        {
            // STORE system call
            int value_to_store, memory_number;
            read(server_fifo, &value_to_store, sizeof(value_to_store));
            read(server_fifo, &memory_number, sizeof(memory_number));

            if (memory_number >= 0 && memory_number < MEMORY_SIZE)
            {
                memory[memory_number] = value_to_store;
                printf("Value %d stored in memory location %d.\n", value_to_store, memory_number);
            }
            else
            {
                printf("Invalid memory location. Memory location should be between 0 and %d.\n", MEMORY_SIZE - 1);
            }
        }
        else if (system_call_number == 5)
        {
            // RECALL system call
            int memory_number;
            read(server_fifo, &memory_number, sizeof(memory_number));

            int recalled_value = 0;  // Default value if memory location is invalid

            if (memory_number >= 0 && memory_number < MEMORY_SIZE)
            {
                recalled_value = memory[memory_number];
            }

            // Send the recalled value back to the client
            char client_reply_fifo[20];
            snprintf(client_reply_fifo, sizeof(client_reply_fifo), "client_fifo_%d", client_number);

            mkfifo(client_reply_fifo, 0666);

            int reply_fifo = open(client_reply_fifo, O_WRONLY);

            write(reply_fifo, &recalled_value, sizeof(recalled_value));

            close(reply_fifo);
            unlink(client_reply_fifo);

            printf("Recalled value from memory location %d: %d\n", memory_number, recalled_value);
        }
        else if (system_call_number == -1)
        {
            // EXIT/TERMINATE system call
            close(server_fifo);
            printf("TERMINATING...\n");
            printf("TERMINATION SUCCESSFUL!\n\n");
            break;
        }
        else
        {
            char error_message[] = "Invalid system call or incorrect number of parameters.\n";
            write(STDOUT_FILENO, error_message, sizeof(error_message) - 1);
        }
    }

    return 0;
}

