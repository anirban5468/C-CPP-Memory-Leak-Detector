# C-CPP-Memory-Leak-Detector
This project implements a simple yet effective memory leak detector library in C++ for tracking and reporting memory leaks in applications. It features a structured approach using classes and templates to manage and monitor dynamically allocated memory.

It maintains two databases - Structure Database and Object Database.

User Application registers all its structures with MLD Library at the time of initialization.

Whenever application xcalloc() an object , object record entry is inserted in object database.

If there is any Memory Leak in Application Code, "report_leaked_objects()" function detects that Memory Leak.

### Key Features:

Struct Database: Stores information about user-defined structures and their fields.
Object Database: Tracks allocated objects, their types, sizes, and relationships.
Detection and Reporting: Identifies memory leaks by comparing allocated objects with rooted objects.
Integration: Easily integrate into C++ applications to detect and report memory leaks during runtime.

### Compilation - 

gcc -g -c mld.c -o mld.o

gcc -g -c app.c -o app.o

gcc -g app.o mld.o -o exe

For testing modify app.c file. Create some memory leaks. Do compile as mentioned above. Run the exe file.

./exe

### Usage:

Define your structures using the provided StructDBRec structure and add them to the StructDB.
Allocate memory using the ObjectDB and mark root objects as necessary.
Run the MLD algorithm to detect memory leaks.
Use reportLeakedObjects() to print information about leaked objects.

## High Level Design

![hld](https://user-images.githubusercontent.com/47227715/138119173-d18d1baf-ca91-44f9-a1fb-bf096e4aa106.png)

## Testing MLD Library

![Screenshot (88)](https://user-images.githubusercontent.com/47227715/129093013-b998f506-de19-4222-96c8-be81e13d8393.png)

![Screenshot (89)](https://user-images.githubusercontent.com/47227715/129093018-2a518fb5-12d9-46de-b935-29afc8994097.png)

![Screenshot (90)](https://user-images.githubusercontent.com/47227715/129093029-ed8917fe-3642-4e11-9df0-106c4a1d6c40.png)

![Screenshot (85)](https://user-images.githubusercontent.com/47227715/129093080-47215fdc-713e-412d-890c-7f934dff7552.png)

![Screenshot (86)](https://user-images.githubusercontent.com/47227715/129093085-067dff81-4048-4e46-81f9-991e245f791b.png)

![Screenshot (87)](https://user-images.githubusercontent.com/47227715/129093149-3dfbb894-54a9-43cd-ab89-cad9d0dd60c9.png)



### Contributions and Issues:
Contributions and suggestions for improvements are welcome! If you encounter any issues or have questions, please open an issue on GitHub.
