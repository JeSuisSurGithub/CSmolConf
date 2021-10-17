#include "c_smol_conf.h"
#include <stdio.h>

int main()
{
    csc_config* configuration0;
    csc_read_config(&configuration0, "test1.cscf");
    csc_config* configuration1 = csc_init_csc_config(16);

    csc_append_kvstr_csc_config(configuration1, "TEST0", "firsttest");
    csc_append_float_csc_config(configuration1, "use", 3.4F);
    csc_append_float_csc_config(configuration1, "use", 3.4F);

    csc_append_config(configuration1, "test.cscf");
    printf("%s\n", csc_find(configuration1, "TEST0"));
    printf("%i\n", csc_is_def(configuration1, "TEST0"));

    csc_concat_csc_configs(configuration0, configuration1);
    csc_write_config(configuration0, "out.cscf");

    for (size_t index = 0; index < configuration0->size; index++)
    {
        printf("%s => %s\n", configuration0->config[index].key->str, configuration0->config[index].value->str);
    }
    csc_free_csc_config(configuration0);
    csc_free_csc_config(configuration1);
}