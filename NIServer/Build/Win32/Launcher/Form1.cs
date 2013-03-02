using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace LaunchTray
{
    public partial class Form1 : Form
    {
        private bool isRunning = false;
        private Process niProcess;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            notifyIcon1.ContextMenuStrip = notifyContextMunuStrip;
            RunNIServer();
        }

        private void RunNIServer()
        {
            if (isRunning == true)
            {
                return;
            }

            // Launch process
            ProcessStartInfo info = new ProcessStartInfo();
            info.FileName = "NIServer.exe";
            info.UseShellExecute = false;
            info.CreateNoWindow = true;

            try
            {
                niProcess = Process.Start(info);
                notifyIcon1.BalloonTipText = "NI Server is running..";
                notifyIcon1.ShowBalloonTip(500);
                isRunning = true;
            }
            catch (System.Exception ex)
            {
                notifyIcon1.BalloonTipText = "NI Server not found..";
                notifyIcon1.ShowBalloonTip(500);
                isRunning = false;
            }
        }

        private void runToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RunNIServer();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            notifyIcon1.Dispose();
            if (isRunning)
            {
                niProcess.Kill();
            }
            Application.Exit();
        }
    }
}
