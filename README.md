# Smol Conf
Small header only parser with all functions ready for reading simple data and storing them. <br/>
There is not structure just a key (matching with ctype.h isalnum()) and value (matching with ctype.h isprint())<br/>

## Syntax
Comment ```# COMMENT ```<br/>
Syntax: ```Key=Value```

## Example
[in.scfg](/in.scfg)
```
is_working=maybe i don't know
```
[in2.scfg](/in2.scfg)
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
other_config=in.scfg
```
[test.c](/test.c)
```c
#include "smol_conf.h"
#include <stdio.h>

int main()
{
    scfg_config* config_in1;
    scfg_read_config(&config_in1, "in.scfg");
    printf("%s\n", scfg_find(config_in1, "is_working"));

    scfg_config* config_in2 = scfg_init_scfg_config(16);
    scfg_append_config(config_in2, "in2.scfg");
    bool in_bool;
    int in_int;
    unsigned int in_uint;
    float in_float;
    const char* path;
    scfg_get_bool(config_in2, "bool_true", &in_bool);
    scfg_get_int(config_in2, "integer", &in_int, scfg_NO_CLAMP_INT_MIN, scfg_NO_CLAMP_INT_MAX);
    scfg_get_uint(config_in2, "unsigned", &in_uint, scfg_NO_CLAMP_UINT_MIN, scfg_NO_CLAMP_UINT_MAX);
    scfg_get_float(config_in2, "float", &in_float, scfg_NO_CLAMP_FLT_MIN, scfg_NO_CLAMP_FLT_MAX);
    scfg_get_path(config_in2, "other_config", &path);
    printf("%i %i %u %f %s\n", in_bool, in_int, in_uint, in_float, path);

    scfg_config* config_out = scfg_init_scfg_config(16);
    scfg_config_append_kv(config_out, "Testing", "eyyy");

    scfg_append_bool_scfg_config(config_out, "outbool", 1);
    scfg_append_int_scfg_config(config_out, "outint", -888888);
    scfg_append_uint_scfg_config(config_out, "outuint", 4000000000);
    scfg_append_float_scfg_config(config_out, "outfloat", 4.44);
    scfg_concat_scfg_configs(config_out, config_in2);
    scfg_concat_scfg_configs(config_out, config_in1);
    scfg_write_config(config_out, "out.scfg");

    scfg_free_scfg_config(config_in1);
    scfg_free_scfg_config(config_in2);
    scfg_free_scfg_config(config_out);
}
```