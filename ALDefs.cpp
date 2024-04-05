#include "ALDefs.h"

MemStatus Get_Last_Status()
{
    return LastStatus;
}
MemStatus Set_New_Status(MemStatus Status)
{
    try
    {
        LastStatus = Status;
        return LastStatus;
    }
    catch (...)
    {
        return MemStatus::MemStatusFailed;
    }
    
    
}
MemChunk Acquire_New_Chunk(size_t Size)
{
    //search through each mem chunk to find a free space, check between end and start address of different chunks to 
    // try and find a free space in between, if can't find a free space, return a chunk after the last chunk in the list

    // MemChunks is the pointer

    try
    {
    MemChunk NewChunk;
    NewChunk.Size = Size;
    for (int i = 0; i < MemChunkCount; i++)
    {
        if (MemChunks[i].End_Address < MemChunks[i + 1].Start_Address)
        {
            NewChunk.Start_Address = MemChunks[i].End_Address;
            NewChunk.End_Address = MemChunks[i].End_Address + Size;
            Set_New_Status(MemStatus::MemChunkSuccess);
            return NewChunk;
        }
    }
    NewChunk.Start_Address = MemChunks[MemChunkCount - 1].End_Address;
    NewChunk.End_Address = MemChunks[MemChunkCount - 1].End_Address + Size;
    Set_New_Status(MemStatus::MemChunkSuccess);
    return NewChunk;
    }
    catch (...)
    {
        Set_New_Status(MemStatus::MemChunkFailed);
        return MemChunk();
    }
}
MemStatus Register_Mem_Chunk(MemChunk Chunk)
{
    try
    {
        for (int i = 0; i < MemChunkCount; i++)
        {
            if (MemChunks[i].Start_Address == Chunk.Start_Address && MemChunks[i].End_Address == Chunk.End_Address)
            {
                Set_New_Status(MemStatus::MemChunkAlreadyRegistered);
                return MemStatus::MemChunkAlreadyRegistered;
            }
        }
        MemChunks[MemChunkCount] = Chunk;
        MemChunkCount++;
        Set_New_Status(MemStatus::MemChunkSuccess);
        return MemStatus::MemChunkSuccess;
    }
    catch (...)
    {
        Set_New_Status(MemStatus::MemChunkFailed);
        return MemStatus::MemChunkFailed;
    }
    
}
LockedMemoryInfo Lock_Mem_Chunk(MemChunk Chunk, int LockCode)
{
    try
    {
        for (int i = 0; i < MemChunkCount; i++)
        {
            if (MemChunks[i].Start_Address == Chunk.Start_Address && MemChunks[i].End_Address == Chunk.End_Address)
            {
                LockedMemoryInfo LockedMemory;
                LockedMemory.LockCode = LockCode;
                LockedMemory.Lock.Locked = true;
                LockedMemory.Lock.Chunk = Chunk;
                Set_New_Status(MemStatus::MemSuccess);
                return LockedMemory;
            }
        }
        Set_New_Status(MemStatus::MemChunkInvalid);
        return LockedMemoryInfo();
    }
    catch (...)
    {
        Set_New_Status(MemStatus::MemChunkFailed);
        return LockedMemoryInfo();
    }
    
}
MemStatus Unlock_Mem_Chunk(LockedMemoryInfo Chunk, int LockCode)
{
    try
    {
        if (Chunk.Lock.Locked == false)
        {
            Set_New_Status(MemStatus::MemUnlockAlreadyUnlocked);
            return MemStatus::MemUnlockAlreadyUnlocked;
        }
        if (Chunk.Lock.Chunk.Start_Address == Chunk.Lock.Chunk.Start_Address && Chunk.Lock.Chunk.End_Address == Chunk.Lock.Chunk.End_Address && Chunk.LockCode == LockCode)
        {
            Chunk.Lock.Locked = false;
            Set_New_Status(MemStatus::MemSuccess);
            return MemStatus::MemSuccess;
        }
        Set_New_Status(MemStatus::MemUnlockWrongCode);
        return MemStatus::MemUnlockWrongCode;
    }
    catch (...)
    {
        Set_New_Status(MemStatus::MemChunkFailed);
        return MemStatus::MemChunkFailed;
    }
    
}
MemStatus Write_Mem_Chunk(MemChunk Chunk, void* Data, size_t Size)
{
    //check if the chunk is locked if not write, if locked return error
    try
    {
        for (int i = 0; i < MemChunkCount; i++)
        {
            if (MemChunks[i].Start_Address == Chunk.Start_Address && MemChunks[i].End_Address == Chunk.End_Address)
            {
                if (MemChunks[i].Locked == true)
                {
                    Set_New_Status(MemStatus::MemWriteLocked);
                    return MemStatus::MemWriteLocked;
                }
                if (Size > MemChunks[i].Size)
                {
                    Set_New_Status(MemStatus::MemWriteOverFlow);
                    return MemStatus::MemWriteOverFlow;
                }
                MemChunks[i].Data = Data;
                Set_New_Status(MemStatus::MemSuccess);
                return MemStatus::MemSuccess;
            }
        }
        Set_New_Status(MemStatus::MemChunkInvalid);
        return MemStatus::MemChunkInvalid;
    }
    catch (...)
    {
        Set_New_Status(MemStatus::MemChunkFailed);
        return MemStatus::MemChunkFailed;
    }
}
MemStatus Read_Mem_Chunk(MemChunk Chunk, void* Data, size_t Size)
{
    //check if the chunk is locked if not read, if locked return error
    try
    {
        for (int i = 0; i < MemChunkCount; i++)
        {
            if (MemChunks[i].Start_Address == Chunk.Start_Address && MemChunks[i].End_Address == Chunk.End_Address)
            {
                if (MemChunks[i].Locked == true)
                {
                    Set_New_Status(MemStatus::MemReadLocked);
                    return MemStatus::MemReadLocked;
                }
                if (Size > MemChunks[i].Size)
                {
                    Set_New_Status(MemStatus::MemReadOverFlow);
                    return MemStatus::MemReadOverFlow;
                }
                Data = MemChunks[i].Data;
                Set_New_Status(MemStatus::MemSuccess);
                return MemStatus::MemSuccess;
            }
        }
        Set_New_Status(MemStatus::MemChunkInvalid);
        return MemStatus::MemChunkInvalid;
    }
    catch (...)
    {
        Set_New_Status(MemStatus::MemChunkFailed);
        return MemStatus::MemChunkFailed;
    }
}
MemStatus Free_Mem_Chunk(MemChunk Chunk)
{
    try
    {
        for (int i = 0; i < MemChunkCount; i++)
        {
            if (MemChunks[i].Start_Address == Chunk.Start_Address && MemChunks[i].End_Address == Chunk.End_Address)
            {
                MemChunks[i].Start_Address = 0;
                MemChunks[i].End_Address = 0;
                MemChunks[i].Size = 0;
                MemChunks[i].Data = nullptr;
                Set_New_Status(MemStatus::MemSuccess);
                return MemStatus::MemSuccess;
            }
        }
        Set_New_Status(MemStatus::MemChunkInvalid);
        return MemStatus::MemChunkInvalid;
    }
    catch (...)
    {
        Set_New_Status(MemStatus::MemChunkFailed);
        return MemStatus::MemChunkFailed;
    }
}
MemStatus Free_Mem_Lock(LockedMemoryInfo Lock, int LockCode)
{
    try
    {
        for (int i = 0; i < MemChunkCount; i++)
        {
            if (MemChunks[i].Start_Address == Lock.Lock.Chunk.Start_Address && MemChunks[i].End_Address == Lock.Lock.Chunk.End_Address)
            {
                if (Lock.Lock.Locked == false)
                {
                    Set_New_Status(MemStatus::MemUnlockAlreadyUnlocked);
                    return MemStatus::MemUnlockAlreadyUnlocked;
                }
                if (LockCode == Lock.LockCode)
                {
                    Lock.Lock.Locked = false;
                    Set_New_Status(MemStatus::MemSuccess);
                    return MemStatus::MemSuccess;
                }
                Set_New_Status(MemStatus::MemUnlockWrongCode);
                return MemStatus::MemUnlockWrongCode;
            }
        }
        Set_New_Status(MemStatus::MemChunkInvalid);
        return MemStatus::MemChunkInvalid;
    }
    catch (...)
    {
        Set_New_Status(MemStatus::MemChunkFailed);
        return MemStatus::MemChunkFailed;
    }
}