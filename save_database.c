
#include "insearch.h"

int save_database(c_db *hash_t)
{
    // Declare an array to store the backup database filename
    char database[100];

    // Get the database filename from the user
    printf("Enter the database name (eg: backup.txt): ");
    scanf("%s", database);

    // Open the database file in write mode
    FILE *fp = fopen(database, "w");

    // Check whether the file is opened successfully
    if (fp == NULL)
    {
        printf("ERROR: Could not create/open '%s' for writing.\n", database);
        return FAILURE;
    }

    // Traverse all 28 hash table indices
    for (int key = 0; key < 27; key++)
    {
        // Check whether the current hash index contains data
        if (hash_t[key].m_link != NULL)
        {
            // Point to the first main node
            m_node *m_temp = hash_t[key].m_link;

            // Traverse the main linked list
            while (m_temp != NULL)
            {
                // Save hash index, word and file count into the file
                fprintf(fp, "#%d;%s;%d;", key, m_temp->word, m_temp->file_count);

                // Point to the first sub node
                s_node *s_temp = m_temp->sub_link;

                // Traverse the sub linked list
                while (s_temp != NULL)
                {
                    // Save filename and word count into the file
                    fprintf(fp, "%s;%d;", s_temp->file_name, s_temp->word_count);

                    // Move to the next sub node
                    s_temp = s_temp->s_sub_link;
                }

                // Mark the end of the current word record
                fprintf(fp, "#\n");

                // Move to the next main node
                m_temp = m_temp->main_link;
            }
        }
    }

    // Display success message after exporting the database
    printf("INFO: Database exported successfully to '%s'.\n", database);

    // Close the database file
    fclose(fp);

    return SUCCESS;
}
