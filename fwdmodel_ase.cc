//  fwdmodel_ase.cc - Implements a fitting model for ASE data
#include "fwdmodel_ase.h"

#include <fabber_core/fwdmodel.h>
#include <fabber_core/tools.h>
#include <fabber_core/priors.h>

#include <math.h>

using namespace std;
using namespace NEWMAT;

FactoryRegistration<FwdModelFactory, AseFwdModel> AseFwdModel::registration("ase");

FwdModel *AseFwdModel::NewInstance()
{
    return new AseFwdModel();
}

string AseFwdModel::ModelVersion() const
{
    return "1.0";
}

string AseFwdModel::GetDescription() const
{
    return "Fits ASE data to get an estimate of OEF";
}

static OptionSpec OPTIONS[] = {
    { "tau", OPT_MATRIX, "Time from ideal spin echo time (ms)", OPT_REQ, "" },
    { "te", OPT_MATRIX, "Echo times (ms)", OPT_REQ, "" },
    { "tau_cut", OPT_FLOAT, "Cutoff time for long and short period of model (ms)", OPT_NONREQ, "15.0" },
    { "" }
};

void AseFwdModel::GetOptions(vector<OptionSpec> &opts) const
{
    for (int i = 0; OPTIONS[i].name != ""; i++)
    {
        opts.push_back(OPTIONS[i]);
    }
}

void AseFwdModel::Initialize(FabberRunData& rundata)
{
    // tau
    string tau_file = rundata.ReadWithDefault("tau", "");
    if (tau_file != "")
    {
        m_tau = fabber::read_matrix_file(tau_file);
    }
    
    // te
    string te_file = rundata.ReadWithDefault("te", "");
    if (te_file != "")
    {
        m_te = fabber::read_matrix_file(te_file);
    }
    
    m_tau_cut = rundata.GetIntDefault("tau_cut", 15);
     
}

void AseFwdModel::GetParameterDefaults(std::vector<Parameter> &params) const
{
    params.clear();

    int p=0;
    params.push_back(Parameter(p++, "s0", DistParams(500, 1E4), DistParams(500, 1E4), PRIOR_NORMAL, TRANSFORM_SOFTPLUS()));
    params.push_back(Parameter(p++, "lambda", DistParams(0.1, 0.3), DistParams(0.1, 0.3), PRIOR_NORMAL, TRANSFORM_SOFTPLUS()));
    params.push_back(Parameter(p++, "omega", DistParams(0.1, .04), DistParams(0.1, 0.04), PRIOR_NORMAL, TRANSFORM_SOFTPLUS()));
    params.push_back(Parameter(p++, "t2", DistParams(100, 1E3), DistParams(100, 1E3), PRIOR_NORMAL, TRANSFORM_SOFTPLUS()));
}

void AseFwdModel::EvaluateModel(const NEWMAT::ColumnVector &params, 
                                NEWMAT::ColumnVector &result, 
                                const std::string &key) const
{
    result.ReSize(data.Nrows());
    result = 0;
    double s0 = params(1);
    double lambda = params(2);
    double omega = params(3);
    double t2 = params(4);
    
    for (int i=0; i < data.Nrows(); i++)
    {
        double tau = m_tau(i + 1);
        double te = m_te(i + 1);
        if (tau <= m_tau_cut){
            result(i + 1) = s0 * exp(-0.3 * lambda * pow(omega * tau, 2) - te / t2);
        } else {
            result(i + 1) = s0 * exp(-lambda * omega * tau + lambda - te / t2);
        }
    }
}

void AseFwdModel::InitVoxelPosterior(MVNDist &posterior) const
{
    double data_max = data.Maximum();
    posterior.means(1) = data_max;
}
