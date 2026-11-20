
#include "insearch.h"

int create_database(f_list *head, c_db *arr)
{
    // Initialize temp with the head of the file linked list
    f_list *temp = head;
    char word[30];

    // Traverse all input files
    while (temp != NULL)
    {
        // Open the current file in read mode
        FILE *fptr = fopen(temp->f_name, "r");

        if (fptr == NULL)
        {
            printf("ERROR: Could not open %s for database creation.\n", temp->f_name);
            return FAILURE;
        }

        printf("INFO: Processing file: %s...\n", temp->f_name);

        // Read each word from the current file
        while (fscanf(fptr, "%29s", word) == 1)
        {
            // Find hash index using the first character of the word
            int key = get_hash_index(word[0]);

            // Check whether the hash index is empty
            if (arr[key].m_link == NULL)
            {
                // Allocate memory for main node and sub node
                m_node *m_new = malloc(sizeof(m_node));
                s_node *s_new = malloc(sizeof(s_node));

                
                if (!m_new || !s_new)
                {
                    free(m_new);
                    free(s_new);
                    fclose(fptr);
                    return FAILURE;
                }

                // Store word details in main node
                m_new->file_count = 1;
                strcpy(m_new->word, word);
                m_new->main_link = NULL;
                m_new->sub_link = s_new;

                // Store file details in sub node
                s_new->word_count = 1;
                strcpy(s_new->file_name, temp->f_name);
                s_new->s_sub_link = NULL;

                // Connect main node to hash table
                arr[key].m_link = m_new;
            }
            else
            {
                // Traverse the main linked list at this hash index
                m_node *mtemp = arr[key].m_link;
                m_node *mprev = NULL;
                int word_found = 0;

                while (mtemp != NULL)
                {
                    // Check whether the word already exists
                    if (strcmp(mtemp->word, word) == 0)
                    {
                        word_found = 1;

                        // Traverse the sub linked list to find the filename
                        s_node *stemp = mtemp->sub_link;
                        s_node *sprev = NULL;
                        int file_found = 0;

                        while (stemp != NULL)
                        {
                            // Check whether the current filename already exists
                            if (strcmp(stemp->file_name, temp->f_name) == 0)
                            {
                                // Increase word count if file already exists
                                stemp->word_count++;
                                file_found = 1;
                                break;
                            }

                            sprev = stemp;
                            stemp = stemp->s_sub_link;
                        }

                        // If word exists but filename is not present
                        if (!file_found)
                        {
                            // Create a new sub node
                            s_node *s_new = malloc(sizeof(s_node));
                            if (!s_new) 
                            {
                                fclose(fptr);
                                return FAILURE;
                            }

                            s_new->word_count = 1;
                            strcpy(s_new->file_name, temp->f_name);
                            s_new->s_sub_link = NULL;

                            // Connect new sub node to the existing sub list
                            sprev->s_sub_link = s_new;

                            // Increase the number of files containing this word
                            mtemp->file_count++;
                        }
                        break;
                    }

                    // Move to the next main node
                    mprev = mtemp;
                    mtemp = mtemp->main_link;
                }

                // If the word is not found, create a new main node
                if (!word_found)
                {
                    // Allocate memory for new main and sub nodes
                    m_node *m_new = malloc(sizeof(m_node));
                    s_node *s_new = malloc(sizeof(s_node));

                    if (!m_new || !s_new)
                    {
                        free(m_new);
                        free(s_new);
                        fclose(fptr);
                        return FAILURE;
                    }

                    // Initialize the new main node
                    m_new->file_count = 1;
                    strcpy(m_new->word, word);
                    m_new->main_link = NULL;
                    m_new->sub_link = s_new;

                    // Initialize the new sub node
                    s_new->word_count = 1;
                    strcpy(s_new->file_name, temp->f_name);
                    s_new->s_sub_link = NULL;

                    // Connect new main node to the main linked list
                    mprev->main_link = m_new;
                }
            }
        }

        printf("SUCCESS: Indexing complete for %s\n", temp->f_name);

        // Close the current file
        fclose(fptr);

        // Move to the next file in the file linked list
        temp = temp->link;
    }

    return SUCCESS;
}
