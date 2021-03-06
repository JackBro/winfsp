/**
 * @file dll/fuse/library.h
 *
 * @copyright 2015-2016 Bill Zissimopoulos
 */
/*
 * This file is part of WinFsp.
 *
 * You can redistribute it and/or modify it under the terms of the
 * GNU Affero General Public License version 3 as published by the
 * Free Software Foundation.
 *
 * Licensees holding a valid commercial license may use this file in
 * accordance with the commercial license agreement provided with the
 * software.
 */

#ifndef WINFSP_DLL_FUSE_LIBRARY_H_INCLUDED
#define WINFSP_DLL_FUSE_LIBRARY_H_INCLUDED

#include <dll/library.h>
#include <fuse/fuse.h>
#include <fuse/fuse_opt.h>

#define FSP_FUSE_LIBRARY_NAME           LIBRARY_NAME "-FUSE"

#define FSP_FUSE_HDR_FROM_CONTEXT(c)    \
    (struct fsp_fuse_context_header *)((PUINT8)(c) - sizeof(struct fsp_fuse_context_header))
#define FSP_FUSE_CONTEXT_FROM_HDR(h)    \
    (struct fuse_context *)((PUINT8)(h) + sizeof(struct fsp_fuse_context_header))

struct fuse
{
    struct fsp_fuse_env *env;
    int set_umask, umask;
    int set_uid, uid;
    int set_gid, gid;
    struct fuse_operations ops;
    void *data;
    UINT32 DebugLog;
    FSP_FILE_SYSTEM_OPERATION_GUARD_STRATEGY OpGuardStrategy;
    FSP_FSCTL_VOLUME_PARAMS VolumeParams;
    PWSTR MountPoint;
    FSP_FILE_SYSTEM *FileSystem;
    BOOLEAN fsinit;
    FSP_SERVICE *Service; /* weak */
};

struct fsp_fuse_context_header
{
    FSP_FSCTL_TRANSACT_REQ *Request;
    FSP_FSCTL_TRANSACT_RSP *Response;
    char *PosixPath;
    __declspec(align(MEMORY_ALLOCATION_ALIGNMENT)) UINT8 ContextBuf[];
};

struct fsp_fuse_file_desc
{
    char *PosixPath;
    BOOLEAN IsDirectory;
    int OpenFlags;
    UINT64 FileHandle;
    PVOID DirBuffer;
    ULONG DirBufferSize;
};

struct fuse_dirhandle
{
    PVOID Buffer;
    ULONG Length;
    ULONG BytesTransferred;
    BOOLEAN NonZeroOffset;
    BOOLEAN DotFiles, HasChild;
};

struct fsp_fuse_dirinfo
{
    UINT16 Size;
    FSP_FSCTL_FILE_INFO FileInfo;
    BOOLEAN FileInfoValid;
    UINT64 NextOffset;
    char PosixNameBuf[];                /* includes term-0 (unlike FSP_FSCTL_DIR_INFO) */
};

NTSTATUS fsp_fuse_op_enter(FSP_FILE_SYSTEM *FileSystem,
    FSP_FSCTL_TRANSACT_REQ *Request, FSP_FSCTL_TRANSACT_RSP *Response);
NTSTATUS fsp_fuse_op_leave(FSP_FILE_SYSTEM *FileSystem,
    FSP_FSCTL_TRANSACT_REQ *Request, FSP_FSCTL_TRANSACT_RSP *Response);

extern FSP_FILE_SYSTEM_INTERFACE fsp_fuse_intf;

#endif
