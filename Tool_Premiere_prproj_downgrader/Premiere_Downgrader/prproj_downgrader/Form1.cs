using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Compression;
using System.Windows.Forms;
using System.Xml;


namespace prproj_downgrader {

    public partial class Form1 : Form {
        public static bool IsChineseSimple() {
            return System.Threading.Thread.CurrentThread.CurrentCulture.Name == "zh-CN";
        }

        string inFileName;
         
        public Form1() {
            InitializeComponent();
            var selected1 = "1. 选择.prproj文件";
            var selected2 = "2. 选择目标版本号";
            var selected3 = "3. 转换并保存";
            var b1 = "打开文件";
            var b2 = "转换！";
            var title = ".prproj 转换器";
            if (IsChineseSimple()) {
                label1.Text = selected1;
                label2.Text = selected2;
                label3.Text = selected3;
                button1.Text = b1;
                button2.Text = b2;
                this.Text = title;
            }
        }

        private void label1_Click(object sender, EventArgs e) {

        }

        private void label2_Click(object sender, EventArgs e) {

        }

        private void button1_Click(object sender, EventArgs e) {

            var title = "请选择Adobe Premiere Pro CC工程文件";
            var filter = "Pr项目文件(*.prproj)|*.prproj";
            var selected = "1. 项目文件已选中";
            if (!IsChineseSimple()) {
                title = "Please choose Adobe Premiere Pro CC project file";
                filter = "Pr project file(*.prproj)|*.prproj";
                selected = "1. File Selected.";
            }

            OpenFileDialog ofd = new OpenFileDialog {
                Multiselect = false,
                Title = title,
                Filter = filter,
                ValidateNames = true,
                CheckPathExists = true,
                CheckFileExists = true,
                ShowHelp = true
            };
            ofd.HelpRequest += new EventHandler(ofd_HelpRequest);
            if (ofd.ShowDialog() == DialogResult.OK) {
                inFileName = ofd.FileName;
                this.comboBox1.Enabled = true;
                label1.Text = selected;
                label1.BackColor = Color.Lime;
            }
            ofd.Dispose();
        }


        private void ofd_HelpRequest(object sender, EventArgs e) {
            // MessageBox.Show("Please visit: https://github.com/yo1995/Daily_GUI_Tools/tree/master/Tool_Premiere_prproj_downgrader to submit issues.");
            System.Diagnostics.Process.Start("https://github.com/yo1995/Daily_GUI_Tools/tree/master/Tool_Premiere_prproj_downgrader");
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e) {

        }

        private void label3_Click(object sender, EventArgs e) {

        }

        private void comboBox1_SelectedValueChanged(object sender, EventArgs e) {
            var item = this.comboBox1.GetItemText(this.comboBox1.SelectedItem);
            if (item == "Manually assign") {
                this.textBox1.Enabled = true;
            }
            else {
                this.textBox1.Enabled = false;
            }

            
            var selected = "2. 版本号已选中";
            if (!IsChineseSimple()) {
                selected = "2. Version Selected.";
            }

            label2.Text = selected;
            label2.BackColor = Color.Lime;
            this.button2.Enabled = true;
        }

        private void button2_Click(object sender, EventArgs e) {
            int version = 0;
            var item = this.comboBox1.GetItemText(this.comboBox1.SelectedItem);
            switch (item) {
                case "Premiere Pro CC 2019.4 - 37":
                    version = 37;
                    break;
                case "Premiere Pro CC 2019 - 36":
                    version = 36;
                    break;
                case "Premiere Pro CC 2018 - 35":
                    version = 35;
                    break;
                case "Premiere Pro CC 2017.4 - 34":
                    version = 34;
                    break;
                case "Premiere Pro CC 2017.1 - 33":
                    version = 33;
                    break;
                case "Premiere Pro CC 2017 - 32":
                    version = 32;
                    break;
                case "Premiere Pro CC 2015.5 - 31":
                    version = 31;
                    break;
                case "Premiere Pro CC 2015.2 - 30":
                    version = 30;
                    break;
                case "Manually assign":
                    version = Int32.Parse(this.textBox1.Text);
                    break;
                case "Universal - 0":
                    version = 0;
                    break;
                default:
                    version = 0;
                    break;
            }
            String outFilename;
            String extension = System.IO.Path.GetExtension(inFileName);
            String fileNameWithoutExtension = System.IO.Path.GetFileNameWithoutExtension(inFileName);
            outFilename = System.IO.Path.GetDirectoryName(inFileName) + "\\" + fileNameWithoutExtension + "_v" + version.ToString() + extension;
            Decompress(inFileName, outFilename);
            Change_Version(outFilename, version);

            var selected1 = "1. 选择.prproj文件";
            var selected2 = "2. 选择目标版本号";
            if (!IsChineseSimple()) {
                selected1 = "1. Select .prproj file";
                selected2 = "2. Select target version";
            }
            // clear the text in combo box and textbox for custom version number
            this.comboBox1.Text = null;
            this.textBox1.Text = null;

            // reset values
            label1.Text = selected1;
            label2.Text = selected2;
            label1.BackColor = SystemColors.Control;
            label2.BackColor = SystemColors.Control;
            this.button2.Enabled = false;  // only convert one file once
            this.comboBox1.Enabled = false;
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e) {
            if (e.KeyChar != 8 && !Char.IsDigit(e.KeyChar)) {
                e.Handled = true;
            }
            if (textBox1.Text != null && textBox1.Text != "") {
                int version = Int32.Parse(this.textBox1.Text);
                if ((version < 0)) {
                    this.textBox1.Text = 0.ToString();
                }
                if ((version > 40)) {  // current cc2018 is 35, update later but reserve surplus
                    this.textBox1.Text = 35.ToString();
                }
            }
        }

        private void Form1_Load(object sender, EventArgs e) {
            // this.comboBox1.SelectedIndex = this.comboBox1.Items.IndexOf("Premiere Pro CC 2017 - 32");
            this.button2.Enabled = false;  // falsify on load so no wrong click
            this.comboBox1.Enabled = false;
        }

        private void Change_Version(string out_file_name, int version) {
            // .prproj is an xml file intrinsically
            XmlDocument document = new XmlDocument();
            document.Load(out_file_name);
            XmlElement root = document.DocumentElement;
            // XmlNodeList Nodes = root.GetElementsByTagName("Project");
            XmlNode selectedNode = root.SelectSingleNode("descendant::Project[@ObjectID='1']");

            var msg = "转换成功！原版本号为v" + selectedNode.Attributes["Version"].Value;
            if (!IsChineseSimple()) {
                msg = "Conversion done! The original project version is: v" + selectedNode.Attributes["Version"].Value;
            }

            MessageBox.Show(msg);
            selectedNode.Attributes["Version"].Value = version.ToString();
            document.Save(out_file_name);  // overwrite the file
        }

        private static int Decompress(string in_file_name, string out_file_name) {
            int iRet = 0;
            byte[] buf = new byte[1024 * 1024];
            try {
                FileStream in_fs = new FileStream(in_file_name, FileMode.Open, FileAccess.Read); 
                FileStream out_fs = new FileStream(out_file_name, FileMode.Create, FileAccess.Write);
                GZipStream decompressedStream = new GZipStream(in_fs, CompressionMode.Decompress, true);

                int count = 0;
                while ((count = decompressedStream.Read(buf, 0, buf.Length)) > 0) {
                    //向目标文件流写入byte[]中转数组
                    out_fs.Write(buf, 0, count);
                }
                iRet = (int)out_fs.Length;
                in_fs.Close();
                out_fs.Close();
            }
            finally {

            }
            return iRet;
        }

    }
}
