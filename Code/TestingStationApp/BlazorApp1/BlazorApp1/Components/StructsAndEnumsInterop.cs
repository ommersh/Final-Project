﻿using System.Runtime.InteropServices;


[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct TestInfo
{
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 40)]
    public string name;
    public TestStatus status;
    public CommonTestRecipe recipe;
    public double realTCA;
    public double realDistance;
    public double distance;
    public double TCA;
    public double runTime;
    public SatelliteDataFormat format;
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 80)]
    public string firstElemData;
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 80)]
    public string secondElemData;
}

public enum TestStatus
{
    NotStarted,
    InProgress,
    Completed,
    Failed
}

// Define enum equivalent to Algorithm in C++
public enum Algorithm
{
    Ancas,
    Catch,
    SBO_Ancas
}

// Define enum equivalent to SatelliteDataFormat in C++
public enum SatelliteDataFormat
{
    XML,
    Text
}


[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct elsetrec
{
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 6)]
    public string SatNum;
    public int EpochYear;
    public int EpochTynumrev;
    public int Error;
    public char OperationMode;
    public char Init;
    public char Method;

    // Near Earth properties
    public int Isimp;
    public double Aycof, Con41, Cc1, Cc4, Cc5, D2, D3, D4,
        Delmo, Eta, Argpdot, Omgcof, Sinmao, T, T2cof, T3cof,
        T4cof, T5cof, X1mth2, X7thm1, Mdot, Nodedot, Xlcof, Xmcof,
        Nodecf;

    // Deep Space properties
    public int Irez;
    public double D2201, D2211, D3210, D3222, D4410, D4422, D5220, D5232,
        D5421, D5433, Dedt, Del1, Del2, Del3, Didt, Dmdt,
        Dnodt, Domdt, E3, Ee2, Peo, Pgho, Pho, Pinco,
        Plo, Se2, Se3, Sgh2, Sgh3, Sgh4, Sh2, Sh3,
        Si2, Si3, Sl2, Sl3, Sl4, Gsto, Xfact, Xgh2,
        Xgh3, Xgh4, Xh2, Xh3, Xi2, Xi3, Xl2, Xl3,
        Xl4, Xlamo, Zmol, Zmos, Atime, Xli, Xni;

    public double A, Altp, Alta, Epochdays, Jdsatepoch, JdsatepochF, Nddot, Ndot,
        Bstar, Rcse, Inclo, Nodeo, Ecco, Argpo, Mo, NoKozai;

    // Additional elements
    public char Classification;
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 11)]
    public string Intldesg;
    public int Ephtype;
    public long Elnum, Revnum;
    public double No_unkozai;
    public double Am, Em, Im, Om, Omegam, Mm, Nm;
    public double Tumin, Mus, Radiusearthkm, Xke, J2, J3, J4, J3oj2;
    public long Dia_mm;
    public double Period_sec;
    public byte Active;
    public byte Not_orbital;
    public double Rcs_m2;
}

[StructLayout(LayoutKind.Sequential)]
public struct CommonTestRecipe
{
    public int testId;
    public double timeInterval;
    public int iterations;
    public Algorithm alg;
    public int catchPolynomDeg;
    public elsetrec firstElemObj;
    public elsetrec secondElemObj;
    public int numOfTimePoints;
}


[StructLayout(LayoutKind.Sequential)]
public struct TestIdsStruct
{
    public IntPtr ids; //Pointer to array of test Ids
    public int size; //Size of ids array
};