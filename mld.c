/* mld.c */
#include <stdio.h>
#include <stdlib.h>

#include "mld.h"
#include <assert.h>
#include <memory.h>

char *DATA_TYPE[] = {"UINT8", "UINT32", "INT32",
                     "CHAR", "OBJ_PTR", "VOID_PTR", "FLOAT",
                     "DOUBLE", "OBJ_STRUCT"};

/* Dumping Function */
void print_structure_rec(struct_db_rec_t *struct_rec)
{
    if (!struct_rec)
        return;

    int j = 0;

    field_info_t *field = NULL;

    printf("|------------------------------------------------------|\n");
    printf("| %-20s | size = %-8d | #flds = %-3d |\n", struct_rec->struct_name, struct_rec->ds_size, struct_rec->n_fields);
    printf("|------------------------------------------------------|------------------------------------------------------------------------------------------|\n");

    for (j = 0; j < struct_rec->n_fields; j++)
    {
        field = &struct_rec->fields[j];

        printf("  %-20s |", "");
        printf("%-3d %-20s | dtype = %-15s | size = %-5d | offset = %-6d|  nstructname = %-20s  |\n",
               j, field->fname, DATA_TYPE[field->dtype], field->size, field->offset, field->nested_str_name);
        printf("  %-20s |", "");
        printf("--------------------------------------------------------------------------------------------------------------------------|\n");
    }
}

void print_structure_db(struct_db_t *struct_db)
{
    if (!struct_db)
        return;

    printf("Printing STRUCTURE DATABASE : \n\n");

    int i = 0;

    struct_db_rec_t *struct_rec = NULL;
    struct_rec = struct_db->head;

    printf("No of Structures Registered = %d\n\n", struct_db->count);

    while (struct_rec)
    {
        printf("structure No : %d (%p)\n", i++, struct_rec);
        print_structure_rec(struct_rec);
        printf("\n");

        struct_rec = struct_rec->next;
    }
}

int add_structure_to_struct_db(struct_db_t *struct_db, struct_db_rec_t *struct_rec)
{
    struct_db_rec_t *head = struct_db->head;

    if (!head)
    {
        struct_db->head = struct_rec;
        struct_rec->next = NULL;
        struct_db->count++;

        return 0;
    }

    struct_rec->next = head;
    struct_db->head = struct_rec;
    struct_db->count++;

    return 0;
}

static struct_db_rec_t *struct_db_look_up(struct_db_t *struct_db, char *struct_name)
{
    struct_db_rec_t *head = struct_db->head;

    if (!head)
        return NULL;

    for (; head; head = head->next)
    {
        if (strncmp(head->struct_name, struct_name, MAX_STRUCTURE_NAME_SIZE) == 0)
            return head;
    }

    return NULL;
}

static object_db_rec_t *object_db_look_up(object_db_t *object_db, void *ptr)
{
    object_db_rec_t *head = object_db->head;

    if (!head)
        return NULL;

    for (; head; head = head->next)
    {
        if (head->ptr == ptr)
            return head;
    }

    return NULL;
}

/* Working with objects */
static void add_object_to_object_db(object_db_t *object_db, void *ptr, int units, struct_db_rec_t *struct_rec, mld_boolean_t is_root)
{
    object_db_rec_t *obj_rec = object_db_look_up(object_db, ptr);

    /* Don't add the same object twice */
    assert(!obj_rec);

    obj_rec = calloc(1, sizeof(object_db_rec_t));

    obj_rec->next = NULL;
    obj_rec->ptr = ptr;
    obj_rec->units = units;
    obj_rec->struct_rec = struct_rec;
    obj_rec->is_visited = MLD_FALSE;
    obj_rec->is_root = is_root;

    object_db_rec_t *head = object_db->head;

    if (!head)
    {
        object_db->head = obj_rec;
        obj_rec->next = NULL;
        object_db->count++;

        return;
    }

    obj_rec->next = head;
    object_db->head = obj_rec;
    object_db->count++;
}

void *xcalloc(object_db_t *object_db, char *struct_name, int units)
{
    struct_db_rec_t *struct_rec = struct_db_look_up(object_db->struct_db, struct_name);

    assert(struct_rec);

    void *ptr = calloc(units, struct_rec->ds_size);

    add_object_to_object_db(object_db, ptr, units, struct_rec, MLD_FALSE); /* xcalloc by default sets the object as non-root */

    return ptr;
}

static void delete_object_record_from_object_db(object_db_t *object
