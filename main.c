#include "insearch.h"

int main(int argc, char *argv[]) {
     
    int option; // menu option
    
    f_list *head = NULL; // Head of the input file list
    
    db_state state = DB_EMPTY; // Current database state
    
    c_db hash_table[27]; // Hash table used for the database

    // Initialize hash table entries
    for (int i = 0; i < 27; i++) {
        hash_table[i].key = i;    // store bucket key/index
        hash_table[i].m_link = NULL; // no entries yet
    }

    // Require at least one input file argument
    if (argc < 2) {
        printf("Usage: ./invsearch <file1.txt> <file2.txt> <...>\n");
        return FAILURE;
    }

    // Read and validate input files into `head`
    if (read_and_validate(argc, argv, &head) != SUCCESS) {
        return FAILURE;
    }

    // Main interactive loop
    while(1) {
        // Display menu options
        printf("\n1. Create Database\n"
                 "2. Display Database\n"
                 "3. Search Database\n"
                 "4. Update Database\n"
                 "5. Save Database\n"
                 "6. Exit\n"
                 "Enter your Choice: ");

        // Read the user's menu choice (default to 0 on failure)
        if (scanf("%d", &option) != 1) option = 0;

        switch (option) {

            case 1:
                // Create the database from the input list if not already created
                if (head == NULL)
                {
                    printf("INFO: No new files to add to the database!\n");
                    break;
                }
                if (state == DB_EMPTY) {
                    if (create_database(head, hash_table) == SUCCESS) {
                        state = DB_CREATED;
                        printf("SUCCESS: Full database creation from file list complete.\n");
                    }
                }
                else {
                    printf("WARNING: Database already exists. Operation cancelled.\n");
                }
                break;
            case 2:
                // Display current database contents
                if (state != DB_EMPTY)
                    display_database(hash_table);
                else
                    printf("INFO: Database is empty.\n");
                break;
            case 3:
                // Search the database for a given word
                if (state != DB_EMPTY) {
                    char word[30];
                    printf("Enter the word to search: ");
                    scanf("%s", word);
                    search_database(hash_table, word);
                } else {
                    // Prompt to create DB first
                    printf("Error: Database is empty. Create it first.\n");
                }
                break;
            case 4:
                if (state == DB_EMPTY) {
                    // 1. Load backup and 2. Remove duplicates from 'head'
                    if (update_database(hash_table, &head) == SUCCESS) {
                        state = DB_UPDATED;
                    }
                }
                else {
                    printf("ERROR: Update is only allowed on an empty database (before creation).\n");
                }
                break;
            case 5:
                // Save database functionality placeholder
                if(save_database(hash_table) == FAILURE){
                    printf("Error: Database not saved.\n");
                } else {
                    printf("Database Saved\n");
                }
                break;
            case 6:
                // Exit the program successfully
                printf("Exiting program...!\n");
                return SUCCESS;
        }
        // Consume leftover newline or input characters
        getchar(); // clear buffer
    }

    return SUCCESS;
}