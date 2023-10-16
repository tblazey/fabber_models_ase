// fwdmodel_ase.h - A model to fit asymmetric spin echo data
#pragma once

#include "fabber_core/fwdmodel.h"

#include "armawrap/newmat.h"

#include <string>
#include <vector>

class AseFwdModel : public FwdModel {
public:
    static FwdModel* NewInstance();

    std::string ModelVersion() const;
    std::string GetDescription() const;
    void GetOptions(std::vector<OptionSpec> &opts) const;

    void Initialize(FabberRunData &args);
    void EvaluateModel(const NEWMAT::ColumnVector &params,
                       NEWMAT::ColumnVector &result,
                       const std::string &key="") const;

    void InitVoxelPosterior(MVNDist &posterior) const;

protected:
    void GetParameterDefaults(std::vector<Parameter> &params) const;

private:
    NEWMAT::ColumnVector m_tau;
    NEWMAT::ColumnVector m_te;
    double m_tau_cut;
    static FactoryRegistration<FwdModelFactory, AseFwdModel> registration;
};
