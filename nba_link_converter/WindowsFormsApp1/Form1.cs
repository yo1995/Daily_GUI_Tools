using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
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

        String nba_processing(string link) {
            string result_link;
            string pattern1, pattern2, pattern3, pattern4;
            // prefix styles. e.g. 
            // https://cdn.nba.net/nba-drupal-prod/styles/landscape/http/i2.cdn.turner.com/nba/nba/dam/assets/160528142233-paul-pierce-celtics-2008-finals.1280x720.jpg?itok=U5YOwvaD
            // https://images.nba.net/nba/w_225,h_127,c_fill/https%3A%2F%2Fcdn.nba.net%2Fnba-drupal-prod%2Fstyles%2Flandscape%2Fs3%2F2018-04%2FTOP%252010.jpg%3Fitok%3DoFM_xMRU
            pattern1 = "https://(.+?)c_fill/";
            pattern2 = "https://cdn(.+?)/http/";
            //middle style.
            pattern3 = "/styles(.+?)s3";
            pattern4 = ".1280.720";
            Regex rgx1 = new Regex(pattern1);
            Regex rgx2 = new Regex(pattern2);
            Regex rgx3 = new Regex(pattern3);
            Regex rgx4 = new Regex(pattern4);
            result_link = HttpUtility.UrlDecode(link);
            result_link = rgx1.Replace(result_link, "");
            result_link = rgx2.Replace(result_link, "http://");
            result_link = rgx3.Replace(result_link, "");
            result_link = rgx4.Replace(result_link, "");

            return result_link;
        }

        private void textBox1_Click(object sender, EventArgs e)
        {
            string result_link;
            IDataObject data = Clipboard.GetDataObject();
            if (data.GetDataPresent(DataFormats.Text))
            {
                acquired_link = (string)data.GetData(DataFormats.UnicodeText);
            }
            
            string pattern0, pattern1;
            pattern0 = "src\\s*=\\s*\"(.+?)\"";  // src\s*=\s*"(.+?)"
            pattern1 = "http(.+?)nba(.+?)itok.{8}";  // https://cdn.nba.net/nba-drupal-prod/styles/square/s3/2018-05/rozierlebron.jpg?itok=zEjaJn2
            Regex rgx0 = new Regex(pattern0);

            if (acquired_link.Contains("src=\"")) {
                Match m1 = rgx0.Match(acquired_link);
                acquired_link = m1.Groups[0].Value;
                int length = acquired_link.Length;
                acquired_link = acquired_link.Substring(5, length - 6);  // src=""
            }
            if (acquired_link.Contains("nba")) {
                Regex rgx1 = new Regex(pattern1);
                Match m2 = rgx1.Match(acquired_link);
                acquired_link = m2.Groups[0].Value;
            }


            result_link = nba_processing(acquired_link);
            textBox1.Text = result_link;


            if (!result_link.Contains("http:") && !result_link.Contains("https:")) {
                MessageBox.Show("invalid link");
                return;  // invalid string returns here
            }
            // MessageBox.Show(result_link);
            System.Diagnostics.Process.Start(result_link);
        }

        private void Form1_Load(object sender, EventArgs e) {

        }
    }
}
