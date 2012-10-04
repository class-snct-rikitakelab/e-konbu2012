using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports; //for serial communicate

namespace LogTool
{
    public partial class LogToolForm : Form
    {
        public LogToolForm()
        {
            InitializeComponent();
            init();

        }

        private void ConnectButton_Click(object sender, EventArgs e)
        {
            connectBlueToothDevice();
        }

    }
}