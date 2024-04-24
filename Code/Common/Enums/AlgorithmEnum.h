#ifndef ALGORITHEM_ENUMS_H
#define ALGORITHEM_ENUMS_H
#pragma pack(push, 1)

namespace AlgorithmsEnums
{
    /// <summary>
    /// Which algorithm to test
    /// </summary>
    enum Algorithm {
        CATCH,
        ANCAS,
        SBO_ANCAS
    };

    /// <summary>
    /// Whar root finding algorithm to use for CATCH
    /// </summary>
    enum CatchRootsAlg {
        EigenCompanionMatrix,
        ArmadilloCompanionMatrix
    };
}

#pragma pack(pop)
#endif