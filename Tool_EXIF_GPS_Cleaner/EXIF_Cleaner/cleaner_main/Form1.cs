using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.IO;

namespace cleaner_main {
    public partial class Form1 : Form {
        
        public Form1() {
            InitializeComponent();
        }

        private void ofd_HelpRequest(object sender, EventArgs e) {
            System.Diagnostics.Process.Start("https://github.com/yo1995/Daily_GUI_Tools/tree/master/Tool_EXIF_GPS_Cleaner");
        }

        /* Tab1 related */
        #region Tab1

        private void tabPage1_DragOver(object sender, DragEventArgs e) {
            tabPage1.BackColor = Color.LightGreen;
            tabPage1.BackgroundImage = null;
        }

        private void tabPage1_DragLeave(object sender, EventArgs e) {
            tabPage1.BackColor = Color.Transparent;
            tabPage1.BackgroundImage = cleaner_main.Properties.Resources.drag_and_drop;
        }

        private void tabPage1_DragEnter(object sender, DragEventArgs e) {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                e.Effect = DragDropEffects.Link;
            else
                e.Effect = DragDropEffects.None;
        }

        private void tabPage1_DragDrop(object sender, DragEventArgs e) {
            Array file = (System.Array)e.Data.GetData(DataFormats.FileDrop);
            String fileText = null;
            int i = 0;
            foreach (object I in file) {
                i++;
                if (i > 1) {
                    MessageBox.Show("Info: Only allow one file to see in map!" , "File amount limit", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    break;  // only allow one file for tab1, refactor in the future.
                } 
                fileText += I.ToString();
                // fileText += "\n";
            }
            String inFileName = fileText;
            // label1.Text = fileText;  // for debug use
            tabPage1.BackColor = Color.Transparent;
            tabPage1.BackgroundImage = cleaner_main.Properties.Resources.drag_and_drop;
            get_GPS_info(inFileName);
        }

        // adapted from StackOverflow.
        private static double ExifGpsToDouble(PropertyItem propItemRef, PropertyItem propItem) {
            double degreesNumerator = BitConverter.ToUInt32(propItem.Value, 0);
            double degreesDenominator = BitConverter.ToUInt32(propItem.Value, 4);
            double degrees = degreesNumerator / (double)degreesDenominator;

            double minutesNumerator = BitConverter.ToUInt32(propItem.Value, 8);
            double minutesDenominator = BitConverter.ToUInt32(propItem.Value, 12);
            double minutes = minutesNumerator / (double)minutesDenominator;

            double secondsNumerator = BitConverter.ToUInt32(propItem.Value, 16);
            double secondsDenominator = BitConverter.ToUInt32(propItem.Value, 20);
            double seconds = secondsNumerator / (double)secondsDenominator;

            double coorditate = degrees + (minutes / 60d) + (seconds / 3600d);
            string gpsRef = System.Text.Encoding.ASCII.GetString(new byte[1] { propItemRef.Value[0] });  // N, S, E, or W
            if (gpsRef == "S" || gpsRef == "W") coorditate = coorditate * -1;  // as the definition of coord in maps
            return coorditate;
        }

        

        private void get_GPS_info(String inFileName) {
            String longitude = null;
            String latitude = null;
            Image img;
            try {
                img = Image.FromFile(inFileName);
            }
            catch (OutOfMemoryException) {
                MessageBox.Show("Error: Cannot recognize this file. Try again with a valid image!", "File format error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            catch (FileNotFoundException) {
                MessageBox.Show("Error: File not found!", "File not found", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            
            PropertyItem[] propItems = img.PropertyItems;

            //Property Item 0x0002 - PropertyTagGpsLatitude
            //Property Item 0x0004 - PropertyTagGpsLongitude
            try {
                PropertyItem propItemLatRef = img.GetPropertyItem(1);
                PropertyItem propItemLat = img.GetPropertyItem(2);
                PropertyItem propItemLongRef = img.GetPropertyItem(3);
                PropertyItem propItemLong = img.GetPropertyItem(4);
                longitude = ExifGpsToDouble(propItemLongRef, propItemLong).ToString("f5");  // float with %.5fm just as the exp of google.
                latitude = ExifGpsToDouble(propItemLatRef, propItemLat).ToString("f5");
            }
            catch (ArgumentException) {
                MessageBox.Show("Warning: File doesn't contain GPS metadata!", "File not valid", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            
            img.Dispose();
            GC.Collect();

            if (checkBox1.Checked) {
                query_google_map(latitude, longitude);
            }
            if (checkBox2.Checked) {
                query_baidu_map(latitude, longitude);
            }
            if (checkBox3.Checked) {
                query_bing_map(latitude, longitude);
            }
            if (!checkBox1.Checked && !checkBox2.Checked && !checkBox3.Checked) {
                MessageBox.Show("Warning: Please at least check one box!", "Checkbox empty", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                checkBox1.Checked = true;
            }
            
        }

        private void query_google_map(String longitude, String latitude) {
            // Map coord format: https://support.google.com/maps/answer/18539
            String google_url = "http://maps.apple.com/maps?daddr=";  // seems it is an api reserved for Apple Inc. just use it to avoid trouble.
            // http://maps.apple.com/maps?daddr=37.7886,-122.407159
            // https://www.google.com/maps/search/@37.7886,-122.407159,15z
            google_url += longitude + "," + latitude;
            System.Diagnostics.Process.Start(google_url);  // visit google map page.
        }

        private void query_baidu_map(String longitude, String latitude) {
            String baidu_url = "http://api.map.baidu.com/lbsapi/getpoint/index.html";
            String query_str = latitude + "," + longitude;
            Clipboard.SetDataObject(query_str);
            MessageBox.Show("Info: 由于百度地图限制，请手动勾选坐标反查。\n经纬度已复制到剪贴板！", "手动查询", MessageBoxButtons.OK, MessageBoxIcon.Information);
            System.Diagnostics.Process.Start(baidu_url);
        }

        private void query_bing_map(String longitude, String latitude) {
            String bing_url = "https://www.bing.com/maps/directions?cp=";
            bing_url += longitude + "~" + latitude + "&lvl=15&typ=d&sty=r";
            System.Diagnostics.Process.Start(bing_url);
        }

        private void tabPage1_MouseClick(object sender, MouseEventArgs e) {
            OpenFileDialog ofd = new OpenFileDialog {
                Multiselect = false,
                Title = "Please choose the image with GPS to show in map.",
                Filter = "JPEG Image (*.jpg)|*.jpg;*.jpeg|WebP Image (*.webp)|*.webp|TIFF Image (*.tiff) | *.tif; *.tiff",
                ValidateNames = true,
                CheckPathExists = true,
                CheckFileExists = true,
                ShowHelp = true
            };
            ofd.HelpRequest += new EventHandler(ofd_HelpRequest);
            if (ofd.ShowDialog() == DialogResult.OK) {
                get_GPS_info(ofd.FileName);
            }
            ofd.Dispose();
            GC.Collect();
        }

        private void tabPage1_MouseDoubleClick(object sender, MouseEventArgs e) {
            OpenFileDialog ofd = new OpenFileDialog {
                Multiselect = false,
                Title = "Please choose the image with GPS to show in map.",
                Filter = "JPEG Image (*.jpg)|*.jpg;*.jpeg|WebP Image (*.webp)|*.webp|TIFF Image (*.tiff) | *.tif; *.tiff",
                ValidateNames = true,
                CheckPathExists = true,
                CheckFileExists = true,
                ShowHelp = true
            };
            ofd.HelpRequest += new EventHandler(ofd_HelpRequest);
            if (ofd.ShowDialog() == DialogResult.OK) {
                get_GPS_info(ofd.FileName);
            }
            ofd.Dispose();
            GC.Collect();
        }
        #endregion


        /* Tab2 related */
        #region Tab2

        private bool clear_GPS_info(String inFileName) {
            // to see items id related with GPS, see: https://docs.microsoft.com/en-us/dotnet/api/system.drawing.imaging.propertyitem.id
            Image img;
            try {
                img = Image.FromFile(inFileName);
            }
            catch (OutOfMemoryException) {
                return false;
            }
            catch (FileNotFoundException) {
                return false;
            }

            return true;
        }

        private void tabPage2_DragDrop(object sender, DragEventArgs e) {
            Array file = (System.Array)e.Data.GetData(DataFormats.FileDrop);
            String error_string = "";
            tabPage2.BackColor = Color.Transparent;
            tabPage2.BackgroundImage = cleaner_main.Properties.Resources.drag_and_drop;
            foreach (object I in file) {
                if (!clear_GPS_info(I.ToString())) {
                    error_string += I.ToString();
                    error_string += "\n";
                }
            }
            if (error_string.Length > 0) {
                error_string = "Error! Following files cannot be processed.\n" + error_string;
                MessageBox.Show(error_string, "Exception list", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            else {
                MessageBox.Show("Succeed!", "File processing complete", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
            }
        }

        private void tabPage2_DragEnter(object sender, DragEventArgs e) {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                e.Effect = DragDropEffects.Link;
            else
                e.Effect = DragDropEffects.None;
        }

        private void tabPage2_DragLeave(object sender, EventArgs e) {
            tabPage2.BackColor = Color.Transparent;
            tabPage2.BackgroundImage = cleaner_main.Properties.Resources.drag_and_drop;
        }

        private void tabPage2_DragOver(object sender, DragEventArgs e) {
            tabPage2.BackColor = Color.LightGreen;
            tabPage2.BackgroundImage = null;
        }

        // deal with multiselect
        private void tabPage2_MouseClick(object sender, MouseEventArgs e) {
            OpenFileDialog ofd = new OpenFileDialog {
                Multiselect = true,
                Title = "Please choose the image with GPS to show in map.",
                Filter = "JPEG Image (*.jpg)|*.jpg;*.jpeg|WebP Image (*.webp)|*.webp|TIFF Image (*.tiff) | *.tif; *.tiff",
                ValidateNames = true,
                CheckPathExists = true,
                CheckFileExists = true,
                ShowHelp = true
            };
            ofd.HelpRequest += new EventHandler(ofd_HelpRequest);
            if (ofd.ShowDialog() == DialogResult.OK) {
                // get_GPS_info(ofd.FileName);
            }
            ofd.Dispose();
            GC.Collect();
        }
        #endregion



        /* Tab3 related */
        #region Tab3

        private bool clear_EXIF_info(String inFileName) {
            Image img;
            try {
                img = Image.FromFile(inFileName);
            }
            catch (OutOfMemoryException) {
                return false;
            }
            catch (FileNotFoundException) {
                return false;
            }
            // https://docs.microsoft.com/en-us/dotnet/framework/winforms/advanced/how-to-read-image-metadata#type
            try {
                foreach (PropertyItem item in img.PropertyItems) {
                    if (item.Id == 0x112) {  // remove all the EXIF items except rotation, PropertyTagOrientation
                        continue;
                    }
                    if (item.Type == 1) {
                        img.RemovePropertyItem(item.Id);
                    }
                    //remove manufacturer model software date
                    if (item.Type == 2) {
                        img.RemovePropertyItem(item.Id);
                    }
                    //remove color space flash light ISO exposure
                    if (item.Type == 3) {
                        img.RemovePropertyItem(item.Id);
                    }

                    if (item.Type == 4) {
                        img.RemovePropertyItem(item.Id);
                    }
                    //remove GPS aperture size focus length
                    if (item.Type == 5) {
                        img.RemovePropertyItem(item.Id);
                    }
                    //remove speed EV
                    if (item.Type == 10) {
                        img.RemovePropertyItem(item.Id);
                    }
                }
            }
            catch (ArgumentException) {
                return false;
            }
            // might be concerns about the quality.
            String fileNameTemp = inFileName + ".temp";
            img.Save(fileNameTemp);
            img.Dispose();
            GC.Collect();
            System.IO.File.Delete(inFileName);
            // rename
            if (System.IO.File.Exists(fileNameTemp)) {
                System.IO.File.Move(fileNameTemp, inFileName);
            }
            return true;
        }
        
        private void tabPage3_DragDrop(object sender, DragEventArgs e) {
            Array file = (System.Array)e.Data.GetData(DataFormats.FileDrop);
            String error_string = "";
            tabPage3.BackColor = Color.Transparent;
            tabPage3.BackgroundImage = cleaner_main.Properties.Resources.drag_and_drop;
            foreach (object I in file) {
                if (!clear_EXIF_info(I.ToString())) {
                    error_string += I.ToString();
                    error_string += "\n";
                }
            }
            if (error_string.Length > 0) {
                error_string = "Error! Following files cannot be processed.\n" + error_string;
                MessageBox.Show(error_string, "Exception list", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            else {
                MessageBox.Show("Succeed!", "File processing complete", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
            }
        }

        private void tabPage3_DragEnter(object sender, DragEventArgs e) {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                e.Effect = DragDropEffects.Link;
            else
                e.Effect = DragDropEffects.None;
        }

        private void tabPage3_DragLeave(object sender, EventArgs e) {
            tabPage3.BackColor = Color.Transparent;
            tabPage3.BackgroundImage = cleaner_main.Properties.Resources.drag_and_drop;
        }

        private void tabPage3_DragOver(object sender, DragEventArgs e) {
            tabPage3.BackColor = Color.LightGreen;
            tabPage3.BackgroundImage = null;
        }

        private void tabPage3_Click(object sender, EventArgs e) {

        }

        #endregion
    }
}
