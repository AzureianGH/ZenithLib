#ifndef __ALDEFS_H
#define __ALDEFS_H
typedef unsigned int size_t;
#define BASICHANDLE void*
#define NOTYPEADDRESS void*
#define STARTADDRESS NOTYPEADDRESS
#define ENDADDRESS NOTYPEADDRESS
#define NULL '\0'
#define true 1
#define false 0
#define uchar unsigned char
#define ushort unsigned short
#define uint unsigned int
#define ulong unsigned long
#define byte unsigned char
#define word unsigned short
#define dword unsigned int
#define qword unsigned long long
#define int8 char
#define int16 short
#define int32 int
#define int64 long long
//uint
#define uint8 unsigned char
#define uint16 unsigned short
#define uint32 unsigned int
#define uint64 unsigned long long
//define bool with min of 0 and max of 1
//save the address of the next free

typedef enum _MemStatusStct
{
    /// @brief WRITE
    MemWriteSuccess = 0, // The data is written successfully
    MemWriteFailed = 1, // Unknown error
    MemWriteLocked = 2, // The data is locked
    MemWriteInvalid = 3, // The data is invalid
    MemWriteOverFlow = 4, // The data is too large to fit in the chunk
    /// @brief READ
    MemReadSuccess = 5, // The data is read successfully
    MemReadFailed = 6, // Unknown error
    MemReadLocked = 7, // The data is locked
    MemReadInvalid = 8, // The data is invalid
    MemReadOverFlow = 9, // The data requested is too large to fit in the chunk
    /// @brief LOCK
    MemLockSuccess = 10, // The data is locked successfully
    MemLockFailed = 11, // Unknown error
    MemLockInvalid = 12, // The data is invalid
    MemLockAlreadyLocked = 13, // The data is already locked
    /// @brief UNLOCK
    MemUnlockSuccess = 14, // The data is unlocked successfully
    MemUnlockFailed = 15, // Unknown error
    MemUnlockInvalid = 16, // The data is invalid
    MemUnlockAlreadyUnlocked = 17, // The data is already unlocked
    MemUnlockWrongCode = 18, // The code is wrong
    /// @brief CHUNK
    MemChunkSuccess = 19, // The chunk is created successfully
    MemChunkFailed = 20, // Unknown error
    MemChunkInvalid = 21, // The chunk is invalid
    MemChunkAlreadyRegistered = 22, // The chunk is already registered
    MemChunkNotEnoughMemory = 24, // Not enough memory to create the chunk
    
    /// @brief Misc
    MemSuccess = 25, // The operation is successful
    MemFailed = 26, // Unknown error
    OutOfMemory = 27, // Out of memory
    InvalidParameter = 28, // Invalid parameter
    InvalidOperation = 29, // Invalid operation
    InvalidStartAddress = 30, // Invalid address (start)
    InvalidEndAddress = 31, // Invalid address (end)
    InvalidSize = 32, // Invalid size
    InvalidData = 33, // Invalid data
    InvalidCode = 34, // Invalid code
    InvalidLock = 35, // Invalid lock
    InvalidUnlock = 36, // Invalid unlock
    InvalidRead = 37, // Invalid read
    InvalidWrite = 38, // Invalid write
    /// @brief MemStatus
    MemStatusSuccess = 39, // The status is set successfully
    MemStatusFailed = 40, // Unknown error
    
} MemStatus;
static MemStatus LastStatus;
typedef struct _MemCNKStct
{
    size_t Size;
    STARTADDRESS Start_Address;
    ENDADDRESS End_Address;
} MemChunk;
typedef struct _MemLCKStct
{
    bool Locked;
    MemChunk Chunk;
} MemLock;
typedef struct _MemSAFEStct
{
    int LockCode;
    MemLock Lock;
} LockedMemoryInfo;
static MemChunk* MemChunks;
static int MemChunkCount = 0;
static MemLock* MemLocks;
static int MemLockCount = 0;
MemChunk Acquire_New_Chunk(size_t Size);
MemStatus Register_Mem_Chunk(MemChunk Chunk);
LockedMemoryInfo Lock_Mem_Chunk(MemChunk Chunk, int LockCode); // Require a code to unlock
MemStatus Unlock_Mem_Chunk(LockedMemoryInfo Chunk, int LockCode);
MemStatus Write_Mem_Chunk(MemChunk Chunk, void* Data, size_t Size);
MemStatus Read_Mem_Chunk(MemChunk Chunk, void* Data, size_t Size);
MemStatus Free_Mem_Chunk(MemChunk Chunk);
MemStatus Free_Mem_Lock(LockedMemoryInfo Lock, int LockCode);
MemStatus Get_Last_Status();
MemStatus Set_New_Status(MemStatus Status);
#endif

