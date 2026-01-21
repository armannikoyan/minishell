#include <stdio.h>

#include "../../libs/libft/libft.h"
#include "hash_table.h"
#include "utils.h"

bool is_ev_correct(const char *str, size_t *len) {
    if (!str || !(ft_isalpha(str[0]) || str[0] == '_'))
        return false;
    (*len)++;
    while (str[*len] && (ft_isalpha(str[*len]) || str[*len] == '_'))
        (*len)++;
    return true;
}

static void fantastic_25_lines(char *key, const size_t len, t_hash_table *ht, char *argv) {
    if (argv[len] == '\0' && ht_get(ht, argv) == NULL)
        ht_create_bucket(ht, argv, NULL, false);
    else if (argv[len] == '\0')
        ht_get(ht, argv)->is_local = false;
    if (argv[len] == '=' && argv[len + 1] == '\0')
        {
        argv[len] = '\0';
        if (ht_get(ht, argv) == NULL)
            ht_create_bucket(ht, argv, "", false);
        else
            {
            ht_get(ht, argv)->is_local = false;
            ht_update_value(ht, argv, "");
        }
    }
    else if (argv[len] == '=')
        {
        ft_strlcpy(key, argv, len + 1);
        if (ht_get(ht, key) == NULL)
            ht_create_bucket(ht, key, argv + len + 1, false);
        else
            {
            ht_get(ht, key)->is_local = false;
            ht_update_value(ht, key, argv + len + 1);
        }
    }
}

static bool insert(char *key, size_t i, t_hash_table *ht, char **argv) {
    bool is_err;
    size_t len;

    is_err = false;
    while (argv[i]) {
        len = 0;
        if (is_ev_correct(argv[i], &len) == false) {
            is_err = true;
            (print_error("export: `", true), print_error(argv[i], true));
            print_error("': not a valid identifier\n", true);
        }
        fantastic_25_lines(key, len, ht, argv[i]);
        if (argv[i][len] != '\0' && argv[i][len] != '=') {
            is_err = true;
            (print_error("export: `", true), print_error(argv[i], true));
            print_error("': not a valid identifier\n", true);
        }
        ++i;
    }
    return is_err;
}

static void print_exported(t_hash_table *ht) {
    t_entry *entry;
    int i;

    i = 0;
    while (i < ht->size) {
        entry = ht->buckets[i];
        while (entry) {
            if (entry->is_local == false) {
                if (entry->val == NULL)
                    printf("declare -x %s\n", entry->key);
                else if (entry->val[0] == '\0')
                    printf("declare -x %s=""\n", entry->key);
                else
                    printf("declare -x %s=%s\n", entry->key, entry->val);
            }
            entry = entry->next;
        }
        i++;
    }
}

int ft_export(int argc, char **argv, t_hash_table *ht) {
    size_t longest;
    size_t tmp;
    size_t i;
    char *key;

    if (argc == 1)
        return (print_exported(ht), 0);
    i = 1;
    longest = 0;
    while (argv[i]) {
        tmp = 0;
        if (is_ev_correct(argv[i], &tmp) && tmp > longest)
            longest = tmp;
        i++;
    }
    key = ft_calloc(longest + 1, sizeof(char));
    //TODO: make normal error
    if (key == NULL)
        return (print_error("export: malloc", false), 2);
    if (insert(key, 1, ht, argv))
        return (free(key), 2);
    return (free(key), 0);
}
