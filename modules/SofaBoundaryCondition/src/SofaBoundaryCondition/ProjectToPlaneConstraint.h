/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2006 INRIA, USTL, UJF, CNRS, MGH                     *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#pragma once
#include <SofaBoundaryCondition/config.h>

#include <sofa/core/behavior/ProjectiveConstraintSet.h>
#include <sofa/core/behavior/MechanicalState.h>
#include <sofa/core/topology/BaseMeshTopology.h>
#include <sofa/core/objectmodel/Event.h>
#include <sofa/defaulttype/BaseMatrix.h>
#include <sofa/defaulttype/BaseVector.h>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/helper/vector.h>
#include <sofa/defaulttype/Mat.h>
#include <SofaBaseTopology/TopologySubsetData.h>
#include <SofaEigen2Solver/EigenSparseMatrix.h>
#include <set>

namespace sofa::component::projectiveconstraintset
{

/// This class can be overridden if needed for additionnal storage within template specializations.
template <class DataTypes>
class ProjectToPlaneConstraintInternalData
{

};

/** Project particles to an affine plane.
  @author Francois Faure, 2012
  @todo Optimized versions for planes parallel to the main directions
*/
template <class DataTypes>
class ProjectToPlaneConstraint : public core::behavior::ProjectiveConstraintSet<DataTypes>
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(ProjectToPlaneConstraint,DataTypes),SOFA_TEMPLATE(sofa::core::behavior::ProjectiveConstraintSet, DataTypes));

    using Index = sofa::Index;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::MatrixDeriv MatrixDeriv;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename DataTypes::CPos CPos;
    typedef typename MatrixDeriv::RowIterator MatrixDerivRowIterator;
    typedef typename MatrixDeriv::RowType MatrixDerivRowType;
    typedef Data<VecCoord> DataVecCoord;
    typedef Data<VecDeriv> DataVecDeriv;
    typedef Data<MatrixDeriv> DataMatrixDeriv;
    typedef helper::vector<Index> Indices;
    typedef sofa::component::topology::PointSubsetData< Indices > IndexSubsetData;
    typedef linearsolver::EigenBaseSparseMatrix<SReal> BaseSparseMatrix;
    typedef linearsolver::EigenSparseMatrix<DataTypes,DataTypes> SparseMatrix;
    typedef typename SparseMatrix::Block Block;                                       ///< projection matrix of a particle displacement to the plane
    enum {bsize=SparseMatrix::Nin};                                                   ///< size of a block
    typedef sofa::defaulttype::Vector3 Vector3;

protected:
    ProjectToPlaneConstraint();

    virtual ~ProjectToPlaneConstraint();

public:
    IndexSubsetData f_indices;  ///< the particles to project
    Data<CPos> f_origin;       ///< A point in the plane
    Data<CPos> f_normal;       ///< The normal to the plane. Will be normalized by init().
    Data<SReal> f_drawSize;    ///< The size of the display of the constrained particles

    /// Link to be set to the topology container in the component graph.
    SingleLink<ProjectToPlaneConstraint<DataTypes>, sofa::core::topology::BaseMeshTopology, BaseLink::FLAG_STOREPATH | BaseLink::FLAG_STRONGLINK> l_topology;

protected:
    ProjectToPlaneConstraintInternalData<DataTypes>* data;
    friend class ProjectToPlaneConstraintInternalData<DataTypes>;


public:
    void clearConstraints();
    void addConstraint(Index index);
    void removeConstraint(Index index);

    // -- Constraint interface
    void init() override;
    void reinit() override;

    void projectResponse(const core::MechanicalParams* mparams, DataVecDeriv& resData) override;
    void projectVelocity(const core::MechanicalParams* mparams, DataVecDeriv& vData) override;
    void projectPosition(const core::MechanicalParams* mparams, DataVecCoord& xData) override;
    void projectJacobianMatrix(const core::MechanicalParams* mparams, DataMatrixDeriv& cData) override;

    using core::behavior::ProjectiveConstraintSet<DataTypes>::applyConstraint;
    void applyConstraint(defaulttype::BaseMatrix *mat, unsigned int offset);
    void applyConstraint(defaulttype::BaseVector *vect, unsigned int offset);

    /** Project the the given matrix (Experimental API).
      Replace M with PMP, where P is the projection matrix corresponding to the projectResponse method, shifted by the given offset, i.e. P is the identity matrix with a block on the diagonal replaced by the projection matrix.
      */
    void projectMatrix( sofa::defaulttype::BaseMatrix* /*M*/, unsigned /*offset*/ ) override;


    void draw(const core::visual::VisualParams* vparams) override;


    class FCPointHandler : public component::topology::TopologyDataHandler<core::topology::BaseMeshTopology::Point, Indices >
    {
    public:
        typedef typename ProjectToPlaneConstraint<DataTypes>::Indices Indices;
        typedef typename sofa::core::topology::Point Point;
        FCPointHandler(ProjectToPlaneConstraint<DataTypes>* _fc, component::topology::PointSubsetData<Indices>* _data)
            : sofa::component::topology::TopologyDataHandler<core::topology::BaseMeshTopology::Point, Indices >(_data), fc(_fc) {}


        using component::topology::TopologyDataHandler<core::topology::BaseMeshTopology::Point, Indices >::applyDestroyFunction;
        void applyDestroyFunction(Index /*index*/, core::objectmodel::Data<value_type>& /*T*/);


        bool applyTestCreateFunction(Index /*index*/,
                const sofa::helper::vector< Index > & /*ancestors*/,
                const sofa::helper::vector< double > & /*coefs*/);
    protected:
        ProjectToPlaneConstraint<DataTypes> *fc;
    };

protected :
    /// Handler for subset Data
    FCPointHandler* m_pointHandler;

    SparseMatrix jacobian; ///< projection matrix in local state
    SparseMatrix J;        ///< auxiliary variable
};


#if  !defined(SOFA_COMPONENT_PROJECTIVECONSTRAINTSET_ProjectToPlaneConstraint_CPP)
extern template class SOFA_SOFABOUNDARYCONDITION_API ProjectToPlaneConstraint<defaulttype::Vec3Types>;
extern template class SOFA_SOFABOUNDARYCONDITION_API ProjectToPlaneConstraint<defaulttype::Vec2Types>;

#endif

} // namespace sofa::component::projectiveconstraintset

