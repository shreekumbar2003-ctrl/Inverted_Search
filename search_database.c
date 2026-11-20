
#include "insearch.h"

int search_database(c_db *hash_table, char *word)
{
    // Check empty search input
    if (word == NULL || word[0] == '\0')
    {
        printf("Invalid search word.\n");
        return DATA_NOT_FOUND;
    }

    // Find hash index
    int index = get_hash_index(word[0]);

    // Get main node
    m_node *m_temp = hash_table[index].m_link;

    // Traverse main node list
    while (m_temp != NULL)
    {
        // Compare word
        if (strcmp(m_temp->word, word) == 0)
        {
            printf("The word '%s' is present in %d file(s):\n",
                   word, m_temp->file_count);

            // Traverse sub node list
            s_node *s_temp = m_temp->sub_link;

            while (s_temp != NULL)
            {
                printf("-> File: %-10s | Count: %d\n",
                       s_temp->file_name, s_temp->word_count);

                s_temp = s_temp->s_sub_link;
            }

            return SUCCESS;
        }

        m_temp = m_temp->main_link;
    }

    printf("Error: Word '%s' not found in the database.\n", word);

    return DATA_NOT_FOUND;
}

// Find hash index
int get_hash_index(char ch)
{
    if (isalpha((unsigned char)ch))
    {
        return tolower((unsigned char)ch) - 'a';
    }
    else
    {
        return 26;
    }
}
