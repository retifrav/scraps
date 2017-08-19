using System.Threading;
using System.Threading.Tasks;

...

Semaphore s = new Semaphore(3, 3);

int threadsCount = 0;
int maxThreads = 2;

void mainFunction()
{
    int big = 0;
    while (big < 8)
    {
        big++;
        if (threadsCount >= maxThreads) { Debug.WriteLine("busy"); continue; }

        int x = 0;
        while (x < 5)
        {
            x++;
            Task.Factory.StartNew(() =>
            {
                s.WaitOne();
                doParallelStuff(big);
                s.Release();
            })
            .ContinueWith((finishedTask) =>
            {
                finishedTask.Dispose();
                //Debug.WriteLine(finishedTask. Id + " finished");
            });
        }
        Thread.Sleep(20);
    }
}

void doParallelStuff(int crnt)
{
    Interlocked.Increment(ref threadsCount);
    Debug.WriteLine(crnt.ToString() + " threads: " + threadsCount.ToString());
    Interlocked.Decrement(ref threadsCount);
}

1 threads: 3
1 threads: 2
1 threads: 1
2 threads: 3
2 threads: 3
busy
3 threads: 1
3 threads: 3
3 threads: 3
3 threads: 3
3 threads: 3
4 threads: 3
4 threads: 3
5 threads: 3
5 threads: 3
4 threads: 3
busy
6 threads: 1
6 threads: 3
6 threads: 2
6 threads: 3
6 threads: 3
7 threads: 1
7 threads: 3
7 threads: 3
7 threads: 2
7 threads: 3
8 threads: 3
8 threads: 3
8 threads: 3
8 threads: 3
8 threads: 3