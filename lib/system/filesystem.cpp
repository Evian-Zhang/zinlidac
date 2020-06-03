#include "system.h"
#include "util.h"

#include <cstdio>
#ifdef __linux__
#include <unistd.h>
#include <limits.h>
#include <sys/statvfs.h>
#include <sys/vfs.h>
#include <linux/magic.h>
#include <mntent.h>
#endif

namespace libzinlidac {
// throws a `StatvfsError` if statvfs fails
unsigned long get_filesystem_capacity() {
    struct statvfs vfs;
    if (statvfs("/", &vfs) == 0) {
        return vfs.f_frsize * vfs.f_blocks;
    } else {
        throw system::StatvfsError();
    }
}

// throws a `StatvfsError` if statvfs fails
unsigned long get_filesystem_free_size() {
    struct statvfs vfs;
    if (statvfs("/", &vfs) == 0) {
        return vfs.f_bfree * vfs.f_frsize;
    } else {
        throw system::StatvfsError();
    }
}

static const struct {
    unsigned long magic;
    const char *type;
} FilesystemTypes[] = {
    {EXT4_SUPER_MAGIC, "ext4"},
    {TMPFS_MAGIC, "tmpfs"}
};

// throws a `SpecialError` if type is unknown or statfs fails
std::string get_filesystem_type() {
    #ifdef __linux__
    // see https://stackoverflow.com/a/54298866/10005095
    struct statfs fs;
    if (statfs("/", &fs) == 0) {
        for (int i = 0; i < sizeof(FilesystemTypes) / sizeof(FilesystemTypes[0]); i++) {
            if (FilesystemTypes[i].magic == fs.f_type) {
                return FilesystemTypes[i].type;
            }
            throw system::SpecialError("Canont determine filesystem type");
        }
    } else {
        throw system::SpecialError("Cannot determine using statfs");
    }
    #endif
}

// throws a `FileReadError` if cannot open /etc/mtab
std::vector<FilesystemInfo> get_current_mounted_filesystems() {
    #ifdef __linux__
    std::vector<FilesystemInfo> filesystem_infos;
    FILE *mtab_file = setmntent("/etc/mtab", "r");
    if (mtab_file == NULL) {
        throw system::FileReadError("/etc/mtab");
    }
    struct mntent *mounted;
    while ((mounted = getmntent(mtab_file)) != NULL) {
        filesystem_infos.push_back((FilesystemInfo){
            .name = mounted->mnt_fsname,
            .dir = mounted->mnt_dir,
            .mount_type = mounted->mnt_type,
            .mount_options = mounted->mnt_opts
        });
    }
    endmntent(mtab_file);
    return filesystem_infos;
    #endif
}

// throws a `FileReadError` if cannot open /etc/fstab
std::vector<FilesystemInfo> get_configured_mounted_filesystems() {
    #ifdef __linux__
    std::vector<FilesystemInfo> filesystem_infos;
    FILE *fstab_file = setmntent("/etc/fstab", "r");
    if (fstab_file == NULL) {
        throw system::FileReadError("/etc/fstab");
    }
    struct mntent *mounted;
    while ((mounted = getmntent(fstab_file)) != NULL) {
        filesystem_infos.push_back((FilesystemInfo){
            .name = mounted->mnt_fsname,
            .dir = mounted->mnt_dir,
            .mount_type = mounted->mnt_type,
            .mount_options = mounted->mnt_opts
        });
    }
    endmntent(fstab_file);
    return filesystem_infos;
    #endif
}
}