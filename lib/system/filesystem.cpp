#include "system.h"
#include "util.h"

#include <cstdio>
#include <unistd.h>
#include <limits.h>
#include <sys/statvfs.h>
#include <sys/vfs.h>
#include <linux/magic.h>
#include <mntent.h>

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
    long magic;
    const char *type;
} FilesystemTypes[] = {
    {EXT4_SUPER_MAGIC, "ext4"},
    {TMPFS_MAGIC, "tmpfs"}
};

// throws a `SpecialError` if statfs failed
std::string get_filesystem_type() {
    // see https://stackoverflow.com/a/54298866/10005095
    struct statfs fs;
    if (statfs("/", &fs) == 0) {
        for (unsigned int i = 0; i < sizeof(FilesystemTypes) / sizeof(FilesystemTypes[0]); i++) {
            if (FilesystemTypes[i].magic == fs.f_type) {
                return FilesystemTypes[i].type;
            }
        }
        return std::string("Unknown");
    } else {
        throw system::SpecialError("Cannot determine using statfs");
    }
}

// throws a `FileReadError` if cannot open /etc/mtab
std::vector<FilesystemInfo> get_current_mounted_filesystems() {
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
}

// throws a `FileReadError` if cannot open /etc/fstab
std::vector<FilesystemInfo> get_configured_mounted_filesystems() {
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
}
}