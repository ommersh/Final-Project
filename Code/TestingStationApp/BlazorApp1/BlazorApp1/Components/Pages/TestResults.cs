using Microsoft.AspNetCore.Components;
using System.Runtime.InteropServices;
using System.Xml;


namespace BlazorApp1.Components.Pages
{

    public partial class TestResults : ComponentBase
    {
        private TestInfo? SelectedTest;
        private IntPtr labPtr;
        private string? testName;
        private int[]? uniqueIds;
        private List<TestInfo> allTests;
        private void SelectTest(TestInfo test)
        {
            SelectedTest = test;
            testName = test.name;
            StateHasChanged();

        }

        protected override async Task OnInitializedAsync()
        {
            labPtr = LabInterop.Lab_Create();
            TestIdsStruct testsIdsStruct;
            await AsyncGetAllIds(out testsIdsStruct); // await LabInterop.Lab_GetAllTestIds(labPtr);
            uniqueIds = new int[testsIdsStruct.size];

            Marshal.Copy(testsIdsStruct.ids, uniqueIds, 0, testsIdsStruct.size);

            for(int i = 0; i < uniqueIds.Length; i++) 
            {
                Console.WriteLine($"IdInArr: {uniqueIds[i]}");
                int ptr = uniqueIds[i];

            }
            allTests = new List<TestInfo>();
            foreach (int id in uniqueIds)
            {
                Console.WriteLine($"Id: {id}");


                TestInfo testInfo;
                await AsyncGetTestInfo(id, out testInfo);
                allTests.Add(testInfo);
            }
            StateHasChanged();

        }

        private Task AsyncGetAllIds(out TestIdsStruct testsIds)
        {
            testsIds = LabInterop.Lab_GetAllTestIds(labPtr);
            return Task.CompletedTask;
        }

        private Task AsyncGetTestInfo(int id, out TestInfo testInfo)
        {
            testInfo = LabInterop.Lab_GetTestInfo(labPtr, id);
            return Task.CompletedTask;
        }

        private string GetStatusIcon(string status)
        {
            return status switch
            {
                "Completed" => "bi bi-check-circle-fill text-success",
                "Failed" => "bi bi-x-circle-fill text-danger",
                "InProgress" => "bi bi-arrow-repeat text-warning",
                _ => ""
            };
        }
    }
}

