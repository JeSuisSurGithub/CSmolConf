# Smol Conf
Small header only parser with all functions ready for reading simple data and storing them. <br/>
There is not structure just a key (matching with ctype.h isalnum()) and value (matching with ctype.h isprint())<br/>

## Syntax
Comment ```# COMMENT ```<br/>
Syntax: ```Key=Value```

## Example
[in.scnf](/in.scnf)
```
is_working=maybe i don't know
```
[in2.scnf](/in2.scnf)
```
# some comments
    # some comments
    bool_true=true
    bool_false=false
    bool_on=on
    bool_off=off
    bool_1=1
    bool_0=0
integer=-25555
unsigned=4294967295
float=3.14
other_config=in.scnf
```
[test.c](/test.c)
```c
#include "smol_conf.h"
#include <stdio.h>

int main()
{
    scnf_config* config_in1;
    scnf_read_config(&config_in1, "in.scnf");
    printf("%s\n", scnf_find(config_in1, "is_working"));

    scnf_config* config_in2 = scnf_init_scnf_config(16);
    scnf_append_config(config_in2, "in2.scnf");
    bool in_bool;
    int in_int;
    unsigned int in_uint;
    float in_float;
    const char* path;
    scnf_get_bool(config_in2, "bool_true", &in_bool);
    scnf_get_int(config_in2, "integer", &in_int, SCNF_NO_CLAMP_INT_MIN, SCNF_NO_CLAMP_INT_MAX);
    scnf_get_uint(config_in2, "unsigned", &in_uint, SCNF_NO_CLAMP_UINT_MIN, SCNF_NO_CLAMP_UINT_MAX);
    scnf_get_float(config_in2, "float", &in_float, SCNF_NO_CLAMP_FLT_MIN, SCNF_NO_CLAMP_FLT_MAX);
    scnf_get_path(config_in2, "other_config", &path);
    printf("%i %i %u %f %s\n", in_bool, in_int, in_uint, in_float, path);

    scnf_config* config_out = scnf_init_scnf_config(16);
    scnf_config_append_kv(config_out, "Testing", "eyyy");

    scnf_append_bool_scnf_config(config_out, "outbool", 1);
    scnf_append_int_scnf_config(config_out, "outint", -888888);
    scnf_append_uint_scnf_config(config_out, "outuint", 4000000000);
    scnf_append_float_scnf_config(config_out, "outfloat", 4.44);
    scnf_concat_scnf_configs(config_out, config_in2);
    scnf_concat_scnf_configs(config_out, config_in1);
    scnf_write_config(config_out, "out.scnf");

    scnf_free_scnf_config(config_in1);
    scnf_free_scnf_config(config_in2);
    scnf_free_scnf_config(config_out);
}
```