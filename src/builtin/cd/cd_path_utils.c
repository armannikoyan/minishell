#include "../../../libs/libft/libft.h"
#include "cd_path_utils.h"
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

void set_oldpwd_and_pwd(const char *pwd, t_hash_table *ht) {
    char cwd[PATH_MAX];
    ht_update_value(ht, "OLDPWD", pwd);
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        ht_update_value(ht, "PWD", cwd);
}

int try_change_dir(const char *path, t_hash_table *ht) {
    if (!path) {
        //TODO: make normal error
        print_error("cd: no such directory\n", true);
        return 2;
    }
    if (check_path(path) != 0)
        return 2;
    //TODO: make normal error
    if (chdir(path) < 0) {
        print_error("cd: chdir", false);
        return 2;
    }
    set_oldpwd_and_pwd(path, ht);
    return 0;
}
