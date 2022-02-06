## README 

# bridge between codesys and fast dds using shared memory

## steps to use this example 

### 1. Clone the Repository or download the .zip and build the project
```console
user@plc:~$ mkdir yourWorkspaceName
user@plc:~$ cd yourWorkspaceName
user@plc:~/yourWorkspaceName$ git clone https://github.com/magatela/CodesysFastDDS.git
user@plc:~/yourWorkspaceName$ cd CodesysFastDDS
user@plc:~/yourWorkspaceName/CodesysFastDDS$ mkdir build
user@plc:~/yourWorkspaceName/CodesysFastDDS$ cd build
user@plc:~/yourWorkspaceName/CodesysFastDDS/build$ cmake ..
user@plc:~/yourWorkspaceName/CodesysFastDDS/build$ make
```

### 2. create a new codesys project
Select **File** ‣ **New Project** and in the template Projects **Standard project**. Enter a name and a location in the file system. 

![Select the command File ‣ New Project and then the template Projects ‣ Standard project . Enter a name, in this project myProject, and a location in the file system.](/images/01_selectProject.png "create a new codesys standart project").

select the device where codesys is installed. As programming language select structured text

![select the device where codesys is installed.](/images/02_selectDevice.png "select the device where codesys is installed").

Right click  in **Library repository** then **add library**. In the new window click **Advanced...** 

![add libraries.](/images/03_addLibrariesA.png "add libraries").



![advanced window.](/images/04_addLibrariesB.png "advanced window").

>The necessary libraries to use shared memory are 
* SysSem, 3.5.17.0 (System)
* SysShm, 3.5.17.0 (System)
* Sys Types Interfaces, 3.5.2.0 (System)

## 3. create a new **structur** for data exchange

for data exchange it is necessary to create a new structur in codesys and c++ so that they have the same format, this structure will be stored in a shared memory block and both processes will have access to it through pointers.

In codesys right Click **Application** and choose **Add Object** then **DUT**...  
Name the new DUT **DataExcahnge** and choose the option **STRUCTURE**

![add new DUT.](/images/05_addNewDUT.png "add new DUT").

after that open the new **DataExcahnge** DUT and check that the structure in c++ is the same as in codesys

![open new DUT.](/images/06_openNewDUT.png "open new DUT").

```code
/*  struct in C++ you can find it in /yourWorkspaceName/header/dataTypeHeader/codesysToDDS.h    */
typedef struct {
    int32_t endLoopSignal;
    int32_t synchronizer;
    int32_t diValue1;
    int32_t diValue2;
    int32_t diValue3;
    int32_t diValue4;
}DataExchange;
```

The following table shows the basic IDL types supported by Fast DDS-Gen and how they are mapped to C++11 and Codesys

|IDL | C++11 | IEC 61131-3 |
|:---|:---| :--- |
| char | char | |
|octet | uint8_t |  | 
| short | int16_t | INT |
| unsigned short | uint16_t | UDINT |
| long | int32_t | DINT |
| unsigned long | uint32_t | UDINT |
| long long | int64_t | LINT |
| unsigned long long | uint64_t | ULINT |
| float | float | REAL |
| double | double | LREAL |
| long double | long double | 
| boolean | bool | USINT |
| string | std::string | STRING |

To understand how to write a Publisher Subscriber application using eProsima Fast DDS read this tutorial **Writing a simple publisher and subscriber application**
https://fast-dds.docs.eprosima.com/en/latest/fastdds/getting_started/simple_app/simple_app.html

## 4. writing the code in codesys
To access the shared memory in Codesys these four variables must be declared

```code
PROGRAM PLC_PRG 
VAR
    sharedMemoryBlock: RTS_IEC_HANDLE := SysTypes.RTS_INVALID_HANDLE;
    ResultMemoryBlock: RTS_IEC_RESULT;
    pointerToData : POINTER TO DataExchange;
    ulSize: UDINT := SIZEOF(DataExchane);
END_VAR

```
finally in your code block you have to initialize the shared memory

```code
// initialize the shared memory
IF sharedMemoryBlock = SysTypes.RTS_INVALID_HANDLE THEN
    sharedMemoryBlock := SysSharedMemoryCreate("memory_test", 0, ADR(ulSize), ADR(ResultMemoryBlock))
END_IF    

// proofing if shared memory was successfully initialized
IF sharedMemoryBlock <> SysTypes.RTS_INVALID_HANDLE THEN THEN
    pointerToData := SysSharedMemoryGetPointer(hShm := sharedMemoryBlock, 0);
END_IF

IF sharedMemoryBlock <> 0 THEN
    // Initialitation ok, hier you can write your code...
END_IF
```
now using **pointerToData** you can access (read/write) variables located in shared memory

```code
pointerToData^.endLoopSignal;
pointerToData^.synchronizer;
pointerToData^.diValue1;
pointerToData^.diValue2;
pointerToData^.diValue3;
pointerToData^.diValue4;
```
## 5. Codesys code
For this example the following codesys code will be used
![Codesys code for testing.](/images/07_codesysCode.png "Codesys code for testing").

## 6. runing the Codesys publisher and subscriber
in two differen terminals run the publisher and subscriber using sudo rights  

**Terminal 1** 
```console
user@plc:~/yourWorkspaceName/CodesysFastDDS/build$ sudo ./DDSCodesysSubscriber 
```   
**Terminal 2** 
```console
user@plc:~/yourWorkspaceName/CodesysFastDDS/build$ sudo ./DDSCodesysPublischer 
```
if there is no communication between the nodes it is because they are not using the same environment variable. In this case you can use the option **sudo su** an then **./DDSCodesysPublischer** again

In case of probleme with **#include <boost/function.hpp> not found** 
instal it: **sudo apt-get install boost**

