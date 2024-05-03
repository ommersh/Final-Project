using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct UserTestData
{
    public int testID;
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 80)]
    public string testName;
    public int catchPolynomialDegree;
    public int numberOfPointsPerSegment;
    public CatchRootsAlg catchRootsAlg;
    public Algorithm testedAlgorithm;
    public uint numberOfIterations;
    public int TminFactor;
    public double timeIntervalSizeSec;
    public double TOLdKM;
    public double TOLtSec;
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 262)]
    public string orbitingElementData1;
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 262)]
    public string orbitingElementData2;
    public double julianDate;
    public int initialNumberOfPoints;
    public double segmentSizeSec;
    public SatelliteDataFormat format;
    public TestStatus status;
    public double timeOfTcaFromStartingPointSec;
    public double distanceOfTcaKM;
    public int numberOfPointsTheAlgUsed;
    public double runTimeMicro;
    public double avgRunTimeMicro;
    public double minRunTimeMicro;
    public double realTCASec;
    public double realTCAdistanceKM;
    public double TCAErrorSec;
    public double TCAErrorKm;

}


public enum TestStatus
{
    Failed,
    InProgress,
    Completed,
    ErrorInDataGeneration
}

// Define enum equivalent to Algorithm in C++ Algorithm AlgorithmsEnums
public enum Algorithm
{
    ANCAS,
    CATCH,
    SBO_ANCAS
}

// Define enum equivalent to SatelliteDataFormat in C++
public enum SatelliteDataFormat
{
    XML,
    Text
}


[StructLayout(LayoutKind.Sequential)]
public struct TestIdsStruct
{
    public IntPtr ids; //Pointer to array of test Ids
    public int size; //Size of ids array
};


public enum CatchRootsAlg
{
    EigenCompanionMatrix,
    ArmadilloCompanionMatrix
}

public class TestDataWrapper
{
    public required string testName;
    public int catchPolynomialDegree;              // Degree of the polynomial, should be numberOfPointsPerSegment - 1
    public int numberOfPointsPerSegment;           // Use for data generations
    public CatchRootsAlg catchRootsAlg;            // what variation to use for catch roots finding
    public Algorithm testedAlgorithm;
    public int numberOfIterations;
    public int TminFactor;                         // 2/4/8 what we divide the smaller iteration by to get Gamma
    public double timeIntervalSizeSec;
    public required string TOLdKM;                          // tolerance of distance for SBO ANCAS in KM
    public required string TOLtSec;                         // tolerance of time for SBO ANCAS in SEC
    public required string orbitingElementData1;
    public required string orbitingElementData2;
    public SatelliteDataFormat format;

}