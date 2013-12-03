using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using ManagedNI;

namespace ManagedNIEngineTester
{
    class Program
    {
        static ManagedNIEngine engine = new ManagedNIEngine();

        static void Main(string[] args)
        {
            BackgroundWorker niEngineWorker = new BackgroundWorker();
            niEngineWorker.DoWork += new DoWorkEventHandler(niEngineWorker_DoWork);
            niEngineWorker.RunWorkerAsync();

            engine.Started += new EventHandler(engine_Started);
            engine.Stopped += new EventHandler(engine_Stopped);
            engine.Ended += new EventHandler(engine_Ended);
            Console.WriteLine("Engine running... \nPress 'S' to start \nPress 'T' to stop \nPress 'E' to end");

            ConsoleKeyInfo cki;
            do 
            {
                cki = Console.ReadKey();

                if (cki.Key == ConsoleKey.S)
                {
                    Console.WriteLine("\nStarting...");
                    engine.Start();
                }
                
                if (cki.Key == ConsoleKey.T)
                {
                    Console.WriteLine("\nStopping...");
                    engine.Stop();
                }

                if (cki.Key == ConsoleKey.E)
                {
                    Console.WriteLine("\nEnding...");
                    engine.End();
                }

            } while (cki.Key != ConsoleKey.Escape);

        }

        static void engine_Ended(object sender, EventArgs e)
        {
            Console.WriteLine("\nEngine ended.");
        }

        static void engine_Stopped(object sender, EventArgs e)
        {
            Console.WriteLine("\nStopped.");
        }

        static void engine_Started(object sender, EventArgs e)
        {
            Console.WriteLine("\nStarted.");
        }

        static void niEngineWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            // Do it in worker thread, as currently this function is still blocking.
            engine.Init();
        }

    }
}
