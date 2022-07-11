/*
* Program to generate a report on the students having the top CGPAs
***************************************************************
* Author   Dept.          Date       Notes
***************************************************************
*haley tai Comp. Science. 2021.04.11 Initial version.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define the node in linked list
struct StudentRecord
{
    long sid;
    char email[30];
    char lname[20];
    char fname[20];
    float cgpa;
    struct StudentRecord *next;
};

// create a node from a line of file
struct StudentRecord * createNodeFromLine(char * line)
{
    struct StudentRecord * node;
    char * p;
    node = (struct StudentRecord *) malloc(sizeof(struct StudentRecord));
    if (!node)
    {
        // If the program runs out of memory
        printf("Error! program ran out of memory\n");
        exit(1);
    }
    memset(node, 0, sizeof(struct StudentRecord));
    if (line)
    {
        // split each field
        p = strtok(line, ",");
        node->sid = atol(p);

        p = strtok(NULL, ",");
        strcpy(node->email, p);

        p = strtok(NULL, ",");
        strcpy(node->lname, p);

        p = strtok(NULL, ",");
        strcpy(node->fname, p);

        p = strtok(NULL, "\n");
        node->cgpa = atof(p);
    }

    return node;
}

// insert a node into list, keep in sorted order
void ListInsert(struct StudentRecord * head, struct StudentRecord * node)
{
    struct StudentRecord * cur = head;
    // find the location
    while (cur->next && cur->next->cgpa >= node->cgpa)
    {
        cur = cur->next;
    }
    node->next = cur->next;
    cur->next = node;
}

// free the list
void ListDestroy(struct StudentRecord * head)
{
    if (head)
    {
        ListDestroy(head->next);
        free(head);
    }
}



int main(int argc, char *argv[])
{
    const char * input_filename;
    const char * output_filename;
    FILE * fin, *fout;
    int cnt;
    float top5Score;
    char line[256];
    char header[256];

    // If the program is not passed exactly 2 arguments
    if (argc != 3)
    {
        printf("Usage ./topcgpas <sourcecsv> <outputcsv>\n");
        return 1;
    }
    input_filename = argv[1];
    output_filename = argv[2];

    fin = fopen(input_filename, "r");

    // If the program cannot open the input le
    if (!fin)
    {
        printf("Error! Unable to open the input file %s\n", input_filename);
        return 1;
    }

    // The 1st line is the header (names of fields)
    // read header
    if (!fgets(header, 256, fin))
    {
        // If the input le is empty
        printf("Error! Input CSV file %s is empty\n", input_filename);
        return 1;
    }


    struct StudentRecord * head, *node;
    head = createNodeFromLine(NULL);

    //////////////////////////////////////////
    //    open input  file and read  data   //
    //////////////////////////////////////////

    while (fgets(line, 256, fin))
    {
        if (strlen(line) > 1)
        {
            node = createNodeFromLine(line);
            ListInsert(head, node);
        }
    }

    fclose(fin);

    // if the input file only has the header
    if (!head->next)
    {
        ListDestroy(head);
        printf("Error! Input CSV file %s is empty\n", input_filename);
        return 1;
    }

    //////////////////////////////////////////
    //    open output file and write data   //
    //////////////////////////////////////////

    fout = fopen(output_filename, "w");
    if (!fout)
    {
        printf("Error! Unable to open the output file %s\n", output_filename);
        return 1;
    }

    fputs(header, fout);

    node = head->next;
    cnt = 0;
    while (node && cnt < 5)
    {
        fprintf(fout, "%ld,%s,%s,%s,%.1f\n", node->sid, node->email, node->lname, node->fname, node->cgpa);
        top5Score = node->cgpa;
        node = node->next;
        cnt++;
    }

    // If there are any other students with the same CPGA as the fifth student that was written to the output
    // all those additional students must also be written to the output
    while (node && node->cgpa == top5Score)
    {
        fprintf(fout, "%ld,%s,%s,%s,%.1f\n", node->sid, node->email, node->lname, node->fname, node->cgpa);
        node = node->next;
    }
    fclose(fout);

    // free the linked list
    ListDestroy(head);

    return 0;
}


