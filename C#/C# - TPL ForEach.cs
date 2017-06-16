private static int threadsCount = 0;

void prepareForParallel()
{
    List<string> someVals = new List<string>();
    for (int i = 1; i < 999; i++)
    {
        someVals.Add(i.ToString());
    }

    Stopwatch sw = new Stopwatch();
    sw.Start();

    ParallelOptions options = new ParallelOptions();
    options.MaxDegreeOfParallelism = 3;
    Parallel.ForEach<string>(someVals, options, doParallelStuff);

    sw.Stop();
    Console.WriteLine(string.Format("Прошло времени: {0}", sw.ElapsedMilliseconds));
}

void doParallelStuff(string someVal)
{
    Interlocked.Increment(ref threadsCount);
    Debug.WriteLine("threads: " + threadsCount.ToString() + ", value: " + someVal);
    Interlocked.Decrement(ref threadsCount);
}