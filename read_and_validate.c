
#include "insearch.h"

int read_and_validate(int argc, char *argv[], f_list **head)
{
    // Traverse all command-line arguments
    for (int i = 1; i < argc; i++)
    {
        // Check filename length
        if (strlen(argv[i]) >= 30)
        {
            printf("ERROR: File name is too long: %s\n", argv[i]);
            continue;
        }

        // Check whether the file is valid
        if (open_files(argv[i]) == SUCCESS)
        {
            // Point temp to the first node of the file linked list
            f_list *temp = *head;
            int duplicate = 0;

            // Check whether the filename is already present
            while (temp != NULL)
            {
                if (strcmp(temp->f_name, argv[i]) == 0)
                {
                    // Display warning if the file is already present
                    printf("WARNING: Duplicate file skipped: %s\n", argv[i]);
                    duplicate = 1;
                    break;
                }

                // Move to the next file node
                temp = temp->link;
            }

            // Add the file if it is not a duplicate
            if (!duplicate)
            {
                // Allocate memory for a new file node
                f_list *new = malloc(sizeof(f_list));

                if (new == NULL)
                {
                    return FAILURE;
                }

                // Copy the filename into the new node
                strcpy(new->f_name, argv[i]);

                // Connect the new node to the existing list
                new->link = *head;

                // Update head to point to the new node
                *head = new;

                printf("INFO: File '%s' validated and added to list successfully.\n", argv[i]);
            }
        }
        else
        {
            // Display error if the file is invalid
            printf("ERROR: File '%s' is empty, not a .txt file, or does not exist.\n", argv[i]);
        }
    }

    return SUCCESS;
}

int open_files(char *argv)
{
    // Find the last dot in the filename
    char *ext = strrchr(argv, '.');

    // Check whether the file extension is .txt
    if (ext && strcmp(ext, ".txt") == 0)
    {
        // Open the file in read mode
        FILE *fptr = fopen(argv, "r");

        if (fptr)
        {
            // Move the file pointer to the end of the file
            fseek(fptr, 0, SEEK_END);

            // Check whether the file is not empty
            if (ftell(fptr) > 0)
            {
                // Close the file
                fclose(fptr);
                return SUCCESS;
            }

            // Close the empty file
            fclose(fptr);
        }
    }

    // Return FAILURE if the file is invalid
    return FAILURE;
}
