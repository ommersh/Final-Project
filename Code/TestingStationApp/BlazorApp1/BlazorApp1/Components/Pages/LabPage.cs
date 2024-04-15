using Microsoft.AspNetCore.Components;
using static System.Net.Mime.MediaTypeNames;


namespace BlazorApp1.Components.Pages
{

    public partial class LabPage : ComponentBase
    {
        private int? createdTestId;
        private UserTestData? testInfo;
        private string? testName;

        private IntPtr labPtr;

        protected override void OnInitialized()
        {
            labPtr = LabInterop.Lab_Create();
        }

        //private void DeleteTest(int testId)
        //{
        //    LabInterop.Lab_DeleteTest(labPtr, testId);
        //}

        private void GetTestInfo()
        {
            int? testId = 1000;
            if(createdTestId.HasValue)
            {
                testId = createdTestId;
            }
            testInfo = LabInterop.Lab_GetTestInfo(labPtr, testId.Value);
            testName=testInfo.Value.testName;
            StateHasChanged();

        }

        // private void CreateTest(string name, double timeInterval, int iterations, Algorithm alg, int catchPolynomDeg, int numOfTimePoints, string elemDataOne, string elemDataTwo, SatelliteDataFormat format)
        // {
        //     LabInterop.Lab_CreateTest(labPtr, name, timeInterval, iterations, alg, catchPolynomDeg, numOfTimePoints, elemDataOne, elemDataTwo, format);
        //     StateHasChanged();
        // }

        private void CreateTest()
        {
            UserTestData testData = new UserTestData();
            testData.testName = "test";
            testData.timeIntervalSizeSec = 1.0;
            testData.numberOfIterations = 10;
            testData.testedAlgorithm = Algorithm.Catch;
            testData.numberOfPointsPerSegment = 16;
            testData.orbitingElementData1 = "G:\\לימודים\\3. תיקיות קורסים\\שנה ד\\פרויקט גמר\\repo2\\Final-Project\\SGP4\\COSMOS.xml";
            testData.orbitingElementData1 = "G:\\לימודים\\3. תיקיות קורסים\\שנה ד\\פרויקט גמר\\repo2\\Final-Project\\SGP4\\LEMUR2.xml";
            testData.format = SatelliteDataFormat.XML;
            //createdTestId = LabInterop.Lab_CreateTest(labPtr, test, 1.0, 10, Algorithm.Ancas, 16, 100, "ElemDataOne", "ElemDataTwo", SatelliteDataFormat.Text);
            createdTestId = LabInterop.Lab_CreateTest(labPtr, testData);

            //createdTestId = LabInterop.Lab_CreateTest(labPtr, "test");
            StateHasChanged();

        }
    }


}

