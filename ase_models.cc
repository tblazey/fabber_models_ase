#include "ase_models.h"
#include "fwdmodel_ase.h"

#include <fabber_core/fwdmodel.h>

extern "C" {
int CALL get_num_models()
{
    return 1;
}

const char *CALL get_model_name(int index)
{
    switch (index)
    {
    case 0:
        return "ase";
        break;
    default:
        return NULL;
    }
}

NewInstanceFptr CALL get_new_instance_func(const char *name)
{
    if (string(name) == "ase")
    {
        return AseFwdModel::NewInstance;
    }
    else
    {
        return NULL;
    }
}
}
