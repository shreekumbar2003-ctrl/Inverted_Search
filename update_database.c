
#include "insearch.h"

int update_database(c_db *hash_t, f_list **head)
{
    FILE *fp;
    char filename[100];
    char word[30];
    char file_name[30];
    char ch, first, last = '\0';
    char *ext;
    int index, file_count, word_count;
    int i;

    printf("Enter the backup filename: ");
    scanf("%99s", filename);

    // 1. Check file extension
    ext = strrchr(filename, '.');

    if (ext == NULL || strcmp(ext, ".txt") != 0)
    {
        printf("Invalid file extension\n");
        return FAILURE;
    }

    // 2. Check file availability
    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("File not available\n");
        return FAILURE;
    }

    // 3. Check file status
    first = fgetc(fp);

    if (first == EOF)
    {
        printf("File is empty\n");
        fclose(fp);
        return FAILURE;
    }

    // 4. Check first and last character
    last = first;

    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch != '\n' && ch != '\r' &&
            ch != ' ' && ch != '\t')
        {
            last = ch;
        }
    }

    if (first != '#' || last != '#')
    {
        printf("Invalid backup file\n");
        fclose(fp);
        return FAILURE;
    }

    // 5. Read backup file from beginning
    rewind(fp);

    // 6. Read main node information until EOF
    while (fscanf(fp, " #%d;%29[^;];%d;",
                  &index, word, &file_count) == 3)
    {
        if (index < 0 || index >= 27 || file_count <= 0)
        {
            printf("Invalid backup data\n");
            fclose(fp);
            return FAILURE;
        }

        // 7. Create main node
        m_node *m_new = malloc(sizeof(m_node));

        if (m_new == NULL)
        {
            fclose(fp);
            return FAILURE;
        }

        strcpy(m_new->word, word);
        m_new->file_count = file_count;
        m_new->main_link = NULL;
        m_new->sub_link = NULL;

        // 8. Read subnode information
        for (i = 0; i < file_count; i++)
        {
            if (fscanf(fp, "%29[^;];%d;",
                       file_name, &word_count) != 2)
            {
                printf("Invalid subnode information\n");
                fclose(fp);
                return FAILURE;
            }

            if (word_count <= 0)
            {
                printf("Invalid word count\n");
                fclose(fp);
                return FAILURE;
            }

            // 9. Create subnode
            s_node *s_new = malloc(sizeof(s_node));

            if (s_new == NULL)
            {
                fclose(fp);
                return FAILURE;
            }

            strcpy(s_new->file_name, file_name);
            s_new->word_count = word_count;
            s_new->s_sub_link = NULL;

            // 10. Insert subnode
            if (m_new->sub_link == NULL)
            {
                m_new->sub_link = s_new;
            }
            else
            {
                s_node *temp = m_new->sub_link;

                while (temp->s_sub_link != NULL)
                {
                    temp = temp->s_sub_link;
                }

                temp->s_sub_link = s_new;
            }

            // 11. Delete backup filename from file list
            f_list *temp = *head;
            f_list *prev = NULL;

            while (temp != NULL)
            {
                if (strcmp(temp->f_name, file_name) == 0)
                {
                    if (prev == NULL)
                    {
                        *head = temp->link;
                    }
                    else
                    {
                        prev->link = temp->link;
                    }

                    free(temp);
                    break;
                }

                prev = temp;
                temp = temp->link;
            }
        }

        // 12. Check ending character
        if (fscanf(fp, " %c", &ch) != 1 || ch != '#')
        {
            printf("Invalid backup record\n");
            fclose(fp);
            return FAILURE;
        }

        // 13. Insert main node into hash table
        if (hash_t[index].m_link == NULL)
        {
            hash_t[index].m_link = m_new;
        }
        else
        {
            m_node *temp = hash_t[index].m_link;

            while (temp->main_link != NULL)
            {
                temp = temp->main_link;
            }

            temp->main_link = m_new;
        }
    }

    fclose(fp);

    // 14. Create database for remaining files
    if (*head != NULL)
    {
        if (create_database(*head, hash_t) == FAILURE)
        {
            return FAILURE;
        }
    }

    printf("Database updated successfully\n");

    return SUCCESS;
}
