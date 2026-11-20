#include "insearch.h"

int display_database(c_db *hash_t)
{
    printf("\n%s\n", "-------------------------------------------------------------");
    printf("%-7s %-13s %-14s %-12s %-10s\n", "Index", "Word", "File Count", "File Name", "Word Count");
    printf("%s\n", "-------------------------------------------------------------");

    for (int key=0; key<27; key++)
    {
        if (hash_t[key].m_link != NULL)
        {
            m_node *m_temp=hash_t[key].m_link;
            
            while (m_temp != NULL)
            {
                printf("[%02d]    %-18s %-10d", key, m_temp->word, m_temp->file_count);

                s_node *s_temp = m_temp->sub_link;
                int first_sub_node = 1;

                while (s_temp != NULL)
                {
                    if (!first_sub_node)
                    {
                        printf("\n%37s", ""); 
                    }
                    printf("%-15s %-10d", s_temp->file_name, s_temp->word_count);
                    
                    s_temp = s_temp->s_sub_link;
                    first_sub_node = 0;
                }
                printf("\n");
                m_temp = m_temp->main_link;
            }
        }
    }
    printf("%s\n", "-------------------------------------------------------------");
    return SUCCESS;
}