#include "../../libs/libft/libft.h"
#include "hash_table.h"

static bool is_ev_correct(const char *str) {
    size_t len;

    if (!str || !(ft_isalpha(str[0]) || str[0] == '_'))
        return false;
    len = 1;
    while (str[len] && (ft_isalpha(str[len]) || str[len] == '_'))
        len++;
    return true;
}

int ft_unset(int argc, char **argv, t_hash_table *ht) {
    bool is_err;
    int i;

    if (argc == 1)
        return 0;
    i = 0;
    is_err = false;
    while (argv[i]) {
        if (is_ev_correct(argv[i]) == true)
            ht_delete(ht, argv[i]);
        else
            is_err = true;
    }
    return 2 * is_err;
}