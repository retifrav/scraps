// put that in the App.xaml.cs
protected override void OnStartup(StartupEventArgs e)
{
    base.OnStartup(e);

    string[] cmdArgs = Environment.GetCommandLineArgs();
    Dictionary<string, string> args = new Dictionary<string, string>();
    if (cmdArgs.Length > 1)
    {
        try
        {
            for (int i = 1; i < cmdArgs.Length; i += 2)
            {
                args.Add(cmdArgs[i], cmdArgs[i + 1]);
            }
        }
        catch (Exception ex)
        {
            MessageBox.Show(
                "You passed some wrong arguments. Start the application without any arguments and press F1 for help.",
                "Error parsing command line arguments",
                MessageBoxButton.OK,
                MessageBoxImage.Error
                );
        }
    }
    else
    {
        new MainWindow().ShowDialog();
    }
    this.Shutdown();
}