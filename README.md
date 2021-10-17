# C Smol Conf
Small header only parser with all functions ready for reading simple data and storing them. <br/>
No sections whatsoever just a key (alphanumeric characters + '_') and value a (any characters returning true with [isprint()](https://en.cppreference.com/w/c/string/byte/isprint)) <br/>

CSmolConf is dual licensed <br/>
LICENSE A - Apache 2.0 License, see [LICENSE.md](/LICENSE.md) <br/>
LICENSE B - Public Domain (Unlicense), see [UNLICENSE.md](/UNLICENSE.md)

## Info
Comment ```# COMMENT ```<br/>
Syntax: ```Key=Value;```

## Example
example.cscf
```
# Example comment
example_string_key=example string value;
example_uint_key=360;
example_float_key=3.14;
```
example.c
```c
#include "c_smol_conf.h"
#include <stdio.h>
#include <stdint.h>
#include <float.h>

int main()
{
    csc_config* in_config;
    csc_read_config(&in_config, "example.cscf");
    printf("%s\n", csc_find(in_config, "example_string_key"));

    unsigned int example_uint;
    float example_float;
    csc_get_uint(in_config, "example_uint_key", &example_uint, 0, UINT_MAX);
    csc_get_float(in_config, "example_float_key", &example_float, FLT_MIN, FLT_MAX);
    printf("%u\n", example_uint);
    printf("%f\n", example_float);

    csc_get_uint(in_config, "example_uint_key", &example_uint, 0, 10);
    printf("%u\n", example_uint);

    for (size_t index = 0; index < in_config->size; index++)
    {
        printf("%s => %s\n", in_config->config[index].key->str, in_config->config[index].value->str);
    }
    csc_free_csc_config(in_config);

    csc_config* out_config = csc_init_csc_config(16);
    csc_append_kvstr_csc_config(out_config, "example_key", "some value with spaces");
    csc_append_float_csc_config(out_config, "pi", 3.14F);
    csc_write_config(out_config, "out_example.cscf");
    csc_free_csc_config(out_config);
}
```