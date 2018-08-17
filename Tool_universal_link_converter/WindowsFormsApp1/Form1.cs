using System;

using System.Drawing;
using System.Windows.Forms;

using System.Text.RegularExpressions;
using System.Web;


namespace WindowsFormsApp1
{
    
    public partial class Form1 : Form
    {
        string acquired_link;

        public Form1()
        {
            InitializeComponent();
        }

        #region Move_without_border

        bool b = false;
        Point loc;
        Point p;

        private void Form1_MouseDown(object sender, MouseEventArgs e) {
            if (e.Button == MouseButtons.Left) {
                textBox1.Text = "";
                b = true;
                loc = this.Location;
                p = MousePosition;
            }
        }

        private void Form1_MouseMove(object sender, MouseEventArgs e) {
            if (b == true) {
                Point p1 = Control.MousePosition;
                int x = loc.X + p1.X - p.X;
                int y = loc.Y + p1.Y - p.Y;
                this.Location = new Point(x, y);
            }
        }


        private void Form1_MouseUp(object sender, MouseEventArgs e) {
            b = false;
        }
        #endregion

        private void button1_Click(object sender, EventArgs e)
        {
            if (this.TopMost) {
                this.TopMost = false;
                button1.Text = "窗口置顶";
            }
            else {
                this.TopMost = true;
                button1.Text = "取消置顶";
            }

        }


        private void button2_Click(object sender, EventArgs e)
        {
            Close();
        }


        private void textBox1_Click(object sender, EventArgs e)
        {
            string result_link = null;
            acquired_link = null;  // clear the var to ensure nothing goes wrong
            IDataObject data = Clipboard.GetDataObject();
            if (data.GetDataPresent(DataFormats.Text))
            {
                acquired_link = (string)data.GetData(DataFormats.UnicodeText);
            }
            
            string pattern0;
            pattern0 = "\\?(.+)";  // src\s*=\s*"(.+?)"
            Regex rgx0 = new Regex(pattern0);
            
            
            if (acquired_link.Contains("w=") || acquired_link.Contains("h=")) {
                result_link = rgx0.Replace(acquired_link, "?w=9999&h=9999");
                textBox1.Text = result_link;
            }
            else {
                MessageBox.Show("invalid link");
                textBox1.Text = "invalid link";
                return;  // invalid string returns here
            }
            // MessageBox.Show(result_link);
            System.Diagnostics.Process.Start(result_link);
        }

        private void Form1_Load(object sender, EventArgs e) {

        }
    }
}
