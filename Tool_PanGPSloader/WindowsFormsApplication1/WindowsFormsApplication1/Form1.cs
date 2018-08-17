using System;
using System.Diagnostics;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //开启服务   
            ProcessStartInfo a = new ProcessStartInfo(@"c:/windows/system32/cmd.exe", "/c  net start PanGPS");
            a.WindowStyle = ProcessWindowStyle.Hidden;
            Process process = Process.Start(a);
            Process.Start(@"c:/ProgramData/Microsoft/Windows/Start Menu/Programs/Palo Alto Networks/GlobalProtect/GlobalProtect.lnk");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            ProcessStartInfo a = new ProcessStartInfo(@"c:/windows/system32/cmd.exe", "/c  net stop PanGPS");
            a.WindowStyle = ProcessWindowStyle.Hidden;
            Process process = Process.Start(a);
        }
    }
}
