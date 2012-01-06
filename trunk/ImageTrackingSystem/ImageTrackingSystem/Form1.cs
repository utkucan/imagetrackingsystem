using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Threading;

namespace ImageTrackingSystem
{
    public partial class Form1 : Form
    {
        List<Thread> thrdlst;
        static int threadId = 0;
        public Form1()
        {
            InitializeComponent();
            thrdlst = new List<Thread>();
            //string imagename = "C:/Users/sercan/Desktop/seniorProj/dene6/ImageTrackingSystem/ImageTrackingSystem/m4.jpg";
           // ITS.facedetect fc = new ITS.facedetect();
        
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog fd = new OpenFileDialog();
            fd.Multiselect = true;
            if (fd.ShowDialog() == DialogResult.OK)
            {
                TextWriter tw = new StreamWriter("imagedir" + thrdlst.Count + ".txt");
                
                for(int i = 0; i<fd.FileNames.Length; i++)
                {
                    tw.WriteLine(fd.FileNames[i]);
                }
                tw.Close();
            }
            thrdlst.Add(new Thread(new ThreadStart(threadfunc)));
            thrdlst[thrdlst.Count - 1].Start();
        }

       
        private void threadfunc() {
         //   ITS.facedetect fc = new ITS.facedetect();
            ITS.facedetect fc = new ITS.facedetect(thrdlst.Count-1);
            while (fc.isfin() == false) ;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            label1.Text = "running";
            for (int i = 0; i < thrdlst.Count; i++)
            {
                while (thrdlst[i].ThreadState != ThreadState.Stopped) ;
            }
            label1.Text = "finnish";
        }

    }
}
