using Microsoft.AspNetCore.Components;
using Microsoft.AspNetCore.Components.Forms;
using System.IO;


namespace BlazorApp1.Components.Pages
{

    public partial class CreateTestPage : ComponentBase
    {

        public IBrowserFile elemenetFile1;
        public IBrowserFile elemenetFile2;

        TestDataWrapper userTestDataWrapper = new TestDataWrapper()
        {
            testName = "",
            orbitingElementData1 = "",
            orbitingElementData2 = "",
        };

        private IntPtr labPtr;

        protected override void OnInitialized()
        {
            labPtr = LabInterop.Lab_Create();
        }

        void HandleValidSubmit()
        {;
            UserTestData testData = CreateStructFromWrapper(userTestDataWrapper);
            LabInterop.Lab_CreateTest(labPtr, testData);
        }

        void HandleFileSelection(InputFileChangeEventArgs e, int orbitingElementIndex)
        {
            var file = e.File;
            if (file != null)
            {
                string? rootDirectory = AppDomain.CurrentDomain.BaseDirectory;
                string? savePath = Path.Combine("C:/OmmXMLs", file.Name);
                //string savePath = Path.Combine(rootDirectory, $"OmmXml/{file.Name}");


                if (orbitingElementIndex == 1)
                {
                    elemenetFile1 = file;
                    SaveFile(elemenetFile1, savePath);
                    userTestDataWrapper.orbitingElementData1 = savePath;
                }
                else if (orbitingElementIndex == 2)
                {
                    elemenetFile2 = file;
                    SaveFile(elemenetFile2, savePath);
                    userTestDataWrapper.orbitingElementData2 = savePath;


                }
            }
        }


        private UserTestData CreateStructFromWrapper(TestDataWrapper userTestDataWrapper)
        {
            UserTestData userTestData = new UserTestData();
            userTestData.testName = userTestDataWrapper.testName;
            userTestData.numberOfPointsPerSegment = userTestDataWrapper.numberOfPointsPerSegment;
            userTestData.catchPolynomialDegree = userTestDataWrapper.numberOfPointsPerSegment -1;
            userTestData.catchRootsAlg = userTestDataWrapper.catchRootsAlg;
            userTestData.testedAlgorithm = userTestDataWrapper.testedAlgorithm;
            userTestData.numberOfIterations = (uint)userTestDataWrapper.numberOfIterations;
            userTestData.TminFactor = userTestDataWrapper.TminFactor;
            userTestData.timeIntervalSizeSec = userTestDataWrapper.timeIntervalSizeSec;
            userTestData.TOLdKM = userTestDataWrapper.TOLdKM;
            userTestData.TOLtSec = userTestDataWrapper.TOLtSec;
            userTestData.format = userTestDataWrapper.format;

            userTestData.orbitingElementData1 = userTestDataWrapper.orbitingElementData1;
            userTestData.orbitingElementData2 = userTestDataWrapper.orbitingElementData2;



            return userTestData;

        }

        public async Task<string> SaveFile(IBrowserFile file, string savePath)
        {
            try
            {
                if (file != null && file.Size > 0)
                {

                    //Console.WriteLine($"Save path: {savePath}");

                    if (File.Exists(savePath))
                    {
                        File.Delete(savePath);
                    }

                    Directory.CreateDirectory(Path.GetDirectoryName(savePath));

                    using (var fileStream = new FileStream(savePath, FileMode.Create, FileAccess.Write, FileShare.None))
                    {
                        await file.OpenReadStream().CopyToAsync(fileStream);
                        await fileStream.FlushAsync();
                    }
                }
                else
                {
                    throw new ArgumentException("File is null or empty");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error saving file: {ex.Message}");
                // Handle or log the exception as needed
            }

            return savePath;
        }
    }
}

    

