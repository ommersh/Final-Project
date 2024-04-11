﻿using System.Runtime.InteropServices;

public class LabInterop
{
    [DllImport("TestingStationApp.dll")]
    public static extern IntPtr Lab_Create();

    [DllImport("TestingStationApp.dll")]
    public static extern TestInfo Lab_GetTestInfo(IntPtr lab, int testId);

    //[DllImport("TestingStationApp.dll")]
    //public static extern void Lab_DeleteTest(IntPtr lab, int testId);

    [DllImport("TestingStationApp.dll")]
    public static extern int Lab_CreateTest(IntPtr lab, [MarshalAs(UnmanagedType.LPStr)] string name, double timeInterval, int iterations, Algorithm alg, int catchPolynomDeg, int numOfTimePoints, [MarshalAs(UnmanagedType.LPStr)] string elemDataOne, [MarshalAs(UnmanagedType.LPStr)] string elemDataTwo, SatelliteDataFormat format);

    [DllImport("TestingStationApp.dll")]
    public static extern TestIdsStruct Lab_GetAllTestIds(IntPtr lab);
}