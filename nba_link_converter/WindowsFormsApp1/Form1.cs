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
        string result_link;
        public Form1()
        {
            InitializeComponent();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            
        }


        private void button1_Click(object sender, EventArgs e)
        {
            string acquired_link;
            string pattern1, pattern2;
            pattern1 = "https://(.+?)c_fill/";
            pattern2 = "/styles(.+?)s3";
            Regex rgx1 = new Regex(pattern1);
            Regex rgx2 = new Regex(pattern2);
            acquired_link = textBox1.Text;
            result_link = HttpUtility.UrlDecode(acquired_link);
            result_link = rgx1.Replace(result_link, "");
            result_link = rgx2.Replace(result_link, "");
            System.Diagnostics.Process.Start(result_link);
        }


        private void button2_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void textBox1_Click(object sender, EventArgs e)
        {
            IDataObject data = Clipboard.GetDataObject();
            if (data.GetDataPresent(DataFormats.Text))
            {
                textBox1.Text = (string)data.GetData(DataFormats.UnicodeText);
            }
        }
    }
}
