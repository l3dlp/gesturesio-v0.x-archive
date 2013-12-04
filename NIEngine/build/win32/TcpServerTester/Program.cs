using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Net.Sockets;

namespace TcpServerTester
{
    class Program
    {
        static void Main(string[] args)
        {

            ConsoleKeyInfo cki;
            do
            {
                cki = Console.ReadKey();

                TcpClient client = new TcpClient("127.0.0.1", 5678);
                try
                {
                    Stream s = client.GetStream();
                    StreamReader sr = new StreamReader(s);
                    StreamWriter sw = new StreamWriter(s);
                    sw.AutoFlush = true;
                    Console.WriteLine("Please enter the command:\n");
                    string cmd = Console.ReadLine();
                    sw.WriteLine(cmd);
                    s.Close();
                }
                finally
                {
                    client.Close();
                }

            } while (cki.Key != ConsoleKey.Escape);

            Console.ReadKey();
        }
    }
}
