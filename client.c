#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define FIFO_NAME "client2Server"

int main()
{
    int server_fifo, client_fifo;
    int client_number, system_call_number, num_parameters;
    char client_fifo_name[20];

    server_fifo = open(FIFO_NAME, O_WRONLY);

    printf("Enter your client ID number: ");
    scanf("%d", &client_number);

    printf("\n");

    while (1)
    {
        printf("What would you like to do next?\n");
        printf("1 - Connect to server\n");
        printf("2 - Double a number\n");
        printf("3 - Triple a number\n");
        printf("4 - Store a value\n");
        printf("5 - Recall a value\n");
        printf("6 - EXIT/TERMINATE\n");
        printf("Enter your choice: ");
        
        int choice;
        scanf("%d", &choice);

        if (choice == 1)
        {
            // CONNECT system call
            int connect_system_call = 1;
            int num_parameters = 1;

            // Send the CONNECT request to the server
            write(server_fifo, &client_number, sizeof(client_number));
            write(server_fifo, &connect_system_call, sizeof(connect_system_call));
            write(server_fifo, &num_parameters, sizeof(num_parameters));

            // Read client-specific FIFO from the server
            char client_reply_fifo[20];
            read(server_fifo, client_reply_fifo, sizeof(client_reply_fifo));

            int reply_fifo = open(client_reply_fifo, O_RDONLY);

            int server_pid;
            read(reply_fifo, &server_pid, sizeof(server_pid));

            printf("Connected to server with PID: %d\n\n", server_pid);

            close(reply_fifo);
        }
        else if (choice == 2)
        {
            // DOUBLE system call
            int number_to_double;
            printf("Enter the number to double: ");
            scanf("%d", &number_to_double);
            printf("\n");

            int num_parameters = 1;

            // Send the DOUBLE request to the server
            write(server_fifo, &client_number, sizeof(client_number));
            write(server_fifo, &choice, sizeof(choice));
            write(server_fifo, &num_parameters, sizeof(num_parameters));
            write(server_fifo, &number_to_double, sizeof(number_to_double));

            // Read the result from the server
            int result;

            // Create a client-specific FIFO to receive the result
            char client_reply_fifo[20];
            snprintf(client_reply_fifo, sizeof(client_reply_fifo), "client_fifo_%d", client_number);

            mkfifo(client_reply_fifo, 0666);

            int reply_fifo = open(client_reply_fifo, O_RDONLY);

            read(reply_fifo, &result, sizeof(result));

            printf("Doubled value: %d\n\n", result);

            close(reply_fifo);
            unlink(client_reply_fifo);
        }
        else if (choice == 3)
        {
            // TRIPLE system call (similar to DOUBLE)
            int number_to_triple;
            printf("Enter the number to triple: ");
            scanf("%d", &number_to_triple);
            printf("\n");

            int num_parameters = 1;

            // Send the TRIPLE request to the server
            write(server_fifo, &client_number, sizeof(client_number));
            write(server_fifo, &choice, sizeof(choice));
            write(server_fifo, &num_parameters, sizeof(num_parameters));
            write(server_fifo, &number_to_triple, sizeof(number_to_triple));

            // Read the result from the server
            int result;

            // Create a client-specific FIFO to receive the result
            char client_reply_fifo[20];
            snprintf(client_reply_fifo, sizeof(client_reply_fifo), "client_fifo_%d", client_number);

            mkfifo(client_reply_fifo, 0666);

            int reply_fifo = open(client_reply_fifo, O_RDONLY);

            read(reply_fifo, &result, sizeof(result));

            printf("Tripled value: %d\n\n", result);

            close(reply_fifo);
            unlink(client_reply_fifo);
        }
        else if (choice == 4)
        {
            // STORE system call
            int value_to_store, memory_number;
            printf("Enter the value to store: ");
            scanf("%d", &value_to_store);
            printf("Enter the memory number (1-9) to store in: ");
            scanf("%d", &memory_number);
            printf("\n");

            int num_parameters = 2;

            // Send the STORE request to the server
            write(server_fifo, &client_number, sizeof(client_number));
            write(server_fifo, &choice, sizeof(choice));
            write(server_fifo, &num_parameters, sizeof(num_parameters));
            write(server_fifo, &value_to_store, sizeof(value_to_store));
            write(server_fifo, &memory_number, sizeof(memory_number));

            printf("Value %d stored in memory %d.\n\n", value_to_store, memory_number);
        }
        else if (choice == 5)
        {
            // RECALL system call
            int memory_number;
            printf("Enter the memory number (1-9) to recall: ");
            scanf("%d", &memory_number);
            printf("\n");

            int num_parameters = 1;

            // Send the RECALL request to the server
            write(server_fifo, &client_number, sizeof(client_number));
            write(server_fifo, &choice, sizeof(choice));
            write(server_fifo, &num_parameters, sizeof(num_parameters));
            write(server_fifo, &memory_number, sizeof(memory_number));

            // Read the recalled value from the server
            int recalled_value;

            // Create a client-specific FIFO to receive the value
            char client_reply_fifo[20];
            snprintf(client_reply_fifo, sizeof(client_reply_fifo), "client_fifo_%d", client_number);

            mkfifo(client_reply_fifo, 0666);

            int reply_fifo = open(client_reply_fifo, O_RDONLY);

            read(reply_fifo, &recalled_value, sizeof(recalled_value));

            printf("Recalled value from memory %d: %d\n\n", memory_number, recalled_value);

            close(reply_fifo);
            unlink(client_reply_fifo);
        }
        else if (choice == 6)
        {
            // EXIT/TERMINATE system call
            int exit_signal = -1;
            write(server_fifo, &client_number, sizeof(client_number));
            write(server_fifo, &exit_signal, sizeof(exit_signal));
            printf("EXITING/TERMINATING...\n");
            printf("EXIT/TERMINATION SUCCESSFUL!\n\n");
            break;
        }
        else
        {
            printf("ERROR: Invalid choice!\n\n");
        }
    }

    close(server_fifo);

    return 0;
}

