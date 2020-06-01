namespace libzinlidac {
// --------------------------- POSIX ---------------------------
// get the version number of POSIX.1 that the library and kernel support
// return -1 both if the implementation does not impose a limit, and in case of an error
unsigned long get_posix1_version();
// get the version number of POSIX.2 that the system utilities support
// return -1 both if the implementation does not impose a limit, and in case of an error
unsigned long get_posix2_version();

// --------------------------- Filesystem ---------------------------
// get max length of file path
// return -1 both if no limit for path length
long int get_path_max();
// get max length of filename
// return -1 both if no limit for filename length
long int get_filename_max();
}