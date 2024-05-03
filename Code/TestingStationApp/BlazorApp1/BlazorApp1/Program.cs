using BlazorApp1.Components;
using System.Diagnostics;
using static System.Net.Mime.MediaTypeNames;

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
builder.Services.AddRazorComponents()
    .AddInteractiveServerComponents();

var app = builder.Build();

// Configure the HTTP request pipeline.
if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/Error", createScopeForErrors: true);
    // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
    app.UseHsts();
}

app.UseHttpsRedirection();

app.UseStaticFiles();
app.UseAntiforgery();

app.MapRazorComponents<App>()
    .AddInteractiveServerRenderMode();

// Adding a minimal API endpoint for checking connection
app.MapGet("/api/checkconnection", () => {
    return LabInterop.Lab_CheckConnection();
});

// Open the browser to the application's URL after the app starts
var url = "http://localhost:5000"; // Modify the URL as needed
try
{
    if (app.Environment.IsProduction())
    {
        Console.WriteLine("Trying to open browser");
        System.Diagnostics.Process.Start(new System.Diagnostics.ProcessStartInfo(url) { UseShellExecute = true });
    }
}
catch (Exception ex)
{
    Console.WriteLine($"Could not open the browser: {ex.Message}");
}

Console.WriteLine("Running app");
app.Run();

