#include "mld.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Application Structures */
typedef struct emp_ {
    char emp_name[30];
    unsigned int emp_id;
    unsigned int age;
    struct emp_ *mgr;
    float salary;
    int *p;
} emp_t;

typedef struct student_ {
    char stud_name[32];
    unsigned int rollno;
    unsigned int age;
    float aggregate;
    struct student_ *best_colleague;
} student_t;

int main(int argc, char **argv) {
    /* Step 1: Initialize a new structure database */
    struct_db_t *struct_db = calloc(1, sizeof(struct_db_t));
    mld_init_primitive_data_types_support(struct_db);

    /* Step 2: Create structure record for structure emp_t */
    static field_info_t emp_fields[] = {
        FIELD_INFO(emp_t, emp_name, CHAR, 0),
        FIELD_INFO(emp_t, emp_id, UINT32, 0),
        FIELD_INFO(emp_t, age, UINT32, 0),
        FIELD_INFO(emp_t, mgr, OBJ_PTR, emp_t),
        FIELD_INFO(emp_t, salary, FLOAT, 0),
        FIELD_INFO(emp_t, p, OBJ_PTR, 0)
    };

    /* Step 3: Register the structure in the structure database */
    REG_STRUCT(struct_db, emp_t, emp_fields);

    /* Step 4: Create structure record for structure student_t */
    static field_info_t stud_fields[] = {
        FIELD_INFO(student_t, stud_name, CHAR, 0),
        FIELD_INFO(student_t, rollno, UINT32, 0),
        FIELD_INFO(student_t, age, UINT32, 0),
        FIELD_INFO(student_t, aggregate, FLOAT, 0),
        FIELD_INFO(student_t, best_colleague, OBJ_PTR, student_t)
    };
    REG_STRUCT(struct_db, student_t, stud_fields);

    /* Step 5: Verify the correctness of the structure database */
    print_structure_db(struct_db);

    /* Working with object database */
    /* Step 1: Initialize a new Object database */
    object_db_t *object_db = calloc(1, sizeof(object_db_t));
    object_db->struct_db = struct_db;
    
    /* Step 2: Create some sample objects */
    student_t *anirban = xcalloc(object_db, "student_t", 1);
    mld_set_dynamic_object_as_root(object_db, anirban);

    student_t *rohit = xcalloc(object_db, "student_t", 1);
    strncpy(rohit->stud_name, "rohit", strlen("rohit"));

    //anirban->best_colleague = rohit;

    emp_t *john = xcalloc(object_db, "emp_t", 1);
    mld_set_dynamic_object_as_root(object_db, john);
    john->p = xcalloc(object_db, "int", 1);

    /* Intentionally creating a leak by not freeing 'john->p' */
    john->p = NULL;

    print_object_db(object_db);

    run_mld_algorithm(object_db);

    printf("\n\nLeaked Objects: \n");
    report_leaked_objects(object_db);

    return 0;
}
