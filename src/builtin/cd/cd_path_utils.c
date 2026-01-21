#include "../../../libs/libft/libft.h"
#include "cd_path_utils.h"

#include "error_codes.h"
#include "path_utils.h"
#include "utils.h"

// Checks whether the given path exists, is a directory and has execute permission.
// All filesystem-related validation for the `cd` builtin is centralized here.
// Returns 0 on success, 1 on failure and prints an appropriate error message.
int check_path(const char *path) {
    int check;

    check = check_access(path);
    if (!((check) & EXISTS_BIT)) {
        // TODO: make normal error
        print_error("cd: no such file or directory", false);
        return 1;
    }
    if (((check) & STAT_FAIL_BIT)) {
        // TODO: make normal error
        print_error("cd: stat failed", false);
        return 1;
    }
    if (!((check) & DIR_BIT)) {
        // TODO: make normal error
        print_error("cd: not a directory\n", true);
        return 1;
    }
    if (!((check) & EXEC_BIT)) {
        // TODO: make normal error
        print_error("cd: permission denied", false);
        return 1;
    }
    return 0;
}

void set_oldpwd_and_pwd(const char *oldpwd, const char *pwd, t_hash_table *ht) {
    int res;

    res = ht_update_value(ht, "OLDPWD", pwd);
    if (res == 1)
        ht_create_bucket(ht, "OLDPWD", pwd, false);
    res = ht_update_value(ht, "PWD", oldpwd);
    if (res == 1)
        ht_create_bucket(ht, "PWD", oldpwd, false);
}

int try_change_dir(const char *path, t_hash_table *ht, const char *cwd) {
    if (!path) {
        //TODO: make normal error
        print_error("cd: no such directory\n", true);
        return BUILTIN_ERROR;
    }
    if (check_path(path) != 0)
        return BUILTIN_ERROR;
    //TODO: make normal error
    if (chdir(path) < 0) {
        print_error("cd: chdir", false);
        return BUILTIN_ERROR;
    }
    set_oldpwd_and_pwd(cwd, path, ht);
    return 0;
}
